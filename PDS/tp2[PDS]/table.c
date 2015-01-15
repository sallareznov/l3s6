#include <stdio.h>
#include <stdlib.h>
#include "table.h"

#define TRUE 1
#define FALSE 0

void init_table() {
	int i;

	table = (struct inode **) calloc(100,sizeof(struct inode *));
	for (i = 0 ; i < 100 ; i++) {
		table[i] = (struct inode *) malloc(sizeof(struct inode *));
	}
}

int calcul_adresse(int num_inode) {
	return (num_inode % 100);
}

void ajoute_inode(int num_inode) {
	struct inode *noeud_a_ajouter;
	struct inode *noeud_precedent;
	const int adresse = calcul_adresse(num_inode);

	noeud_a_ajouter = (struct inode *) malloc(sizeof(struct inode *));
	noeud_a_ajouter->num_inode = num_inode;
	noeud_a_ajouter->noeud_suivant = NULL;
	
	noeud_precedent = table[adresse];
	while (noeud_precedent->noeud_suivant != NULL) {
		noeud_precedent = noeud_precedent->noeud_suivant;
	}
	noeud_precedent->noeud_suivant = noeud_a_ajouter;
}

int est_present(int num_inode) {
	struct inode *noeud;
	const int adresse = calcul_adresse(num_inode);

	noeud = table[adresse];
	while (noeud->noeud_suivant != NULL) {
		if (noeud->noeud_suivant->num_inode == num_inode)
			return TRUE;
		else
			noeud = noeud->noeud_suivant;
	}
	return FALSE;
}

void libere_table() {
	int i;
	for (i = 0 ; i < 100 ; i++) {
		free(table[i]);
	}
	free(table);
}
