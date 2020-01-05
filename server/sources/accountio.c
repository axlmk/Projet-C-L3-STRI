#include "../headers/accountio.h"

int readAccount(char *filename, account *a, int cur) {
    FILE *f = fopen(filename, "r");

    if(!f) {
        fprintf(stderr, RED "An error occured while opening the file <%s>\n" RESET, filename);
        return 1;
    }

    if(!fread(a, sizeof(account), 1, f)) {
        fprintf(stderr, RED "An error occured while reading the file <%s>\n" RESET, filename);
        fclose(f);
        return 2;
    }
    
    fclose(f);
    return 0;
}

int writeAccount(char *filename, account a, int cur) {
    FILE *f = fopen(filename, "a");

    if(!f) {
        fprintf(stderr, RED "An error occured while opening the file <%s>\n" RESET, filename);
        return 1;
    }
    fseek(f, cur, SEEK_SET);
    if(!fwrite(&a, sizeof(account), 1, f)) {
        fprintf(stderr, RED "An error occured while writing the file <%s>\n" RESET, filename);
        fclose(f);
        return 2;
    }
    
    fclose(f);
    return 0;
}