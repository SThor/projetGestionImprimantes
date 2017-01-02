/*
 ============================================================================
 Name        : serveurImpression.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "serveurImpression.h"

#define CHEMIN_CONFIGURATION "configuration"

Serveur serveur;

Imprimante * imprimantes;
int nbImprimantes;

/* Lecture du fichier de configuration */
void* lectureConfiguration() {
	imprimantes = (Imprimante *) malloc(sizeof(Imprimante));
	nbImprimantes = 0;
	FILE* fichier = NULL;
    char cheminFichier[256];
	int caractereActuel = 0;
	char* contenu = "";
    printf("\tRecherche du fichier de configuration %s...\n", CHEMIN_CONFIGURATION);
	fichier = fopen(CHEMIN_CONFIGURATION, "r");
	if (fichier != NULL) {
    	printf("\tLecture du fichier de configuration %s...\n", CHEMIN_CONFIGURATION);
		char ligne[256];
		
		while (fgets(ligne, sizeof ligne, fichier) != NULL) {
			Imprimante imprimante;
			char* nom;
			int type;
			char* contenuLigne = strtok(ligne, " ");
			if (strcmp(&contenuLigne[0], "serveur") == 0) {
				contenuLigne = strtok(NULL, " ");
				serveur.nom = contenuLigne;		
			} else if (strcmp(&contenuLigne[0], "locale") == 0) {
				type = 0;
				contenuLigne = strtok(NULL, " ");
				for (int i = 0; contenuLigne[i] != '\0'; i++) {
					if (contenuLigne[i] == '\n') {
					  	contenuLigne[i] = '\0';
					}
				}
				nom = contenuLigne;
				imprimante = creerImprimante(nom, type);
				if (nbImprimantes != 0) {
					imprimantes = (Imprimante *) realloc(imprimantes, sizeof(Imprimante)*(nbImprimantes+1));
					imprimantes[nbImprimantes++] = imprimante;
				}
			} else if (strcmp(&contenuLigne[0], "distante") == 0) {
				type = 1;
				contenuLigne = strtok(NULL, " ");
				for (int i = 0; contenuLigne[i] != '\0'; i++) {
					if (contenuLigne[i] == '\n') {
					  	contenuLigne[i] = '\0';
					}
				}
				nom = contenuLigne;
				imprimante = creerImprimante(nom, type);
				if (nbImprimantes != 0) {
					imprimantes = (Imprimante *) realloc(imprimantes, sizeof(Imprimante)*(nbImprimantes+1));
					imprimantes[nbImprimantes++] = imprimante;
				}
			}
		}	
		fclose(fichier);
	} else {
		fprintf(stderr, "%s/!\\ Le fichier configuration %s n'existe pas. /!\\%s", RED, CHEMIN_CONFIGURATION, RESET);
		exit(1);
	}
}

/* Creation d'une imprimante */
Imprimante creerImprimante(char* nom, int type) {
	Imprimante imprimante;
	
	imprimante.nom = nom;
	imprimante.type = type;
	if (type == 0){
		printf("\t- L'imprimante locale %s a ete detectee\n", imprimante.nom);
	} else {
		printf("\t- L'imprimante distante %s a ete detectee\n", imprimante.nom);
	}
	
	char* cheminImprimante = (char*) malloc(sizeof(char)*3 + sizeof(imprimante.nom));
	sprintf(cheminImprimante, "../imprimantes/%s", imprimante.nom);
	
	FILE* fichier = NULL;
	fichier = fopen(cheminImprimante, "w");
	if (fichier != NULL) {
		fputs("", fichier);
		fclose(fichier);
	}
	
	return imprimante;
}

/* Recherche d'une imprimante */
Imprimante trouverImprimante(char* nom) {}

/* Demarrage de l'imprimante */
void* demarrerImprimante(Imprimante imprimante) {}

/* Arret de l'imprimante */
void* arreterImprimante(Imprimante imprimante) {}

/* Traiter une demande d'impression */
void* imprimer(Imprimante imprimante, Requete requete) {}

/* Arret d'une demande d'impression */
void* arreterImpression(Imprimante imprimante) {}

/* Creation d'un serveur */
Serveur creerServeur() {}

/* Tache CUPS Scheduler */
void* cupsScheduler() {}

/* Tache CUPS Filter */
void* cupsFilter() {}

/* Tache CUPS Backend */
void* cupsBackend() {}
 
/* Tache Gestion Imprimante */
void* gestionImprimante() {}

int main(int argc, char** argv) {
	lectureConfiguration();
}
