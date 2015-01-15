#! /bin/sh

# commenter les lignes de creation des fichiers après la premiere
# execution

# modifier les appels a mtail suivant les options que vous avez
# choisies

MCMD=$1
rm ./mtail
echo "Running " $MCMD
`ln -fs $MCMD ./mtail`

# [1.1] Fichier creux
echo "==== [1.1] Fichier creux ===================================="
dd if=/dev/null of=_sparse-file bs=1k seek=5120 count=1;
tail _sparse-file ; ./mtail _sparse-file    ;
echo "  [OPTION] -3" ;  
tail -3 _sparse-file     ; ./mtail -3 _sparse-file ;

# [1.2] Fichier de 5 lignes
echo "==== [1.2] Fichier de 5 lignes =============================="
touch _5lignes; for i in `seq 1 5`; do echo "ligne $i"; done >> _5lignes; 
tail _5lignes ; ./mtail _5lignes    ;
echo "  [OPTION] -3" ;  
tail -3 _5lignes     ; ./mtail -3 _5lignes ;

# [1.3] Fichier de 10 lignes
echo "==== [1.3] Fichier de 10 lignes ============================="
touch _10lignes; for i in `seq 1 10`; do echo "ligne $i"; done >> _10lignes; 
tail _10lignes ; ./mtail _10lignes    ;
echo "  [OPTION] -3" ;  
tail -3 _10lignes     ; ./mtail -3 _10lignes ;

# [1.4] Fichier de 100 lignes
echo "==== [1.4] Fichier de 100 lignes ============================"
touch _100lignes; for i in `seq 1 100`; do echo "ligne $i"; done >> _100lignes;
tail _100lignes ; ./mtail _100lignes    ;
echo "  [OPTION] -3" ;  
tail -3 _100lignes     ; ./mtail -3 _100lignes ;

