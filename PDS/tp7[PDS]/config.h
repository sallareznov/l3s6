/* ------------------------------
   $Id: config.h,v 1.2 2005/04/07 12:56:45 marquet Exp $
   ------------------------------------------------------------

   mtcs - a multithreaded chat serveur
   Philippe Marquet, Mar 2005

   Configuration generale
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DEFAULT_PORT	8011

#define MAX_CONNECTION 	32
#define MAX_PENDING   	10
#define MAX_TENTATIVE 	10
#define MAX_BUFFER	1024

#define SIGSTAT		SIGUSR1

#endif
