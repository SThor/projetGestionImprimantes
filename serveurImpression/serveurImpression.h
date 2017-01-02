#ifndef SERVEUR_IMPRESSION
#define SERVEUR_IMPRESSION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "../communication/communication.h"
#include "../tamponPartage/tamponPartage.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define NB_IMPRIMANTES_MAX 10
#define NB_FILTRES_MAX 10

#define FILE_SCHEDULER_MAX 10
#define FILE_FILTER_MAX 10
#define FILE_BACKEND_MAX 10 
#define FILE_IMPRESSIONS_MAX 5


typedef struct{
	char nomImprimante[50];
	int typeImprimante;
	char socket[30];
} Imprimante;

typedef struct {
	char nomServeur[50];
	int numServeur;
} Serveur;

typedef struct {
	int nbCopies;
	int rectoVerso;
} Options;

typedef enum { 
	IMPRESSION, ETAT_IMPRESSION, ANNULATION_IMPRESSION, ETAT_IMPRIMANTE
} TypeRequete;

typedef enum { 
	ATTENTE, EN_COURS, TERMINEE 
} EtatImpression;

typedef enum { 
	IMPRESSION_EN_COURS, VIDE, PLEINE 
} EtatImprimante;

typedef struct {	
	pid_t emetteur;
	int idDemande;
	TypeRequete type;
	char* fichier;
	char* typeFichier;
	long tailleFichier;
	char* nomImprimante;
	char* nomFichier;
	Options options;
} Requete;

/* Initialisation du serveur d'impression */
int initialiserServeurImpression();

/* Initialisation de la configuration du systeme */
void initialiserConfiguration();

/* Ajout d'une imprimante */
void ajouterImprimante(Imprimante imprimante);

/* Recherche d'une imprimante */
Imprimante* chercherImprimante(char* nom);

/* Demarrage des imprimantes distantes via le backend */
void* demarrerCUPSBackend(void* args);

/* Demarrage des filtres */
void* demarrerCUPSFilter(void* args);

/* Demarrage du scheduler */
void* demarrerCUPSScheduler(void* args);

/* Demarrage des imprimantes locales et distantes */
void* demarrerImprimantesLocales(void* args);

/* Traiter une demande d'impression */
void imprimer(Imprimante imprimante, Requete requete);

/* Arret d'une demande d'impression */
void arreterImpression(Imprimante imprimante);
 
/* Tache Gestion Imprimante */
void gestionImprimante();

#endif
