#include "utils.h"
#include "message.h"

#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[]){
    int port_diff, port_multicast;
    char *adresse_diff;
    char *adresse_multicast;
    if(argc != 5 && argc!=3){
        printf("Erreur : Il n'y a pas le bon nombre d'arguments...\n");
        printf("Rappel des formats clients :\n\t./client adresse_diff port_diff\n\t./client adresse_multicast port_multicast\n\t./client addresse_multicast port_multicast adresse_diff port_diff");
        return EXIT_FAILURE;
    }

    struct sockaddr_in adr_sock;
    adr_sock.sin_family = AF_INET;
    adr_sock.sin_port = htons(port_diff);
    if(inet_aton(adresse_diff, &adr_sock.sin_addr) == 0){
        printf("Adress unvalid...");
        return EXIT_FAILURE;
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("Erreur lors de la création de la socket...\n");
        return EXIT_FAILURE;
    }
    

    return 0;
}