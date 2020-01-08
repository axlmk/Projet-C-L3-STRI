#ifndef __ACCOUNT_VERIFIER__
#define __ACCOUNT_VERIFIER__

#include "accountio.h"
#include "data_structures.h"

/*
    Check if the login/password couple requested by the client is valid
    Input:
        id : char * : login requested by the client
        pass : char * : password requested by the client
    Return:
        TRUE or FALSE depending on the validity of the login/password couple
*/
/* a changer selon pdu */
boolean connectionAuthorized(char *id, char *pass);

/*
    Test if the login/password couple is identical between two accounts
    Input:
        a : account : account to compare
        b : account : account to compare
    Return:
        TRUE or FALSE
*/
boolean areCredentialsEquals(account a, account b);

#endif