#ifndef SERVEUR_IMPRESSION
#define SERVEUR_IMPRESSION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "../communication/communication.h"
#include "../tamponPartage/tamponPartage.h"
#include "../utilisateur/utilisateur.h"

typedef struct{
	pid_t numImprimante;
	char *nom;
	int type;
} Imprimante;

typedef struct{
	int numServeur;
	char* nom;
} Serveur;

/* Lecture du fichier de configuration */
void* lectureConfiguration();

/* Creation d'une imprimante */
Imprimante creerImprimante();

/* Recherche d'une imprimante */
Imprimante* chercherImprimante(char* nom);

/* Demarrage de l'imprimante */
void* demarrerImprimante(Imprimante imprimante);

/* Arret de l'imprimante */
void* arreterImprimante(Imprimante imprimante);

/* Traiter une demande d'impression */
void* imprimer(Imprimante imprimante, Requete requete);

/* Arret d'une demande d'impression */
void* arreterImpression(Imprimante imprimante);

/* Creation d'un serveur */
Serveur creerServeur();

/* Tache CUPS Scheduler */
void* cupsScheduler();

/* Tache CUPS Filter */
void* cupsFilter();

/* Tache CUPS Backend */
void* cupsBackend();
 
/* Tache Gestion Imprimante */
void* gestionImprimante();

#endif
