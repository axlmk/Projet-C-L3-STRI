#include "../headers/directory.h"

pdu addReader(char *request) {

    account t;
    pdu res;
    int pos, i = 0, err;
    char **settings = malloc(sizeof(char *) * 2);
    getD_AParameters(request, &settings);

    strcpy(t.username, settings[1]);
    if((pos = seekAccount(PATH_ACCOUNT_STORAGE, t)) < 0) {
        res = generateReturnedPdu(KO, "Error, targeted user doesn't exist.\n");
        free(settings);
        return res;
    }

    if((err = readAccount(PATH_ACCOUNT_STORAGE, &t, pos)) == 1) {
        res = generateReturnedPdu(KO, "Error file doesn't exist.\n");
        free(settings);
        return res;
    } else if(err == 2) {
        res = generateReturnedPdu(KO, "Error from the server, error read.\n");
        free(settings);
        return res;
    }
    if(isFull(t)) {
        res = generateReturnedPdu(KO, "Error, user cannot receive another directory\n");
        free(settings);
        return res;
    }
    if(userHasDirectory(t, settings[0]) > -1) {
        res = generateReturnedPdu(KO, "Error, the targeted user already has this directory.\n");
        free(settings);
        return res;
    }
    i = 0;
    while(strcmp(t.sharedDirectory[i], ""))
        i++;
    strcpy(t.sharedDirectory[i], settings[0]);
    free(settings);

    if((err = writeAccount(PATH_ACCOUNT_STORAGE, t, pos)) == 1) {
        res = generateReturnedPdu(KO, "Error from the server, file error.\n");
        return res;
    } else if(err == 2) {
        res = generateReturnedPdu(KO, "Error from the server, write error.\n");
        return res;
    }
    res = generateReturnedPdu(OK, "The user has now the authorization to read your directory.\n");
    
    return res;
}

pdu rmReader(char *request) {
account t;
    pdu res;
    int pos, err, dpos;
    char **settings = malloc(sizeof(char *) * 2);
    getD_AParameters(request, &settings);

    strcpy(t.username, settings[1]);
    if((pos = seekAccount(PATH_ACCOUNT_STORAGE, t)) < 0) {
        res = generateReturnedPdu(KO, "Error, targeted user doesn't exist.\n");
        free(settings);
        return res;
    }
    
    if((err = readAccount(PATH_ACCOUNT_STORAGE, &t, pos)) == 1) {
        res = generateReturnedPdu(KO, "Error file doesn't exist.\n");
        free(settings);
        return res;
    } else if(err == 2) {
        res = generateReturnedPdu(KO, "Error from the server, error read.\n");
        free(settings);
        return res;
    }
    if((dpos = userHasDirectory(t, settings[0])) == -1) {
        res = generateReturnedPdu(KO, "Error, the targeted user doesn't have this directory.\n");
        free(settings);
        return res;
    }
    free(settings);
    strcpy(t.sharedDirectory[dpos], "");
    if((err = writeAccount(PATH_ACCOUNT_STORAGE, t, pos)) == 1) {
        res = generateReturnedPdu(KO, "Error from the server, file error.\n");
        return res;
    } else if(err == 2) {
        res = generateReturnedPdu(KO, "Error from the server, write error.\n");
        return res;
    }
    res = generateReturnedPdu(OK, "Success : The user will not be able to read your directory.\n");
    return res;
}

pdu displayDirectory(char *request) {
    char *newRequest = malloc(sizeof(char) * (strlen(request) + 4));
    pdu temp[NDIRECTORY];
    pdu res;
    int i, len = 0;
    for(i=0;i<NDIRECTORY;i++) {
        sprintf(newRequest, "%s %d\n", request, i);
        temp[i] = displayRecord(newRequest);
        if(temp[i].code == KO)
            return temp[i];
        len += (strlen(temp[i].request) + 1);
    }

    res.request = malloc(sizeof(char) * (len + 1));
    for(i=0;i<NDIRECTORY;i++) {
        strcat(res.request, temp[i].request);
        strcat(res.request, "\n");
    }
    res.code = OK; 
    return res;
}