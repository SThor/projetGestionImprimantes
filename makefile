CFLAGS=-Wall -g -std=c99
EXEC= serveurImpressionExe imprimanteDistanteExe utilisateurExe

all: $(EXEC)

serveurImpressionExe: serveurImpression/serveurImpression.o communication/communication.o tamponPartage/tamponPartage.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -g -lpthread 
imprimanteDistanteExe: imprimantes/imprimanteDistante.o communication/communication.o tamponPartage/tamponPartage.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -g
utilisateurExe: utilisateur/utilisateur.o communication/communication.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -g -lpthread
demonstration: tamponPartage/tamponPartage.o communication/communication.o imprimantes/imprimanteDistante.o serveurImpression/serveurImpression.o utilisateur/utilisateur.o demonstration.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -g  -lpthread	
clean:
	rm -rf client/*.o serveur/*.o communication/*.o serveurImpression/*.o imprimantes/*.o tamponPartage/*.o utilisateur/*.o *.exe *.out *.o $(EXEC)
	
