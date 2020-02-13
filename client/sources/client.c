
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "../headers/client.h"

#include "../../server/headers/utils.h"
#include "../../server/headers/pdu.h"
#include "../../server/headers/data_structures.h"

#define TRUE 1
#define FALSE 0

#define LONGUEUR_TAMPON 4096

/* Variables cachees */

/* le socket client */
int socketClient;
/* le tampon de reception */
char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;

/* Initialisation.
 * Connexion au serveur sur la machine donnee.
 * Utilisez localhost pour un fonctionnement local.
 */
int Initialisation(char *machine) {
	return InitialisationAvecService(machine, "13214");
}

/* Initialisation.
 * Connexion au serveur sur la machine donnee et au service donne.
 * Utilisez localhost pour un fonctionnement local.
 */
int InitialisationAvecService(char *machine, char *service) {
	int n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(machine, service, &hints, &res)) != 0)  {
     		fprintf(stderr, "[\033[0;31m!\033[0m] Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
     		return 0;
	}
	ressave = res;

	do {
		socketClient = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketClient < 0)
			continue;	/* ignore this one */

		if (connect(socketClient, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		close(socketClient);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {
     		perror("[\033[0;31m!\033[0m] Initialisation, erreur de connect.");
     		return 0;
	}

	freeaddrinfo(ressave);
	printf("Connexion avec le serveur reussie.\n");

	return 1;
}

/* Recoit un message envoye par le serveur.
 */
char *Reception() {
	int retour = 0;
	memset(tamponClient,0,LONGUEUR_TAMPON);
	retour = recv(socketClient, tamponClient, LONGUEUR_TAMPON-1, 0);
	if (retour < 0) {
		perror("Reception, erreur de recv.");
		return NULL;
	} else if(retour == 0) {
		fprintf(stderr, "Reception, le serveur a ferme la connexion.\n");
		return NULL;
	} else {
		return strdup(tamponClient);
	}
}


/* Envoie un message au serveur.
 * Attention, le message doit etre termine par \n
 */
int Emission(char *message) {
	if(strstr(message, "\n") == NULL) {
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
	}
	int taille = strlen(message);
	if (send(socketClient, message, taille,0) == -1) {
        perror("Emission, probleme lors du send.");
        return 0;
	}
	//printf("Emission de %d caracteres.\n", taille+1);
	return 1;
}

/* Recoit des donnees envoyees par le serveur.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax) {
	int dejaRecu = 0;
	int retour = 0;
	/* on commence par recopier tout ce qui reste dans le tampon
	 */
	while((finTampon > debutTampon) && (dejaRecu < tailleMax)) {
		donnees[dejaRecu] = tamponClient[debutTampon];
		dejaRecu++;
		debutTampon++;
	}
	/* si on n'est pas arrive au max
	 * on essaie de recevoir plus de donnees
	 */
	if(dejaRecu < tailleMax) {
		retour = recv(socketClient, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if(retour < 0) {
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		} else if(retour == 0) {
			fprintf(stderr, "ReceptionBinaire, le serveur a ferme la connexion.\n");
			return 0;
		} else {
			/*
			 * on a recu "retour" octets en plus
			 */
			return dejaRecu + retour;
		}
	} else {
		return dejaRecu;
	}
}

/* Envoie des donn�es au serveur en pr�cisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille) {
	int retour = 0;
	retour = send(socketClient, donnees, taille, 0);
	if(retour == -1) {
		perror("Emission, probleme lors du send.");
		return -1;
	} else {
		return retour;
	}
}


/* Ferme la connexion.
 */
void Terminaison() {
	close(socketClient);
}

void print_usage(char *pr_name){
	printf("Usage: %s <ip>:<port> <request>\n",pr_name);
}

int init(char **argv){
	int port=0;
	char *iterator=strtok(argv[1],":");
	struct in_addr *a=malloc(sizeof(struct in_addr));
	int ret=0;
	char *iterator2=iterator;
	if(strcmp(iterator,"localhost")!=0){
		ret=inet_pton(AF_INET,iterator,a);
		if(ret!=1){
			exit(1);
		}
	}
	iterator=strtok(NULL,":");
	port = atoi(iterator);
	if(port<=80){
		return 1;
	}
	printf("[\033[0;32m*\033[0m] Connecting to %s:%s\n",iterator2,iterator);
	if(InitialisationAvecService(iterator2,iterator)!=1){
		exit(1);
	}

	return 0;
}

int parseCommand(char *command){
	char extract[500];
	strncpy(extract,command,500*sizeof(char));
	char *code=strtok(extract," ");
	if(strcmp(code,"login")==0){
		connectionAuthorized(command);
	} else if(strcmp(code,"createAccount")==0){
		printf("[" GREEN "*" RESET "] Creating an account\n");
		executeCommand(command, A_C);
	} else if(strcmp(code,"modifyAccount")==0){
		printf("[" GREEN "*" RESET "] Modifying an account\n");
		executeCommand(command, A_M);
	} else if(strcmp(code,"deleteAccount")==0){
		printf("[" GREEN "*" RESET "] Deleting an account\n");
		executeCommand(command, A_D);
	} else if(strcmp(code,"dirdump")==0){
		printf("[" GREEN "*" RESET "] Displaying a directory\n");
		executeCommand(command, D_D);
	} else if(strcmp(code,"createRecord")==0){
		printf("[" GREEN "*" RESET "] Creating a record\n");
		executeCommand(command, R_C);
    } else if(strcmp(code,"modifyRecord")==0){
		printf("[" GREEN "*" RESET "] Modifying an account\n");
		executeCommand(command, R_M);
	} else if(strcmp(code,"deleteRecord")==0){
		printf("[" GREEN "*" RESET "] Deleting an account\n");
		executeCommand(command, R_D);
	} else {
		printf("[" RED "!" RESET "] Bad command\n");
		print_cmdline_help();
	}
	return 0;
}

void print_cmdline_help(void){
	printf("Syntax: \n");
    printf("\tlogin <id> <pass>\n");
    printf("\t[FOR NOW] createAccount <admin_user> <username> <pass>\n");
    printf("\t[FOR NOW] modifyAccount <admin_user> <username> <fieldName> <fieldValue>\n");
    printf("\t[FOR NOW] deleteAccount <admin_user> <username>\n");
    printf("\t[FOR NOW] createRecord <user> <record_number> name:<name> firstName:<fisrtName> email:<email> [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]\n");
    printf("\t[FOR NOW] modifyRecord <user> <record_number> [name:<name>] [firstName:<fisrtName>] [email:<email>] [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]\n");
    printf("\t[FOR NOW] deleteRecord <user> <record_number>\n");
}

boolean isSyntaxCorrect(char *command, pdu_code co) {
    char *token = NULL;
    char delim[2] = " ";
    char *temp_str = malloc(sizeof(char) * (strlen(command) + 1));
    strcpy(temp_str, command);
    token = strtok(temp_str, delim);
  
    switch (co) {
        case A_C:
            while( token != NULL ) {
                //TO-CHANGE
                printf( " %s\n", token );
                token = strtok(NULL, s);
            }

    }
    return TRUE;
}

char *showSyntax(pdu_code co) {
    switch (co) {
        case A_C:
            return "[FOR NOW] createAccount <admin_user> <username> <pass>";   
        default:
            return "Command doesn't exist";
    }
}

char *clearRequest(char *command) {
    int i = 0;
    while(command[i++] != ' ');
    return &(command[i+1]);
}

int executeCommand(char *command, pdu_code co){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
    char *message;
    if(!isSyntaxCorrect(command, co)) {
        printf("[" RED "!" RESET "] The syntax for the choosen command isn't correct. This is the valid way:\n");
        printf("\t%s\n", showSyntax(co));
    }

    pdu t = generateReturnedPdu(co, clearRequest(command));
    PDUToMessage(t, &message);

    Emission(message);
    retour = Reception();

    messageToPDU(&t, retour);
    if(t.code == OK)
        printf("[" GREEN "*" RESET "] :" GREEN "%s" RESET "\n", t.request);
    else
        printf("[" RED "!" RESET "] :" RED "%s" RESET "\n", t.request);
    free(retour);
    free(t.request);
    return 0; 
}