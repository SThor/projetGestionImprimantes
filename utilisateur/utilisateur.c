/*
 ============================================================================
 Name        : utilisateur.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "utilisateur.h"
 
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

int main(int argc, char** argv) {
	Requete requete;
	TypeRequete type;
    int choixInterface;
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
		        printf("\n\n%s---------- Impression d'un fichier ----------\n%s", GRN, RESET);
		        type = IMPRESSION;
		        requete.type = type;
				FILE* fichier = NULL;
		        char cheminFichier[256];
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
					
					requete.fichier = malloc(sizeof(char) * tailleFichier);
					requete.fichier = contenu;
					requete.tailleFichier = tailleFichier;
					requete.typeFichier = typeFichier;
				
					//TODO demander au serveur les imprimantes disponibles
				
					printf("Veuillez entrer le nom de l'imprimante sur laquelle imprimer le fichier : ");
					char* nomImprimante;
					scanf("%s", nomImprimante);
					requete.nomImprimante = nomImprimante;
					int nbCopies = 0;
					while (nbCopies <= 0) {
						printf("Veuillez entrer le nombre de copies souhaitee (> 0) : ");
						scanf("%d", &nbCopies);
					}
					requete.nbCopies = nbCopies;
					int rectoVerso = -1;
					while (rectoVerso != 0 && rectoVerso != 1) {
						printf("Veuillez entrer le type d'impression souhaitee (0 = Recto, 1 = Recto/Verso) : ");
						scanf("%d", &rectoVerso);
					}
					requete.rectoVerso = rectoVerso;
				} else {
					printf("%s/!\\ Le chemin de fichier specifie ne pointe aucun fichier. /!\\%s", RED, RESET);
					sleep(1);
				}
		        break;
		    case 2:
		        printf("\n\n%s---------- Etat d'avancement d'une impression ----------\n%s", GRN, RESET);
		        type = ETAT_IMPRESSION;
		        requete.type = type;
		        requete.fichier = NULL;
		        requete.nomImprimante = "";
		        
		        //TODO creer requete de demande d'etat d'avancement d'une impression
		        
		        break;
		    case 3:
		   	 	printf("\n\n%s---------- Annulation d'une impression ----------\n%s", GRN, RESET);
		        type = ANNULATION_IMPRESSION;
		        requete.type = type;
		        
		        //TODO creer requete d'annulation d'une impression
		        
		        break;
		    case 4:
		   	 	printf("\n\n%s---------- Etat d'une imprimante ----------\n%s", GRN, RESET);
		        type = ETAT_IMPRIMANTE;
		        requete.type = type;
		        
		        //TODO creer requete de demande d'etat d'une imprimante
		        
		        break;
		   default:
		   		printf("\n%s/!\\ Le choix effectue n'est pas attribue pour le moment. /!\\%s", RED, RESET);
				printf("\n");
				break;
        }
        
        if (requete.type == IMPRESSION || requete.type == ETAT_IMPRESSION || requete.type == ANNULATION_IMPRESSION || requete.type == ETAT_IMPRIMANTE) {
        	//TODO effectuer l'envoi de la requete
        	//demanderCommunication (attendre l'acceptation du serveur)
        	//envoyerOctets
        }
        
        choixInterface = 0;
    }
    return 0;
}


