#include "../headers/accountio.h"

int readAccount(char *filename, account *a, int cur) {
    FILE *f = fopen(filename, "r");

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
    FILE *f = fopen(filename, "a");

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
    int i = 0;
    account it;
    while(!readAccount(filename, &it, i)) {
        if(accountCompare(a, it))
            return i;
        i++;
    }
    return -1;
}

boolean accountCompare(account a, account b) {
    return !(strcmp(a.username, b.username) || strcmp(a.password, b.password));
}