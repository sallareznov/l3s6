/* ------------------------------
   $Id: sighandlers.h,v 1.1 2005/03/17 13:00:46 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#ifndef _SIGHANDLERS_H_
#define _SIGHANDLERS_H_

typedef void handler_t(int);

extern int signal_wrapper(int signum, handler_t *handler);
extern void sigchld_handler(int sig);
extern void sigint_handler(int sig);
extern void sigtstp_handler(int sig);
extern void sigquit_handler(int sig);

#endif
