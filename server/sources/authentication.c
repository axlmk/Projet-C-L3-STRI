#include "../headers/authentication.h"


pdu connectionAuthorized(char *request) {

    char **settings = malloc(sizeof(char) * 2);
    getAuthParameters(request, &settings);


    account acc, t;
    int res = 0;
    strcpy(acc.username, settings[0]);
    strcpy(acc.password, settings[1]);

    free(settings);

    if((res = seekAccount(PATH_ACCOUNT_STORAGE, acc)) == -2)
        return generateReturnedPdu(KO, "An error occured, the storage file doesn't exist");
    else if(res == -1)
        return generateReturnedPdu(KO, "An error occured, the user doesn't exist");
    if(readAccount(PATH_ACCOUNT_STORAGE, &t, res) == 2)
        return generateReturnedPdu(KO, "An error occured while reading the file");
    if(areCredentialsEquals(acc, t))
       return generateReturnedPdu(OK, "Connection allowed");
    return generateReturnedPdu(KO, "Connection denied");
}

boolean areCredentialsEquals(account a, account b) {
    return !strcmp(a.username, b.username) && !strcmp(a.password, b.password);
}

void getAuthParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}