#include "utils.h"
#include "message.h"

#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 512


int interact_with_manager(char *adress, int port){
    struct sockaddr_in adr_sock;
    int sock;
    /*if(handle_connection_with_manager(&adr_sock, &sock, port, adress)!=1){
        printf("Problem of connection\n");
        return EXIT_FAILURE;
    }*/
    adr_sock.sin_family = AF_INET;
    adr_sock.sin_port = htons(port);
    inet_aton(adress, &adr_sock.sin_addr);
    /*if(inet_aton(adress, &adr_sock->sin_addr) == 0){
        printf("Adress unvalid...\n");
        return EXIT_FAILURE;
    }*/

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("Erreur lors de la création de la socket...\n");
        return EXIT_FAILURE;
    }

    int r = connect(sock, (struct sockaddr *)&adr_sock, sizeof(struct sockaddr_in));
    if(r == -1){
        printf("Erreur connect\n");
        return EXIT_FAILURE;
    }

    char *mess = "LIST\t\n"/*malloc(10)*/;
    //create_message(mess, LIST);
    send(sock, mess, strlen(mess), 0);
    printf("YOU : %s\n", mess);

    char buf[BUFSIZE];
    r = recv(sock, buf, BUFSIZE-1, 0);
    if(r <= 0){
        perror("recv");
        return EXIT_FAILURE;
    }
    buf[r] = '\0';
    printf("%s\n", buf);
    enum msg_type type = get_msg_type(buf);
    printf("%d\n", type);
    int num_diff;
    char numdi[3];
    if(type == LINB) {
        numdi[0] = buf[5];
        numdi[1] = buf[6];
        numdi[2] = '\0';
        num_diff = atoi(numdi);
    }
    else {
        printf("ERREUR TYPE MESSAGE\n");
        return EXIT_FAILURE;
    }
    printf("Manager : %s\n", buf);

    for(int  i = 0; i < num_diff; i++){
        char buf_diff[BUFSIZE];
        r = recv(sock, buf_diff, BUFSIZE-1, 0);
        if(r <= 0){
            perror("recv");
            return EXIT_FAILURE;
        }
        buf_diff[r] = '\0';

        enum msg_type type = get_msg_type(buf_diff);
        printf("Manager : %s\n", buf_diff);
        
        if(type != ITEM){
            printf("ERREUR MAUVAIS TYPE DE MESSAGES\n");
            return EXIT_FAILURE;
        }
    }
    return 1;
}

int interact_with_diff_and_multi(char *adress_d, int port_d, char *adress_m, int port_m){
    struct sockaddr_in adr_sockd;
    int sockd;
    
    adr_sockd.sin_family = AF_INET;
    adr_sockd.sin_port = htons(port_d);
    if(inet_aton(adress_d, &adr_sockd.sin_addr) == 0){
        printf("Adress unvalid...\n");
        return EXIT_FAILURE;
    }

    sockd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockd == -1){
        printf("Erreur lors de la création de la socket...\n");
        return EXIT_FAILURE;
    }

    int r = connect(sockd, (struct sockaddr *)&adr_sockd, sizeof(struct sockaddr_in));
    if(r == -1){
        printf("Erreur connect\n");
        return EXIT_FAILURE;
    }

    char pseudo_id[9];
    printf("Pouvez entrer votre pseudo de 8 caractères svp ?\n");
    if(*(fgets(pseudo_id, 8, stdin)) != '\n'){
        pseudo_id[8] = '\0';
        printf("Nous vous confirmons votre pseudo : %s\n", pseudo_id);
    }



    char mes_type[5];
    printf("Vous pouvez faire ces actions : MESS ou LAST\n");
    if(*(fgets(mes_type, 4, stdin)) != '\n'){
        mes_type[4] = '\0';
    }else{
        printf("Chaine vide avec le diff... Deconnexion du client\n");
        return EXIT_FAILURE;
    }

    if(strcmp(mes_type, "MESS") == 0 || strcmp(mes_type, "mess") == 0){
        printf("Vous pouvez dès à présent entrer votre message.\n");
        char mes_buf[141];
        char mess[160];
        if(*(fgets(mes_buf, 140, stdin)) != '\n'){
            mes_buf[140] = '\0';
            create_message(mess, MESS, pseudo_id, mes_buf);
        } 
        else {
            printf("Chaine vide avec le diff... Deconnexion du client\n");
            return EXIT_FAILURE;
        }
        send(sockd, mess, strlen(mess), 0);
        printf("YOU : %s\n", mess);

        char buf[10];
        r = recv(sockd, buf, BUFSIZE-1, 0);
        if(r <= 0){
            perror("recv");
            return EXIT_FAILURE;
        }
        buf[r] = '\0';
        printf("%s\n", buf);
        if(strcmp(buf, "ACKM\t\n") != 0){
            printf("Erreur message retour du diff en post_mess\n");
            return EXIT_FAILURE;
        }
    } 
    else if(strcmp(mes_type, "LAST") == 0 || strcmp(mes_type, "last") == 0){

    } 
    else {
        printf("Mauvaise commande avec le diffuseur... Deconnexion du client\n");
        return EXIT_FAILURE;
    }
    //create_message(mess, LIST);
    /*send(sock, mess, strlen(mess), 0);
    printf("YOU : %s\n", mess);

    char buf[BUFSIZE];
    r = recv(sock, buf, BUFSIZE-1, 0);
    if(r <= 0){
        perror("recv");
        return EXIT_FAILURE;
    }
    buf[r] = '\0';
    printf("%s\n", buf);
    enum msg_type type = get_msg_type(buf);
    printf("%d\n", type);
    int num_diff;
    char numdi[3];
    if(type == LINB) {
        numdi[0] = buf[5];
        numdi[1] = buf[6];
        numdi[2] = '\0';
        num_diff = atoi(numdi);
    }
    else {
        printf("ERREUR TYPE MESSAGE\n");
        return EXIT_FAILURE;
    }
    printf("Manager : %s\n", buf);

    for(int  i = 0; i < num_diff; i++){
        char buf_diff[BUFSIZE];
        r = recv(sock, buf_diff, BUFSIZE-1, 0);
        if(r <= 0){
            perror("recv");
            return EXIT_FAILURE;
        }
        buf_diff[r] = '\0';

        enum msg_type type = get_msg_type(buf_diff);
        printf("Manager : %s\n", buf_diff);
        
        if(type != ITEM){
            printf("ERREUR MAUVAIS TYPE DE MESSAGES\n");
            return EXIT_FAILURE;
        }
    }*/
    return 1;
}

int main(int argc, char *argv[]){
    int port_diff, port_multicast, port_manager;
    char adresse_diff[16];
    char adresse_multicast[16];
    char adresse_manager[16];
    printf("Salut1\n");
    if(argc!=5 && argc!=3){
        printf("Erreur : Il n'y a pas le bon nombre d'arguments...\n");
        printf("Rappel des formats clients :\n\t./client adresse_diff port_diff\n\t./client adresse_multicast port_multicast\n\t./client addresse_multicast port_multicast adresse_diff port_diff");
        return EXIT_FAILURE;
    }
    else if(argc==5){
        strcpy(adresse_diff, argv[1]);
        adresse_diff[15] = '\0';
        port_diff = atoi(argv[2]);
        strcpy(adresse_multicast, argv[3]);
        adresse_multicast[15] = '\0';
        port_multicast = atoi(argv[4]);
        //Fonction qui lance la connection au diffuseur et au multicast, 
        //lance les messages du multidiffuseur et attend une commande pour un diffuseur
    }
    else {
        strcpy(adresse_manager, argv[1]);
        adresse_manager[15] = '\0';
        printf("adresse : %s\n", adresse_manager);
        port_manager = atoi(argv[2]);
        printf("Salut %d\n", port_manager);
        if(interact_with_manager(adresse_manager, port_manager) == -1){
            printf("Il y a des erreurs dans la communications entre le client et le gestionnaire...\n");
            return EXIT_FAILURE;
        }
        //Fonction qui lance la connection avec le gestionnaire puis qui attend des messages.
    }
    
    return 0;
}