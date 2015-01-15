#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include "forkfork.h"

/* fonction qui fait une impression banale */
void function(void *arg) {
	printf("Exécution de la fonction f\n");
}

/* utilise la technique du double fork pour faire exécuter la fonction f() par un processus petit-fils */
void forkfork(func_t f, void *arg) {
	pid_t pid, pid_pere, pid_fils, pid_petit_fils;

	pid_pere = getpid();
	printf("Processus père = [%d]\n",pid_pere);

	/* fork() sur le père */
	printf("[%d] Je vais engendrer\n",pid_pere);
	pid = fork();

	if (pid == -1) {
		perror("erreur fork père");
	}

	/* fils */
	else if (pid == 0) {
		pid_fils = getpid();
		printf("Processus fils = [%d]\n",pid_fils);
		printf("[%d] Je viens de naître\n",pid_fils);
		printf("[%d] Mon père est %d\n",pid_fils,getppid());
		printf("[%d] Je vais engendrer\n",pid_fils);

		/* fork() sur le fils pour engendrer le petit-fils */
		pid = fork();

		if (pid == -1) {
			perror("erreur fork fils");
		}

		/* petit-fils */
		else if (pid == 0) {
			pid_petit_fils = getpid();
			printf("Processus petit-fils = [%d]\n",pid_petit_fils);
			printf("[%d] Je viens de naître\n",pid_petit_fils);
			printf("[%d] Mon père est %d\n",pid_petit_fils,getppid());

			/* on attend que le processus fils se termine */
			while (getppid() == pid_fils) {}
			printf("[%d] Je termine\n",pid_fils);

			/* si le petit-fils a été adopté par init... */
			if (getppid() == 1) {
				printf("[%d] Je suis devenu orphelin, je suis maintenant adopté par init\n",pid_petit_fils);
				printf("[%d] Mon nouveau père est %d\n",pid_petit_fils,getppid());
			}

			/* exécution de la fonction f sur le petit-fils */
			f(arg);
			printf("[%d] J'ai exécuté la fonction f\n",pid_petit_fils);
			printf("[%d] Je termine\n",pid_petit_fils);
		}
		else {
			printf("[%d] J'ai engendré\n",pid_fils);
			printf("[%d] Mon fils est %d\n",pid_fils,pid);
			exit(EXIT_SUCCESS);
		}
	}

	/* le père attend son fils */
	else {
		printf("[%d] J'ai engendré\n",pid_pere);
		printf("[%d] Mon fils est %d\n",pid_pere,pid);
		(void) wait((int *) 0);
		exit(EXIT_SUCCESS);
	}

	/* le père termine */
	printf("[%d] Je termine\n",pid_pere);
	exit(EXIT_SUCCESS);
}
