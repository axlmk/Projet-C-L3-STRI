#include "../headers/accountio.h"

int readAccount(char *filename, account *a, int cur) {
    FILE *f = fopen(filename, "rb");

    if(!f)
        return 1;

    cur *= sizeof(account);
    fseek(f, cur, SEEK_SET);
    if(!fread(a, sizeof(account), 1, f)) {
        fclose(f);
        return 2;
    }
    
    fclose(f);
    return 0;
}

int writeAccount(char *filename, account a, int cur) {
    FILE *f = fopen(filename, "ab");
    fprintf(stderr, "kesskidi %s %d\n", a.username, cur);

    if(!f)
        return 1;

    cur *= sizeof(account);
    fseek(f, cur, SEEK_SET);
    if(!fwrite(&a, sizeof(account), 1, f)) {
        fclose(f);
        return 2;
    }
    
    fclose(f);
    return 0;
}

int seekAccount(char *filename, account a) {
    int i = 0, res = 0;
    account it;
    while(!(res = readAccount(filename, &it, i))) {
        if(areLoginsEquals(a, it))
            return i;
        i++;
    }
    if(res == 1)
        return -2;
    return -1;
}

boolean areLoginsEquals(account a, account b) {
    return !strcmp(a.username, b.username);
}

int acclen(char *filename) {
    account acc;
    if(readAccount(filename, &acc, 0) == 1)
        return -1;
    int i = 0, res;
    while(!(res = readAccount(filename, &acc, i)))
        i++;
    return i;
}

void getA_DParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, " ");
    (*settings)[2] = strtok(NULL, "\n");
}

boolean A_DAuthorization(char *login) {
    account a, b;
    strcpy(a.username, login);
    if(readAccount(PATH_ACCOUNT_STORAGE, &b, 0))
            return FALSE;
    return areLoginsEquals(a, b);
}


pdu deleteAccount(char *request) {
    //function set up
    pdu res;
    char **settings = malloc(sizeof(char*) * 3);
    getA_DParameters(request, &settings);
    if(!A_DAuthorization(settings[0])) {
        res = generateReturnedPdu(KO, "You'r not allowed to perform this operation\n");
        free(settings);
        return res;
    }

    //get all the account except the deleted one 
    int len = acclen(PATH_ACCOUNT_STORAGE);
    account *arr = malloc(sizeof(account) * (len) - 1);
    int i = 0, j = 0, err;

    for(i=0;i<len;i++) {
        if((err = readAccount(PATH_ACCOUNT_STORAGE, &arr[j], i)) == 1) {
            res = generateReturnedPdu(KO, "Error from the server, file doesn.\n");
            free(arr);
            free(settings);
            return res;
        } else if(err == 2) {
            res = generateReturnedPdu(KO, "Error from the server, problem with read.\n");
            free(arr);
            free(settings);
            return res;
        }
        if(strcmp(arr[j].username, settings[2]))
            j++;
    }
    free(settings);

    //remove and re-write the account file
    if (!remove(PATH_ACCOUNT_STORAGE)) {
        for(i=0;i<len-1;i++) {
            if((err = writeAccount(PATH_ACCOUNT_STORAGE, arr[i], i)) == 1) {
                res = generateReturnedPdu(KO, "Error from the server, problem with file.\n");
                free(arr);
                return res;
            }
            else if(err == 2) {
                res = generateReturnedPdu(KO, "Error from the server, problem with write.\n");
                free(arr);
                return res;
            }
        }
    }
    else
        res = generateReturnedPdu(OK, "Account file modified\n"); 
    free(arr);
    return res;
}