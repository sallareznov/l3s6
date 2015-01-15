#include <stdio.h>
#include <limits.h>
#include <unistd.h>

/* affiche les valeurs de NAME_MAX et PATH_MAX */
void prlimit(void) {
	printf("NAME_MAX = %d\n",NAME_MAX);
	printf("PATH_MAX = %d\n",PATH_MAX);
}

int main(int argc, char *argv[]) {
	/* on invoque la méthode prlimit() et on sort du programme sur un succès */
	prlimit();
	return 0;
}
