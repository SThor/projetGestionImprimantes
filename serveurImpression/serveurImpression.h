#ifndef SERVEUR_IMPRESSION
#define SERVEUR_IMPRESSION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "../communication/communication.h"
#include "../tamponPartage/tamponPartage.h"
#include "../utils.h"

/* Initialisation du serveur d'impression */
int initialiserServeurImpression();

/* Initialisation de la configuration du systeme */
void initialiserConfiguration();

/* Recherche d'une imprimante */
Imprimante* chercherImprimante(char* nom);

/* Recherche du numéro d'une imprimante */
int chercherIdImprimante(char* nom);

/* backend */
void* cupsBackend(void* args);

/* filter */
void* cupsFilter(void* args);

/* scheduler */
void* cupsScheduler(void* args);

/* imprimante locale */
void* imprimantesLocale(void* args);

/* Application du filtre sur la requete */
void filtrerRequete(Requete requete);

/* Authentification de l'utilisateur */
int authentifierUtilisateur(pid_t nomUtilisateur);

/* Traitement des requetes d'impression */
void traiterImpression(Requete requete);

/* Traitement des requetes d'etat d'impression */
void traiterEtatImpression(Requete requete, int numCommunication);

/* Traitement des requetes d'annulation d'impression */
void traiterAnnulationImpression(Requete requete, int numCommunication);

/* Traitement des requetes d'etat d'imprimante */
void traiterEtatImprimante(Requete requete, int numCommunication);

/* Traitement de la requete */
void traiterRequete(Requete requete, int numCommunication);

#endif
