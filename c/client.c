/* client.c */
#include "client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "message.h"

#define BUFSIZE 512

// TODO : close en cas d'erreur
// TODO : free en cas d'erreur
char** get_streamer_list (const char *manager_ip, int manager_port, int *size)
{
  struct sockaddr_in adr_sock;
  int sock;
    
  adr_sock.sin_family = AF_INET;
  adr_sock.sin_port = htons(manager_port);
  if (inet_aton(manager_ip, &adr_sock.sin_addr) == 0)
    {
      return NULL;
    }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    {
      perror("get_streamer_list: socket");
      return NULL;
    }

  int r = connect(sock, (struct sockaddr *)&adr_sock, sizeof(struct sockaddr_in));
  if(r == -1)
    {
      perror("get_streamer_list: connect");
      return NULL;
    }

  char mess[BUFSIZE];
  create_message(mess, LIST);
  r = send(sock, mess, msglen(LIST), 0);
  if(r == -1)
    {
      perror("get_streamer_list: send");
      return NULL;
    }

  char buf[BUFSIZE];
  r = recv(sock, buf, BUFSIZE-1, 0);
  if(r < 0)
    {
      perror("get_streamer_list: recv");
      return NULL;
    }
  buf[r] = '\0';
  
  msg_type type = get_msg_type(buf);
  char numdi[3];
  if(type == LINB)
    {
      numdi[0] = buf[5];
      numdi[1] = buf[6];
      numdi[2] = '\0';
      *size = atoi(numdi);
    }
  else
    {
      return NULL;
    }
  
  char **list_diff = malloc((*size)*sizeof(char *));
  for (int i = 0; i < *size; i++)
    {
      list_diff[i] = malloc(msglen(ITEM) + 1);
      r = recv(sock, list_diff[i], msglen(ITEM), 0);
      if(r < 0)
	{
	  perror("get_streamer_list: recv");	  
	  return NULL;
	}
      list_diff[i][r] = '\0';

      msg_type type = get_msg_type(list_diff[i]);
      if(type != ITEM)
	{
	  return NULL;
	}
    }

  shutdown (sock, SHUT_RDWR);
  return list_diff;
}

int send_mess_to_streamer (const char* streamer_ip, int streamer_port, const char *client_id, const char *client_msg)
{
  struct sockaddr_in adr_sockd;
  int sockd;
    
  adr_sockd.sin_family = AF_INET;
  adr_sockd.sin_port = htons(streamer_port);
  if (inet_aton(streamer_ip, &adr_sockd.sin_addr) == 0)
    {
      return -1;
    }
    
  sockd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockd == -1)
    {
      perror ("send_mess_to_streamer: socket");
      return -1;
    }
    
  int r = connect(sockd, (struct sockaddr *)&adr_sockd, sizeof(struct sockaddr_in));
  if (r == -1)
    {
      perror ("send_mess_to_streamer: connect");
      return -1;
    }

  char mess[MESS_LEN + 1];
  create_message(mess, MESS, client_id, client_msg);
  r = send(sockd, mess, MESS_LEN, 0);
  if (r == -1)
    {
      perror ("send_mess_to_streamer: send");
      return -1;
    }
        
  char ackm[ACKM_LEN];
  r = recv(sockd, ackm, ACKM_LEN, 0);
  if (r < 0)
    {
      perror ("send_mess_to_streamer: recv");
      return -1;
    }

  msg_type type_buf = get_msg_type(ackm);
  if (type_buf != ACKM)
    {      
      return -1;
    }
    
  shutdown (sockd, SHUT_RDWR);
  return 0;
}

char** get_last_mess (const char* streamer_ip, int streamer_port, int *nb_mess)
{
  struct sockaddr_in adr_sockd;
  int sockd;
    
  adr_sockd.sin_family = AF_INET;
  adr_sockd.sin_port = htons(streamer_port);
  if (inet_aton(streamer_ip, &adr_sockd.sin_addr) == 0)
    {
      return NULL;
    }
    
  sockd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockd == -1)
    {
      perror ("get_last_mess: socket");
      return NULL;
    }
    
  int r = connect(sockd, (struct sockaddr *)&adr_sockd, sizeof(struct sockaddr_in));
  if (r == -1)
    {
      perror ("get_last_mess: connect");
      return NULL;
    }    
  
  char last[LAST_LEN+1];
  create_message(last, LAST, *nb_mess);
  r = send(sockd, last, LAST_LEN, 0);
  if (r == -1)
    {
      perror("get_last_mess: send");
      return NULL;
    }

  char **last_mess = malloc((*nb_mess)*sizeof(char *));
  
  for(int i = 0; i <= *nb_mess; i++)
    {
      last_mess[i] = malloc(msglen(OLDM) + 1);
      
      r = recv(sockd, last_mess[i], msglen(OLDM), 0);
      last_mess[i][r] = '\0';

      msg_type type_buf = get_msg_type(last_mess[i]);
      if(type_buf == ENDM)
	{
	  free(last_mess[i]);
	  *nb_mess = i;
	  break;
	}
      else if (type_buf != OLDM)
	{
	  return NULL;
	}
    }

  shutdown (sockd, SHUT_RDWR);
  return last_mess;
}
