/* manager.c */
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_REGISTERED_BROADCASTER 100
#define REGI_LEN 57


char registered_broadcaster[MAX_REGISTERED_BROADCASTER][REGI_LEN+1];


int init_manager (struct sockaddr_in *address, int port)
{
  
  int fd, r;

  
  // registered_broadcaster
  for (int i=0; i < MAX_REGISTERED_BROADCASTER; i++)
    registered_broadcaster[i][0] = '\0';

  
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

int start_manager (int serverfd)
{
  struct sockaddr client_addr;
  socklen_t client_addr_len;  
  
  while (1)
    {
      int* clientsockfd = malloc(sizeof(int));
      *clientsockfd = accept (serverfd, &client_addr, &client_addr_len);

      if (clientsockfd < 0)
	{
	  perror("accept");
	  free(clientsockfd);
	  continue;
	}
      
      shutdown(*clientsockfd, SHUT_RDWR);
      free(clientsockfd);
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
  if (!port)
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
