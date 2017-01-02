/*
 ============================================================================
 Name        : utilisateur.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "utilisateur.h"
#include "../communication/communication.h"

#define NOM_SERVEUR "serveurImpression1"
 
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

void creerRequeteImpression(Requete * requete){
	TypeRequete type;
	char nomImprimante[255];

    printf("\n\n%s---------- Impression d'un fichier ----------\n%s", GRN, RESET);
    type = IMPRESSION;
    requete->type = type;
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
		
		requete->fichier = malloc(sizeof(char) * tailleFichier);
		requete->fichier = contenu;
		requete->tailleFichier = tailleFichier;
		requete->typeFichier = typeFichier;
	
		//TODO demander au serveur les imprimantes disponibles
	
		printf("Veuillez entrer le nom de l'imprimante sur laquelle imprimer le fichier : ");
		scanf("%s", nomImprimante);
		requete->nomImprimante = nomImprimante;
		int nbCopies = 0;
		while (nbCopies <= 0) {
			printf("Veuillez entrer le nombre de copies souhaitee (> 0) : ");
			scanf("%d", &nbCopies);
		}
		requete->nbCopies = nbCopies;
		int rectoVerso = -1;
		while (rectoVerso != 0 && rectoVerso != 1) {
			printf("Veuillez entrer le type d'impression souhaitee (0 = Recto, 1 = Recto/Verso) : ");
			scanf("%d", &rectoVerso);
		}
		requete->rectoVerso = rectoVerso;
	} else {
		printf("%s/!\\ Le chemin de fichier specifie ne pointe aucun fichier. /!\\%s", RED, RESET);
		sleep(1);
	}
}

void creerRequeteEtatImprimante(Requete * requete){
	TypeRequete type;
	char nomImprimante[255];

	printf("\n\n%s---------- Etat d'une imprimante ----------\n%s", GRN, RESET);
    type = ETAT_IMPRIMANTE;
    requete->type = type;
    printf("Veuillez entrer le nom de l'imprimante dont vous voulez l'etat : ");
	scanf("%s", nomImprimante);
	requete->nomImprimante = nomImprimante;
}

int envoyerRequete(Requete * requete){
	if (requete->type == IMPRESSION || requete->type == ETAT_IMPRESSION || requete->type == ANNULATION_IMPRESSION || requete->type == ETAT_IMPRIMANTE) {
        	int numCommunication;
        	int lgRequete;

		   	printf("\n\n%s---------- Envoi de la requete ----------\n%s", GRN, RESET);

        	// se connecter au serveur
			if ((numCommunication = demanderCommunication(NOM_SERVEUR)) < 0)
				{
				fprintf(stderr, "Client: erreur connexion %s\n", messageErreur((RetourCommunication)numCommunication));
				return 1;
				}

			// envoyer la requête
			lgRequete = sizeof(Requete);
		    if (envoyerOctets(numCommunication, requete, lgRequete) != lgRequete)
				{
				fprintf(stderr, "Client: erreur envoi de la requête %s\n", requete->type);
				return 2;
				}
		}
}

int main(int argc, char** argv) {
	Requete requete;
	TypeRequete type;
    int choixInterface;
	char nomImprimante[255];
    while (1) {
    	printf("\n%s__________________________________________________________________\n", YEL);
        printf("\n---------- Service d'impression et taches utilisatrices ---------- \n");
        printf("__________________________________________________________________%s\n\n", RESET);
        printf("\t%s1. Impression d'un fichier\n\t2. Etat d'avancement d'une impression\n\t3. Arret d'une impression\n\t4. Etat d'une imprimante\n%s", CYN, RESET);
        printf("\nVotre choix : ");
        scanf("%d", &choixInterface);
        requete.emetteur = getpid();
        switch (choixInterface) {
		    case 1 :
		    	creerRequeteImpression(&requete);
		        break;
		    case 2:
		        printf("\n\n%s---------- Etat d'avancement d'une impression ----------\n%s", GRN, RESET);
		        type = ETAT_IMPRESSION;
		        requete.type = type;
		        requete.fichier = NULL;
		        printf("Veuillez entrer le nom de l'imprimante sur laquelle vous voulez l'état de l'impression : ");
				scanf("%s", nomImprimante);
				requete.nomImprimante = nomImprimante;
				
		        //TODO creer requete de demande d'etat d'avancement d'une impression
		        
		        break;
		    case 3:
		   	 	printf("\n\n%s---------- Annulation d'une impression ----------\n%s", GRN, RESET);
		        type = ANNULATION_IMPRESSION;
		        requete.type = type;
		        
		        //TODO creer requete d'annulation d'une impression
		        
		        break;
		    case 4:
		    	creerRequeteEtatImprimante(&requete);
		        break;
		   default:
		   		printf("\n%s/!\\ Le choix effectue n'est pas attribue pour le moment. /!\\%s", RED, RESET);
				printf("\n");
				break;
        }
        
        int numErreur;
        if(numErreur = envoyerRequete(&requete)!=0){
			fprintf(stderr, "Client: erreur envoi de la requete %s\n", requete.type);
			return numErreur;
        }

			//TODO gérer la réponse du serveur
		    	
        
        choixInterface = 0;
    }
    return 0;
}


