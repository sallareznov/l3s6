/* structure représentant l'inode d'un dossier
elle est définie par son numéro d'inode et un pointeur vers l'inode du dossier suivant */
struct inode {
	int num_inode;
	struct inode *noeud_suivant;
};

/* la table de hachage, définie comme un tableau d'inodes */
struct inode **table;

/* permet d'initialiser la table en allouant de l'espace mémoire à lui et à ses indices */
void init_table();

/* calcul l'adresse d'un numéro d'inode */
int calcul_adresse(int num_inode);

/* ajoute un inode à la table */
void ajoute_inode(int num_inode);

/* renvoie 1 si le numéro d'inode est présent dans la table, et 0 sinon */
int est_present(int num_inode);

/* libère l'espace mémoire occupé par la table */
void libere_table();