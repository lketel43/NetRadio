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


int main(int argc, char *argv[]){
    int port_diff, port_multicast, port_manager;
    char *adresse_diff;
    char *adresse_multicast;
    char *adresse_manager;
    if(argc!=4 && argc!=3){
        printf("Erreur : Il n'y a pas le bon nombre d'arguments...\n");
        printf("Rappel des formats clients :\n\t./client adresse_diff port_diff\n\t./client adresse_multicast port_multicast\n\t./client addresse_multicast port_multicast adresse_diff port_diff");
        return EXIT_FAILURE;
    }
    if(argc==4 && strcmp(argv[1], "d")==0){
        strcpy(adresse_diff, argv[2]);
        port_diff = atoi(argv[3]);
    }
    else if(argc==4 && strcmp(argv[1], "m")==0){
        strcpy(adresse_multicast, argv[2]);
        port_multicast = atoi(argv[3]);
    }
    else {
        int i, j;
        if(argc==3){ i = 1; j = 2; }
        else { i = 2; j = 3; }
        strcpy(adresse_manager, argv[i]);
        port_manager = atoi(argv[j]);
    }
    

    struct sockaddr_in adr_sock;
    adr_sock.sin_family = AF_INET;
    adr_sock.sin_port = htons(port_manager);
    if(inet_aton(adresse_manager, &adr_sock.sin_addr) == 0){
        printf("Adress unvalid...\n");
        return EXIT_FAILURE;
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("Erreur lors de la création de la socket...\n");
        return EXIT_FAILURE;
    }

    int r  = connect(sock, (struct sockaddr *)&adr_sock, sizeof(struct sockaddr_in));
    if(r == -1){
        printf("Erreur connect\n");
        return EXIT_FAILURE;
    }

    char *mess = malloc(10);
    create_message(mess, LIST);
    send(sock, mess, msglen(LIST), 0);

    char buf[BUFSIZE];
    r = recv(sock, buf, BUFSIZE-1, 0);
    if(r <= 0){
        perror("recv");
        return EXIT_FAILURE;
    }
    buf[r] = '\0';
    

    char * type = get_msg_type(buf);
    int num_diff;
    switch(type){
        case LINB : 
            num_diff = atoi(buf[5]);
        default :
            printf("ERREUR TYPE MESSAGE\n");
            return EXIT_FAILURE;
    }

    for(int  i = 0; i < num_diff; i++){
        // recv ITEM
    }



    return 0;
}