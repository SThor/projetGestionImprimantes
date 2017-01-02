#ifndef TAMPON_PARTAGE
#define TAMPON_PARTAGE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define NB_CASES_MAX 15

typedef struct{
	void * buffer;
	int size;
} Message;
	
typedef struct {
	int nbCases;
	int nbCasesVides;
	int indiceProchainDepot;
	int indiceProchainRetrait;
	pthread_cond_t condCasesVides;
	pthread_cond_t condCasesPleines;
	pthread_mutex_t mutex;
	
	Message message[NB_CASES_MAX];
} Moniteur;

/* Creation d'un moniteur suivant le modele producteurs/consommateurs */
Moniteur creerMoniteur();

/* Synchronisation sur le debut du depot */
int debutDeposer(Moniteur *moniteur);

/* Synchronisation sur la fin du depot */
void finDeposer(Moniteur *moniteur);

/* Synchronisation sur le debut du retrait */
int debutRetirer(Moniteur *moniteur);

/* Synchronisation sur la fin du retrait */
void finRetirer(Moniteur *moniteur);

/* Depot synchronise */
void deposer(Moniteur *moniteur, void *buffer, int size);

/* Retrait synchronise qui retourne la taille du pointeur du buffer de retrait */
int retirer(Moniteur *moniteur, void **buffer);

/* Retourne le nombre de cases remplies */
int getNbCasesRemplies(Moniteur *moniteur);

#endif
