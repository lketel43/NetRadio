#include "message.h"

#include <stdio.h>

#include "minunit.h"

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


static char *all_tests()
{
  mu_run_test(test_msglen);
  
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
