#ifndef UTILISATEUR
#define UTILISATEUR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../communication/communication.h"
#include "../tamponPartage/tamponPartage.h"
#include "../utils.h"

#define NOM_SERVEUR "serveurImpression1"

/* Recuperation de la taille d'un fichier */
long recupererTailleFichier(const char* cheminFichier);

/* Recuperation du type d'un fichier */
char* recupererTypeFichier(const char* cheminFichier);

/* Impression d'une requete */
void imprimerRequete(Requete requete);

/* Envoi d'une requete */
int envoyerRequete(Requete requete, int numCommunication);

/* Creation ID Requete */
int etablirIdRequete(pid_t emetteur, int nbRequetes);

#endif
