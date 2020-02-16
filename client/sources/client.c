
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

#define TRUE 1
#define FALSE 0

#define LONGUEUR_TAMPON 8092

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
		fprintf(stderr, "Le serveur a ferme la connexion.\n");
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
	printf("[" GREEN "*" RESET "] Connecting to %s:%s\n",iterator2,iterator);
	if(InitialisationAvecService(iterator2,iterator)!=1){
		exit(1);
	}

	return 0;
}

int parseCommand(char *command, account *user) {
	if(strlen(command)==1){
		print_cmdline_help();
		return 1;
	}
	char extract[500];
	strncpy(extract,command,500*sizeof(char));
	char *code=strtok(extract," \n");


	if(strcmp(code,"login")==0){
        printf("[" GREEN "*" RESET "] Logging\n");
        executeCommand(command, AUTH, user);
	} else if(strcmp(code,"createAccount")==0){
		printf("[" GREEN "*" RESET "] Creating an account\n");
        executeCommand(command, A_C, user);
	} else if(strcmp(code,"modifyAccount")==0){
		printf("[" GREEN "*" RESET "] Modifying an account\n");
        executeCommand(command, A_M, user);
	} else if(strcmp(code,"deleteAccount")==0){
		printf("[" GREEN "*" RESET "] Deleting an account\n");
        executeCommand(command, A_D, user);
	} else if(strcmp(code,"displayDirectory")==0){
		printf("[" GREEN "*" RESET "] Displaying a directory\n");
        executeCommand(command, D_P, user);
    } else if(strcmp(code,"displayRecord")==0){
		printf("[" GREEN "*" RESET "] Displaying a directory\n");
        executeCommand(command, R_P, user);
	} else if(strcmp(code,"createRecord")==0){
		printf("[" GREEN "*" RESET "] Creating a record\n");
        executeCommand(command, R_C, user);
    } else if(strcmp(code,"modifyRecord")==0){
		printf("[" GREEN "*" RESET "] Modifying an account\n");
        executeCommand(command, R_M, user);
	} else if(strcmp(code,"deleteRecord")==0){
		printf("[" GREEN "*" RESET "] Deleting an account\n");
        executeCommand(command, R_D, user);
	} else if(strcmp(code,"addReader")==0){
		printf("[" GREEN "*" RESET "] Adding an account\n");
        executeCommand(command, D_A, user);
	} else if(strcmp(code,"rmReader")==0){
		printf("[" GREEN "*" RESET "] Removing an account\n");
        executeCommand(command, D_R, user);
	} else {
		printf("[" RED "!" RESET "] Bad command\n");
		print_cmdline_help();
	}
	return 0;
}

void print_cmdline_help(void){
	printf("Syntax: \n");
    printf("\t- " MAGENTA "login" RESET " <id> <pass>\n");
    printf("\t- " MAGENTA "createAccount" RESET " <username> <pass> <mode>\n");
    printf("\t- " MAGENTA "modifyAccount" RESET " <username> mdp|username|mode <fieldValue>\n");
    printf("\t- " MAGENTA "deleteAccount" RESET " <username>\n");
    printf("\t- " MAGENTA "createRecord" RESET " <record_number> name:<name> firstName:<fisrtName> email:<email> [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]\n");
    printf("\t- " MAGENTA "modifyRecord" RESET " <record_number> [name:<name>] [firstName:<fisrtName>] [email:<email>] [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]\n");
    printf("\t- " MAGENTA "deleteRecord" RESET " <record_number>\n");
    printf("\t- " MAGENTA "addReader" RESET " <username>\n");
    printf("\t- " MAGENTA "rmReader" RESET " <username>\n");
    printf("\t- " MAGENTA "displayDirectory" RESET " me|<username>\n");
    printf("\t- " MAGENTA "displayRecord" RESET " me|<username> <record_number>\n");
}

boolean isSyntaxCorrect(char *command, pdu_code co) {
    char *token = NULL;
    char delim[3] = " \n";
    char *temp_str = malloc(sizeof(char) * (strlen(command) + 1));
    strcpy(temp_str, command);
    token = strtok(temp_str, delim);

    int i = 0;
    while(token) {
        switch (co) {
            case R_C:
                if(i == 1)
                    if(!matchField(token, "^[0-9]{1,2}$"))
                        return FALSE;
                break;
            case R_M:
                if(i == 1)
                    if(!matchField(token, "^[0-9]{1,2}$"))
                        return FALSE;
                break;
            case R_P:
                if(i == 2)
                    if(!matchField(token, "^[0-9]{1,2}$"))
                        return FALSE;
            default:
                break;
        }
        token = strtok(NULL, delim);
        i++;
    }
    switch (co) {
        case AUTH:
            if(i!=3)
                return FALSE;
        break;
        case A_C:
            if(i!=4)
                return FALSE;
        break;
        case A_M:
            if(i!=4)
                return FALSE;
        break;
        case A_D:
            if(i!=2)
                return FALSE;
        break;
        case R_C:
            if(i<5 || i>8)
                return FALSE;
        break;
        case R_M:
            if(i<3 || i>8)
                return FALSE;
        break;
        case R_D:
            if(i!=2)
                return FALSE;
        break;
        case D_A:
            if(i!=2)
                return FALSE;
        break;
        case D_R:
            if(i!=2)
                return FALSE;
        break;
        case D_P:
            if(i!=2)
                return FALSE;
        break;
        case R_P:
            if(i!=3)
                return FALSE;
        break;
        default:
            fprintf(stderr, "[" RED "!" RESET "] : " RED "Unknowned command." RESET "\n");
    }
    return TRUE;
}

boolean isLogged(account a) {
    if(strlen(a.username))
        return TRUE;
    return FALSE;
}

char *showSyntax(pdu_code co) {
    switch (co) {
        case A_C:
            return "createAccount <username> <pass> <mode>";
        case A_M:
            return "modifyAccount <field> <new_value>";
        case A_D:
            return "deleteAccount <username>";
        case AUTH:
            return "login <username> <pass>";
        case R_C:
            return "createRecord <record_number> name:<name> firstName:<fisrtName> email:<email> [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]";
        case R_M:
            return "modifyRecord <record_number> [name:<name>] [firstName:<fisrtName>] [email:<email>] [comments:<comments>] [birthDate:<birthDate] [phone:<phone>]";
        case R_D:
            return "deleteRecord <record_number>";
        case D_A:
            return "addReader <username>";
        case D_R:
            return "rmReader <username>";
        case D_P:
            return "displayDirectory me|<username>";
        case R_P:
            return "displayRecord me|<username> <record_number>";
        default:
            return "Command doesn't exist";
    }
}

char *clearRequest(char *command) {
    return &(strchr(command, ' ')[1]);
}

int executeCommand(char *command, pdu_code co, account *user){
	char *retour;//=malloc(LONGUEUR_TAMPON*sizeof(char));
    char *message;
    char *name=NULL;

    if(!isSyntaxCorrect(command, co)) {
        printf("[" RED "!" RESET "] Syntax incorrect. Valid syntax:\n");
        printf("\t%s\n", showSyntax(co));
        return 3;
    }

    char *pduRequest;
    name = clearRequest(command);
    if(co != AUTH) {
        if(!isLogged(*user)) {
            printf("[" RED "!" RESET "] : " RED "You must be logged to use this command" RESET "\n");
            return 1;
        }
        pduRequest = malloc(sizeof(char) * (strlen(user->username) + 2 + strlen(name)));
        sprintf(pduRequest, "%s %s", user->username, name);
    } else {
        pduRequest = name;
    }

    pdu t = generateReturnedPdu(co, pduRequest);
    PDUToMessage(t, &message);

    Emission(message);
    retour = Reception();
		if(!retour){
			exit(1);
		}

    messageToPDU(&t, retour);
    if(t.code == OK) {
        if(co == AUTH) {
            int u = 0;
            char *name = clearRequest(command);
            memset(user->username, 0, LNAME);
            while(name[u] != ' ') {
                user->username[u] = name[u];
                u++;
            }
            readAccount(PATH_ACCOUNT_STORAGE, user, seekAccount(PATH_ACCOUNT_STORAGE, *user));
        }
        printf("[" GREEN "*" RESET "] : " GREEN "%s" RESET "\n", t.request);
    }
    else
        printf("[" RED "!" RESET "] : " RED "%s" RESET "\n", t.request);
    free(retour);
    free(t.request);
    return 0;
}
