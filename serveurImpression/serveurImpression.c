/*
 ============================================================================
 Name        : serveurImpression.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "serveurImpression.h"

Imprimante * imprimantes = NULL;
char nomServeur[50];
int nbImprimantes;
int nbFiltres;

Moniteur moniteurFilter;
Moniteur moniteurBackend;
Moniteur* moniteurImprimanteLocale;

pthread_t * thd_Scheduler;
pthread_t * thd_Filter;
pthread_t * thd_Backend;
pthread_t * thd_Imprimantes;

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
			switch (ligne[0]) {
				case 's' : //Serveur
					sscanf(&ligne[2], "%s", nomServeur);
					break;
				case 'l' : //Imprimante locale
					sscanf(&ligne[2], "%s", nomImprimante);
					i = 1;
					while (chercherIdImprimante(nomImprimante) != -1) {
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
					while (chercherIdImprimante(nomImprimante) != -1) {
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
		fprintf(stderr, "%s/!\\ Erreur d'initialisation du serveur d'impression %s : %s /!\\%s\n", RED, nom, messageErreur(numServeur), RESET);
		exit(1);
	}
	printf("\tLe %s (Numéro : %d) est opérationnel\n", nom, numServeur);
	return numServeur;
}

/* Recherche d'une imprimante */
Imprimante* chercherImprimante(char* nom) {
	for (int i = 0; i < nbImprimantes; i++) {
		if ((strcmp(imprimantes[i].nomImprimante, nom)) == 0) {
			return &imprimantes[i];
		}
	}
	return NULL;
}

/* Recherche du numéro d'une imprimante */
int chercherIdImprimante(char* nom) {
	for (int i = 0; i < nbImprimantes; i++) {
		if (strcmp(nom, imprimantes[i].nomImprimante) == 0) {
			if (i < nbImprimantes) {
				return i;
			}
		}
	}
	return -1;
}

/* imprimante locale */
void* imprimanteLocale(void* args) {
	Requete requete;
	int numeroImprimante = *(int*) args;
	char* cheminImprimante = (char*) malloc(sizeof(char)*28 + sizeof(int));
	int numeroIncremente = numeroImprimante+1;
	sprintf(cheminImprimante, "imprimantes/imprimante%d", numeroIncremente);
	
	FILE* fichier = NULL;
	fichier = fopen(cheminImprimante, "w");
	
	while(1) {
		requete = retirer(&moniteurImprimanteLocale[numeroImprimante]);
		int idImprimante;
		idImprimante = chercherIdImprimante(requete.nomImprimante);
		if (idImprimante != -1) {
			printf("\tImprimante locale %s : Impression de la requete %d\n", requete.nomImprimante, requete.idRequete);
			
			if (fichier != NULL) {
				fprintf(fichier, "%s", requete.fichier);
				fclose(fichier);
			}
		}
	}
	pthread_exit(NULL);
}

/* backend */
void* cupsBackend(void* args) {
	Requete requete;
	int numeroBackend = *(int*) args;
	int numCommunication;
	while(1) {
		requete = retirer(&moniteurBackend);
		int idImprimante;
		idImprimante = chercherIdImprimante(requete.nomImprimante);
		if (idImprimante != -1) {
			printf("\tCUPS Backend n°%d : Envoi de la requete %d a l'imprimante %d\n", numeroBackend, requete.idRequete, idImprimante);
			Imprimante* imprimante = chercherImprimante(requete.nomImprimante);
			if (imprimante->typeImprimante == 0) {
				printf("TEST\n");
				deposer(&moniteurImprimanteLocale[idImprimante], requete);
			} else {
				while (1) {
					if ((numCommunication = demanderCommunication(requete.nomImprimante)) < 0) {
						fprintf(stderr, "%sCUPS Scheduler : /!\\ Erreur de demande de communication avec imprimante %s : %s /!\\%s\n", RED, requete.nomImprimante, messageErreur((RetourCommunication)numCommunication), RESET);
						deposer(&moniteurBackend, requete);
					}
					if (envoyerOctets(numCommunication, &requete, sizeof(Requete)) != sizeof(Requete)) {
						fprintf(stderr, "%sCUPS Scheduler : /!\\ Erreur d'envoi de la requete n°%d : %s /!\\%s\n", RED, requete.idRequete, messageErreur((RetourCommunication)numCommunication), RESET);
						deposer(&moniteurBackend, requete);
					}
	 				cloreCommunication(numCommunication);
	 			}
			}
		}
	}
	pthread_exit(NULL);
}

/* Application du filtre sur la requete */
void filtrerRequete(Requete requete) {	
	char* fichier;
	printf("Transformation du fichier %s de type %s pour l'impression de la requete %d sur %s\n", requete.nomFichier, requete.typeFichier, requete.idRequete, requete.nomImprimante);
	if (strcmp(requete.typeFichier, "txt") == 0) {
		fichier = (char*) malloc(sizeof(char)*requete.tailleFichier); 
		sprintf(fichier, "EN-TETE POUR FICHIER TXT\n%s", requete.fichier);
		memcpy(requete.fichier, fichier, requete.tailleFichier + sizeof(char)*28);
	} else if (strcmp(requete.typeFichier, "png") == 0) {
		fichier = (char*) malloc(sizeof(char)*28 + requete.tailleFichier); 
		sprintf(fichier, "EN-TETE POUR FICHIER PNG\n%s", requete.fichier);
		memcpy(requete.fichier, fichier, requete.tailleFichier + sizeof(char)*28);
	} else if (strcmp(requete.typeFichier, "pdf") == 0) {
		fichier = (char*) malloc(sizeof(char)*28 + requete.tailleFichier); 
		sprintf(fichier, "EN-TETE POUR FICHIER PDF\n%s", requete.fichier);
		memcpy(requete.fichier, fichier, requete.tailleFichier + sizeof(char)*28);
	}
}

/* filter */
void* cupsFilter(void* args) {
	Requete requete;
	int numeroRequete;
	int numeroFiltre = *(int*) args;
	while(1) {
		requete = retirer(&moniteurFilter);
		printf("\tCUPS Filter n°%d : Filtrage de la requete %d\n", numeroFiltre, requete.idRequete);
		numeroRequete = chercherIdImprimante(requete.nomImprimante);
		if (numeroRequete != 1) {
   			printf("\tCUPS Filter n°%d : ", numeroFiltre);
			filtrerRequete(requete);
			deposer(&moniteurBackend, requete);
		}
	}
	pthread_exit(NULL);
}

/* Authentification de l'utilisateur */
int authentifierUtilisateur(pid_t nomUtilisateur) {
	int i = 1; //Toujours authentifie
	if (i == 1) {
		// Cas authentifie
		return i;
	} else {
		// Cas non authentifie
		return i;
	}
}

/* Traitement des requetes d'impression */
void traiterImpression(Requete requete) {
	deposer(&moniteurFilter, requete);
}

/* Traitement des requetes d'etat d'impression */
void traiterEtatImpression(Requete requete, int numCommunication) {
	deposer(&moniteurBackend, requete);
}

/* Traitement des requetes d'annulation d'impression */
void traiterAnnulationImpression(Requete requete, int numCommunication) {
	deposer(&moniteurBackend, requete);
}

/* Traitement des requetes d'etat d'imprimante */
void traiterEtatImprimante(Requete requete, int numCommunication) {
	int numeroImprimante;
	int nbImpressions;
	int nbImpressionsMax;
	Imprimante* imprimante = chercherImprimante(requete.nomImprimante);
	numeroImprimante = chercherIdImprimante(requete.nomImprimante);
	if (numeroImprimante != -1) {
		if (imprimante->typeImprimante == 0) {
			// Cas ou l'imprimante est locale
			nbImpressions = getNbCasesRemplies(&moniteurImprimanteLocale[numeroImprimante]);
			nbImpressionsMax = getNbCases(&moniteurImprimanteLocale[numeroImprimante]);
		} else {
			// Cas ou l'imprimante est distante
			deposer(&moniteurBackend, requete);
		}
	
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
			fprintf(stderr, "%sCUPS Scheduler : /!\\ Erreur d'envoi de la reponse a la requete n°%d : %s /!\\%s\n", RED, requete.idRequete, messageErreur((RetourCommunication)numCommunication), RESET);
		}
	} else {
		EtatImprimante ei = -1;
		if (envoyerOctets(numCommunication, &ei, sizeof(EtatImprimante)) != sizeof(EtatImprimante)) {
			fprintf(stderr, "%sCUPS Scheduler : /!\\ Erreur d'envoi de la reponse a la requete n°%d : %s /!\\%s\n", RED, requete.idRequete, messageErreur((RetourCommunication)numCommunication), RESET);
		}
	}
}

/* Traitement de la requete */
void traiterRequete(Requete requete, int numCommunication) {
	if (authentifierUtilisateur(requete.emetteur) != 0) {
		switch (requete.type) {
			case IMPRESSION:
				printf("Traitement de la requete %d d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
				traiterImpression(requete);
				break;
			case ETAT_IMPRESSION:
				printf("Traitement de la requete %d d'etat d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
				traiterEtatImpression(requete, numCommunication);
				break;
			case ANNULATION_IMPRESSION:
				printf("Traitement de la requete %d d'annulation d'impression sur %s\n", requete.idRequete, requete.nomImprimante);
				traiterAnnulationImpression(requete, numCommunication);
				break;
			case ETAT_IMPRIMANTE:
				printf("Traitement de la requete %d d'etat de %s\n", requete.idRequete, requete.nomImprimante); 
				traiterEtatImprimante(requete, numCommunication);
				break;
		}
	}
}

/* scheduler */
void* cupsScheduler(void* args) {
	int numServeur;
	Serveur * serveur = (Serveur*) args;
	numServeur = serveur->numServeur;
	int numCommunication;
	Requete requete;
	while((numCommunication = accepterCommunication(numServeur)) >= 0) {
		recevoirOctets(numCommunication, &requete, sizeof(Requete));
   		printf("\tCUPS Scheduler : Une nouvelle requete a ete recue (Emetteur : %d | ID requete : %d)\n", requete.emetteur, requete.idRequete);
   		printf("\tCUPS Scheduler : ");
   		traiterRequete(requete, numCommunication);
   		cloreCommunication(numCommunication);
	}
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	Serveur serveur;
	int numServeur;
	
	printf("\nInitialisation de la configuration\n");
	initialiserConfiguration();
	
	printf("\nInitialisation du serveur d'impression\n");
	numServeur = initialiserServeurImpression(nomServeur);
	strcpy(serveur.nomServeur, nomServeur);
	serveur.numServeur = numServeur;
	
	printf("\nInitialisation des moniteurs de file d'attentes\n");
	moniteurFilter = creerMoniteur();
	moniteurBackend = creerMoniteur();
	int nbImprimantesLocales = 0;
	for (int i = 0; i < nbImprimantes; i++){
		if (imprimantes[i].typeImprimante == 0) {
			nbImprimantesLocales++;
		}
	}
	moniteurImprimanteLocale = (Moniteur*) malloc(sizeof(Moniteur) * nbImprimantesLocales);
	for (int i = 0; i < nbImprimantes; i++){
		if (imprimantes[i].typeImprimante == 0) {
			moniteurImprimanteLocale[i] = creerMoniteur();
		}
	}
	
	printf("\nInitialisation des threads\n");
	thd_Scheduler = (pthread_t *) malloc(sizeof(pthread_t));
	thd_Filter = (pthread_t *) malloc(NB_FILTERS_MAX * sizeof(pthread_t));
	thd_Backend = (pthread_t *) malloc(NB_BACKENDS_MAX * sizeof(pthread_t));
	thd_Imprimantes = (pthread_t *) malloc(NB_IMPRIMANTES_MAX * sizeof(pthread_t));
	
	int etatActuel;
	
	//Demarrages
	printf("\tDemarrage du CUPS Scheduler\n");
	if ((etatActuel = pthread_create(thd_Scheduler, NULL, cupsScheduler, &serveur)) != 0) {
		fprintf(stderr, "%s/!\\ Erreur d'execution du CUPS Scheduler. /!\\%s\n", RED, RESET);
		exit(1);
	}
	printf("\tCUPS Scheduler en marche\n");
	
	printf("\tDemarrage des CUPS Filters\n");
	int* idFiltres;
	idFiltres = malloc(sizeof(int) * NB_FILTERS_MAX);
	for (int i = 0; i < NB_FILTERS_MAX; i++){
		idFiltres[i] = i;
		if ((etatActuel = pthread_create(&thd_Filter[i], NULL, cupsFilter, &idFiltres[i])) != 0) {
			fprintf(stderr, "%s/!\\ Erreur d'execution des CUPS Filters. /!\\%s\n", RED, RESET);
			exit(1);
		}
	}
	printf("\tCUPS Filters en marche\n");
	
	printf("\tDemarrage des CUPS Backends et des imprimantes locales\n");
	int* idImprimantes;
	idImprimantes = malloc(sizeof(int) * nbImprimantes);
	for (int i = 0; i < nbImprimantes; i++){
		idImprimantes[i] = i;
		if (imprimantes[i].typeImprimante == 1) {
			if ((etatActuel = pthread_create(&thd_Backend[i], NULL, cupsBackend, &idImprimantes[i])) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'execution des CUPS Backend. /!\\%s\n", RED, RESET);
				exit(1);
			}
		} else if (imprimantes[i].typeImprimante == 0) {
			if ((etatActuel = pthread_create(&thd_Imprimantes[i], NULL, imprimanteLocale, &idImprimantes[i])) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'execution des imprimantes locales. /!\\%s\n", RED, RESET);
				exit(1);
			}
		}
	}
	printf("\tCUPS Backends et imprimantes locales en marche\n");
	
	while(1) {
	
	}
	
	printf("\tArret du CUPS Scheduler\n");
	if ((etatActuel = pthread_join(*thd_Scheduler, NULL)) != 0) {
		fprintf(stderr, "%s/!\\ Erreur d'extinction du CUPS Scheduler. /!\\%s\n", RED, RESET);
		exit(1);
	}
	printf("\tCUPS Scheduler correctement arrete\n");
	
	printf("\tArret des CUPS Filters\n");
	for (int i = 0; i < NB_FILTERS_MAX; i++){
		if ((etatActuel = pthread_join(thd_Filter[i], NULL)) != 0) {
			fprintf(stderr, "%s/!\\ Erreur d'extinction des CUPS Filters. /!\\%s\n", RED, RESET);
			exit(1);
		}
	}
	printf("\tCUPS Filters correctement arrete\n");
	
	printf("\tArret des CUPS Backends et des imprimantes locales\n");
	for (int i = 0; i < nbImprimantes; i++){
		if (imprimantes[i].typeImprimante == 1) {
			if ((etatActuel = pthread_join(thd_Backend[i], NULL)) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'extinction des CUPS Backend. /!\\%s\n", RED, RESET);
				exit(1);
			}
		} else if (imprimantes[i].typeImprimante == 0) {
			if ((etatActuel = pthread_join(thd_Imprimantes[i], NULL)) != 0) {
				fprintf(stderr, "%s/!\\ Erreur d'extinction des imprimantes locales. /!\\%s\n", RED, RESET);
				exit(1);
			}
		}
	}
	printf("\tCUPS Backends et imprimantes locales correctement arrete\n");
	printf("\tArret general\n");
	arreterServeur(numServeur);
	exit(EXIT_SUCCESS);
}
