#include "../headers/account_verifier.h"

/* a changer selon pdu */
boolean connectionAuthorized(char *id, char *pass) {
    account acc, t;
    int res = 0;
    strcpy(acc.username, id);
    strcpy(acc.password, pass);

    if((res = seekAccount(PATH_ACCOUNT_STORAGE, acc)) == -1)
        return FALSE;
    readAccount(PATH_ACCOUNT_STORAGE, &t, res);
    return areCredentialsEquals(acc, t);
}

boolean areCredentialsEquals(account a, account b) {
    return !(!strcmp(a.username, b.username) || !strcmp(b.password, b.password));
}