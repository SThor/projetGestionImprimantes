#ifndef IMPRIMANTE_DISTANTE
#define IMPRIMANTE_DISTANTE

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

/* Creation d'une imprimante */
int creerImprimante(char* nom);

/* Traiter une demande d'impression */
void lancerImpression(Requete requete);

/* Annuler d'une demande d'impression */
void annulerImpression();

/* Etat d'une demande d'impression */
void etatImpression();

/* Traitement de la requete */
void traiterRequete(Requete requete, int numCommunication);

#endif
