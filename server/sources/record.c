#include <stdio.h>
#include <stdlib.h>
#include "../headers/server.h"

/*Modifie les informations du compte
la request de la PDU est sous cette forme :
name firstname phone address email birthdate comments*/
int ModifyRecord(pdu requete){
  char *tokens[];
  int cpt=0;
  char[] path = "../storage/directory/";
  *p = NULL;
  record temp;
  char verif;

  /*Parsage de la requète contenue dans la PDU en entrée*/
  *tokens[cpt] = strtok(*requete.request, " ");
  while(cpt<4){
    *tokens[cpt] = strtok(p, " ");
    cpt++;
  }

  /*Ecriture des nouvelles informations dans l'annuaire local à la fonction.*/
  username = tokens[0];
  temp.name = tokens[1];
  temp.firstName = tokens[2];
  temp.phone = tokens[3];
  temp.address = tokens[4];
  temp.mail = tokens[5];
  temp.birthDate = tokens[6];
  temp.comments = tokens[7];
  strcat(path,tokens[0]);

  /*Ecriture des nouvelles informations dans l'enregistrement de l'annuaire*/
  fopen(path,"a")

}
