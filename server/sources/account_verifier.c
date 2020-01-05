#include "../headers/account_verifier.h"

boolean connectionAuthorized(char *id, char *pass) { //à changer une fois qu'on aura le format de la pdu

    account acc;
    strcpy(acc.username, id);
    strcpy(acc.password, pass);

    if(seekAccount("account_test", acc) == -1)
        return FALSE;
    return TRUE;
    
}