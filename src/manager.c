#include "manager.h"
#include "message.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_REGISTERED_BROADCASTER 100
#define REGI_LEN 57

struct manager
{
  int sockfd;
  struct sockaddr_in address;
  char registered_broadcaster[MAX_REGISTERED_BROADCASTER][REGI_LEN+1];
};



struct manager* create_manager (int port)
{
  int r;
  struct manager* ret = malloc (sizeof(struct manager));
  assert(ret);
  

  // address
  ret->address.sin_family = AF_INET;
  ret->address.sin_port = htons (port);
  ret->address.sin_addr.s_addr = htonl(INADDR_ANY);

  
  // registered_broadcaster
  for (int i=0; i < MAX_REGISTERED_BROADCASTER; i++)
    ret->registered_broadcaster[i][0] = '\0';

  
  // sockfd
  ret->sockfd = socket (PF_INET, SOCK_STREAM, 0);
  if (ret->sockfd < 0)
    {
      perror("socket");
      free (ret);
      return NULL;
    }

  // On bind la socket à un port
  r = bind(ret->sockfd, (struct sockaddr*)&ret->address, sizeof ret->address);
  if (r < 0)
    {
      perror("bind");
      goto error;
    }

  // La socket devient une socket serveur
  r = listen(ret->sockfd, 0);
  if (r < 0)
    {
      perror("listen");
      goto error;
    }
  
  return ret;

  
 error:
  shutdown (ret->sockfd, SHUT_RDWR);
  free(ret);
  return NULL;  
}

int start_manager (struct manager* man)
{
  int serversockfd;
  struct sockaddr client_addr;
  socklen_t client_addr_len;

  serversockfd = man->sockfd;
  
  while (1)
    {
      int* clientsockfd = malloc(sizeof(int));
      *clientsockfd = accept (serversockfd, &client_addr, &client_addr_len);

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

void destroy_manager(struct manager* man)
{
  if (!man)
    return;

  shutdown(man->sockfd, SHUT_RDWR);

  free(man);
}
