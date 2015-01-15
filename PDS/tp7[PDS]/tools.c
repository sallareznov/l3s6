/* ------------------------------
   $Id: tools.c,v 1.2 2007/04/04 15:18:50 marquet Exp $
   ------------------------------------------------------------

   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Outils divers
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "tools.h"

int verbose = 0;

void nyi_(const char*funcname, const char *s) {
    fprintf(stderr, "mtcs: (%s) %s not yet implemented, exiting\n",
	    funcname, s);
    exit(EXIT_FAILURE);
}

void pgrs_(const char*funcname, const char *s) {
    if (verbose)
	fprintf(stderr, "(%lu) %s(): %s\n",
		(unsigned long) pthread_self(), funcname, s);
}

