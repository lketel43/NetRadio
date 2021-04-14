/* manager.c */

#include "message.h"

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_REGISTERED_BROADCASTER 100
#define BUFSIZE 512
/* Délai en secondes pour tester la présence d'un diffuseur */
#define CHECK_DELAY 47
#define TIMEOUT_SEC 2
#define TIMEOUT_USEC 0


/* Registre des diffuseurs */
char broadcaster_register[MAX_REGISTERED_BROADCASTER][REGI_LEN+1];
pthread_mutex_t register_mutex = PTHREAD_MUTEX_INITIALIZER;


char *register_broadcaster (const char *regi_msg);
int monitor_broadcaster (int broadcasterfd, char *canary);
int handle_broadcaster (int broadcasterfd, const char *msg);
int copy_broadcaster_register (char buffer[MAX_REGISTERED_BROADCASTER][REGI_LEN+1]);
int handle_user (int usersockfd);
void* handle_connection (void *arg);
int init_manager (struct sockaddr_in *address, int port);
int start_manager (int serverfd);


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
char *register_broadcaster (const char *regi_msg)
{
  char *ret = NULL;
  
  for (int i=0; i < MAX_REGISTERED_BROADCASTER && !ret; i++)
    {      
      pthread_mutex_lock (&register_mutex);
      
      if (broadcaster_register[i][0] == '\0')
	{
	  ret = broadcaster_register[i];
	  strcpy (ret, regi_msg);
	  strncpy (ret, msg_type_to_str(ITEM), MSG_TYPE_SIZE);
	}
      
      pthread_mutex_unlock (&register_mutex);
    }
  
  return ret;
}

/*
 * Surveille indéfiniment la présence d'un diffuseur
 *
 * On change la valeur de CANARY si le diffuseur est inaccessible
 */
int monitor_broadcaster (int broadcasterfd, char *canary)
{
  char msg_buf[BUFSIZE];
  struct timeval timeout;
  int r, quit;
  
  timeout.tv_sec = TIMEOUT_SEC;
  timeout.tv_usec = TIMEOUT_USEC;
  
  if (setsockopt (broadcasterfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
    {
      perror ("setsockopt");
      return -1;
    }
  
  quit = 0;
  
  while (!quit)
    {
      create_message (msg_buf, RUOK);
      send (broadcasterfd, msg_buf, msglen(RUOK), 0);

      sleep (CHECK_DELAY);
      
      r = recv(broadcasterfd, msg_buf, BUFSIZE, 0);
      if (r < 0 || r != msglen(IMOK))
	break;
	
      switch(get_msg_type(msg_buf))
	{
	case IMOK:
	  break;
	default:
	  quit = 1;
	}
    }

  *canary = '\0';

  return -1;
}

/*
 * Gère la demande d'un diffuseur
 */
int handle_broadcaster (int broadcasterfd, const char *regi_msg)
{
  char *canary;
  char msg_buf[BUFSIZE];
  
  canary = register_broadcaster (regi_msg);

  if (!canary) // NULL
    {
      create_message (msg_buf, RENO);
      send (broadcasterfd, msg_buf, msglen(RENO), 0);
      return 0;
    }

  create_message (msg_buf, REOK);
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
  
  for (int i=0, j=0; i < MAX_REGISTERED_BROADCASTER; i++)
    {      
      pthread_mutex_lock (&register_mutex);
      
      if (broadcaster_register[i][0] != '\0')
	{	  
	  memmove (buffer[j], broadcaster_register[i], REGI_LEN + 1);
	  size++;
	  j++;
	}
      
      pthread_mutex_unlock (&register_mutex);
    }

  return size;
}

/*
 * Gère la demande d'un utilisateur
 */
int handle_user (int usersockfd)
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
  send (usersockfd, msg_buf, msglen(LINB), 0);

  
  /*
   * On envoit NB message(s). Chaque message envoyé a la forme suivante 
   * [ITEM id ip1 port1 ip2 port2] où les informations transmises sont 
   * similaires à celles contenues dans les messages de type REGI
   */
  for (int i = 0; i < nb; i++)
    {     
      send (usersockfd, registered_broadcaster[i], msglen(ITEM), 0);
    }
  
  return 0;
}


/*
 * Routine utilisée pour la création des threads
 */
void* handle_connection (void *arg)
{
  int clientsockfd, r;
  char msg_buf[BUFSIZE];
  enum msg_type type;

  
  clientsockfd = *(int*)arg;
  
  r = recv(clientsockfd, msg_buf, BUFSIZE, 0);
  if (!r)
    {
      goto quit;
    }
  else if (r < 0)
    {
      perror ("recv");
      goto quit;
    }
  else if (r != REGI_LEN && r != LIST_LEN)
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
 * * BROADCASTER_REGISTER est réinitialisée
 * * ADDRESS est remplit à l'aide de PORT
 * 
 * Renvoit une socket bind'ée et listen'ée; -1 si erreur
 */ 
int init_manager (struct sockaddr_in *address, int port)
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
int start_manager (int serverfd)
{
  struct sockaddr client_addr;
  socklen_t client_addr_len;  
  
  while (1)
    {
      int* clientsockfd = malloc (sizeof(int));
      *clientsockfd = accept (serverfd, &client_addr, &client_addr_len);

      if (clientsockfd < 0)
	{
	  perror("accept");
	  free(clientsockfd);
	  continue;
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


int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s PORT\n", argv[0]);
      return EXIT_FAILURE;
    }

  struct sockaddr_in address;
  int sockfd, port, r;  
  
  // Port
  port = strtol(argv[1], NULL, 10);
  if (port <= 0)
    {
      fprintf (stderr, "Error in port number\n");
      return EXIT_FAILURE;
    }
  
  sockfd = init_manager (&address, port);
  
  if (sockfd < 0)
    return EXIT_FAILURE;

  r = start_manager (sockfd);
  
  shutdown(sockfd, SHUT_RDWR);
  
  return r;
}
