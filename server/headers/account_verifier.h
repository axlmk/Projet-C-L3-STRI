#ifndef __ACCOUNT_VERIFIER__
#define __ACCOUNT_VERIFIER__

#include "accountio.h"
#include "../headers/data_structures.h"

/*
    Check if the login/password couple requested by the client is valid
    Input:
        id : char * : login requested by the client
        pass : char * : password requested by the client
    Return:
        TRUE or FALSE depending on the validity of the login/password couple
*/
boolean connectionAuthorized(char *id, char *pass);

#endif