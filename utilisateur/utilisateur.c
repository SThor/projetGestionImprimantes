/*
 ============================================================================
 Name        : utilisateur.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "utilisateur.h"

int nbRequetes;
 
long recupererTailleFichier(const char* cheminFichier) { 
   struct stat s; 
 
   if (stat(cheminFichier,&s) != 0) { 
      printf("Erreur de récupération de la taille du fichier.\n"); 
      return 0; 
   }
 
   return s.st_size;
} 
 
char* recupererTypeFichier(const char* cheminFichier) {
	char *point = strrchr(cheminFichier, '.');
    if(!point || point == cheminFichier) return "";
    return point + 1;
}

void imprimerRequete(Requete requete){
	printf("Emetteur : %d\n", requete.emetteur);
	printf("idRequete : %d\n", requete.idRequete);
	char typeRequete[100];
	switch (requete.type){
		case IMPRESSION:
			strcpy(typeRequete, "Impression");
			break;
		case ETAT_IMPRESSION:
			strcpy(typeRequete, "Etat d'une impression");
			break;
		case ANNULATION_IMPRESSION:
			strcpy(typeRequete, "Arret d'une impression");
			break;
		case ETAT_IMPRIMANTE:
			strcpy(typeRequete, "Etat d'une imprimante");
			break;
		default:
			strcpy(typeRequete, "Type de requete inconnu");
			break;
	}
	printf("Type de requete : %s\n", typeRequete);
	printf("Imprimante : %s\n", requete.nomImprimante);
}

int envoyerRequete(Requete requete, int numCommunication){
	printf("\n\n%s---------- Envoi de la requete ----------\n%s", GRN, RESET);
    int lgRequete;
	imprimerRequete(requete);
	
	// envoyer la requête
	lgRequete = sizeof(Requete);
	if (envoyerOctets(numCommunication, &requete, lgRequete) != lgRequete) {
		fprintf(stderr, "Client : %s/!\\ Erreur d'envoi de la requete %d : %s /!\\%s\n", RED, requete.idRequete, messageErreur((RetourCommunication)numCommunication), RESET);
		return 2;
	}
	
	return 0;
}

int etablirIdRequete(pid_t emetteur, int nbRequetes) {
 	int puissance = 10;
    while (nbRequetes >= puissance) {
    	puissance *= 10;
    }
    int resultat = emetteur * puissance + nbRequetes;
    return resultat;
}

int main(int argc, char** argv) {
	Requete requete;
	nbRequetes = 0;
    while (1) {
    	int choixInterface;
    	printf("\n%s__________________________________________________________________\n", YEL);
        printf("\n---------- Service d'impression et taches utilisatrices ---------- \n");
        printf("__________________________________________________________________%s\n\n", RESET);
        printf("\t%s1. Impression d'un fichier\n\t2. Etat d'avancement d'une impression\n\t3. Arret d'une impression\n\t4. Etat d'une imprimante\n%s", CYN, RESET);
        printf("\nVotre choix : ");
        scanf("%d", &choixInterface);
        requete.emetteur = getpid();
        requete.idRequete = etablirIdRequete(requete.emetteur, nbRequetes);
        int res;
		TypeRequete type;
		char nomImprimante[50];
        switch (choixInterface) {
		    case 1 :
				printf("\n\n%s---------- Impression d'un fichier ----------\n%s", GRN, RESET);
				type = IMPRESSION;
				requete.type = type;
				FILE* fichier = NULL;
				char cheminFichier[255];
				long tailleFichier = 0;
				char* typeFichier = "";
				int caractereActuel = 0;
				char* contenu = "";
				printf("Veuillez entrer le chemin vers le fichier a imprimer : ");
				scanf("%s", cheminFichier);
				fichier = fopen(cheminFichier, "r");
				if (fichier != NULL) {
					tailleFichier = recupererTailleFichier(cheminFichier);
					typeFichier = recupererTypeFichier(cheminFichier);
					contenu = malloc(sizeof(char) * tailleFichier);
					for (int i = 0; i < tailleFichier - 1; i++){
						caractereActuel = fgetc(fichier);
						contenu[i] = caractereActuel;
					}
		
					fclose(fichier);
		
					requete.nomFichier = malloc(sizeof(char) * sizeof(cheminFichier));
					strcpy(requete.nomFichier, cheminFichier);
		
					requete.fichier = malloc(sizeof(char) * tailleFichier);
					requete.fichier = contenu;
					requete.tailleFichier = tailleFichier;
					requete.typeFichier = typeFichier;
	
					//TODO amelioration : demander au serveur les imprimantes disponibles
	
					printf("Veuillez entrer le nom de l'imprimante : ");
					scanf("%s", nomImprimante);
					strcpy(requete.nomImprimante, nomImprimante);
					int nbCopies = 0;
					while (nbCopies <= 0) {
						printf("Veuillez entrer le nombre de copies (> 0) : ");
						scanf("%d", &nbCopies);
					}
					requete.nbCopies = nbCopies;
					int rectoVerso = -1;
					while (rectoVerso != 0 && rectoVerso != 1) {
						printf("Veuillez entrer le type d'impression (0 = Recto, 1 = Recto/Verso) : ");
						scanf("%d", &rectoVerso);
					}
					requete.rectoVerso = rectoVerso;
					res = 1;
				} else {
					printf("%s/!\\ Le chemin de fichier specifie ne pointe aucun fichier. /!\\%s\n", RED, RESET);
					res = 0;
				}
		        break;
		    case 2:
				printf("\n\n%s---------- Etat d'avancement d'une impression ----------\n%s", GRN, RESET);
				type = ETAT_IMPRESSION;
				requete.type = type;
				requete.fichier = NULL;
				printf("Veuillez entrer le nom de l'imprimante : ");
				scanf("%s", nomImprimante);
				strcpy(requete.nomImprimante, nomImprimante);
	
				//TODO Completer la requete avec les informations de l'impression
	
				if(strcmp(requete.nomImprimante, "") == 0) {
					res = 0;
				} else {
					res = 1;
				}
		        break;
		    case 3:
				printf("\n\n%s---------- Annulation d'une impression ----------\n%s", GRN, RESET);
				type = ANNULATION_IMPRESSION;
				requete.type = type;
				requete.fichier = NULL;
				printf("Veuillez entrer le nom de l'imprimante : ");
				scanf("%s", nomImprimante);
				strcpy(requete.nomImprimante, nomImprimante);
	
				//TODO Completer la requete avec les informations de l'impression
	
				if(strcmp(requete.nomImprimante, "") == 0) {
					res = 0;
				} else {
					res = 1;
				}
		        break;
		    case 4:
				printf("\n\n%s---------- Etat d'une imprimante ----------\n%s", GRN, RESET);
				type = ETAT_IMPRIMANTE;
				requete.type = type;
				printf("Veuillez entrer le nom de l'imprimante : ");
				scanf("%s", nomImprimante);
				strcpy(requete.nomImprimante, nomImprimante);
	
				if(strcmp(requete.nomImprimante, "") == 0) {
					res = 0;
				} else {
					res = 1;
				}
		        break;
		   	default:
		   		printf("\n%s/!\\ Le choix effectue n'est pas attribue pour le moment. /!\\%s\n", RED, RESET);
				printf("\n");
				break;
        }
        
		int numCommunication;

		// se connecter au serveur
		if ((numCommunication = demanderCommunication(NOM_SERVEUR)) < 0) {
			fprintf(stderr, "Client : %s/!\\ Erreur de connexion au serveur %s : %s /!\\%s\n", RED, NOM_SERVEUR, messageErreur((RetourCommunication)numCommunication), RESET);
			return 1;
		}
        
        if(res != 0) {
		    if (requete.type == IMPRESSION || requete.type == ETAT_IMPRESSION || requete.type == ANNULATION_IMPRESSION || requete.type == ETAT_IMPRIMANTE) {
		    	int numErreur;
				if ((numErreur = envoyerRequete(requete, numCommunication)) != 0){
					fprintf(stderr, "Client : %s/!\\ Erreur d'envoi de la requete %d /!\\%s\n", RED, requete.idRequete, RESET);
					return numErreur;
				}
			}
		}
		
		printf("\n\n%s---------- Reception reponse ----------\n%s", GRN, RESET);
		
		EtatImprimante eimprim;
		switch (requete.type) {
		    case IMPRESSION:
		    	//TODO Reception reponse impression
		        break;
		    case ETAT_IMPRESSION:
		        //TODO Reception reponse etat impression
		        break;
		    case ANNULATION_IMPRESSION:
		   	 	//TODO Reception reponse annulation impression
		        break;
		    case ETAT_IMPRIMANTE:
		    	recevoirOctets(numCommunication, &eimprim, sizeof(EtatImprimante));
   				cloreCommunication(numCommunication);
   				if(eimprim != -1){
					switch (eimprim) {
						case IMPRESSIONS_EN_COURS:
							printf("L'imprimante \"%s\" traite actuellement des impressions\n", requete.nomImprimante);
							break;
						case VIDE:
							printf("L'imprimante \"%s\" est actuellement VIDE\n", requete.nomImprimante);
							break;
						case PLEINE:
					   	 	printf("L'imprimante \"%s\" est actuellement PLEINE\n", requete.nomImprimante);
							break;
					}
		    	} else {
		    		printf("L'imprimante n'existe pas");
		    	}
		        break;
		   	default:
				break;
        }

		//TODO gérer la réponse du serveur
    }
    return 0;
}
