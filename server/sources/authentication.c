#include "../headers/authentication.h"


boolean connectionAuthorized(char *request) {

    char **settings = malloc(sizeof(char) * 2);
    getAuthParameters(request, &settings);


    account acc, t;
    int res = 0;
    strcpy(acc.username, settings[0]);
    strcpy(acc.password, settings[1]);

    free(settings);

    if((res = seekAccount(PATH_ACCOUNT_STORAGE, acc)) == -1)
        return FALSE;
    readAccount(PATH_ACCOUNT_STORAGE, &t, res);
    return areCredentialsEquals(acc, t);
}

boolean areCredentialsEquals(account a, account b) {
    return !strcmp(a.username, b.username) && !strcmp(b.password, b.password);
}

void getAuthParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}