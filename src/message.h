/* message.h -- manipulation des messages
 */

#ifndef MESSAGE_H
#define MESSAGE_H

// Taille des caractéristiques d'un message en octets
#define MSG_ID_SIZE 8
#define MSG_IP_SIZE 15
#define MSG_MESS_SIZE 140
#define MSG_NB_MESS_SIZE 3
#define MSG_NUM_DIFF_SIZE 2
#define MSG_NUM_MESS_SIZE 4
#define MSG_PORT_SIZE 4
#define MSG_TYPE_SIZE 4

#define MSG_END "\r\n"
#define MSG_END_SIZE 2

// Types possibles des messages
enum msg_type
  {
    ACKM,
    DIFF,
    ENDM,
    IMOK,
    ITEM,
    LAST,
    LINB,
    LIST,
    MESS,
    OLDM,
    REGI,
    RENO,
    REOK,
    RUOK
  };

#endif
