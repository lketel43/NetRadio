#include "utils.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>

void print_sockaddr_in_info (const struct sockaddr_in *sockaddr)
{
  char ip[INET_ADDRSTRLEN];
  
  inet_ntop(AF_INET, &sockaddr->sin_addr, ip, INET_ADDRSTRLEN);

  printf("Port client    : %d\n", ntohs(sockaddr->sin_port));
  printf("IP client      : %s\n", ip);  
}


static void print_raw_char (char c)
{
  if (isprint(c))
    {
      printf("%c", c);
    }
  else
    {
      switch (c)
	{
	case '\n':
	  printf("\\n");
	  break;
	case '\t':
	  printf("\\t");
	  break;
	case '\r':
	  printf("\\r");
	  break;
	case '\v':
	  printf("\\v");
	  break;
	default:
	  printf("%02x", c);
	  break;
	}
    }
}

void print_raw_string (const char *str)
{
  for (int i=0; str[i] != '\0'; i++)
    {
      print_raw_char (str[i]);
    }
}
