#include "../headers/account.h"

int readAccount(char *filename, account *a, int cur) {
    FILE *f = fopen(filename, "rb");

    if(!f)
        return 1;

    int size = (sizeof(char) * LNAME) + (sizeof(char) * NDIRECTORY * LNAME) + (sizeof(char) * LPASS) + sizeof(user_type);
    cur *= size;
    fseek(f, cur, SEEK_SET);
    if(!fread(a, size, 1, f)) {
        fclose(f);
        return 2;
    }

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(a->username)));
    sprintf(path, "%s%s", PATH_STORAGE, a->username);
    readDirectory(path, a->ownedDirectory);
    free(path);

    fclose(f);
    return 0;
}

int writeAccount(char *filename, account a, int cur) {
    FILE *f = fopen(filename, "r+b");
    if(!f)
        return 1;

    int size = (sizeof(char) * LNAME) + (sizeof(char) * NDIRECTORY * LNAME) + (sizeof(char) * LPASS) + sizeof(user_type);
    cur *= size;
    fseek(f, cur, SEEK_SET);
    if(!fwrite(&a, size, 1, f)) {
        fclose(f);
        return 2;
    }

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(a.username)));
    sprintf(path, "%s%s", PATH_STORAGE, a.username);
    writeDirectory(path, a.ownedDirectory);
    free(path);

    fclose(f);
    return 0;
}
