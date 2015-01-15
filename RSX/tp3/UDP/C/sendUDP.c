#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

static int msocket;
static struct sockaddr_in adresse_serveur;
static unsigned short int port;

void envoieMessage(char *message) {
	sendto(msocket,message,strlen(message),0,(const struct sockaddr *) &adresse_serveur,sizeof(adresse_serveur));
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		fprintf(stderr,"Usage : ./sendUDP hostname port message\n");
		fprintf(stderr,"	- hostname[char *] : nom de la machine destinataire du message\n");
		fprintf(stderr,"	- port[int] : le numéro de port\n");
		fprintf(stderr,"	- message[char *] : le message à envoyer\n");
		fprintf(stderr,"Fonction : Ce programme envoie un message à la machine hostname\n");
		fprintf(stderr,"	   sur le port UDP port\n");
		return EXIT_FAILURE;
	}
	else {
		msocket = socket(AF_INET,SOCK_DGRAM,0);
		port = atoi(argv[2]);
		adresse_serveur.sin_family = AF_INET;
		adresse_serveur.sin_port = htons(port);
		inet_aton(argv[1], &adresse_serveur.sin_addr);
		connect(msocket,(const struct sockaddr *)&adresse_serveur,sizeof(adresse_serveur));
		envoieMessage(argv[3]);
		return EXIT_SUCCESS;
	}
}
