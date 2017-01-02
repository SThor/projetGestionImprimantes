/*
 ============================================================================
 Name        : tamponPartage.c
 Authors     : Al Masri Safwan & Givel Paul
 ============================================================================
 */
 
#include "tamponPartage.h"

/* Creation d'un moniteur suivant le modele producteurs/consommateurs */
Moniteur creerMoniteur() {
	Moniteur moniteur;
	
	moniteur.nbCases = NB_CASES_MAX;
	moniteur.nbCasesVides = NB_CASES_MAX;
	moniteur.indiceProchainDepot = 0;
	moniteur.indiceProchainRetrait = 0;
	pthread_cond_init(&moniteur.condCasesVides, NULL);
	pthread_cond_init(&moniteur.condCasesPleines, NULL);
	pthread_mutex_init(&moniteur.mutex, NULL);
	
	return moniteur;
}

/* Synchronisation sur le debut du depot */
int debutDeposer(Moniteur* moniteur) {
	pthread_mutex_lock(&moniteur->mutex);
	
	while(moniteur->nbCasesVides == 0){
		pthread_cond_wait(&moniteur->condCasesVides, &moniteur->mutex);
	}
	int indiceDepot = moniteur->indiceProchainDepot;
	int indiceIncremente = moniteur->indiceProchainDepot + 1;
	moniteur->indiceProchainDepot = indiceIncremente % moniteur->nbCases;
	
	pthread_mutex_unlock(&moniteur->mutex);
	
	return indiceDepot;
}

/* Synchronisation sur la fin du depot */
void finDeposer(Moniteur *moniteur) {
	pthread_mutex_lock(&moniteur->mutex);
	
	moniteur->nbCasesVides--;
	pthread_cond_signal(&moniteur->condCasesPleines);
	
	pthread_mutex_unlock(&moniteur->mutex);
}

/* Synchronisation sur le debut du retrait */
int debutRetirer(Moniteur *moniteur) {
	pthread_mutex_lock(&moniteur->mutex);
	
	while(moniteur->nbCasesVides == moniteur->nbCases){
		pthread_cond_wait(&moniteur->condCasesPleines, &moniteur->mutex);
	}
	int indiceRetrait = moniteur->indiceProchainRetrait;
	int indiceIncremente = moniteur->indiceProchainRetrait + 1;
	moniteur->indiceProchainRetrait = indiceIncremente % moniteur->nbCases;
	
	pthread_mutex_unlock(&moniteur->mutex);
	
	return indiceRetrait;	
}

/* Synchronisation sur la fin du retrait */
void finRetirer(Moniteur *moniteur) {
	pthread_mutex_lock(&moniteur->mutex);
	
	moniteur->nbCasesVides++;
	pthread_cond_signal(&moniteur->condCasesVides);
	
	pthread_mutex_unlock(&moniteur->mutex);
} 

/* Depot synchronise */
void deposer(Moniteur *moniteur, void *buffer, int size) {
	int indiceDepot = debutDeposer(moniteur);
	
	moniteur->message[indiceDepot].buffer = malloc(size);
	moniteur->message[indiceDepot].size = size;
	memcpy(moniteur->message[indiceDepot].buffer, buffer, size);
	
	finDeposer(moniteur);
}

/* Retrait synchronise qui retourne la taille du pointeur du buffer de retrait */
int retirer(Moniteur *moniteur, void **buffer) {
	int indiceRetrait = debutRetirer(moniteur);
	
	Message messageARetirer = moniteur->message[indiceRetrait];
	*buffer = malloc(messageARetirer.size);
	memcpy(*buffer, messageARetirer.buffer, messageARetirer.size);

	finRetirer(moniteur);
	
	return messageARetirer.size;
}

/* Retourne le nombre de cases remplies */
int getNbCasesRemplies(Moniteur *moniteur) {
	pthread_mutex_lock(&moniteur->mutex);
	
	int resultat = moniteur->nbCases - moniteur->nbCasesVides;
	
	pthread_mutex_unlock(&moniteur->mutex);
	
	return resultat;
}
