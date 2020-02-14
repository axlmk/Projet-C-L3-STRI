#include "../headers/record.h"

pdu createRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.");
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
                return generateReturnedPdu(KO, "Error name size too long.");
            }
            cmpt++;
            strcpy(r.name, field);
        } else if(!strcmp(field, "firstName")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error first name size too long.");
            }
            cmpt++;
            strcpy(r.firstName, field);
        } else if(!strcmp(field, "phone")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{10}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error phone syntax doesn't match.");
            }
            strcpy(r.phone, field);
        } else if(!strcmp(field, "address")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            strcpy(r.address, field);
        } else if(!strcmp(field, "email")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[-_.[:alnum:]]+@[-_[:alnum:]]+\\.[[:alnum:]]{2,4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error email syntax doesn't match.");
            }
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            cmpt++;
            strcpy(r.email, field);
        } else if(!strcmp(field, "birthDate")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{2}/[0-9]{2}/[0-9]{4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error birthdate syntax doesn't match.");
            }
            strcpy(r.birthDate, field);
        } else if(!strcmp(field, "comment")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LCOMMENTS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            strcpy(r.comments, field);
        } else {
            free(settings);
            return generateReturnedPdu(KO, "Error from the request. Field not valid");
        }
    }
    
    if(cmpt < 3) {
        free(settings);
        return generateReturnedPdu(KO, "Error, the name, first name and email fields are mandatory to create a record.");
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
        return generateReturnedPdu(KO, "Error from the request. File can't be opened");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. File can't be writtent");
    else
        return generateReturnedPdu(OK, "Record created successfully");
}


pdu modifyRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.");
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
                return generateReturnedPdu(KO, "Error name size too long.");
            }
            strcpy(r.name, field);
        } else if(!strcmp(field, "firstName")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LNAME) {
                free(settings);
                return generateReturnedPdu(KO, "Error first name size too long.");
            }
            strcpy(r.firstName, field);
        } else if(!strcmp(field, "phone")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{10}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error phone syntax doesn't match.");
            }
            strcpy(r.phone, field);
        } else if(!strcmp(field, "address")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            strcpy(r.address, field);
        } else if(!strcmp(field, "email")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[-_.[:alnum:]]+@[-_[:alnum:]]+\\.[[:alnum:]]{2,4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error email syntax doesn't match.");
            }
            if(strlen(field) > LADDRESS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            strcpy(r.email, field);
        } else if(!strcmp(field, "birthDate")) {
            field = strtok(NULL, " ");
            if(!matchField(field, "^[0-9]{2}/[0-9]{2}/[0-9]{4}$")) {
                free(settings);
                return generateReturnedPdu(KO, "Error birthdate syntax doesn't match.");
            }
            strcpy(r.birthDate, field);
        } else if(!strcmp(field, "comment")) {
            field = strtok(NULL, " ");
            if(strlen(field) > LCOMMENTS) {
                free(settings);
                return generateReturnedPdu(KO, "Error address size too long.");
            }
            strcpy(r.comments, field);
        } else {
            free(settings);
            return generateReturnedPdu(KO, "Error from the request. Field not valid");
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
        return generateReturnedPdu(KO, "Error from the request. Field can't be opened");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field can't be written");
    else
        return generateReturnedPdu(OK, "Record changed successfully");
}

pdu deleteRecord(char *request) {
    int n;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.");

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
        return generateReturnedPdu(KO, "Error from the request. Field can't be opened");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field can't be written");
    else
        return generateReturnedPdu(OK, "Record deleted successfully");
}

pdu displayRecord(char *request) {
    char **data = malloc(sizeof(char *) * 3);
    pdu res;
    getR_DParameters(request, &data);

    record r;
    char *path;
    if(!strcmp(data[1], "me")) {
        if(!R_DAuthorization(data[0], NULL)) {
            res = generateReturnedPdu(KO, "You'r not allowed to perform this operation");
            free(data);
            return res;
        }
        path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(data[0]) + 1));
        sprintf(path, "%s%s", PATH_STORAGE, data[0]);
    } else {
        if(!R_DAuthorization(data[0], data[1])) {
            res = generateReturnedPdu(KO, "You'r not allowed to perform this operation");
            free(data);
            return res;
        }
        path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(data[1]) + 1));
        sprintf(path, "%s%s", PATH_STORAGE, data[1]);
    }

    int resu;
    if((resu = readRecord(path, &r, strtol(data[2], NULL, 10))) == 1) {
        res = generateReturnedPdu(KO, "Error opening file");
        free(data);
        free(path);
        return res;
    } else if(resu == 2) {
        res = generateReturnedPdu(KO, "Error reading file");
        free(data);
        free(path);
        return res;
    }

    free(path);
    free(data);


    char *display = malloc(sizeof(char) * (97 + strlen(data[2]) + LNAME + LNAME + 11 + LADDRESS + LADDRESS + 11 + LCOMMENTS));

    //Record n:||Name: ||First name: ||Email: ||Phone number: ||Address: ''||Birth date: ||Comments: ''
    sprintf(display, "Record number %ld:\n\tName: %s\n\tFirst name: %s\n\tEmail: %s\n\tPhone number :%s\n\tAddress: '%s'\n\tBirth date: %s\n\tComments: '%s'", strtol(data[2], NULL, 10), r.name, r.firstName, r.email, r.phone, r.address, r.birthDate, r.comments);
    res = generateReturnedPdu(OK, display);
    free(display);
    return res;

}