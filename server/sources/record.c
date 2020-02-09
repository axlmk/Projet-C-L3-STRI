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

int getSettingsNumber(char *request) {
    int count = 1, i = 0;
    while(request[i++])
        if(request[i] == ' ')
            count++;
    return count;
}

void getR_MParameters(char *request, char ***settings, int N) {
    int i = 0;
    (*settings)[i] = strtok(request, " ");
    for(i=1;i<N;i++)
        (*settings)[i] = strtok(NULL, " ");
}

boolean matchField(char *field, char *str_regex) {
   int err;
   regex_t preg;

   err = regcomp(&preg, str_regex, REG_NOSUB | REG_EXTENDED);
   if (!err) {
        int match;

        match = regexec (&preg, field, 0, NULL, 0);
        regfree (&preg);

        if(!match)
            return TRUE;
        else if (match == REG_NOMATCH)
            return FALSE;
        else {
            char *text;
            size_t size;

            size = regerror (err, &preg, NULL, 0);
            text = malloc (sizeof (*text) * size);
            if (text) {
                regerror (err, &preg, text, size);
                fprintf (stderr, "%s\n", text);
                free (text);
            }
            else {
                fprintf (stderr, "Not enough memory\n");
            }
            return FALSE;
        }
   }
   fprintf(stderr, "Error compiling the regexp\n");
   return FALSE;
}

void clearRecord(record *r) {
    memset(r->address, 0, LADDRESS);
    memset(r->phone, 0, 11);
    memset(r->birthDate, 0, 11);
    memset(r->firstName, 0, LNAME);
    memset(r->name, 0, LNAME);
    memset(r->email, 0, LADDRESS);
    memset(r->comments, 0, LCOMMENTS);
}

pdu createRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.\n");
    char **settings = malloc(sizeof(char*) * n);
    getR_MParameters(request, &settings, n);
    record r;
    clearRecord(&r);
    char *field;
    int i = 2;
    int cmpt = 0;

    while(i < n) {
        field = strtok(settings[i++], ":");
        if(!strcmp(field, "name")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error name size too long.\n");
            }
            cmpt++;
            strcpy(r.name, field);
        } else if(!strcmp(field, "firstName")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error first name size too long.\n");
            }
            cmpt++;
            strcpy(r.firstName, field);
        } else if(!strcmp(field, "phone")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{10}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error phone syntax doesn't match.\n");
            }
            strcpy(r.phone, field);
        } else if(!strcmp(field, "address")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            cmpt++;
            strcpy(r.address, field);
        } else if(!strcmp(field, "email")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[-_.[:alnum:]]+@[-_[:alnum:]]+\\.[[:alnum:]]{2,4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error email syntax doesn't match.\n");
            }
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            strcpy(r.email, field);
        } else if(!strcmp(field, "birthDate")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{2}/[0-9]{2}/[0-9]{4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error birthdate syntax doesn't match.\n");
            }
            strcpy(r.birthDate, field);
        } else if(!strcmp(field, "comment")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LCOMMENTS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            strcpy(r.comments, field);
        } else {
            free(settings);
            return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
        }
    }
    
    if(cmpt < 3) {
        free(settings);
        return generateReturnedPdu(KO, "Error, the name, first name and email fields are mandatory to create a record.\n");
    }

    int len = strlen(PATH_STORAGE) + strlen(settings[0]) + 1;
    int numRecord;
    char *path = malloc(sizeof(char) * len);
    memset(path, 0, len);
    sprintf(path, "%s%s", PATH_STORAGE, settings[0]);
    free(settings);

    numRecord = strtol(settings[1], NULL, 10);
    int res = writeRecord(path, r, numRecord);
    free(path);

    if(res == 1)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
}


//request : user recordIndication field:value [field:value]...
pdu modifyRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.\n");
    char **settings = malloc(sizeof(char*) * n);
    getR_MParameters(request, &settings, n);
    record r;
    char *field;
    int i = 2;

    while(i < n) {
        field = strtok(settings[i++], ":");
        if(!strcmp(field, "name")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error name size too long.\n");
            }
            strcpy(r.name, field);
        } else if(!strcmp(field, "firstName")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error first name size too long.\n");
            }
            strcpy(r.firstName, field);
        } else if(!strcmp(field, "phone")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{10}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error phone syntax doesn't match.\n");
            }
            strcpy(r.phone, field);
        } else if(!strcmp(field, "address")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            strcpy(r.address, field);
        } else if(!strcmp(field, "email")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[-_.[:alnum:]]+@[-_[:alnum:]]+\\.[[:alnum:]]{2,4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error email syntax doesn't match.\n");
            }
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            strcpy(r.email, field);
        } else if(!strcmp(field, "birthDate")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{2}/[0-9]{2}/[0-9]{4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error birthdate syntax doesn't match.\n");
            }
            strcpy(r.birthDate, field);
        } else if(!strcmp(field, "comment")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LCOMMENTS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.\n");
            }
            strcpy(r.comments, field);
        } else {
            free(settings);
            return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
        }
    }
    
    int len = strlen(PATH_STORAGE) + strlen(settings[0]) + 1;
    int numRecord;
    char *path = malloc(sizeof(char) * len);
    memset(path, 0, len);
    sprintf(path, "%s%s", PATH_STORAGE, settings[0]);
    free(settings);

    numRecord = strtol(settings[1], NULL, 10);
    int res = writeRecord(path, r, numRecord);
    free(path);

    if(res == 1)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
}

//request: username recordIndication
pdu deleteRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.\n");

    char **settings = malloc(sizeof(char*) * 2);
    getR_MParameters(request, &settings, 2);
    record r;
    clearRecord(&r);
    
    int len = strlen(PATH_STORAGE) + strlen(settings[0]) + 1;
    int numRecord;
    char *path = malloc(sizeof(char) * len);
    memset(path, 0, len);
    sprintf(path, "%s%s", PATH_STORAGE, settings[0]);
    free(settings);

    numRecord = strtol(settings[1], NULL, 10);
    int res = writeRecord(path, r, numRecord);
    free(path);

    if(res == 1)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
}
