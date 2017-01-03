#ifndef TAMPON_PARTAGE
#define TAMPON_PARTAGE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "../utils.h"

#define NB_CASES_MAX 15
	
typedef struct {
	int nbCases;
	int nbCasesVides;
	int indiceProchainDepot;
	int indiceProchainRetrait;
	pthread_cond_t condCasesVides;
	pthread_cond_t condCasesPleines;
	pthread_mutex_t mutex;
	
	Requete requete[NB_CASES_MAX];
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
void deposer(Moniteur *moniteur, Requete requete);

/* Retrait synchronise  */
Requete retirer(Moniteur *moniteur);

/* Retourne le nombre de cases */
int getNbCases(Moniteur *moniteur);

/* Retourne le nombre de cases remplies */
int getNbCasesRemplies(Moniteur *moniteur);

#endif
