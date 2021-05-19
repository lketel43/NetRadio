/* message.h -- manipulation des messages */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Taille des caracteristiques d'un message en octets
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

#define DEFAULT_CHAR '#'
#define DEFAULT_INT '0'

#define SEPARATOR_CHAR ' '

// Taille des messages
#define MAX_MSG_SIZE 161

#define ACKM_LEN 6
#define REGI_LEN 57
#define LAST_LEN 10
#define LINB_LEN 9
#define LIST_LEN 6
#define MESS_LEN 156

// Types possibles des messages
typedef int msg_type;

#define ACKM 1
#define DIFF 2
#define ENDM 3
#define IMOK 4
#define ITEM 5
#define LAST 6
#define LINB 7
#define LIST 8
#define MESS 9
#define OLDM 10
#define REGI 11
#define RENO 12 
#define REOK 13 
#define RUOK 14

/**
 * Donne le taille en octet nécessaire pour stocker un message d'un certain type 
 */
int msglen (msg_type type);

/**
 * Donne la représentation en string d'un msg_type
 */
const char* msg_type_to_str (msg_type type);

/**
 * Crée un message avec ses caractéristiques
 * 
 * * ACKM, ENDM, IMOK, LIST, RENO, REOK, RUOK : Aucun argument nécessaire
 * * LAST, LINB : int
 * * DIFF, OLDM : int, char*, char*
 * * MESS : char*, char*
 * * ITEM, REGI : char*, struct in_addr*, int, struct in_addr*, int
 *
 * Si `buf == NULL` alors la fonction renvoit un message alloué avec un malloc
 * Sinon le résultat est écrit dans `buf`
 *
 * Attention, `buf` doit être assez grand pour contenir le message + le caractère '\0'
 */
char* create_message (char* buf, msg_type type, ...);

/**
 * Donne le type de message d'un message
 * 
 * On entre un message en paramètre commencant par l'un des 14 types.
*/
msg_type get_msg_type (const char *msg);

/**
 * Vérifie si un message est au bon format
 */
bool verify_msg (const char *msg);

#endif
