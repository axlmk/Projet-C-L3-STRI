#ifndef __AUTHENTICATION__
#define __AUTHENTICATION__

#include "account.h"
#include "data_structures.h"
#include "pdu.h"

/*
    Checks if the login/password couple requested by the client is valid
    Requested format for the request field:
        "login password"
    Input:
        id : char * : login requested by the client
        pass : char * : password requested by the client
    Return:
        OK or KO depending on the validity of the login/password couple
*/
pdu connectionAuthorized(char *request);

/*
    Test if the login/password couple is identical between two accounts
    Input:
        a : account : account to compare
        b : account : account to compare
    Return:
        TRUE or FALSE
*/
boolean areCredentialsEquals(account a, account b);

/*
    Parses the request string into the settings array according to the AUTH rules
    Input:
        request : char * : The request to parse
    Output:
        settints : char *** : The array containing the parsed elements
*/
void getAuthParameters(char *request, char ***settings);

#endif