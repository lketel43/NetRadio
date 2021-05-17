#include "client.h"
#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define PROGRAM_NAME "scenario_many_clients"


static pthread_barrier_t barrier;
static int nb_clients = -1;
const char *streamer_ip = NULL;
int streamer_port = -1;


static void usage (int exit_status);


static void* mess_streamer (void *arg)
{
  int id = *(int*)arg;

  char client_id[MSG_ID_SIZE];
  snprintf(client_id, MSG_ID_SIZE, "cli%d", id);

  char msg[MSG_MESS_SIZE];
  snprintf(msg, MSG_MESS_SIZE, "Bonjour je suis le client n %d. Au revoir !", id);

  // On veut que tous les threads envoient le message en "même temps"
  pthread_barrier_wait (&barrier);
  
  send_mess_to_streamer(streamer_ip, streamer_port, client_id, msg);
    
  free (arg);
  return NULL;
}

int main (int argc, char **argv)
{
  if (argc != 4)
    {
      fprintf (stderr, "Mauvais nombre d'argument(s)\n");
      usage (EXIT_FAILURE);
    }
  // Pas de vérification d'erreurs !!!
  nb_clients = atoi (argv[1]);
  streamer_ip = argv[2];
  streamer_port = atoi(argv[3]);

  // On crée la barrière pour synchroniser nos threads
  pthread_barrier_init (&barrier, NULL, nb_clients);

  // On crée nos NB_CLIENTS threads
  pthread_t *tid = malloc(nb_clients * sizeof(pthread_t));
  for (int i=0; i < nb_clients; i++)
    {
      int *id = malloc(sizeof(int));
      *id = i;

      pthread_create (&tid[i], NULL, mess_streamer, id);
    }

  for(int i=0; i <nb_clients; i++)
    {
      pthread_join(tid[i], NULL);
    }

  free(tid);

  return EXIT_FAILURE;
}

static void usage (int exit_status)
{
  fprintf (stderr, "Usage: %s NB_CLIENTS STREAMER_IP STREAMER_PORT\n", PROGRAM_NAME);
  
  exit (exit_status);
}
