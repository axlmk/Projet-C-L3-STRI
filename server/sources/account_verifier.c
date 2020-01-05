#include "../headers/data_structures.h"

boolean connectionAuthorized(char *id, char *pass) { //à changer une fois qu'on aura le format de la pdu

    account *accounts = NULL;
    account witness;
    strcpy(witness.username, id);
    strcpy(witness.password, pass);
    int accountNbr = 0;
    int i = 0;
    boolean authorized = FALSE;

    accountReader(accounts, &accountNbr); //dépend de Cyril

    while(i < accountNbr && (authorized = accountAuthorized(witness, accounts[i])))
        i++;
    return authorized
    
}

boolean accountCompare(account a, account b) {
    return strcmp(a.username, b.username) || strcmp(a.password, b.password);
}