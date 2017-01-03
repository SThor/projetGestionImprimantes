#ifndef UTILS
#define UTILS

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define NB_FILTERS_MAX 10
#define NB_BACKENDS_MAX 10
#define NB_IMPRIMANTES_MAX 10

#define CHEMIN_CONFIGURATION "configuration"

typedef struct{
	char nomImprimante[50];
	int typeImprimante;
} Imprimante;

typedef struct {
	char nomServeur[50];
	int numServeur;
} Serveur;

typedef enum { 
	IMPRESSION, ETAT_IMPRESSION, ANNULATION_IMPRESSION, ETAT_IMPRIMANTE
} TypeRequete;

typedef enum { 
	ATTENTE, EN_COURS, TERMINEE 
} EtatImpression;

typedef enum { 
	IMPRESSIONS_EN_COURS, VIDE, PLEINE 
} EtatImprimante;

typedef struct {	
	pid_t emetteur;
	int idRequete;
	TypeRequete type;
	char* fichier;
	char* typeFichier;
	long tailleFichier;
	char* nomImprimante;
	char* nomFichier;
	int nbCopies;
	int rectoVerso;
} Requete;

#endif
