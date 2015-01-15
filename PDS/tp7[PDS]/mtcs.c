/* ------------------------------
   $Id: mtcs.c,v 1.3 2007/04/04 15:24:32 marquet Exp $
   ------------------------------------------------------------

   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Point d'entree du programme
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tools.h"
#include "config.h"
#include "cnct.h"
#include "stats.h"

static void usage() {
    printf("Usage: mtcs [-hv]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");

    exit(EXIT_FAILURE);    
}

/* Retourne une socket d'ecoute sur le port passé en argument
   (ce numéro de port peut etre modifie si il etait deja utilise) */
static int open_socket(int *port) {
    int skfd;			/* socket */
    struct sockaddr_in name;
    int try=0;
    
    pgrs_in();

    /*
     * "formules magiques" qui vont bien
     */

    /* incantation créant une socket */
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
	perror("ouverture socket ecoute");
	exit(EXIT_FAILURE);
    }
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(*port);
   
    pgrs("socket creee");

    /* incantation liant la socket au port souhaite */
    while (try<MAX_TENTATIVE && 
	   bind(skfd, (struct sockaddr*)&name, sizeof(name)) < 0) {
	try++;
	(*port)++;
	name.sin_port = htons(*port);
    }
    if (try == MAX_TENTATIVE) {
	perror("bind de la socket");
	exit(EXIT_FAILURE);
    }

    pgrs("binding effectue");

    /* incantation initialisant la socket en ecoute */
    if (listen(skfd, MAX_PENDING) != 0) {
	perror("listen");
	exit(EXIT_FAILURE);
    }

    pgrs("listen ok");
    pgrs_out();

    return skfd;
}

/* Notification d'un refus de connexion sur la socket */
static void noconnect(int socket) {
    const char NOCONNECT[] = "mtcs : connexion refusee, desole\n";
    
    pgrs_in();
    write(socket, NOCONNECT, strlen(NOCONNECT));
    close(socket);
    pgrs_out();
}

/* Etablissement, par le serveur, de la connexion d'un client */
static int create_cnct(int skfd) {
    int skcnct; 		/* socket de connection */
    int status;
    
    pgrs_in();

    /* on recupere la connexion */
    skcnct = accept(skfd, NULL, NULL);
    if (skcnct < 0) {
	perror("ne peut etablir la connexion, accept impossible");
	exit(EXIT_FAILURE);
    }

    /* un thread fils pour gérer cette connection */
    status = manage_cnct(skcnct);
    if (status < 0) {
	noconnect(skcnct);
	pgrs_out();
	return -1;
    }

    pgrs_out();
    return 0;
}

int main(int argc, char *argv[]) {
    int listen;
    int port = DEFAULT_PORT;
    
    char c;

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hv")) != EOF) {
	switch (c) {
	    case 'h':		/* print help message */
		usage();
		break;
	    case 'v':		/* emit additional diagnostic info */
		verbose = 1;
		break;
	    default:
		usage();
	}
    }
    
    pgrs("serveur initialization");
    listen = open_socket(&port);    
    printf("Server (pid %d) open on port %d\n", getpid(), port);
  
    pgrs("server loop");
    init_stats();
    while (1) {
	create_cnct(listen);
    }

    exit(EXIT_FAILURE);
}
