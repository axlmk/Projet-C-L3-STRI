#ifndef __ACCOUNT_VERIFIER__
#define __ACCOUNT_VERIFIER__

/*
    Check if the login/password couple requested by the client is valid
    Input:
        id : char * : login requested by the client
        pass : char * : password requested by the client
    Return:
        TRUE or FALSE depending on the validity of the login/password couple
*/
boolean connectionAuthorized(char *id, char *pass);

/*
    Compare two accounts, based on the username / password couple
    Input:
        a : account : account to compare to
        b : account : account to compare to
    Return:
        TRUE or FALSE
*/
boolean accountCompare(account a, account b);

#endif