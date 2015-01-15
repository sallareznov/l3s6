/* ------------------------------
   $Id: pipe.c,v 1.2 2005/03/29 09:46:52 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "pipe.h"
#include "jobs.h"
#include "cmd.h"

/* variable qui contiendra la commande */
static char cmdline[1024];

/* build_cmdline : construit la ligne de commande */
void build_cmdline(char *cmds[MAXCMDS][MAXARGS],int nbcmd,int bg) {
	int i;
	int j;

	strcpy(cmdline,"");
	j = 0;
	for (i = 0 ; i < nbcmd ; i++) {
		while (cmds[i][j]) {
			strcat(cmdline,cmds[i][j]);
			strcat(cmdline," ");
			j++;
		}
		(i != nbcmd - 1) ? strcat(cmdline,"| ") : strcat(cmdline,"");
		j = 0;
	}

	/* quand la commande est en arrière-plan, on rajoute l'esperluette à sa fin */
	(bg) ? strcat(cmdline," &") : 0;
}

/* reset_signals : remet les signaux à "zéro" */
int reset_signals() {
	struct sigaction sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGCHLD,&sa,NULL);
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGTSTP,&sa,NULL);
	
	return 0;
}

/* do_pipe : exécute une commande composée de pipes */
void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
	int fds[MAXCMDS][2];
	pid_t pid;
	sigset_t mask;
	int status;
	int stdin2, stdout2;
	int i;
	int nb_stopped_commands;
	int nb_cmd_bis;

	(verbose) ? fprintf(stdout,"do_pipe: entering\n") : 0;

	/* on bloque les signaux */
	if (sigemptyset(&mask) < 0)
		unix_error("sigemptyset error");
 	if (sigaddset(&mask, SIGCHLD))
		unix_error("sigaddset error");
  	if (sigaddset(&mask, SIGINT))
		unix_error("sigaddset error");
  	if (sigaddset(&mask, SIGTSTP))
		unix_error("sigaddset error");
  	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
		unix_error("sigprocmask error");

	/* copies de l'entrée et de la sortie standards */
	stdin2 = dup(STDIN_FILENO);
	stdout2 = dup(STDOUT_FILENO);

	/* duplication du shell : le processus dupliqué se chargera de lancer les n commandes, et c'est ce processus qui est ajouté à la liste des travaux */
	switch (pid = fork()) {
		case -1 :
			unix_error("fork mshell: ");
		case 0 :

			/* on attribue ce processus un groupe qui est différent du mshell. Les commandes lancées seront dans le même groupe et recevront les mêmes signaux */
			setpgid(0,0);

			/* lancement des (n-1) premières commandes */
			for (i = 0 ; i < nbcmd - 1 ; i++) {
				/* construction du tube qui relie les commandes i et (i + 1) */
				pipe(fds[i]);

				switch (pid = fork()) {
					case -1 :
						unix_error("fork cmd1: ");
					case 0 :
						/* on débloque les signaux pour la commande i et on ferme son descripteur en lecture */
						sigprocmask(SIG_UNBLOCK,&mask,NULL);
						close(fds[i][0]);
						/* le fils duplique son descripteur d'accès en écriture sur le tube vers sa sortie standard */
						dup2(fds[i][1],STDOUT_FILENO);
						(verbose) ? fprintf(stdout,"do_pipe: %s\n",cmds[i][0]) : 0;
						/* exécution de la commande i */
						execvp(cmds[i][0],cmds[i]);
						unix_error("execvp cmds: ");
					default :
						/* le père redirige son descripteur en lecture sur le tube vers son entrée standard et continue */
						close(fds[i][1]);
						dup2(fds[i][0],STDIN_FILENO);
						break;
				}
			}

			/* lancement de la dernière commande */
			dup2(stdout2,STDOUT_FILENO);
			switch (pid = fork()) {
				case -1 :
					unix_error("fork cmd2: ");
				case 0 :
					/* on débloque les signaux pour la dernière commande */
					sigprocmask(SIG_UNBLOCK,&mask,NULL);
					(verbose) ? fprintf(stdout,"do_pipe: %s\n",cmds[nbcmd - 1][0]) : 0;
					execvp(cmds[nbcmd - 1][0],cmds[nbcmd - 1]);
					unix_error("execvp cmd2: ");
				default :
					/* on débloque les signaux pour le père des n commandes (le fils du mshell donc) */
					sigprocmask(SIG_UNBLOCK,&mask,NULL);
					dup2(stdin2,STDIN_FILENO);
					dup2(stdout2,STDOUT_FILENO);
					close(fds[nbcmd - 1][0]);
					reset_signals();
					nb_stopped_commands = 0;
					nb_cmd_bis = nbcmd;
					/* la commande ne se termine pas tant que toutes les commandes qui la composent ne sont pas terminées */
					while (nb_cmd_bis > 0) {
						/* on attend chaque fils */
						waitpid(-1,&status,WUNTRACED | WCONTINUED);
						/* quand une commande a terminé, on decrémente le nombre de commandes */
						if ((WIFSIGNALED(status)) || (WIFEXITED(status)))
							nb_cmd_bis--;
						/* quand une commande a été stoppée, on incrémente le nombre de commandes stoppées */
						if (WIFSTOPPED(status))
							nb_stopped_commands++;
						/* on décrémente ce nombre quand une commande a été remise en marche */
						if (WIFCONTINUED(status))
							nb_stopped_commands--;
						/* quand toutes les commandes ont été stoppées, on envoie un signal de stop */
						if ((nb_stopped_commands == nb_cmd_bis) && (nb_stopped_commands > 0))
							kill(getpid(),SIGTSTP);
					}

					exit(EXIT_SUCCESS);
			}
		default : 
			/* on construit la commande et on ajoute le processus associé à la liste des travaux */
			build_cmdline(cmds,nbcmd,bg);
			jobs_addjob(pid,(bg ? BG : FG),cmdline);
			/*memset(cmdline,0,1024);*/
			sigprocmask(SIG_UNBLOCK,&mask,NULL);

			if (bg != 1)
				waitfg(pid);

			break;
	}

	(verbose) ? fprintf(stdout,"do_pipe: exiting\n") : 0;

	fflush(stdout);
}
