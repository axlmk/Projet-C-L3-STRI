#include "../headers/account.h"

pdu deleteAccount(char *request) {
    //function set up
    pdu res;
    char **settings = malloc(sizeof(char*) * 2);
    getA_DParameters(request, &settings);
    if(!A_DAuthorization(settings[0])) {
        res = generateReturnedPdu(KO, "You'r not allowed to perform this operation.");
        free(settings);
        return res;
    }

    if(!strcmp(settings[1], "admin")) {
        res = generateReturnedPdu(KO, "You can't delete the admin user.");
        free(settings);
        return res;
    }

    //get all accounts excepted the deleted one 
    int len = acclen(PATH_ACCOUNT_STORAGE);
    account *arr = malloc(sizeof(account) * (len) - 1);
    int i = 0, j = 0, err;
    boolean accountExists = FALSE;

    for(i=0;i<len;i++) {
        if((err = readAccount(PATH_ACCOUNT_STORAGE, &arr[j], i)) == 1) {
            res = generateReturnedPdu(KO, "Error from the server, file doesn't exist.");
            free(arr);
            free(settings);
            return res;
        } else if(err == 2) {
            res = generateReturnedPdu(KO, "Error from the server, problem with read.");
            free(arr);
            free(settings);
            return res;
        }
        if(strcmp(arr[j].username, settings[1]))
            j++;
        else
            accountExists = TRUE;

    }

    if(!accountExists) {
        res = generateReturnedPdu(KO, "This account doesn't exist");
        free(settings);
        free(arr);
        return res;
    }

    deleteSharedAccount(arr, len, settings[1]);
    //remove and re-write the account file

    char *path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(settings[1]) + 1));
    strcpy(path, PATH_STORAGE);
    strcat(path, settings[1]);
    free(settings);
    if (!remove(PATH_ACCOUNT_STORAGE)) {
        if(!createFile(PATH_ACCOUNT_STORAGE)) {
            res = generateReturnedPdu(KO, "Error from the server, problem creating file.");
            free(arr);
            return res;
        }
        for(i=0;i<len-1;i++) {
            if((err = writeAccount(PATH_ACCOUNT_STORAGE, arr[i], i)) == 1) {
                res = generateReturnedPdu(KO, "Error from the server, problem with file.");
                free(arr);
                return res;
            }
            else if(err == 2) {
                res = generateReturnedPdu(KO, "Error from the server, problem with write.");
                free(arr);
                return res;
            }
        }
        if(remove(path)) {
            res = generateReturnedPdu(KO, "Error from the server, file cannot be delete.");
            free(arr);
            return res;
        }
        free(path);


    } else {
        res = generateReturnedPdu(KO, "Error from the server, file cannot be delete.");
        free(arr);
        return res;
    }

    res = generateReturnedPdu(OK, "Account file modified");
    free(arr);
    return res;
}

pdu createAccount(char *request){
    char **data = malloc(sizeof(char *) * 4);
    pdu res;
    getA_CParameters(request, &data);

    if(!A_DAuthorization(data[0])) {
        res = generateReturnedPdu(KO, "You'r not allowed to perform this operation");
        free(data);
        return res;
    }

    account temp;
    clearAccount(&temp); 
    int len, st;
    /*Ajout des information de l'utilisateur*/
    strcpy(temp.username, data[1]);
    strcpy(temp.password, data[2]);
    if(!strcmp(data[3], "admin"))
        temp.userT = ADMIN;
    else if(!strcmp(data[3], "user"))
        temp.userT = USER;
    else {
        res = generateReturnedPdu(KO, "The mode isn't valid");
        free(data);
        return res;
    }

    if(seekAccount(PATH_ACCOUNT_STORAGE, temp) >= 0) {
        res = generateReturnedPdu(KO, "Error, account already exists.");
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
        res = generateReturnedPdu(KO, "An error occured while creating the file.");
        free(path);
        return res;
    }
    if((len = acclen(PATH_ACCOUNT_STORAGE)) == -1) {
        res = generateReturnedPdu(KO, "An error occured while reading the file.");
        free(path);
        return res;
    }
    if((st = writeAccount(PATH_ACCOUNT_STORAGE, temp, len)) == 1) {
        res = generateReturnedPdu(KO, "An error occured while opening the file.");
        free(path);
        return res;
    } else if(st == 2) {
         res = generateReturnedPdu(KO, "An error occured while writing the file.");
        free(path);
        return res;
    }

    if((st = writeDirectory(path, temp.ownedDirectory)) == 1) {
        res = generateReturnedPdu(KO, "An error occured while opening the file.");
        free(path);
        return res;
    } else if(st == 2) {
         res = generateReturnedPdu(KO, "An error occured while writing the file.");
        free(path);
        return res;
    }
    res = generateReturnedPdu(OK, "Success.\n");
    free(path);
    return res;
}


pdu modifyAccount(char  *requete){
    char **tokens = malloc(sizeof(char *) * 4);
    int err, pos;
    account temp;
    getA_MParameters(requete, &tokens);

    if(!A_DAuthorization(tokens[0])) {
        free(tokens);
        return generateReturnedPdu(KO, "You'r not allowed to perform this operation");
    }

    strcpy(temp.username, tokens[1]);
    err = seekAccount(PATH_ACCOUNT_STORAGE, temp);
    if(err == -2){
        return generateReturnedPdu(KO, "An error occured, the storage file doesn't exist");
    }
    else if (err == -1){
        return generateReturnedPdu(KO, "An error occured, the account doesn't exist");
    }
    else {
        if (!strcmp(tokens[2], "mdp")){
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
        } else if(!strcmp(tokens[2], "mode")) {
            err = readAccount(PATH_ACCOUNT_STORAGE, &temp, seekAccount(PATH_ACCOUNT_STORAGE,temp));
            if(err == 1) {
                return generateReturnedPdu(KO, "An error occured, The file couldn't be opened");
            } else if (err==2) {
                return generateReturnedPdu(KO, "An error occured, the file couldn't be written");
            } else {
                if(!strcmp(tokens[3], "admin"))
                    temp.userT = ADMIN;
                else if(!strcmp(tokens[3], "user"))
                    temp.userT = USER;
                else
                    return generateReturnedPdu(KO, "The mode isn't valid");
                err = writeAccount(PATH_ACCOUNT_STORAGE, temp, seekAccount(PATH_ACCOUNT_STORAGE,temp));
                if(err == -2){
                    return generateReturnedPdu(KO, "An error occured, the file doesn't exist");
                }
                else if(err == -1) {
                    return generateReturnedPdu(KO, "An error occured, the account cannot be found");
                }
                else {
                    return generateReturnedPdu(OK, "Success");
                }
            }
        } else if(!strcmp(tokens[2], "username")) {
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
                    return generateReturnedPdu(KO, "An error occured while removing the file.");
                }

                strcpy(temp.username, tokens[3]);
                path = malloc(sizeof(char) * (strlen(PATH_STORAGE) + strlen(temp.username) + 1));
                strcpy(path, PATH_STORAGE);
                strcat(path, temp.username);

                if(createFile(path)==0) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while creating the file.");
                }

                err = writeDirectory(path, temp.ownedDirectory);
                if(err==1) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while opening the file.");
                }
                else if(err == 2) {
                    free(path);
                    return generateReturnedPdu(KO, "An error occured while writing the file.");
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
        } else {
            return generateReturnedPdu(KO, "The field isn't valid");
        }
    }
}