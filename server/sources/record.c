#include "../headers/record.h"

/*Modifie les informations du compte
la request de la PDU est sous cette forme :
oldUserName newUserName oldPassword newPassword
int CreateRecord(pdu pdu){
  char *data[];
  int i=0;
  char[] path = "../storage/directory/";
  *p = NULL;

  Parsage de la requète contenue dans la PDU en entrée
  *data[i] = strtok(*pdu.request, " ");
  while(i<9){
    *data[cpt] = strtok(NULL, " ");
    i++;
  }

  Ajout des information de l'utilisateur
  pdu.name: data [1]
  pdu.firstName: data [2]
  pdu.phone: data [3]
  pdu.address: data [4]
  pdu.email: data [5]
  pdu.birthDate: data [6]
  pdu.comments: data [7]

  Construction du nouveau chemin relatif permettant d'accéder au fichier de
  stockage des annuaires pointé par le pointeur path
  strcat(path,tokens[1]);

}*/

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
    char c;
    int count = 0, i = 0;
    while(request[i++] != EOF)
        if(request[i] == ' ')
            count++;
    return count;
}

void getR_MParameters(char *request, char ***settings) {
    int i = 0;
    (*settings)[i++] = strtok(request, " ");
    while((*settings)[i-1])
        (*settings)[i++] = strtok(NULL, " ");
}

//request : user recordIndication field:value [field:value field:value [etc]]
pdu modifyRecord(char *request) {
    int n, numRecord;
    if(!(n = getSettingsNumber(request)))
        return generateReturnedPdu(KO, "Error from the request.\n");
    char **settings = malloc(sizeof(char*) * n);
    getR_MParameters(request, &settings);

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(settings[0]) + 1));
    sprintf(path, "%s%s", PATH_STORAGE, settings[0]);
    numRecord = strtol(settings[1], NULL, 10);

    record r;
    char *field;
    int i = 2;
    /* a verifier tout ca qu'il y ait pas de magouille de pointeurs/alias */
    while(i < n) {
        field = strtok(settings[i++], ":");
        if(strcmp(field, "name")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.name, field);
        } else if(strcmp(settings[i], "firstName")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.firstName, field);
        } else if(strcmp(settings[i], "phone")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.phone, field);
        } else if(strcmp(settings[i], "address")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.address, field);
        } else if(strcmp(settings[i], "email")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.email, field);
        } else if(strcmp(settings[i], "birthDate")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.birthDate, field);
        } else if(strcmp(settings[i], "comment")) {
            field = strtok(NULL, " ");
            /*assert rules */
            strcpy(r.comments, field);
        } else {
            return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
        }
    }

    int res;
    if((res = writeRecord(path, r, numRecord)) == 1)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else if(res == 2)
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
    else
        return generateReturnedPdu(KO, "Error from the request. Field not valid\n");
}
