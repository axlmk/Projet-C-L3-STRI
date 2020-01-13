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
