/*
 ============================================================================
 Name        : imprimanteDistante.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "imprimanteDistante.h"

Moniteur moniteurImprimanteDistante;

/* Creation d'une imprimante */
int creerImprimante(char* nom) {
	int numServeur;
	printf("Creation de l'imprimante distante\n");
	unlink(nom);
	
	char* cheminImprimante = (char*) malloc(sizeof(char)*28 + sizeof(int));
	sprintf(cheminImprimante, "imprimantes/%s", nom);
	
	FILE* fichier = NULL;
	fichier = fopen(cheminImprimante, "w");
	if (fichier != NULL) {
		fputs("", fichier);
		fclose(fichier);
	}
	
	if ((numServeur = initialiserServeur(nom)) < 0) {
		fprintf(stderr, "%sImprimante distante : /!\\ Erreur d'execution de l'imprimante distante %s : %s /!\\%s\n", RED, nom, messageErreur(numServeur), RESET);
		exit(1);
	}
	
	return numServeur;
}

/* Traiter une demande d'impression */
void lancerImpression(Requete requete) {
	char* cheminImprimante = (char*) malloc(sizeof(char)*28 + sizeof(int));
	sprintf(cheminImprimante, "imprimantes/%s", requete.nomImprimante);
	FILE* fichier = NULL;
	fichier = fopen(cheminImprimante, "w");
	
	printf("\tImprimante distante %s : Impression de la requete %d\n", requete.nomImprimante, requete.idRequete);
			
	if (fichier != NULL) {
		fprintf(fichier, "%s", requete.fichier);
		fclose(fichier);
	}
}

/* Annuler d'une demande d'impression */
void annulerImpression() {
	//TODO Arreter l'impression
}

/* Etat d'une demande d'impression */
void etatImpression() {
	//TODO Retourner etat de l'impression
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage Imprimante Distante : %s nomImprimante\n", argv[0]);
		exit(1);
	}
	int numServeur;
	int numCommunication;
	numServeur = creerImprimante(argv[1]);
	Requete requete;
	while (1) {
		while((numCommunication = accepterCommunication(numServeur)) >= 0) {
			recevoirOctets(numCommunication, &requete, sizeof(Requete));
	   		printf("\tImprimante distante : Une demande d'impression a ete recue (Emetteur : %d | ID requete : %d)\n", requete.emetteur, requete.idRequete);
	   		lancerImpression(requete);
	   		cloreCommunication(numCommunication);
		}
	}
	
	arreterServeur(numServeur);
}
