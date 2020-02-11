#include "../headers/accountio.h"

int readAccount(char *filename, account *a, int cur) {
    FILE *f = fopen(filename, "rb");

    if(!f)
        return 1;

    int size = (sizeof(char) * LNAME) + (sizeof(char) * NDIRECTORY * LNAME) + (sizeof(char) * LPASS);
    cur *= size;
    fseek(f, cur, SEEK_SET);
    if(!fread(a, size, 1, f)) {
        fclose(f);
        return 2;
    }

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(a->username)));
    sprintf(path, "%s%s", PATH_STORAGE, a->username);
    fprintf(stderr, RED "DBG : %s\n" RESET, path);
    readDirectory(path, a->ownedDirectory);
    free(path);

    fclose(f);
    return 0;
}

int writeAccount(char *filename, account a, int cur) {
    FILE *f = fopen(filename, "r+b");
    if(!f)
        return 1;

    int size = (sizeof(char) * LNAME) + (sizeof(char) * NDIRECTORY * LNAME) + (sizeof(char) * LPASS);
    cur *= size;
    fseek(f, cur, SEEK_SET);
    if(!fwrite(&a, size, 1, f)) {
        fclose(f);
        return 2;
    }

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(a.username)));
    sprintf(path, "%s%s", PATH_STORAGE, a.username);
    fprintf(stderr, RED "DBG : %s\n" RESET, path);
    writeDirectory(path, a.ownedDirectory);
    free(path);

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
    account a, b;
    strcpy(a.username, login);
    if(readAccount(PATH_ACCOUNT_STORAGE, &b, 0))
            return FALSE;
    return areLoginsEquals(a, b);
}


pdu deleteAccount(char *request) {
    //function set up
    pdu res;
    char **settings = malloc(sizeof(char*) * 2);
    getA_DParameters(request, &settings);
    if(!A_DAuthorization(settings[0])) {
        res = generateReturnedPdu(KO, "You'r not allowed to perform this operation\n");
        free(settings);
        return res;
    }

    //get all the account except the deleted one
    int len = acclen(PATH_ACCOUNT_STORAGE);
    account *arr = malloc(sizeof(account) * (len) - 1);
    int i = 0, j = 0, err;

    for(i=0;i<len;i++) {
        if((err = readAccount(PATH_ACCOUNT_STORAGE, &arr[j], i)) == 1) {
            res = generateReturnedPdu(KO, "Error from the server, file doesn.\n");
            free(arr);
            free(settings);
            return res;
        } else if(err == 2) {
            res = generateReturnedPdu(KO, "Error from the server, problem with read.\n");
            free(arr);
            free(settings);
            return res;
        }
        if(strcmp(arr[j].username, settings[1]))
            j++;
    }
    //remove and re-write the account file

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(settings[1]) + 1));
    strcpy(path, PATH_STORAGE);
    strcat(path, settings[1]);
    free(settings);
    if (!remove(PATH_ACCOUNT_STORAGE)) {
        if(!createFile(PATH_ACCOUNT_STORAGE)) {
            res = generateReturnedPdu(KO, "Error from the server, problem creating file.\n");
                free(arr);
                return res;
        }
        for(i=0;i<len-1;i++) {
            if((err = writeAccount(PATH_ACCOUNT_STORAGE, arr[i], i)) == 1) {
                res = generateReturnedPdu(KO, "Error from the server, problem with file.\n");
                free(arr);
                return res;
            }
            else if(err == 2) {
                res = generateReturnedPdu(KO, "Error from the server, problem with write.\n");
                free(arr);
                return res;
            }
        }
        if(remove(path)) {
            res = generateReturnedPdu(KO, "Error from the server, file cannot be delete.\n");
            free(arr);
            return res;
        }
        free(path);

    } else {
        res = generateReturnedPdu(KO, "Error from the server, file cannot be delete.\n");
        free(arr);
        return res;
    }

    res = generateReturnedPdu(OK, "Account file modified\n");
    free(arr);
    return res;
}

void getA_CParameters(char *request, char ***data) {
    int i=0;
    /*Parsage de la requète contenue dans la PDU en entrée*/
    (*data)[i] = strtok(request, " ");
    i++;
    while(i<3){
        (*data)[i] = strtok(NULL, " ");
        i++;
    }
}


/*Modifie les informations du compte
la request de la PDU est sous cette forme :
oldUserName newUserName oldPassword newPassword*/
pdu CreateAccount(char *request){
    char **data = malloc(sizeof(char *) * 3);
    pdu res;
    getA_CParameters(request, &data);

    if(!A_DAuthorization(data[0])) {
        res = generateReturnedPdu(KO, "You'r not allowed to perform this operation\n");
        free(data);
        return res;
    }

    account temp;
    int len, st;
    /*Ajout des information de l'utilisateur*/
    strcpy(temp.username, data[1]);
    strcpy(temp.password, data[2]);
    if(seekAccount(PATH_ACCOUNT_STORAGE, temp) >= 0) {
        res = generateReturnedPdu(KO, "Error, account already exists.\n");
        free(data);
        return res;
    }

    /*Construction du nouveau chemin relatif permettant d'accéder au fichier de
    stockage des annuaires pointé par le pointeur path*/
    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(temp.username) + 1));
    strcpy(path, PATH_STORAGE);
    strcat(path, data[1]);
    free(data);

    if(!createFile(path)) {
        res = generateReturnedPdu(KO, "An error occured while creating the file.\n");
        free(path);
        return res;
    }
    if((len = acclen(PATH_ACCOUNT_STORAGE)) == -1) {
        res = generateReturnedPdu(KO, "An error occured while reading the file.\n");
        free(path);
        return res;
    }
    if((st = writeAccount(PATH_ACCOUNT_STORAGE, temp, len)) == 1) {
        res = generateReturnedPdu(KO, "An error occured while opening the file.\n");
        free(path);
        return res;
    } else if(st == 2) {
         res = generateReturnedPdu(KO, "An error occured while writing the file.\n");
        free(path);
        return res;
    }

    if((st = writeDirectory(path, temp.ownedDirectory)) == 1) {
        res = generateReturnedPdu(KO, "An error occured while opening the file.\n");
        free(path);
        return res;
    } else if(st == 2) {
         res = generateReturnedPdu(KO, "An error occured while writing the file.\n");
        free(path);
        return res;
    }
    res = generateReturnedPdu(OK, "Success.\n");
    free(path);
    return res;
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

/*Modifie les informations du compte
la request de la PDU est sous cette forme :
[userName] [userNameModified] [champAModifier] [new]*/
pdu ModifyAccount(char  *requete){
    char **tokens = malloc(sizeof(char *) * 4);
    int err, pos;
    account temp;
    getA_MParameters(requete, &tokens);

    if(!A_DAuthorization(tokens[0])) {
        free(tokens);
        return generateReturnedPdu(KO, "You'r not allowed to perform this operation\n");
    }

    /*Modification du compte*/
    err = seekAccount(PATH_ACCOUNT_STORAGE, temp);
    if(err == -2){
        return generateReturnedPdu(KO, "An error occured, the storage file doesn't exist");
    }
    else if (err == -1){
        return generateReturnedPdu(KO, "An error occured, the account doesn't exist");
    }
    else {
        if (!strcmp(tokens[2], "mdp")){
            strcpy(temp.username, tokens[1]);
            err = readAccount(PATH_ACCOUNT_STORAGE,&temp,seekAccount(PATH_ACCOUNT_STORAGE,temp));
            if (err==1){
                return generateReturnedPdu(KO, "An error occured, The file couldn't be opened");
            }
            else if (err==2){
                return generateReturnedPdu(KO, "An error occured, the file couldn't be written");
            }
            else {
                strcpy(temp.password, tokens[3]);
                err = writeAccount(PATH_ACCOUNT_STORAGE,temp,seekAccount(PATH_ACCOUNT_STORAGE,temp));
                if (err==-2){
                    return generateReturnedPdu(KO, "An error occured, the file doesn't exist");
                }
                else if (err==-1){
                    return generateReturnedPdu(KO, "An error occured, the account cannot be found");
                }
                else{
                    return generateReturnedPdu(OK, "Success");
                }
            }
        }
        else{
            strcpy(temp.username, tokens[1]);
            pos = seekAccount(PATH_ACCOUNT_STORAGE,temp);
            err = readAccount(PATH_ACCOUNT_STORAGE,&temp,pos);
            if (err==1){
                return generateReturnedPdu(KO, "An error occured, The file couldn't be opened");
            }
            else if (err==2){
                return generateReturnedPdu(KO, "An error occured, the file couldn't be written");
            }
            else {
                char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(temp.username) + 1));
                strcpy(path, PATH_STORAGE);
                strcat(path, temp.username);
                if(remove(path)!=0) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while removing the file.\n");
                }

                strcpy(temp.username, tokens[3]);
                path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(temp.username) + 1));
                strcpy(path, PATH_STORAGE);
                strcat(path, temp.username);

                if(createFile(path)==0) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while creating the file.\n");
                }

                err = writeDirectory(path, temp.ownedDirectory);
                if(err==1) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while opening the file.\n");
                }
                else if(err == 2) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while writing the file.\n");
                }
                if (writeAccount(PATH_ACCOUNT_STORAGE,temp,pos)==-2){
                    return generateReturnedPdu(KO, "An error occured, the file doesn't exist");
                }
                else if (writeAccount(PATH_ACCOUNT_STORAGE,temp,pos)==-1){
                    return generateReturnedPdu(KO, "An error occured, the cannot be found");
                }
                else{
                    return generateReturnedPdu(OK, "Success");
                }
            }
        }
    }
}
