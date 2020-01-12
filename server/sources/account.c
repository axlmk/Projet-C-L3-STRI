#include <stdio.h>
#include <stdlib.h>
#include "../headers/server.h"

/*Modifie les informations du compte
la request de la PDU est sous cette forme :
[userName] [champAModifier] [old] [new]*/
int ModifyAccount(pdu requete){
  char *tokens[];
  int cpt=0;
  char[] path = path;
  *p = NULL;
  account temp;
  char verif;

  /*Parsage de la requète contenue dans la PDU en entrée*/
  *tokens[cpt] = strtok(*requete.request, " ");
  while(cpt<4){
    *tokens[cpt] = strtok(p, " ");
    cpt++;
  }


  /*Modification du compte*/
  if(seekAccount(path)==-2){
    printf("Erreur ! Le fichier n'existe pas");
    break;
  }
  else if (seekAccount(path)==-1){
    printf("Erreur ! Le Username saisi n'existe pas");
  }
  else {
    if (tokens[1]=="mdp"){
      temp.username = tokens[0];
      temp.password = tokens[2];
      readAccount(path,&temp,seekAccount(path,temp));
      temp.password = tokens[3];
      writeAccount(path,temp,seekAccount(path,temp));
    }
    else if (tokens[1]=="username"){
      temp.username = tokens[0];
      readAccount(path,&temp,seekAccount(path,temp));
      temp.username = tokens[3];
      temp.ownedDirectory = strcat("../storage/directory/",tokens[3]);
      writeAccount(path,temp,seekAccount(path,temp));
    }

  }
}
