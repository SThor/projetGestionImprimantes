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

/* Traitement de la requete */
void traiterRequete(Requete requete, int numCommunication) {
	printf("\tImprimante distante : Une demande d'impression a ete recue (Emetteur : %d | ID requete : %d)\n", requete.emetteur, requete.idRequete);
	switch (requete.type) {
		case IMPRESSION:
			printf("Traitement de la requete %d d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
			//traiterImpression(requete);
			//TODO Realiser le meme procede que sur le serveur d'impression, avec des depot et retraits sur le moniteur moniteurImprimanteDistante
			break;
		case ETAT_IMPRESSION:
			printf("Traitement de la requete %d d'etat d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
			//traiterEtatImpression(requete, numCommunication);
			break;
		case ANNULATION_IMPRESSION:
			printf("Traitement de la requete %d d'annulation d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
			//traiterAnnulationImpression(requete, numCommunication);
			break;
		case ETAT_IMPRIMANTE:
			printf("Traitement de la requete %d d'etat de %s\n", requete.idRequete, requete.nomImprimante); 
			//traiterEtatImprimante(requete, numCommunication);
			/* TODO Renvoyer l'etat
			int nbImpressions = getNbCasesRemplies(&moniteurImprimanteDistante);
			int nbImpressionsMax = getNbCases(&moniteurImprimanteDistante);
			EtatImprimante ei;
			if (nbImpressions == 0) {
				ei = VIDE;
			} else {
				if (nbImpressions < nbImpressionsMax) {
					ei = IMPRESSIONS_EN_COURS;
				} else {
					ei = PLEINE;
				}
			}
			if (envoyerOctets(numCommunication, &ei, sizeof(EtatImprimante)) != sizeof(EtatImprimante)) {
				fprintf(stderr, "%sImprimante distante : /!\\ Erreur d'envoi de la reponse a la requete n°%d : %s /!\\%s\n", RED, requete.idRequete, messageErreur((RetourCommunication)numCommunication), RESET);
			}*/
			break;
	}
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
	moniteurImprimanteDistante = creerMoniteur();
	
	while (1) {
		while((numCommunication = accepterCommunication(numServeur)) >= 0) {
			recevoirOctets(numCommunication, &requete, sizeof(Requete));
	   		traiterRequete(requete, numCommunication);
	   		cloreCommunication(numCommunication);
		}
	}
	
	arreterServeur(numServeur);
}
