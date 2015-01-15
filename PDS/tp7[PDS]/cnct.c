/* ------------------------------
   $Id: cnct.c,v 1.3 2007/04/04 15:18:50 marquet Exp $
   ------------------------------------------------------------

   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Gestion de la connexion d'un client par un thread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>

#include "config.h"
#include "tools.h"
#include "cnct.h"
#include "stats.h"

/* Gestion des sockets */
static int sockets[MAX_CONNECTION]; /* tableau initialisé a zero */

static pthread_mutex_t verrou_connexion = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t verrou_ecriture = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t verrou_deconnexion = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t verrou_stats = PTHREAD_MUTEX_INITIALIZER;

static void add_socket(int fd) {
    int i;
    
    pgrs_in();
    for (i=0; i<MAX_CONNECTION; i++) {
	if (sockets[i] == 0) {
	    sockets[i] = fd;
	    break;
	}
    }
    assert(i!=MAX_CONNECTION);
    pgrs_out();
}

static void del_socket(int fd) {
    int i;
    
    pgrs_in();
    for (i=0; i<MAX_CONNECTION; i++) {
	if (sockets[i] == fd) {
	    sockets[i] = 0;
	    break;
	}
    }
    assert(i!=MAX_CONNECTION);
    pgrs_out();
}

static void signal_handler(int sig) {
	pgrs_in();
	pgrs("affichage des statistiques...");
	print_stats();
	pgrs("...affichage des statistiques");
	pgrs_out();
}

/* Un client  */
static void repeater(int sckt) {
    char buf[MAX_BUFFER];
    int nbc, i;
    struct sigaction sa;
    const char WELCOME[] = "mtcs : bienvenue\n";

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGSTAT,&sa,(struct sigaction *) 0);

    pgrs_in();
    write(sckt, WELCOME, strlen(WELCOME));

    pgrs("enregistrement d'une socket");
    pthread_mutex_lock(&verrou_connexion);
    add_socket(sckt);
    pthread_mutex_lock(&verrou_stats);
    update_nb_clients(TRUE);
    update_max_clients();
    pthread_mutex_unlock(&verrou_stats);
    pthread_mutex_unlock(&verrou_connexion);
    
    while (1) {
	pgrs("attente read");
	nbc = read(sckt, buf, MAX_BUFFER);
	if (nbc <= 0) {
	    pgrs("fin lecture client");
	    pgrs("desenregistrement d'une socket");
	    pthread_mutex_lock(&verrou_deconnexion);
	    del_socket(sckt);
	    pthread_mutex_lock(&verrou_stats);
	    update_nb_clients(FALSE);
	    pthread_mutex_unlock(&verrou_stats);
	    pthread_mutex_unlock(&verrou_deconnexion);

	    close(sckt);
            pgrs_out();
	    return;
	}
	pgrs("boucle ecriture");
	pthread_mutex_lock(&verrou_ecriture);
	for(i=0; i<MAX_CONNECTION; i++) {
	    if (sockets[i]) {
			write(sockets[i], buf, nbc);
		}
	}
	pthread_mutex_lock(&verrou_stats);
	update_snt_lines();
	update_rcv_lines();
	update_max_rcv_lines_by_client();
	update_max_snt_lines_to_client();
	pthread_mutex_unlock(&verrou_stats);
	pthread_mutex_unlock(&verrou_ecriture);
	pgrs("fin boucle ecriture");
    }
}

void *pt_repeater(void *arg) {
	int n;

	n = (int) arg;
	repeater(n);
	return NULL;
}

int manage_cnct(int fd) {
	pthread_t t;
	int status;

    pgrs_in();

	status = pthread_create(&t,NULL,pt_repeater,(void *) fd);
	assert(!status);
	status = pthread_detach(t);
	assert(!status);

    pgrs_out();
    return 0;
}
