#include "../headers/directory.h"
#include "../headers/record.h"

void getD_AParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}

int userHasDirectory(account a, char *directoryName) {
    int i;
    for(i=0;i<NDIRECTORY;i++) {   
        if(!strcmp(a.sharedDirectory[i], directoryName))
            return i;
    }
    return -1;
}

boolean isFull(account a) {
    int i;
    for(i=0;i<NDIRECTORY;i++) {
        if(!strcmp(a.sharedDirectory[i], ""))
            return FALSE;
    }
    return TRUE;
}

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


int writeDirectory(char *filename, directory t) {
    int i, res = 0;
    for(i=0;i<NRECORDS;i++)
        if((res = writeRecord(filename, t[i], i)))
            return res;
    return res;
}