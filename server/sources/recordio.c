#include "../headers/record.h"

int writeRecord(char *filename, record r, int cur) {
    FILE *f = fopen(filename, "r+b");

    if(!f)
        return 1;

    cur *= sizeof(record);
    fseek(f, cur, SEEK_SET);
    if(!fwrite(&r, sizeof(record), 1, f)) {
        fclose(f);
        return 2;
    }

    fclose(f);
    return 0;
}

int readRecord(char *filename, record *a, int cur) {
    FILE *f = fopen(filename, "rb");

    if(!f)
        return 1;

    cur *= sizeof(record);
    fseek(f, cur, SEEK_SET);
    if(!fread(a, sizeof(record), 1, f)) {
        fclose(f);
        return 2;
    }

    fclose(f);
    return 0;
}