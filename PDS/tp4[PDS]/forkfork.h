/* définit un type */
typedef void (*func_t) (void *);

/* fonction qui fait une impression banale */
void function(void *arg);

/* utilise la technique du double fork pour faire exécuter la fonction f() par un processus petit-fils */
void forkfork(func_t f, void *arg);
