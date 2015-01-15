#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "utail.h"

#define MAXLINE 1024

/* renvoie le nombre de lignes du fichier path */
int nb_lines(const char *path) {
	int cpt;
	char c;
	int fd;
	int status;

	/* après avoir initialisé le compteur du nombre de lignes, on ouvre le fichier en lecture seule et on vérifie si tout est conforme en lisant le premier caractère */
	cpt = 0;
	fd = open(path,O_RDONLY);
	assert(!(fd == -1));

	status = read(fd,&c,1);
	if (status == -1) {
		switch (errno) {
			case EISDIR : fprintf(stderr,"utail: error reading '%s': Is a directory\n",path); break;
			default : {}
		}
		fprintf(stderr,"Try ./utail --help for more informations.\n");
		return -1;
	}

	/* si le premier caractère lu est EOF => le fichier est vide */
	if (c == EOF)
		return 0;
	/* si le premier caractère est un retour à la ligne, on incrémente le compteur du nombre de lignes */
	if (c == '\n')
		cpt++;

	/* on parcourt le fichier octet par octet et on incrémente le compteur à chaque fois qu'on "croise" un retour à la ligne */
	while ((read(fd,&c,1))) {
		if (c == '\n')
			cpt++;
	}
	/* si le dernier caractère lu n'est pas un retour à la ligne, on incrémente le compteur */
	if (c != '\n') {
		cpt++;
	}

	/* on ferme le descripteur de fichier puis on renvoie le compteur */
	close(fd);
	return cpt;
}

/* lit la ligne courant du fichier de description fd, et met le contenu de cette ligne dans la variable line */
int readline(int fd, char *line) {
	int status;

	/* comme auparavant, on lit le premier caractère et on vérifie que tout se passe bien */
	status = read(fd,&line[0],1);
	if (status == -1) {
		switch (errno) {
			case EISDIR : fprintf(stderr,"utail: error reading this file: Is a directory\n"); break;
			default : {}
		}
		fprintf(stderr,"Try ./utail --help for more informations.\n");
		return -1;
	}

	/* status == 0 équivaut à dire que c == EOF et que donc la tête de lecture du descripteur de fichier fd est à la fin du fichier décrit */
	if (status == 0)
		return 0;
	/* si le premier caractère lu est un retour à la ligne, on met à jour line et on retourne 1 */
	if (line[0] == '\n') {
		line[1] = '\0';
		return 1;
	}
	else {
		int i;
		i = 1;
		/* vu que le premier caractère a déjà été lu, on commence notre lecture à partir du 2e, et on s'arrête quand la tête de lecture est sur un "\n"
		(on aura donc lu une ligne entière) */
		while ((read(fd,&line[i],1)) && (line[i] != '\n')) {
			i++;
		}
		line[i] = '\0';
		return 1;
	}
}

/* affiche les ntail dernières lignes du fichier path */
int utail(const char *path, int ntail) {
	int fd;
	char **tampon;
	int ind_to_delete;
	char line[MAXLINE];
	int i;
	/* on travaille ici avec la valeur absolue de ntail */
	const int ntail_abs = fabs(ntail);

	/* on ouvre le fichier en lecture seule et on vérifie que tout se passe bien */
	fd = open(path,O_RDONLY);
	if (fd == -1) {
		switch (errno) {
			case EACCES : fprintf(stderr,"utail: cannot open '%s' for reading: Permission denied\n",path); break;
			case ENOENT : fprintf(stderr,"utail: cannot open '%s' for reading: No such file or directory\n",path); break;
			case ELOOP : fprintf(stderr,"utail: cannot open '%s' for reading: Too many levels of symbolic links\n",path); break;
			case ENOTDIR : fprintf(stderr,"utail: cannot open '%s' for reading: Not a directory\n",path); break;
			default : {}
		}
		fprintf(stderr,"Try ./utail --help for more informations.\n");
		return 1;
	}

	/* on alloue de l'espace mémoire à notre tampon circulaire */
	tampon = (char **) calloc(ntail_abs,sizeof(char *));
	for (i = 0 ; i < ntail_abs ; i++) {
		tampon[i] = (char *) malloc(sizeof(char *));
	}

	/* on lit chaque ligne du fichier et on la met dans l'indice ind_to_delete du tampon */ 
	ind_to_delete = 0;
	while (readline(fd,line)) {
		strcpy(tampon[ind_to_delete],line);
		memset(line,0,MAXLINE);
		ind_to_delete = (ind_to_delete + 1) % ntail_abs;
	}

	/* on affiche le contenu du tampon suivant ind_to_delete, puis on ferme le descripteur */
	for (i = 0 ; i < ntail_abs ; i++) {
		if (strcmp(tampon[ind_to_delete],"\n") != 0) {
			printf("%s\n",tampon[ind_to_delete]);
		}
		else {
			printf("%s",tampon[ind_to_delete]);
		}
		ind_to_delete = (ind_to_delete + 1) % ntail_abs;
	}

	close(fd);
	return 0;
}

int main(int argc, char *argv[]) {
	/* si l'utilisateur n'a entré qu'un seul argument, c'est qu'il veut afficher les 10 lignes (par défaut) du fichier indexé par l'argument
		(la commande affichera une erreur si l'argument ne correspond pas à un nom de fichier) */ 
	if ((argc < 2) || ((argc == 2) && (strcmp(argv[1],"--help") == 0))) {
		fprintf(stderr,"Usage: ./utail -n nb path\n");
		fprintf(stderr,"      Ce programme affiche les nb dernières lignes du fichier path.\n");
		fprintf(stderr,"      Si l'option -n n'est pas spécifiée, le programme affiche les 10\n");
		fprintf(stderr,"      dernières lignes du fichier.\n");
		return 1;
	}
	else {
		/* si l'utilisateur n'a entré qu'un seul argument, c'est qu'il veut afficher les 10 lignes (par défaut) du fichier indexé par l'argument
		(la commande affichera une erreur si l'argument ne correspond pas à un nom de fichier) */ 
		if (argc == 2) {
			return utail(argv[1],10);
		}
		else {
			int i;
			int nb_lines;

			/* on exécute la commande pour tous les noms de fichier entrés (le nombre de lignes varie bien sûr selon la présence ou non de l'option -n) */
			if (*argv[1] == '-') {
				nb_lines = atoi(argv[1]);
				for (i = 2 ; i < argc ; i++) {
					printf("===> %s <===\n",argv[i]);
					utail(argv[i],nb_lines);
				}
				return 0;
			}
			else {
				for (i = 1 ; i < argc ; i++) {
					printf("===> %s <===\n",argv[i]);
					utail(argv[i],10);
				}
			}
		}
		return 0;
	}
}
