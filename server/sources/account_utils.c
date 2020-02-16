#include "../headers/account.h"

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
    (*settings)[1] = strtok(NULL, "\n");
}

boolean A_DAuthorization(char *login) {
    account a;
    strcpy(a.username, login);
    int res;
    if((res = seekAccount(PATH_ACCOUNT_STORAGE, a)) >= 0)
        if(!readAccount(PATH_ACCOUNT_STORAGE, &a, res))
            if(a.userT == ADMIN)
                return TRUE;
    return FALSE;
}



void getA_CParameters(char *request, char ***data) {
    int i=0;
    /*Parsage de la requète contenue dans la PDU en entrée*/
    (*data)[i] = strtok(request, " ");
    i++;
    while(i<4){
        (*data)[i] = strtok(NULL, " \n");
        i++;
    }
}

void getA_MParameters(char *request, char ***data) {
    int cpt=0;
    /*Parsage de la requète contenue dans la PDU en entrée*/
    (*data)[cpt] = strtok(request, " ");
    cpt++;
    while(cpt<4){
        (*data)[cpt] = strtok(NULL, " ");
        cpt++;
    }
}

void clearAccount(account *t) {
    memset(t->username, 0, LNAME);
    memset(t->password, 0, LPASS);
    int i;
    for(i=0;i<NDIRECTORY;i++)
        memset(t->sharedDirectory[i], 0, LNAME);
    for(i=0;i<NRECORDS;i++)
        clearRecord(&(t->ownedDirectory[i]));
    t->userT = USER;
}

void deleteSharedAccount(account *tab, int N, char *deletedUsername) {
    int i, j;
    for(i=0;i<N;i++) {
        for(j=0;j<NDIRECTORY;j++) {
            if(!strcmp(tab[i].sharedDirectory[j], deletedUsername)) {
                memset(tab[i].sharedDirectory[j], 0, LNAME);
            }
        }
    }
}
