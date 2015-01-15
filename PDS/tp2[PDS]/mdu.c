#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include "mdu.h"
#include "table.c"

/* définition des macros TRUE et FALSE dans le fichier "table.c" */

static int opt_follow_links = FALSE;
static int opt_apparent_size = FALSE;
static char **options;
static int invalid_option = FALSE;

int valid_name(const char *name) {
	return ((strcmp(name,".") != 0) && (strcmp(name,"..") != 0));
}

int taille_adequate(struct stat st) {
	if (opt_apparent_size)
		return st.st_size;
	else
		return st.st_blocks;
}

char *nouveau_chemin(const char *pathname, char *extension) {
	char *newc;
	newc = (char *) calloc((int)strlen(pathname) + (int)strlen(extension) + 2,sizeof(char));
	sprintf(newc,"%s/%s",pathname,extension);
	newc[strlen(newc)] = '\0';
	return newc;
}

int struct_stat(const char *pathname, struct stat *st) {
	if (opt_follow_links)
		return stat(pathname,st);
	else
		return lstat(pathname,st);
}

int du_file(const char *pathname) {
	struct stat st;
	int size;
	int status;
	DIR *dirp;
	struct dirent *dp;
	status = access(pathname,F_OK);
	if (status == -1) {
		if (errno == ENOENT) {
			printf("mdu: cannot access '%s': No such file or directory\n",pathname);
			return EXIT_FAILURE;
		}
		if (opt_follow_links && (errno == ELOOP)) {
			printf("mdu: cannot access '%s': Too many levels of symbolic links\n",pathname);
			return EXIT_FAILURE;
		}
	}
	status = struct_stat(pathname,&st);
	assert(!status);
	ajoute_inode(st.st_ino);
	if ((S_ISREG(st.st_mode)) || ((S_ISLNK(st.st_mode)) && (opt_follow_links == FALSE))) {
		return taille_adequate(st);
	}
	else {
		size = taille_adequate(st);

		if ((dirp = opendir(pathname)) == NULL) {
			printf("mdu: cannot access '%s': No such file or directory\n",pathname);
			return EXIT_FAILURE;
		}
		else {
			while ((dp = readdir(dirp))) {
				if (valid_name(dp->d_name)) {
					struct_stat((const char *) nouveau_chemin(pathname,dp->d_name),&st);
					if (!est_present(st.st_ino)) {
						ajoute_inode(st.st_ino);
						size += du_file((const char *) nouveau_chemin(pathname,dp->d_name));
					}
				}
			}
			closedir(dirp);
			printf("%d\t%s\n",size,pathname);
			return size;
		}
	}
}

int is_an_option(char *s) {
	return (s[0] == '-');
}

int is_a_valid_option(char *option) {
	return ((strcmp(option,"-b") == 0) || (strcmp(option,"-L") == 0));
}

int nb_options(int argc, char *argv[]) {
	int nb; int i;

	nb = 0;
	for (i = 0 ; i < argc ; i++) {
		if (argv[i][0] == '-')
			nb++;
	}
	return nb;
}

void check_options(void) {
	int i;
	i = 0;

	while (options[i]) {
		if (!is_a_valid_option(options[i])) {
			printf("mdu: invalid option -- '%s'\n",(options[i] + 1));
			invalid_option = TRUE;
		}
		else {
			if (strcmp(options[i],"-L") == 0)
				opt_follow_links = TRUE;
			if (strcmp(options[i],"-b") == 0)
				opt_apparent_size = TRUE;
		}
		i++;
	}
}

int main(int argc, char *argv[]) {
	if ((argc > 1) && (strcmp(argv[1],"--help") == 0)) {
		printf("Usage: ./mdu ([OPTIONS]<->[FICHIER|REPERTOIRE]*)?\n");
		printf("Rapporte la taille disque utilisée par un fichier ou un répertoire\n");
		printf("et l'ensemble de ses fichiers (y compris ses sous-répertoires).\n");
		printf("les arguments de la commande peuvent être écrits dans n'importe quel ordre\n");
		printf("Options :\n");
		printf("	-L	indique de suivre les liens symboliques\n");
		printf("	-b	indique de rapporter les tailles apparentes\n");
		printf("Par défaut, les tailles réelles sont rapportées, et les liens\n");
		printf("symboliques ne sont pas suivis.\n");
		return 1;
	}
	else {
		int i; int j;
		struct stat st;
		const int nbre_options = nb_options(argc,argv);

		j = 0;
		options = (char **) calloc(nbre_options + 1,sizeof(char *));
		for (i = 1 ; i < argc ; i++) {
			if (is_an_option(argv[i])) {
				options[j] = argv[i];
				j++;
			}
		}
		options[nbre_options] = NULL;
		check_options();
		free(options);
		if (invalid_option == TRUE) {
			printf("Try ./mdu --help for more information.\n");
			return 1;
		}

		init_table();
		if ((nbre_options + 1)  == argc) {
			du_file(".");
		}
		else {
			for (i = 1 ; i < argc ; i++) {
				if (!is_an_option(argv[i])) {
					struct_stat(argv[i],&st);
					if (S_ISREG(st.st_mode) || ((S_ISLNK(st.st_mode))))
						printf("%d\t%s\n",du_file(argv[i]),argv[i]);
					else
						du_file(argv[i]);
				}
			}
		}
		libere_table();
		return 0;
	}
}
