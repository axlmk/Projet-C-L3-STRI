#include "../headers/data_structures.h"
#include "../headers/server.h"
#include "../headers/pdu.h"


int main(int argc, char *argv[]) {

    char *message = NULL;
    /*
    initiateServer();
	Initialisation();

	while(1) {
		int fini = 0;

		AttenteClient();

		while(!fini) {
			message = Reception();
            switch(message) {

            }
			fini  = 1;
            EmissionBinaire();
		}

	}
	TerminaisonClient();
    */
    pdu p;
    p.source = SERVER;
    p.requestSize = 4;
    p.code = A_C;
    p.request = malloc(sizeof(char) * 4);
    strcpy(p.request, "lol");
    PDUToMessage(p, &message);
    printf("%s\n", message);
    free(message);
	return 0;
}
