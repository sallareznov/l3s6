 #include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int msocket;
struct sockaddr_in address;
unsigned short int port;

void envoieMessage(char *message) {
	sendto(msocket,message,strlen(message),0,(const struct sockaddr *) &address,sizeof(address));
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		fprintf(stderr,"Usage : ./sendUDP hostname port message\n");
		fprintf(stderr,"		hostname[char *] : nom de la machine destinataire du message\n");
		fprintf(stderr,"		port[int] : le numéro de port\n");
		fprintf(stderr,"		message[char *] : le message\n");
		return EXIT_FAILURE;
	}
	else {
		msocket = socket(AF_INET,SOCK_DGRAM,0);
		port = atoi(argv[2]);
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		inet_aton(argv[1], &address.sin_addr);
		envoieMessage(argv[3]);
		return EXIT_SUCCESS;
	}
}
