CC=gcc
LDFLAGS=-lpthread

EXEC=demonstration
all: $(EXEC)

clean:
	rm -rf client/*.o serveur/*.o communication/*.o serveurImpression/*.o tamponPartage/*.o utilisateur/*.o *.exe *.out $(EXEC)

demonstration: tamponPartage/tamponPartage.o communication/communication.o serveurImpression/imprimantes/imprimante.o serveurImpression/serveurImpression.o utilisateur/utilisateur.o demonstration.o
	$(CC) $(LDFLAGS) -o $@ $^

