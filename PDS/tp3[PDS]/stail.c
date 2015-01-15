#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "stail.h"

/* renvoie le nombre de lignes du fichier path */
int nb_lines(const char *path) {
	int cpt;
	char c;
	int fd;
	int status;

	/* cpt comptera le nombre de lignes du fichier...on commence par ouvrir le fichier en lecture seule */
	cpt = 0;
	fd = open(path,O_RDONLY);
	assert(!(fd == -1));

	/* on lit d'abord le premier caractère et on vérifie que tout se passe bien (on renvoie -1 sinon) */
	status = read(fd,&c,1);
	if (status == -1) {
		switch (errno) {
			case EISDIR : fprintf(stderr,"stail: error reading '%s': Is a directory\n",path); break;
			default : {}
		}
		fprintf(stderr,"Try ./stail --help for more informations.\n");
		return -1;
	}

	/* si le premier caractère correspond à EOF => le fichier est vide, et donc son nombre de lignes est 0 */
	if (c == EOF)
		return 0;

	/* si le premier caractère est un retour à la ligne, on incrémente le compteur avant de lire les autres caractères */
	if (c == '\n')
		cpt++;

	/* en lisant les autres caractères, on incrémente le compteur à chaque fois que l'on "croise" un retour à la ligne (\n) */
	while ((read(fd,&c,1))) {
		if (c == '\n')
			cpt++;
	}

	/* on incrémente le compteur si le fichier ne se termine pas par \n */
	if (c != '\n')
		cpt++;

	/* on ferme le descripteur du fichier et on renvoie le compteur */
	close(fd);
	return cpt;
}

/* affiche les ntail dernières lignes du fichier path */
int stail(const char *path, int ntail) {
	char c;
	int fd;
	int nb_lines_read;
	int nb_lines_file;
	/* pour que ça marche même si l'utilisateur entre un nombre négatif */
	const int ntail_abs = fabs(ntail);
	
	/* on ouvre le fichier en lecture seule et on vérifie si tout se passe bien (on renvoie -1 sinon) */
	fd = open(path,O_RDONLY);
	if (fd == -1) {
		switch (errno) {
			case EACCES : fprintf(stderr,"stail: cannot open '%s' for reading: Permission denied\n",path); break;
			case ENOENT : fprintf(stderr,"stail: cannot open '%s' for reading: No such file or directory\n",path); break;
			case ELOOP : fprintf(stderr,"stail: cannot open '%s' for reading: Too many levels of symbolic links\n",path); break;
			case ENOTDIR : fprintf(stderr,"stail: cannot open '%s' for reading: Not a directory\n",path); break;
			default : {}
		}
		fprintf(stderr,"Try ./stail --help for more informations.\n");
		return 1;
	}

	nb_lines_read = 0;
	nb_lines_file = nb_lines(path);
	if (nb_lines_file == -1) {
		return -1;
	}

	/* on lit le fichier jusqu'à arriver au début des lignes que l'on veut afficher */
	while ((read(fd,&c,1)) && (nb_lines_read < (nb_lines_file - ntail_abs))) {
		if (c == '\n') {
			nb_lines_read++;
		}
	}

	/* on affiche c d'abord, parce qu'il contient le premier caractère de la première ligne à afficher, puis on affiche les autres */
	printf("%c",c);
	while ((read(fd,&c,1))) {
		printf("%c",c);
	}
	if (c != '\n')
		printf("\n");
	
	/* on ferme le descripteur du fichier et on renvoie 0 */
	close(fd);
	return 0;
}

int main(int argc, char *argv[]) {
	/* on informe l'utilisateur sur ce que fait la commande si celle-ci n'a pas assez d'arguments ou que l'utilisateur a demandé de l'aide */
	if ((argc < 2) || ((argc == 2) && (strcmp(argv[1],"--help") == 0))) {
		fprintf(stderr,"Usage: ./stail -n nb path\n");
		fprintf(stderr,"      Ce programme affiche les nb dernières lignes du fichier path.\n");
		fprintf(stderr,"      Si l'option -n n'est pas spécifiée, le programme affiche les 10\n");
		fprintf(stderr,"      dernières lignes du fichier.\n");
		return 1;
	}
	else {
		/* si l'utilisateur n'a entré qu'un seul argument, c'est qu'il veut afficher les 10 lignes (par défaut) du fichier indexé par l'argument
		(la commande affichera une erreur si l'argument ne correspond pas à un nom de fichier) */ 
		if (argc == 2) {
			return stail(argv[1],10);
		}
		else {
			int i;
			int nb_lines;

			/* on exécute la commande pour tous les noms de fichier entrés (le nombre de lignes varie bien sûr selon la présence ou non de l'option -n) */
			if (*argv[1] == '-') {
				nb_lines = atoi(argv[1]);
				for (i = 2 ; i < argc ; i++) {
					printf("===> %s <===\n",argv[i]);
					stail(argv[i],nb_lines);
				}
				return 0;
			}
			else {
				for (i = 1 ; i < argc ; i++) {
					printf("===> %s <===\n",argv[i]);
					stail(argv[i],10);
				}
			}
		}
		return 0;
	}
}
