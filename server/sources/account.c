#include <stdio.h>
#include <stdlib.h>
#include "../headers/server.h"

/*Modifie les informations du compte
la request de la PDU est sous cette forme :
[userName] [champAModifier] [old] [new]*/
pdu ModifyAccount(pdu requete){
  char *tokens[];
  int cpt=0;
  char[] path = "../storage/account";
  char[] dirPath = "../storage/directory/";
  int *p = NULL;
  account temp;

  /*Parsage de la requète contenue dans la PDU en entrée*/
  *tokens[cpt] = strtok(*requete.request, " ");
  while(cpt<4){
    *tokens[cpt] = strtok(p, " ");
    cpt++;
  }


  /*Modification du compte*/
  if(seekAccount(path)==-2){
    return generateReturnedPdu(KO, "An error occured, the storage file doesn't exist");
  }
  else if (seekAccount(path)==-1){
    return generateReturnedPdu(KO, "An error occured, the account doesn't exist");
  }
  else {
    if (tokens[1]=="mdp"){
      temp.username = tokens[0];
      temp.password = tokens[2];
      if (readAccount(path,&temp,seekAccount(path,temp))==1){
        return generateReturnedPdu(KO, "An error occured, The file couldn't be opened");
      }
      else if (readAccount(path,&temp,seekAccount(path,temp))==2){
        return generateReturnedPdu(KO, "An error occured, the file couldn't be written");
      }
      else {
        temp.password = tokens[3];
        if (writeAccount(path,temp,seekAccount(path,temp))==-2){
          return generateReturnedPdu(KO, "An error occured, the file doesn't exist");
        }
        else if (writeAccount(path,temp,seekAccount(path,temp))==-1){
          return generateReturnedPdu(KO, "An error occured, the cannot be found");
        }
        else{
          return generateReturnedPdu(OK, "Success");
        }
      }
    }
    else if (tokens[1]=="username"){
      temp.username = tokens[0];
      if (readAccount(path,&temp,seekAccount(path,temp))==1){
        return generateReturnedPdu(KO, "An error occured, The file couldn't be opened");
      }
      else if (readAccount(path,&temp,seekAccount(path,temp))==2){
        return generateReturnedPdu(KO, "An error occured, the file couldn't be written");
      }
      else {
        temp.username = tokens[3];
      }
      temp.ownedDirectory = strcat(dirPath,tokens[3]);
      if (writeAccount(path,temp,seekAccount(path,temp))==-2){
        return generateReturnedPdu(KO, "An error occured, the file doesn't exist");
      }
      else if (writeAccount(path,temp,seekAccount(path,temp))==-1){
        return generateReturnedPdu(KO, "An error occured, the cannot be found");
      }
      else{
        return generateReturnedPdu(OK, "Success");
      }    }

  }
}
