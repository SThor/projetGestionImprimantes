#ifndef UTILISATEUR
#define UTILISATEUR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef enum { 
	IMPRESSION, ETAT_IMPRESSION, ANNULATION_IMPRESSION, ETAT_IMPRIMANTE
} TypeRequete;
	 
typedef struct{
	pid_t emetteur;
	TypeRequete type;
	char* fichier;
	char* typeFichier;
	long tailleFichier;
	char* nomImprimante;
	int nbCopies;
	int rectoVerso;
} Requete;

typedef enum { 
	ATTENTE, EN_COURS, TERMINEE 
} EtatImpression;

typedef enum { 
	IMPRESSION_EN_COURS, VIDE, PLEINE 
} EtatImprimante;

#endif
