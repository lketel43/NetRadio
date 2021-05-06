/* manager.c */

#include "message.h"
#include "utils.h"

#include <assert.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#define BUFSIZE 512

#define TIMEOUT_SEC 2
#define TIMEOUT_USEC 0

/* Nombre maximale de diffuseur que le gestionnaire peut gérer */
#define MAX_REGISTERED_BROADCASTER 100

/* Délai en secondes pour tester la présence d'un diffuseur */
#define CHECK_DELAY 5


/* Registre des diffuseurs */
static char broadcaster_register[MAX_REGISTERED_BROADCASTER][REGI_LEN+1];

/* Mutex pour accéder au registre des diffuseurs */
static pthread_mutex_t register_mutex = PTHREAD_MUTEX_INITIALIZER;


/* Si TRUE alors on explique les opérations */
static bool verbose;

/* Options supportées :
 * 
 * -v 
 *    verbose, explique ce qu'il se passe
 */
static const char* options = "v";


static void verbose_print_message (const char *intro, const char *msg)
{
  if (verbose)
    {
      printf("%s", intro);
      print_raw_string(msg);
      printf("\n");
    }
}

/*
 * Enregistre un broadcaster
 * 
 * Cherche la première case disponible dans BROADCASTER_REGISTER puis écrit
 * MSG dedans (REGI est remplacé par LIST).
 *
 * Renvoit la chaîne de caractères écrite dans BROADCASTER_REGISTER
 *  
 * ! THREAD-SAFE : mutex_lock !
 */
static char *register_broadcaster (const char *regi_msg)
{
  char *ret = NULL;
  int i;
  
  pthread_mutex_lock (&register_mutex);
  
  for (i=0; i < MAX_REGISTERED_BROADCASTER; i++)
    {                  
      if (broadcaster_register[i][0] == '\0')
	{
	  ret = broadcaster_register[i];
	  strcpy (ret, regi_msg);
	  strncpy (ret, msg_type_to_str(ITEM), MSG_TYPE_SIZE);
	  break;
	}    
    }

  pthread_mutex_unlock (&register_mutex);

  if (verbose)
    {
      if (ret)
	printf("Enregistrement à la place n°%d\n", i);
      else
	printf("Plus de place disponible\n");
    }
  
  return ret;
}

/*
 * Surveille indéfiniment la présence d'un diffuseur
 *
 * On change la valeur de CANARY si le diffuseur est inaccessible
 */
static int monitor_broadcaster (int broadcasterfd, char *canary)
{
  char msg_buf[BUFSIZE];
  struct timeval timeout;
  int r;
  
  timeout.tv_sec = TIMEOUT_SEC;
  timeout.tv_usec = TIMEOUT_USEC;
  
  if (setsockopt (broadcasterfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
    {
      perror ("monitor_broadcaster: setsockopt");
      goto error;
    }
  
  
  while (1)
    {
      // 1 - SLEEP
      if (verbose)
	printf("On attend %ds\n", CHECK_DELAY);
      sleep (CHECK_DELAY);

      // 2 - SEND RUOK
      create_message (msg_buf, RUOK);
      verbose_print_message ("Message envoyé : ", msg_buf);
      r = send (broadcasterfd, msg_buf, msglen(RUOK), MSG_NOSIGNAL);
      if (r < 0)
	{
	  if (errno == EPIPE)
	    verbose_print_message ("Le diffuseur s'est deconnecté", "");
	  else
	    perror ("monitor_broadcaster: send");
	  
	  goto error;
	}
      
      // 3 - RECV IMOK
      r = recv(broadcasterfd, msg_buf, BUFSIZE, 0);
      if (r < 0)
	{
	  if (errno == EAGAIN)
	    verbose_print_message("Délai d'attente dépassé", "");
	  else
	    perror ("monitor_broadcaster: recv");
	  
	  goto error;
	}
      else if (r == 0)
	{
	  if (verbose)
	    printf("Le diffuseur s'est déconnecté\n");
	  
	  goto error;
	}

      verbose_print_message ("Message recu : ", msg_buf);
      
      switch(get_msg_type(msg_buf))
	{
	case IMOK:
	  break;
	default:
	  fprintf(stderr, "Wrong message type\n");
	  goto error;
	}
    }
  
 error:
  pthread_mutex_lock (&register_mutex);
  *canary = '\0';
  pthread_mutex_unlock (&register_mutex);
  
  return -1;
}

/*
 * Gère la demande d'un diffuseur
 */
static int handle_broadcaster (int broadcasterfd, const char *regi_msg)
{
  char *canary;
  char msg_buf[BUFSIZE];
  
  canary = register_broadcaster (regi_msg);

  if (!canary) // NULL
    {
      create_message (msg_buf, RENO);
      
      verbose_print_message ("Message envoyé : ", msg_buf);

      send (broadcasterfd, msg_buf, msglen(RENO), 0);
      return 0;
    }

  
  create_message (msg_buf, REOK);

  verbose_print_message ("Message envoyé : ", msg_buf);
  
  send (broadcasterfd, msg_buf, msglen(REOK), 0);
  
  return monitor_broadcaster (broadcasterfd, canary);
}

/*
 * Copie les éléments non nul de BROADCASTER_REGISTER dans BUFFER 
 * Renvoit la taille de buffer.
 * 
 * ! THREAD-SAFE : mutex_lock !
 */
int copy_broadcaster_register (char buffer[MAX_REGISTERED_BROADCASTER][REGI_LEN+1])
{
  int size = 0;

  pthread_mutex_lock (&register_mutex);
  for (int i=0, j=0; i < MAX_REGISTERED_BROADCASTER; i++)
    {      
      if (broadcaster_register[i][0] != '\0')
	{	  
	  memmove (buffer[j], broadcaster_register[i], REGI_LEN + 1);
	  size++;
	  j++;
	}      
    }
  pthread_mutex_unlock (&register_mutex);

  return size;
}

/*
 * Gère la demande d'un utilisateur
 */
static int handle_user (int usersockfd)
{
  char msg_buf[BUFSIZE];
  char registered_broadcaster[MAX_REGISTERED_BROADCASTER][REGI_LEN+1];
  int nb;
  
  nb = copy_broadcaster_register (registered_broadcaster);  

  /*
   * On va envoyer un message [LINB num-diff] où num-diff est un entier compris 
   * entre 0 et 99 précisant le nombre de messages de type ITEM qu’on va envoyer
   */
  create_message (msg_buf, LINB, nb);

  verbose_print_message ("Message envoyé : ", msg_buf);  
  
  send (usersockfd, msg_buf, msglen(LINB), 0);

  
  /*
   * On envoit NB message(s). Chaque message envoyé a la forme suivante 
   * [ITEM id ip1 port1 ip2 port2] où les informations transmises sont 
   * similaires à celles contenues dans les messages de type REGI
   */
  for (int i = 0; i < nb; i++)
    {
      verbose_print_message ("Message envoyé : ", registered_broadcaster[i]);
      send (usersockfd, registered_broadcaster[i], msglen(ITEM), 0);
    }
  
  return 0;
}


/*
 * Routine utilisée pour la création des threads
 */
static void* handle_connection (void *arg)
{
  int clientsockfd, r;
  char msg_buf[BUFSIZE];
  enum msg_type type;

  
  clientsockfd = *(int*)arg;
  
  r = recv(clientsockfd, msg_buf, BUFSIZE - 1, 0);
  if (r < 0)
    {
      perror ("recv");
      goto quit;
    }
  else if (r == 0)
    {
      if (verbose)
	printf("Le client s'est déconnecté sans envoyer de message\n");
      
      goto quit;
    }

  msg_buf[r] = '\0';

  verbose_print_message ("Message recu : ", msg_buf);
  
  if (r != REGI_LEN && r != LIST_LEN)
    {
      fprintf(stderr, "Wrong message format\n");
      goto quit;
    }
  
  type = get_msg_type (msg_buf);
  switch (type)
    {
    case REGI:
      handle_broadcaster (clientsockfd, msg_buf);
      break;
      
    case LIST:
      handle_user (clientsockfd);
      break;

    default:
      fprintf(stderr, "Wrong message type\n");
    }  

 quit:
  shutdown(clientsockfd, SHUT_RDWR);
  free(arg);

  return NULL;
}


/*
 * Initialise le manager
 *
 * * BROADCASTER_REGISTER est initialisée
 * * ADDRESS est remplit à l'aide de PORT
 * 
 * Renvoit une socket bind'ée et listen'ée; -1 si erreur
 */ 
static int init_manager (struct sockaddr_in *address, int port)
{  
  int fd, r;
  
  // broadcaster_register
  for (int i=0; i < MAX_REGISTERED_BROADCASTER; i++)
    broadcaster_register[i][0] = '\0';

  
  // address
  address->sin_family = AF_INET;
  address->sin_port = htons (port);
  address->sin_addr.s_addr = htonl (INADDR_ANY);

  // sockfd
  fd = socket (PF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    {
      perror("socket");
      return -1;
    }

  // On bind la socket à un port
  r = bind (fd, (struct sockaddr*)address, sizeof(struct sockaddr_in));
  if (r < 0)
    {
      perror("bind");
      goto error;
    }

  // La socket devient une socket serveur
  r = listen (fd, 0);
  if (r < 0)
    {
      perror("listen");
      goto error;
    }
  
  return fd;
  
 error:
  shutdown (fd, SHUT_RDWR);
  return -1;  
}


/*
 * Démarre le manager
 *
 * Le manager est threadé
 */
static int start_manager (int serverfd)
{
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  while (1)
    {      
      int* clientsockfd = malloc (sizeof(int));
      if (!clientsockfd)
	{
	  perror ("start_manager: malloc");
	  exit(EXIT_FAILURE);
	}
      
      *clientsockfd = accept (serverfd, (struct sockaddr *)&client_addr, &client_addr_len);

      if (clientsockfd < 0)
	{
	  perror("accept");
	  free(clientsockfd);
	  continue;
	}

      if (verbose)
	{
	  printf ("Nouvelle connexion établie\n");
	  print_sockaddr_in_info (&client_addr);
	}
      
      pthread_t thread;
      if (pthread_create (&thread, NULL, handle_connection, clientsockfd) != 0)
	{
	  perror ("pthread_create");
	  shutdown (*clientsockfd, SHUT_RDWR);
	  free (clientsockfd);
	}
    }

  return -1;
}


int main (int argc, char **argv)
{
  struct sockaddr_in address;
  int sockfd, port, r;
  int opt;

  
  opterr = 0;
    
  /* On décode les options */
  verbose = false;
  
  while ((opt = getopt(argc, argv, options)) != -1)
    {
      switch (opt)
	{
	case 'v':
	  verbose = true;
	  break;
	case '?':
	  if (isprint (optopt))
	    fprintf (stderr, "option invalide -- '%c'\n", optopt);
	  else
	    fprintf (stderr, "caractère non reconnu -- '\\x%x'\n", optopt);
	  return EXIT_FAILURE;
	default:
	  abort();
	}
    }

  if (optind != argc - 1)
    {
      fprintf (stderr, "Usage: %s [OPTION]... PORT\n", argv[0]);
      return EXIT_FAILURE;
    }
  
  
  // Port
  port = strtol(argv[optind], NULL, 10);
  if (port <= 0)
    {
      fprintf (stderr, "Erreur dans le port\n");
      return EXIT_FAILURE;
    }
  
  sockfd = init_manager (&address, port);
  
  if (sockfd < 0)
    return EXIT_FAILURE;

  if (verbose)
    printf ("Lancement du gestionnaire sur le port : %d\n", port);
  
  r = start_manager (sockfd);
  
  shutdown(sockfd, SHUT_RDWR);
  
  return r;
}
