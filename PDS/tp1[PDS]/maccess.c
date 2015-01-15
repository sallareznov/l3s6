#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* - tableau de chaînes qui contiendra les modes voulus par l'utilisateur qui tape la commande ("-r","-w","-x" ou "-v")
   - chaîne de caractères contenant le chemin du fichier qui sera entré par l'utilisateur */
static char **modes;
static char *pathname;

/* affiche le message correspondant au type d'erreur détecté */
void printErrorType(void) {
	if (errno) {
		switch (errno) {
			case EACCES : printf("maccess: Le droit d'accès demandé au fichier n'est pas positionné\n"); break;
			case ENOTDIR : printf("maccess: Une des composantes du nom de fichier n'est pas un répertoire\n"); break;
			case ENOENT : printf("maccess: Le fichier n'existe pas\n"); break;
			case ENAMETOOLONG :
				if (strlen(pathname) < PATH_MAX)
					printf("maccess: Le nom du fichier est trop long\n");
				else
					printf("maccess: Une des composantes du nom de fichier est trop longue\n");
			break;
			case ELOOP : printf("maccess: Le nom du fichier comporte trop de liens symboliques\n"); break;
			default : printf("maccess: Autre erreur\n");
		}
	}
}

/* renvoie la valeur entière de la chaîne en paramètre représentant le mode */
int modesCorrespondance(char *mode) {
	if (strcmp(mode,"-r") == 0)
		return R_OK;
	if (strcmp(mode,"-w") == 0)
		return W_OK;
	if (strcmp(mode,"-x") == 0)
		return X_OK;
	if (strcmp(mode,"-f") == 0)
		return F_OK;
	else
		return -1;
}

/* nous renseigne sur la validité d'un mode */
int isValid(char *mode) {
	/* un mode est valide si la chaîne le représentant est "-r", "-w", "-x" ou "-v" */
	return ((strcmp(mode,"-r") == 0) || (strcmp(mode,"-w") == 0) || (strcmp(mode,"-x") == 0) || (strcmp(mode,"-v") == 0) || (strcmp(mode,"-f") == 0));
}

/* renvoie le mode total de toutes les modes */
int totalMode(void) {
	int code_mode;
	int i;
	code_mode = 0;
	i = 0;
	/* on parcourt le tableau des modes entrés par l'utilisateur et on met à jour le code total des modes (règle du OU binaire à chaque fois) */
	while (modes[i]) {
		if ((strcmp(modes[i],"-v") != 0))
			code_mode |= modesCorrespondance(modes[i]);
		i++;
	}
	return code_mode;
}

/* renvoie 0 si la commande access s'est bien passée, -1 sinon */
int maccess(void) {
	int code_mode;
	code_mode = totalMode();
	/* on invoque la méthode access sur le chemin indiqué par l'utilisateur et le code total des modes */
	return access(pathname,code_mode);
}


/* un booléen qui nous dira s'il y a l'option verbose (-v) dans la commande de l'utilisateur (macros TRUE et FALSE déclarées en début de fichier) */
static int verbose_mode = FALSE;

/* vérifie la validité des modes en mettant à jour les deux variables statiques citées plus haut */
void checkVerboseMode(void) {
	int i;
	i = 0;
	while (modes[i]) {
		/* on met le booléen à TRUE quand un élément du tableau est "-v" (mode verbose) */
		if (strcmp(modes[i],"-v") == 0)
			verbose_mode = TRUE;
		/* si un mode est invalide, on l'affecte à la variable incorrect_mode et on sort du programme
		(pas la peine de continuer : mode incorrect = commande invalide) */
		i++;
	}
}

/* méthode main de notre programme */
int main(int argc, char *argv[]) {
	/* une commande access est valide si elle contient au moins 3 arguments : le nom de la commande, une option, le chemin du fichier */
	if (argc < 3) {
		printf("Usage : maccess [mode] pathname\n");
		printf("		[mode] = -f | -r | -w | -x | -v\n");
		return 1;
	}
	else {
		int i;
		/* le chemin du fichier est le dernier argument (donc celui à la position argc - 1 du tableau des arguments argv) [argc = taille du tableau argv] */
		pathname = argv[argc - 1];
		/* on alloue de la mémoire au tableau des modes (déclaré en début de fichier) et on le remplit
		si la commande est valide, les modes sont situés dans l'intervalle d'arguments [1 ; argc - 2] (0 est occupé par le nom du programme et argc - 1 par le chemin du fichier
		y'aura donc argc - 2 modes dans la commande, on lui réserve un espace de taille argc - 1 (dernier élément à NULL pour pouvoir parcourir le tableau)
		*/
		modes = (char **) calloc(argc - 1,sizeof(char *));
		for (i = 1 ; i < argc - 1 ; i++) {
			modes[i-1] = argv[i];
		}
		modes[argc - 2] = NULL;
		/* on vérifie la validité des modes et on invoque enfin la commande de l'utilisateur */
		checkVerboseMode();
		maccess();
		/* une fois qu'on a exécuté la commande de l'utilisateur, on n'a plus besoin de l'espace alloué par le tableau des modes, on peut donc libérer cet espace */
		free(modes);
		/* si on a trouvé un mode incorrect, on le signale et on sort du programme sur un échec */
		
		/* s'il y a un mode -v dans la commande de l'utilisateur, on invoque la méthode printErrorType() qui imprime l'erreur, s'il y en a */
		if (verbose_mode) {
			printErrorType();
		}
		/* si on n'est pas sorti du programme jusque-là, cela équivaut à un succès */
		return 0;
	}
}