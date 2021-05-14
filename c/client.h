/* client.h -- communication du client avec les autres entités
 */

#ifndef CLIENT_H
#define CLIENT_H

/* 
 * Envoie un message de la forme [MESS ...] à un diffuseur
 * 
 * STREAMER_IP contient une chaîne de caractères représentant l'adresse IP du diffuseur, 
 * STREAMER_PORT est le port de communication du diffuseur
 * CLIENT_ID est l'id sous lequel on envoie notre message
 * CLIENT_MSG est le contenu du message
 *
 * Renvoie -1 s'il y a des erreurs; 0 sinon 
 */
int send_mess_to_streamer (const char* streamer_ip, int streamer_port, const char *client_id, const char *client_msg);

/* 
 * Envoie un message de la forme [LAST ...] à un diffuseur
 * 
 * STREAMER_IP contient une chaîne de caractères représentant l'adresse IP du diffuseur, 
 * STREAMER_PORT est le port de communication du diffuseur
 * NB_MESS un pointeur vers un entier indiquant le nombre de messages qu'on souhaite obtenir
 *
 * Renvoie :
 * * Si tout se passe bien un tableau (alloué avec malloc) de chaîne de caractères de la forme [OLDM ...], 
 *   NB_MESS est modifié à la taille du tableau 
 * * NULL si erreur et NB_MESS est...
 */
char** get_last_mess (const char* streamer_ip, int streamer_port, int *nb_mess);

/* 
 * Envoie un message de la forme [LIST] à un diffuseur
 * 
 * MANAGER_IP contient une chaîne de caractères représentant l'adresse IP du gestionnaire
 * MANAGER_PORT est le port de communication du gestionnaire
 * SIZE pointeur vers un entier
 * 
 * Renvoie :
 * * Si tout se passe bien un tableau (alloué avec malloc) de chaîne de caractères de la forme [ITEM ...], 
 *   SIZE est modifié à la taille du tableau 
 * * NULL si erreur et SIZE est...
 */
char** get_streamer_list (const char *manager_ip, int manager_port, int *size);


#endif
