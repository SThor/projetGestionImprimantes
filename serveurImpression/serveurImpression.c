/*
 ============================================================================
 Name        : serveurImpression.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "serveurImpression.h"

#define CHEMIN_CONFIGURATION "configuration"

Imprimante * imprimantes = NULL;
char nomServeur[50];
int nbImprimantes;

Moniteur moniteurScheduler;
Moniteur moniteurFilter;
Moniteur moniteurBackend;
Moniteur moniteurImpressions;

/* Initialisation de la configuration du systeme */
void initialiserConfiguration() {
	int i = 1;
	imprimantes = (Imprimante*) malloc(sizeof(Imprimante));
	nbImprimantes = 0;
	FILE* fichier = NULL;
    printf("\tRecherche du fichier de configuration \"%s\"...\n", CHEMIN_CONFIGURATION);
	fichier = fopen(CHEMIN_CONFIGURATION, "r");
	if (fichier != NULL) {
    	printf("\tLecture du fichier de configuration \"%s\"...\n", CHEMIN_CONFIGURATION);
		char ligne[256];
		
		while (fgets(ligne, sizeof ligne, fichier) != NULL) {
			Imprimante imprimante;
			char nomImprimante[50];
			int typeImprimante;
			switch (ligne[0]) {
				case 's' : //Serveur
					sscanf(&ligne[2], "%s", nomServeur);
					break;
				case 'l' : //Imprimante locale
					sscanf(&ligne[2], "%s", nomImprimante);
					i = 1;
					while (chercherImprimante(nomImprimante) != NULL) {
						sprintf(nomImprimante, "%s_%d", nomImprimante, i++);
					}
					strcpy(imprimante.nomImprimante, nomImprimante);
					imprimante.typeImprimante = 0;
					if (nbImprimantes != 0) {
						imprimantes = (Imprimante*) realloc(imprimantes, sizeof(Imprimante)*(nbImprimantes + 1));
					}
					imprimantes[nbImprimantes] = imprimante;
					nbImprimantes++;
					break;
				case 'd' : //Imprimante distante
					sscanf(&ligne[2], "%s", nomImprimante);
					i = 1;
					while (chercherImprimante(nomImprimante) != NULL) {
						sprintf(nomImprimante, "%s_%d", nomImprimante, i++);
					}
					strcpy(imprimante.nomImprimante, nomImprimante);
					imprimante.typeImprimante = 1;
					if (nbImprimantes != 0) {
						imprimantes = (Imprimante*) realloc(imprimantes, sizeof(Imprimante)*(nbImprimantes + 1));
					}
					imprimantes[nbImprimantes] = imprimante;
					nbImprimantes++;
					break;
			}
		}
		fclose(fichier);
	} else {
		fprintf(stderr, "%s/!\\ Le fichier configuration %s n'existe pas. /!\\%s\n", RED, CHEMIN_CONFIGURATION, RESET);
		exit(1);
	}
	printf("\tLes parametres du fichier de configuration ont bien ete appliques\n");
}

/* Initialisation du serveur d'impression */
int initialiserServeurImpression(char* nom) {
	printf("\tLancement du serveur d'impression %s\n", nom);
	int numServeur;
	unlink(nom);
	if ((numServeur = initialiserServeur(nom)) < 0) {
		fprintf(stderr, "%s/!\\ Erreur d'initialisation du serveur d'impression %s. /!\\%s\n", RED, nom, RESET);
		exit(1);
	}
	printf("\tLe %s (Numéro : %d) est opérationnel\n", nom, numServeur);
	return numServeur;
}

/* Creation d'une imprimante */
void ajouterImprimante(Imprimante imprimante) {
	char* cheminImprimante = (char*) malloc(sizeof(char)*3 + sizeof(imprimante.nomImprimante));
	sprintf(cheminImprimante, "imprimantes/%s", imprimante.nomImprimante);
	
	FILE* fichier = NULL;
	fichier = fopen(cheminImprimante, "w");
	if (fichier != NULL) {
		fputs("", fichier);
		fclose(fichier);
	}
	
	if (imprimante.typeImprimante == 0) {
		printf("\tL'imprimante locale %s a ete creee\n", imprimante.nomImprimante);
	} else {
		printf("\tL'imprimante distante %s a ete creee\n", imprimante.nomImprimante);
	}
}

/* Recherche d'une imprimante */
Imprimante* chercherImprimante(char* nom) {
	for (int i = 0; i < nbImprimantes; i++) {
		if (strcmp(nom, imprimantes[i].nomImprimante) == 0) {
			return &imprimantes[i];
		}
	}
	return NULL;
}

/* Demarrage des imprimantes locales et distantes */
void* demarrerImprimantesLocales(void* args) {
	
}

/* Demarrage des imprimantes distantes via le backend */
void* demarrerCUPSBackend(void* args) {
	
}

/* Demarrage des filtres */
void* demarrerCUPSFilter(void* args) {
	
}

/* Demarrage du scheduler */
void* demarrerCUPSScheduler(void* args) {
	int numServeur;
	Serveur * serveur = (Serveur*) args;
	numServeur = serveur->numServeur;
	int numCommunication;
	Requete requete;
	while(1) {
		if((numCommunication = accepterCommunication(numServeur)) < 0) {
			fprintf(stderr, "%sCUPS Scheduler : /!\\ Erreur d'acceptation de la communication. /!\\%s\n", RED, RESET);
			exit(1);
		}
		recevoirOctets(numCommunication, &requete, sizeof(Requete));
   		printf("\tCUPS Scheduler : Une nouvelle demande a ete recue (Emetteur : %d | ID demande : %d)\n", requete.emetteur, requete.idDemande);
   		//traiterRequete(requete, numCommunication);
   		cloreCommunication(numCommunication);
	}
}

/* Traiter une demande d'impression */
void imprimer(Imprimante imprimante, Requete requete) {
	
}

/* Arret d'une demande d'impression */
void arreterImpression(Imprimante imprimante) {
	
}
 
/* Tache Gestion Imprimante */
void gestionImprimante() {
	
}

int main(int argc, char** argv) {
	Serveur serveur;
	int numServeur;
	
	printf("\nInitialisation de la configuration\n");
	initialiserConfiguration();
	sleep(1);
	
	printf("\nInitialisation du serveur d'impression\n");
	numServeur = initialiserServeurImpression(nomServeur);
	strcpy(serveur.nomServeur, nomServeur);
	serveur.numServeur = numServeur;
	sleep(1);
	
	printf("\nInitialisation des moniteurs de file d'attentes\n");
	moniteurScheduler = creerMoniteur();
	moniteurFilter = creerMoniteur();
	moniteurBackend = creerMoniteur();
	moniteurImpressions = creerMoniteur();
	sleep(1);
	
	printf("\nInitialisation des threads\n");
	pthread_t * thd_Scheduler = (pthread_t *) malloc(FILE_SCHEDULER_MAX * sizeof(pthread_t));
	pthread_t * thd_Filter = (pthread_t *) malloc(FILE_FILTER_MAX * sizeof(pthread_t));
	pthread_t * thd_Backend = (pthread_t *) malloc(FILE_BACKEND_MAX * sizeof(pthread_t));
	pthread_t * thd_Imprimantes = (pthread_t *) malloc(NB_IMPRIMANTES_MAX * sizeof(pthread_t));
	sleep(1);
	
	int etatActuel;
	
	printf("\tDemarrage du CUPS Scheduler\n");
	if ((etatActuel = pthread_create(thd_Scheduler, NULL, demarrerCUPSScheduler, &serveur)) != 0) {
		fprintf(stderr, "%s/!\\ Erreur d'execution du CUPS Scheduler. /!\\%s\n", RED, RESET);
		exit(1);
	}
	printf("\tCUPS Scheduler en marche\n");
	sleep(1);
	
	printf("\tDemarrage des CUPS Filters\n");
	for(int i = 0; i < NB_FILTRES_MAX; i++){
		if ((etatActuel = pthread_create(&thd_Filter[i], NULL, demarrerCUPSFilter, NULL)) != 0) {
			fprintf(stderr, "%s/!\\ Erreur d'execution des CUPS Filters. /!\\%s\n", RED, RESET);
			exit(1);
		}
	}
	printf("\tCUPS Filters en marche\n");
	sleep(1);
	
	printf("\tDemarrage des CUPS Backends et des imprimantes locales\n");
	int* idImprimantes;
	idImprimantes = malloc(sizeof(int) * nbImprimantes);
	for(int i = 0; i < nbImprimantes; i++){
		idImprimantes[i] = i;
		if (imprimantes[i].typeImprimante == 1) {
			if ((etatActuel = pthread_create(&thd_Backend[i], NULL, demarrerCUPSBackend, &idImprimantes[i])) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'execution des CUPS Backend. /!\\%s\n", RED, RESET);
				exit(1);
			}
		} else if (imprimantes[i].typeImprimante == 0) {
			if ((etatActuel = pthread_create(&thd_Imprimantes[i], NULL, demarrerImprimantesLocales, &idImprimantes[i])) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'execution des imprimantes locales. /!\\%s\n", RED, RESET);
				exit(1);
			}
		}
	}
	printf("\tCUPS Backends et imprimantes locales en marche\n");
	
	while(1) {
		sleep(1);
	}
}
