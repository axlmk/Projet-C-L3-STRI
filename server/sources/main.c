#include "../headers/data_structures.h"
#include "../headers/server.h"
#include "../headers/pdu.h"
#include "../headers/install.h"
#include "../headers/authentication.h"
#include "../headers/accountio.h"
#include "../headers/directory.h"
#include "../headers/record.h"


int main(void) {

    char *message = NULL;

    initiateServer();
    Initialisation();
    pdu rPdu, sPdu;
    rPdu.request = malloc(sizeof(char) * 200);
	  while(1) {

    	int fini = 0;
	    AttenteClient();

		while(!fini) {
			      message = Reception();
            if(!message){
              return 0;
            }
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
                case D_P:
                    sPdu.code = 0;
                break;
                case D_A:
                    sPdu = addReader(rPdu.request);
                break;
                case D_R:
                    sPdu = rmReader(rPdu.request);
                break;
                case R_C:
                    sPdu = createRecord(rPdu.request);
                break;
                case R_M:
                    sPdu = modifyRecord(rPdu.request);
                break;
                case R_D:
                    sPdu = deleteRecord(rPdu.request);
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
