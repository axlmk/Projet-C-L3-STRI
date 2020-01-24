#include "../headers/data_structures.h"
#include "../headers/server.h"
#include "../headers/pdu.h"
#include "../headers/install.h"
#include "../headers/authentication.h"
#include "../headers/accountio.h"
#include "../headers/directory.h"


int main(int argc, char *argv[]) {

    char *message = NULL;

    initiateServer();
    Initialisation();
    pdu rPdu, sPdu;
    rPdu.request = malloc(sizeof(char) * 100);
	while(1) {
    
		int fini = 0;
		AttenteClient();

		while(!fini) {
			message = Reception();
            messageToPDU(&rPdu, message);
            switch(rPdu.code) {
                case AUTH:
                    sPdu = connectionAuthorized(rPdu.request);
                break;
                case A_C:
                    sPdu = CreateAccount(rPdu.request);
                break;
                case A_M:
                    sPdu = ModifyAccount(rPdu.request);
                break;
                case A_D:
                    sPdu = deleteAccount(rPdu.request);
                break;
                case D_C:
                    sPdu.code = 0;
                break;
                case D_D:
                    sPdu.code = 0;
                break;
                case D_P:
                    sPdu.code = 0;
                break;
                case D_A:
                    sPdu = addReader(rPdu.request);
                break;
                case D_R:
                    sPdu = rmReader(rPdu.request);
                case R_C:
                    sPdu.code = 0;
                break;
                case R_M:
                    sPdu.code = 0;
                break;
                case R_D:
                    sPdu.code = 0;
                break;
                case R_P:
                    sPdu.code = 0;
                break;
                default:
                    sPdu.code = 0;
            }
            PDUToMessage(sPdu, &message);
            Emission(message);
		}

	}
	TerminaisonClient();
    
    free(rPdu.request);
	return 0;
}
