#include "message.h"

#include <stddef.h>

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
