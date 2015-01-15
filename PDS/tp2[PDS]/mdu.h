/* renvoie 1 si name est n'est pas "." ni ".." (dossier courant et dossier parent) */ 
int valid_name(const char *name);

/* renvoie la taille adéquate selon l'affichage des tailles réelles ou des tailles apparentes (option -b) */
int taille_adequate(struct stat st);

/* construit et renvoie le nouveau chemin vers le dossier/fichier extension, sous-fichier/repertoire de pathname */
char *nouveau_chemin(const char *pathname, char *extension);

/* renvoie la structure stat adéquate selon le suivi ou non des liens symboliques (option -L) */
int struct_stat(const char *pathname, struct stat *st);

/* rapporte la taille disque utilisée par pathname */
int du_file(const char *pathname);

/* renvoie 1 si s est une option */
int is_an_option(char *s);

/* renvoie 1 si option est une option valide (-L ou -b) */
int is_a_valid_option(char *option);

/* renvoie le nombre d'options de la commande entrée par l'utilisateur */
int nb_options(int argc, char *argv[]);

/* vérifie s'il y a une ou des options incorrectes dans la commande entrée par l'utilisateur */
void check_options(void);