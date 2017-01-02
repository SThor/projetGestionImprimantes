/*
 ============================================================================
 Name        : client.c
 Author      : Amal Sayah
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../communication/communication.h"

int client(const char *serveur, const char *fichierDemande)
{
	int numCommunication;
	int nbRecus;
	char tampon[BUFSIZ];
	int lgChemin;
	int nbEnvoyes;

	// se connecter au serveur
	if ((numCommunication = demanderCommunication(serveur)) < 0)
		{
		fprintf(stderr, "Client: erreur %s\n", messageErreur((RetourCommunication)numCommunication));
		return 1;
		}

	// envoyer le path du fichier demandé
	lgChemin = strlen(fichierDemande)+1;
    if (envoyerOctets(numCommunication, fichierDemande, lgChemin) != lgChemin)
		{
		fprintf(stderr, "Client: erreur envoi path %s\n", fichierDemande);
		return 2;
		}

	// recevoir ensuite le contenu du fichier s'il existe
	while ((nbRecus = recevoirOctets(numCommunication, tampon, BUFSIZ)) > 0)
		write(1, tampon, nbRecus);

    // clore la communication
	cloreCommunication(numCommunication);

	return 0;
}

int main(int argc, char *argv[])
{
  int ret;
  if (argc != 3)
  	  {
	  fprintf(stderr, "usage: %s nom_serveur chemin_fichier", argv[0]);
	  exit(2);
  	  }
  ret = client(argv[1], argv[2]);
  exit(ret);
}
