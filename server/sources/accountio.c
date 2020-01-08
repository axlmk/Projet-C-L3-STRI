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