/* renvoie le nombre de lignes du fichier path */
int nb_lines(const char *path);

/* lit la ligne courant du fichier de description fd, et met le contenu de cette ligne dans la variable line
renvoie 1 si la tête de lecture du fichier n'est pas à la fin, 0 sinon...et -1 si erreur */
int readline(int fd, char *line);

/* affiche les ntail dernières lignes du fichier path
renvoie 0 si tout s'est bien passé, 1 sinon */
int utail(const char *path, int ntail);
