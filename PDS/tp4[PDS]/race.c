#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

/* tableau représentant le classement de la course */
static int classement[10];

/* le processus pid "court" */
void run(int pid) {
	int i;

	for (i = 0 ; i < 5000000 ; i++) {}
	printf("[%d] Je suis arrivé à la moitié de la course\n",pid);
	for (i = 0 ; i < 5000000 ; i++) {}
	printf("[%d] J'ai fini la course\n",pid);
}

/* affiche le classement */
void affiche_classement() {
	int i;

	printf("Classement :\n");
	printf("  1er : %d\n",classement[0]);
	for (i = 1 ; i < 10 ; i++) {
		printf("%2dème : %d\n",i + 1,classement[i]);
	}
}

/* simule une course de processus */
void race(void) {
	pid_t pid_pere;
	int i;

	/* je crée dix processus fils à partir de pid_pere, et chaque fils "court" puis se termine */
	for (i = 0 ; i < 10 ; i++) {
		pid_pere = fork();

		if (pid_pere == -1) {
			perror("erreur fork");
		}
		else if (pid_pere == 0) {
			run(getpid());
			/*classement[j++] = getpid();*/
			exit(EXIT_SUCCESS);
		}
	}

	/* à chaque fois qu'un fils termine, on met son numéro de processus dans le tableau */
	for (i = 0 ; i < 10 ; i++) {
		classement[i] = wait((int *) 0);
	}
	/* on affiche le classement une fois que tous les fils sont arrivés */
	affiche_classement();
	exit(EXIT_SUCCESS);
}

int main() {
	race();
	return 0;
}
