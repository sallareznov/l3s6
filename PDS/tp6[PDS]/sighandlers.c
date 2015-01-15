/* ------------------------------
   $Id: sighandlers.c,v 1.1 2005/03/17 13:00:46 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#include "jobs.h"
#include "common.h"
#include "sighandlers.h"

/*
 * Signal - wrapper for the sigaction function
 */
int signal_wrapper(int signum, handler_t *handler) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    sigaction(signum,&action,NULL);
    (verbose) ? fprintf(stdout,"signal_wrapper: new handler for the signal %d\n",signum) : 0;
    
    return 0;
}


/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children
 */
void sigchld_handler(int sig) {
    pid_t pid;
    int status;
    struct job_t *job;

    (verbose) ? fprintf(stdout,"sigchld_handler: entering\n") : 0;

    while ((pid = waitpid(-1,&status,WNOHANG | WUNTRACED)) > 0) {

        job = jobs_getjobpid(pid);

        if (WIFSTOPPED(status)) {
            job->jb_state = ST;
            (verbose) ? fprintf(stdout,"the program %d has been stopped\n",pid) : 0;
        }

        if (WIFSIGNALED(status)) {
            jobs_deletejob(pid);
            (verbose) ? fprintf(stdout,"the program %d terminated anormally with the signal %d\n",pid,status) : 0;
        }

        if (WIFEXITED(status)) {
            jobs_deletejob(pid);
            (verbose) ? fprintf(stdout,"the program %d terminated normally\n",pid) : 0;
        }
    
    }

    (verbose) ? fprintf(stdout,"sigchld_handler: exiting\n") : 0;
    
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
    pid_t pid;

    pid = jobs_fgpid();

    (verbose) ? fprintf(stdout,"sigint_handler: entering\n") : 0;
    
    kill(-pid,SIGINT);
    
    if (verbose) {
        fprintf(stdout,"process terminated %d\n",pid);
        fprintf(stdout,"sigint_handler: exiting\n");
    }

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
    pid_t pid;

    pid = jobs_fgpid();
    
    kill(-pid,SIGTSTP);
    
    if (verbose) {
        fprintf(stdout,"process stopped %d\n",pid);
        fprintf(stdout,"sigtstp_handler: exiting\n");
    }
    
    return;
}
