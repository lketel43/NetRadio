#include "message.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

size_t msglen (enum msg_type type)
{
  size_t len = MSG_TYPE_SIZE + MSG_END_SIZE;
  
  switch (type)
    {
    case DIFF:
    case OLDM:
      return len + MSG_NUM_MESS_SIZE + MSG_ID_SIZE + MSG_MESS_SIZE + 3; // 161

    case MESS:
      return len + MSG_ID_SIZE + MSG_MESS_SIZE + 2; // 156

    case ITEM:
    case REGI:
      return len + MSG_ID_SIZE + 2*MSG_IP_SIZE + 2*MSG_PORT_SIZE + 5; // 57

    case LAST:
      return len + MSG_NB_MESS_SIZE + 1; // 10

    case LINB:
      return len + MSG_NUM_DIFF_SIZE + 1; // 9

    default: //ACKM, ENDM, IMOK, LIST, RENO, REOK, RUOK
      return len; // 6
    } 
}

const char* msg_type_to_str (enum msg_type type)
{
  switch (type)
    {
    case ACKM:
      return "ACKM";
    case DIFF:
      return "DIFF";
    case ENDM:
      return "ENDM";
    case IMOK:
      return "IMOK";
    case ITEM:
      return "ITEM";
    case LAST:
      return "LAST";
    case LINB:
      return "LINB";
    case LIST:
      return "LIST";
    case MESS:
      return "MESS";
    case OLDM:
      return "OLDM";
    case REGI:
      return "REGI";
    case RENO:
      return "RENO";
    case REOK:
      return "REOK";
    case RUOK:
      return "RUOK";
    }

  return NULL;
}

static void set_msg_type (char *buf, enum msg_type type)
{
  strcpy (buf, msg_type_to_str(type));
}

static void set_msg_end (char *buf)
{
  strcpy (buf, MSG_END);
}

char* create_message (char* buf, enum msg_type type, ...)
{
  va_list args;
  char *ret;
  size_t len;

  
  len = msglen (type);
  
  if (buf == NULL)
    {
      ret = malloc (len + 1); // +1 pour le '\0' de fin
      assert (ret);
    }
  else
    {
      ret = buf;
    }

  va_start (args, type);

  
  set_msg_type (ret, type);  

  switch (type)
    {
      // TODO : Traiter ces cas
    case DIFF:
    case OLDM:    
    case MESS:
    case ITEM:
    case REGI:
    case LAST:
    case LINB:      
      if(!buf)
	free(ret);
      va_end (args);
      return NULL;
            
    default: //ACKM, ENDM, IMOK, LIST, RENO, REOK, RUOK
      break;
    } 

  
  va_end (args);

  set_msg_end (ret + len - MSG_END_SIZE);
  
  return ret;
}
