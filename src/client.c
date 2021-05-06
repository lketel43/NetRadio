#include "utils.h"
#include "message.h"

#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 512

int recv_message_gest(int sock, int num_diff){
    int r;
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
            printf("ERREUR TYPE MESSAGE\n");
            return EXIT_FAILURE;
        }
    }
    return 1;
}

int communication_gest(int sock){
    int r;
    char mess[10];
    create_message(mess, LIST);
    send(sock, mess, strlen(mess), 0);
    printf("YOU : %s\n", mess);

    char buf[BUFSIZE];
    r = recv(sock, buf, BUFSIZE-1, 0);
    if(r <= 0){
        perror("recv");
        return EXIT_FAILURE;
    }
    buf[r] = '\0';
    enum msg_type type = get_msg_type(buf);

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

    r = recv_message_gest(sock, num_diff);
    if(!r)
        return EXIT_FAILURE;
    return 1;
}

int interact_with_manager(char *adress, int port){
    struct sockaddr_in adr_sock;
    int sock;
    
    adr_sock.sin_family = AF_INET;
    adr_sock.sin_port = htons(port);
    inet_aton(adress, &adr_sock.sin_addr);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("Erreur création socket\n");
        return EXIT_FAILURE;
    }

    int r = connect(sock, (struct sockaddr *)&adr_sock, sizeof(struct sockaddr_in));
    if(r == -1){
        printf("Erreur connect\n");
        return EXIT_FAILURE;
    }

    r = communication_gest(sock);
    if(!r)
        return EXIT_FAILURE;
    
    return 1;
}

int mess_diff(int sockd, char *pseudo_id){
    char mes_buf[BUFSIZE];
    char mess[BUFSIZE];
    if(*(fgets(mes_buf, BUFSIZE, stdin)) != '\n')
    {
        int i = 0;
        while(mes_buf[i] != '\n' && i < 140)
            i++;
        mes_buf[i] = '\0';
        create_message(mess, MESS, pseudo_id, mes_buf);
    } 
    else 
    {
        printf("Chaine vide avec le diff... Deconnexion du client\n");
        return EXIT_FAILURE;
    }
    send(sockd, mess, strlen(mess), 0);
    printf("YOU : %s\n", mess);
        
    char buf[10];
    int r = recv(sockd, buf, BUFSIZE-1, 0);
    if(r <= 0)
    {
        perror("recv");
        return EXIT_FAILURE;
    }
    buf[r] = '\0';
    printf("%s\n", buf);

    enum msg_type type_buf = get_msg_type(buf);
    if(type_buf != 0){
        printf("Erreur message retour du diff en post_mess\n");
        return EXIT_FAILURE;
    }
    return 1;
}

int last_diff(int sockd){
    int nb_mess, r;
    scanf("%d", &nb_mess);
    printf("Voici les %d derniers messages :\n", nb_mess);
    char mess[BUFSIZE];
    create_message(mess, LAST, nb_mess);
    send(sockd, mess, strlen(mess), 0);
    for(int i = 0; i <= nb_mess; i++){
        char buf[BUFSIZE];
        r = recv(sockd, buf, BUFSIZE-1, 0);
        buf[r] = '\0';
        printf("%s\n", buf);

        enum msg_type type_buf = get_msg_type(buf);
        if(type_buf == ENDM){
            break;
        }
        else if(type_buf != OLDM){
            printf("Erreur de type\n");
            return EXIT_FAILURE;
        }
    }
}

int communication_diff(int sockd, char *pseudo_id){
    int r;
    char mes_type[BUFSIZE];
    printf("Vous pouvez faire ces actions : MESS ou LAST\n");
    if(*(fgets(mes_type, BUFSIZE, stdin)) != '\n'){
        mes_type[4] = '\0';
        printf("Message envoyé : %s\n", mes_type);
    }else{
        printf("Chaine vide avec le diff... Deconnexion du client\n");
        return EXIT_FAILURE;
    }
    
    if(strcmp(mes_type, "MESS") == 0 || strcmp(mes_type, "mess") == 0)
    {
        printf("Vous pouvez dès à présent entrer votre message.\n");
        r = mess_diff(sockd, pseudo_id);
        if(!r)
            return EXIT_FAILURE;
    } 
    else if(strcmp(mes_type, "LAST") == 0 || strcmp(mes_type, "last") == 0)
    {
        printf("Entrez le nombre de message à diffuser.\n");
        r = last_diff(sockd);
        if(!r)
            return EXIT_FAILURE;
    } 
    else {
        printf("Mauvaise commande avec le diffuseur... Deconnexion du client\n");
        return EXIT_FAILURE;
    }
    return 1;
}

int interact_with_diff(char *adress_d, int port_d){
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
    
    char pseudo_id[BUFSIZE];
    printf("Pouvez entrer votre pseudo de 8 caractères svp ?\n");
    if(*(fgets(pseudo_id, BUFSIZE, stdin)) != '\n'){
        int i = 0;
        while(pseudo_id[i] != '\n' && i < 9)i++;
        pseudo_id[i] = '\0';
        printf("Nous vous confirmons votre pseudo : %s\n", pseudo_id);
    }

    int a = communication_diff(sockd, pseudo_id);
    if(!a)
        return EXIT_FAILURE;
    
    return 1;
}

int interact_with_multi(char *adress, int port){
    //struct sockaddr_in adr_sock;
    //int sock;
    //AMELIORER ET RELIRE MULTICAST + THREAD MULTICAST
    //FAIRE D'AUTRES POSSIBILITES D'UTILISATION DU CLIENT
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // allow multiple sockets to use the same PORT number
    //
    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
       perror("Reusing ADDR failed");
       return 1;
    }

        // set up destination address
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr.sin_port = htons(port);

    // bind to receive address
    //
    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    // use setsockopt() to request that the kernel join a multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(adress);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0){
        perror("setsockopt");
        return 1;
    }

    // now just enter a read-print loop
    //
    while (1) {
        char msgbuf[BUFSIZE];
        socklen_t addrlen = sizeof(addr);
        int nbytes = recvfrom(fd, msgbuf, BUFSIZE-1, 0, (struct sockaddr *) &addr, &addrlen);
        if (nbytes < 0) {
            perror("recvfrom");
            return 1;
        }
        msgbuf[nbytes] = '\0';
        printf("%s\n", msgbuf);
    }

    /*sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&adr_sock, 0, sizeof(struct sockaddr_in));
	adr_sock.sin_family = AF_INET;
	adr_sock.sin_port = htons(port);
	if(inet_aton(adress, &adr_sock.sin_addr) == 0){
        printf("Adress unvalid...\n");
        return EXIT_FAILURE;
    }
	char buff[2000];
	socklen_t len;
	while(1){
		char *snd = "";
		sendto(sock, snd, 1, 0, (struct sockaddr *) &adr_sock, (socklen_t)sizeof(struct sockaddr_in));
		int n = recvfrom(sock, buff, 2000, 0, (struct sockaddr *) &adr_sock, &len);
		buff[n] = '\0';
		printf("%s\n", buff);
	}*/

    /*printf("salut 1\n");
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    int ok = 1;
    int r = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &ok, sizeof(ok));
    adr_sock.sin_family=AF_INET;
    adr_sock.sin_port=htons(port);
    if(inet_aton(adress, &adr_sock.sin_addr) == 0){
        printf("Adress unvalid...\n");
        return EXIT_FAILURE;
    }
    printf("salut 2\n");
    r = bind(sock, (struct sockaddr *)&adr_sock, sizeof(struct sockaddr_in));
    printf("salut 3\n");
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("225.1.2.4");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    printf("salut 4\n");
    r = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if(r == -1)
    {
        perror("setsockopt");
        return EXIT_FAILURE;
    }
    printf("salut 5\n");
    char tampon[BUFSIZE];
    while(1){
        printf("salut 6\n");
        int rec = recv(sock, tampon, BUFSIZE-1, 0);
        tampon[rec] = '\0';
        printf("%s\n",tampon);
    }*/
    return 1;
}

int main(int argc, char *argv[]){
    int port_diff, port_multicast, port_manager;
    char adresse_diff[16];
    char adresse_multicast[16];
    char adresse_manager[16];
    
    if(argc!=5 && argc!=3){
        printf("Erreur : Il n'y a pas le bon nombre d'arguments...\n");
        printf("Rappel des formats clients :\n\t./client adresse_diff port_diff\n\t./client adresse_multicast port_multicast\n\t./client addresse_multicast port_multicast adresse_diff port_diff");
        return EXIT_FAILURE;
    }
    else if(argc==5){
        //strcpy(adresse_diff, argv[1]);
        //adresse_diff[15] = '\0';
        //port_diff = atoi(argv[2]);
        strcpy(adresse_multicast, argv[3]);
        adresse_multicast[15] = '\0';
        port_multicast = atoi(argv[4]);
        //interact_with_diff(adresse_diff, port_diff);
        interact_with_multi(adresse_multicast, port_multicast);
        //Fonction qui lance la connection au diffuseur et au multicast, 
        //lance les messages du multidiffuseur et attend une commande pour un diffuseur
    }
    else {
        strcpy(adresse_manager, argv[1]);
        adresse_manager[15] = '\0';
        port_manager = atoi(argv[2]);
        if(interact_with_manager(adresse_manager, port_manager) == -1){
            printf("Il y a des erreurs dans la communications entre le client et le gestionnaire...\n");
            return EXIT_FAILURE;
        }
        //Fonction qui lance la connection avec le gestionnaire puis qui attend des messages.
    }
    
    return 0;
}