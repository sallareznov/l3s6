#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#include "do.h"
#include "makeargv.h"

#define TRUE 1
#define FALSE 0

/* le tableau contenant les options spécifiées par l'utilisateur */
static char **options;
/* le nombre d'options de la commande tapée par l'utilisateur (= la longueur du tableau d'options) */
static int nbre_options;

/* renvoie 1 si option est une option, 0 sinon */
int is_an_option(char *option) {
	return (option[0] == '-');
}

/* renvoie 1 si option est une option valide, 0 sinon */
int is_a_valid_option(char *option) {
	return ((strcmp(option,"--and") == 0) || (strcmp(option,"--or") == 0) || (strcmp(option,"--cc") == 0) || (strcmp(option,"--kill") == 0));
}

/* renvoie 1 si l'option option a été tapée par l'utilisateur, 0 sinon */
int option_present(char *option) {
	int i;

	for (i = 0 ; i < nbre_options ; i++) {
		if (strcmp(options[i],option) == 0)
			return TRUE;
	}
	return FALSE;
}

/* renvoie le nombre d'options du tableau argv de taille argc */
int nb_options(int argc, char *argv[]) {
	int i;
	int cpt;

	cpt = 0;
	for (i = 0 ; i < argc ; i++) {
		if (is_an_option(argv[i]))
			cpt++;
	}
	nbre_options = cpt;
	return cpt;
}

/* construit le tableau d'options */
int build_options(int argc, char *argv[]) {
	int i;
	int j;

	j = 0;
	options = (char **) calloc(nbre_options + 1,sizeof(char *));

	for (i = 0 ; i < argc ; i++) {
		if (is_an_option(argv[i])) {
			options[j] = argv[i];
			j++;
		}
	}
	options[nbre_options] = NULL;

	return 0;
}

/* renvoie la valeur obtenue par l'opération des opérateurs a et b par l'opérateur op */
int right_operation(int a, int b, operateur op) {
	if (a == -1) {
		return b;
	}
	else {
		switch (op) {
			case AND : return (a && b);
			default : return (a || b);
		}
	}
}

/* renvoie 0 si toutes les options tapées par l'utilisateur sont valides, sinon renvoie -1 et affiche des messages d'information */
int check_options_validity() {
	int i;
	int wrong_option;

	wrong_option = FALSE;
	i = 0;

	while (options[i]) {
		if (!is_a_valid_option(options[i])) {
			wrong_option = TRUE;
			fprintf(stderr,"do: invalid option '%s'\n",options[i]);
		}
		i++;
	}

	if (wrong_option == TRUE)
		return -1;
	else
		return 0;
}

/* renvoie 0 si la commande tapée par l'utilisateur est valide, sinon renvoie -1 et affiche un message d'information */
int check_command_validity() {
	if ((option_present("--and") && option_present("--or")) || (option_present("--kill") && !(option_present("--cc")))) {
		fprintf(stderr,"do: invalid command\n");
		return -1;
	}
	else {
		return 0;
	}
}

/* tue tous les processus fils du processus courant */
void kill_all() {
	signal(SIGQUIT,SIG_IGN);
	kill(-1 * getpid(),SIGQUIT);
}

/* renvoie la valeur finale de la commande */
int final_value(char *argv[], int argc, operateur op) {
	int f_value;
	int i;
	pid_t pid;
	int status;
	char **arg_cmd;
	int cc_effect;

	f_value = -1;
	cc_effect = FALSE;

	for (i = 1 ; i < argc ; i++) {
		if (!is_an_option(argv[i])) {
			pid = fork();

			makeargv(argv[i]," \t",&arg_cmd);
			switch (pid) {
				case -1 : perror("erreur fork");
				case 0 : execvp(arg_cmd[0],arg_cmd);
						fprintf(stderr,"%s: No such file or directory\n",argv[i]);
						exit(EXIT_FAILURE);
				default : ;
			}
		}
	}

	for (i = 1 ; i < argc ; i++) {
		pid = wait(&status);

		if (option_present("--cc")) {
			if (((WEXITSTATUS(status) != 0) && (op == AND)) || ((WEXITSTATUS(status) == 0) && (op == OR)))
				cc_effect = TRUE;
		}

		if (option_present("--cc") && cc_effect == TRUE) {
			if (option_present("--kill")) {
				kill_all();
			}
			op == AND ? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
		}

		if (WIFEXITED(status) && (cc_effect == FALSE))
			f_value = right_operation(f_value,!WEXITSTATUS(status),op);
	}

	f_value ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	if ((argc < 2) || ((argc == 2) && (strcmp(argv[1],"--help") == 0))) {
		fprintf(stderr,"USAGE : ./do [--and|--or|--cc|--kill] [command]+\n");
		fprintf(stderr,"	Ce programme exécute indépendamment et simultanément les commandes\n");
		fprintf(stderr,"	command...En cas de succès, la conjonction (--and) ou\n");
		fprintf(stderr,"	la disjonction (--or) des résultats de chaque commande sera renvoyée.\n");
		fprintf(stderr,"	Sinon, 1 est renvoyé. Par défaut, l'option --and est appliquée.\n");
		fprintf(stderr,"OPTIONS :\n");
		fprintf(stderr,"	--and : fait la conjonction des résultats de chaque commande\n");
		fprintf(stderr,"	--or : fait la disjonction des résultats de chaque commande\n");
		fprintf(stderr,"	--cc : renvoie 1 au premier résultat faux si l'option --and est\n");
		fprintf(stderr,"		activée et au premier résultat vrai si l'option --or est activée\n");
		fprintf(stderr,"	--kill : tue les processus suivants quand on obtient le résultat\n");
		fprintf(stderr,"		de la commande avant que tous les programmes ne s'exécutent\n");
		fprintf(stderr,"COMMANDES POSSIBLES :\n");
		fprintf(stderr,"	./do [command]+\n");
		fprintf(stderr,"	./do --cc [command]+\n");
		fprintf(stderr,"	./do --cc --kill [command]+\n");
		fprintf(stderr,"	./do --and [command]+\n");
		fprintf(stderr,"	./do --or [command]+\n");
		fprintf(stderr,"	./do --and --cc [command]+\n");
		fprintf(stderr,"	./do --or --cc [command]+\n");
		fprintf(stderr,"	./do --and --cc --kill [command]+\n");
		fprintf(stderr,"	./do --or --cc --kill [command]+\n");
		fprintf(stderr,"NOTES :\n");
		fprintf(stderr,"	Les options et les commandes peuvent être tapées dans n'importe\n");
		fprintf(stderr,"	quel ordre\n");
		return -1;
	}
	else {
		int status;

		nbre_options = nb_options(argc,argv);
		build_options(argc,argv);

		status = check_options_validity();
		if (status == -1) {
			fprintf(stderr,"Try ./do --help for more informations.\n");
			return -1;
		}

		status = check_command_validity();
		if (status == -1) {
			fprintf(stderr,"Try ./do --help for more informations.\n");
			return -1;
		}

		if (option_present("--or"))
			return final_value(argv,argc,OR);
		else
			return final_value(argv,argc,AND);
	}
}
