#include "../headers/directory.h"

void getD_AParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}

int userHasDirectory(account a, char *directoryName) {
    int i;
    for(i=0;i<NDIRECTORY;i++) {  
            fprintf(stderr, RED "salut '%s' %s %s" RESET "\n", a.sharedDirectory[i], a.username, directoryName);
 
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


int writeDirectory(char *filename, directory t) {
    int i, res = 0;
    for(i=0;i<NRECORDS;i++)
        if((res = writeRecord(filename, t[i], i)))
            return res;
    return res;
}

int readDirectory(char *filename, directory t) {
    int i, res = 0;
    for(i=0;i<NRECORDS;i++)
        if((res = readRecord(filename, &(t[i]), i)))
            return res;
    return res;
}

void getD_DParameters(char *request, char ***settings) {
    (*settings)[0] = strtok(request, " ");
    (*settings)[1] = strtok(NULL, "\n");
}