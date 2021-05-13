#include "message.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "utils.h"
#include "minunit.h"

#define BUFSIZE 512

int tests_run = 0;

static char *test_msglen()
{
  mu_assert ("Error: msglen(ACKM) != 6", msglen(ACKM) == 6);
  mu_assert ("Error: msglen(LINB) != 9", msglen(LINB) == 9);
  mu_assert ("Error: msglen(DIFF) != 161", msglen(DIFF) == 161);
  mu_assert ("Error: msglen(ITEM) != 57", msglen(ITEM) == 57);
  mu_assert ("Error: msglen(LAST) != 10", msglen(LAST) == 10);

  mu_assert ("Error: msglen(123) != -1", msglen(123) == -1);
  mu_assert ("Error: msglen(0) != -1", msglen(0) == -1);
  mu_assert ("Error: msglen(-1) != -1", msglen(-1) == -1);
  
  return NULL;
}

static char *test_create_message ()
{
  char buf[BUFSIZE];
  
  mu_assert ("Error: create_message (buf, 123) != NULL", create_message (buf, 123) == NULL);
  mu_assert ("Error: create_message (buf, 0) != NULL", create_message (buf, 0) == NULL);
  mu_assert ("Error: create_message (buf, -1) != NULL", create_message (buf, -1) == NULL);

  
  create_message (buf, ACKM);
  mu_assert("Error: strcmp(buf, \"ACKM\\r\\n\") != 0", strcmp(buf, "ACKM\r\n") == 0);

  
  struct in_addr multi, tcp;
  inet_aton ("192.168.0.15", &tcp);
  inet_aton ("25.0.0.1", &multi);
  create_message (buf, REGI, "romain", &multi, 4242, &tcp, 5000);
  mu_assert("Error: strcmp(buf, \"REGI romain## 025.000.000.001 4242 192.168.000.015 5000\\r\\n\") != 0",
	    strcmp(buf, "REGI romain## 025.000.000.001 4242 192.168.000.015 5000\r\n") == 0);

  create_message (buf, ITEM, "romainbouarah", &multi, 19000, &tcp, 1234567);
  mu_assert("Error: strcmp(buf, \"ITEM romainbo 025.000.000.001 1900 192.168.000.015 1234\\r\\n\") != 0",
	    strcmp(buf, "ITEM romainbo 025.000.000.001 1900 192.168.000.015 1234\r\n") == 0);

  create_message (buf, DIFF, 5, "cli", "Lucas Ketels, Romain Bouarah et Matthieu Langdorph");
  mu_assert("Error: Wrong create_message for DIFF",
	    strcmp(buf, "DIFF 0005 cli##### Lucas Ketels, Romain Bouarah et Matthieu Langdorph"	\
		   "##########################################################################################"	\
		   "\r\n") == 0);

  create_message (buf, MESS, "lucasgangsta", "Lucas Ketels est un gangster!");
  mu_assert("Error: Wrong create_message for MESS",
	    strcmp(buf, "MESS lucasgan Lucas Ketels est un gangster!#####################" \
		   "##########################################################################################"	\
		   "\r\n") == 0);
  
  return NULL;
}


static char *all_tests()
{
  mu_run_test (test_msglen);
  mu_run_test (test_create_message);
  
  return 0;
}

int main(int argc, char **argv)
{
  char *result = all_tests();
  
  if (result != 0)
    {
      printf("%s\n", result);
    }
  else
    {
      printf("ALL TESTS PASSED\n");
    }
  
  printf("Tests run: %d\n", tests_run);
 
  return result != 0;
}
