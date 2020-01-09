#include "../headers/data_structures.h"
#include "../headers/server.h"
#include "../headers/pdu.h"
#include "../headers/install.h"
#include "../headers/authentication.h"


int main(int argc, char *argv[]) {

    char *message = NULL;

    initiateServer();
    //Initialisation();
    pdu rPdu, sPdu;
    rPdu.request = malloc(sizeof(char) * 13);
    strcpy(rPdu.request, "admin admin\n");
    rPdu.code = AUTH;
	//while(1) {

		int fini = 0;
		//AttenteClient();

		while(!fini) {
			//message = Reception();
            messageToPDU(&rPdu, message);
            switch(rPdu.code) {
                case AUTH: ;
                    sPdu.code = connectionAuthorized(rPdu.request);
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
                case D_D:
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
            }
			fini  = 1;
            //PDUToMessage(sPdu, &message);
            //Emission(message);
		}

	//}
	//TerminaisonClient();
	return 0;
}
