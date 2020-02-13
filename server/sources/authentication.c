#include "../headers/authentication.h"


pdu connectionAuthorized(char *request) {
    account acc, t;
    int res = 0;
    char **settings = malloc(sizeof(char *) * 2);
    settings[0]=NULL;
    settings[1]=NULL;
    getAuthParameters(request, &settings);

    strncpy(acc.username, settings[0],200*sizeof(char));
    strncpy(acc.password, settings[1],200*sizeof(char));


    if((res = seekAccount(PATH_ACCOUNT_STORAGE, acc)) == -2){
      return generateReturnedPdu(KO, "An error occured, the storage file doesn't exist");
    }
    else if(res == -1){
      return generateReturnedPdu(KO, "An error occured, the user doesn't exist");
    }
    if(readAccount(PATH_ACCOUNT_STORAGE, &t, res) == 2){
      return generateReturnedPdu(KO, "An error occured while reading the file");
    }
    if(areCredentialsEquals(acc, t)){
      return generateReturnedPdu(OK, "Connection allowed");
    }
    return generateReturnedPdu(KO, "Connection denied");
}

boolean areCredentialsEquals(account a, account b) {
    return !strcmp(a.username, b.username) && !strcmp(a.password, b.password);
}

void getAuthParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}
