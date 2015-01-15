/* renvoie le nombre de lignes du fichier path */
int nb_lines(const char *path);

/* affiche les ntail dernières lignes du fichier path
renvoie 0 si tout s'est bien passé, 1 sinon */
int stail(const char *path, int ntail);
