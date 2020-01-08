#include "../headers/data_structures.h"
#include "../headers/server.h"
#include "../headers/pdu.h"
#include "../headers/install.h"


int main(int argc, char *argv[]) {

    char *message = NULL;

    initiateServer();
    Initialisation();
    pdu rPdu, sPdu;
	while(1) {

		int fini = 0;
		AttenteClient();

		while(!fini) {
			message = Reception();
            messageToPDU(&rPdu, message);
            switch(rPdu.code) {
                case AUTH:
                    sPdu.code = 0;
                break;
                case A_C:
                    sPdu.code = 0;
                break;
                case A_M:
                    sPdu.code = 0;
                break;
                case A_D:
                    sPdu.code = 0;
                break;
                case D_C:
                    sPdu.code = 0;
                break;
                case D_S:
                    sPdu.code = 0;
                break;
                case R_C:
                    sPdu.code = 0;
                break;
                case R_M:
                    sPdu.code = 0;
                break;
                case R_D:
                    sPdu.code = 0;
                break;
                default:
                    sPdu.code = 0;
                    /* le code de la pdu n'a pas ete reconnu */
            }
			fini  = 1;
            PDUToMessage(sPdu, &message);
            Emission(message);
		}

	}
	TerminaisonClient();
	return 0;
}
