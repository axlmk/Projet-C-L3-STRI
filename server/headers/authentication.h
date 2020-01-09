#ifndef __AUTHENTICATION__
#define __AUTHENTICATION__

#include "accountio.h"
#include "data_structures.h"

/*
    Check if the login/password couple requested by the client is valid
    Requested format for the request field:
        "login password"
    Input:
        id : char * : login requested by the client
        pass : char * : password requested by the client
    Return:
        TRUE or FALSE depending on the validity of the login/password couple
*/
boolean connectionAuthorized(char *request);

/*
    Test if the login/password couple is identical between two accounts
    Input:
        a : account : account to compare
        b : account : account to compare
    Return:
        TRUE or FALSE
*/
boolean areCredentialsEquals(account a, account b);

void getAuthParameters(char *request, char ***settings);

#endif