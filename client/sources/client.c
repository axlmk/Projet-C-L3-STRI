
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
     		fprintf(stderr, "Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
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
     		perror("Initialisation, erreur de connect.");
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
	printf("Emission de %d caracteres.\n", taille+1);
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
			return 1;
		}
	}
	iterator=strtok(NULL,":");
	port = atoi(iterator);
	if(port<=80){
		return 1;
	}
	printf("[+] Connecting to %s:%s\n",iterator2,iterator);
	if(InitialisationAvecService(iterator2,iterator)!=1){
		/* TO CHANGE IMPERATIVELY FOR INTEGRATION*/
		return 0;
	}
	
	return 0;
}

int parseCommand(char *command){
	char extract[500];
	strncpy(extract,command,500*sizeof(char));
	char *code=strtok(extract," ");
	if(strcmp(code,"login")==0){
		printf("[+] Authentification\n");
		login(command);
	}
	else if(strcmp(code,"register")==0){
		printf("[+] Creating and account\n");
		reg(command);
	}else if(strcmp(code,"modifyacc")==0){
		printf("[+] Modifiying an existing account\n");
		modifyacc(command);
	}else if(strcmp(code,"delacc")==0){
		printf("[+] Deleting an existing account\n");
		delacc(command);
	}else if(strcmp(code,"dircreate")==0){
		printf("[+] Creating a directory\n");
		dircreate(command);
	}else if(strcmp(code,"dirdelete")==0){
		printf("[+] Deleting a directory\n");
		dirdelete(command);
	}else if(strcmp(code,"dirdump")==0){
		printf("[+] Dumping a directory\n");
		dirdump(command);
	}else if(strcmp(code,"recordc")==0){
		printf("[+] Creating a record\n");
		dir_recordCreate(command);
	}
	else {
		printf("Bad command\n");
		print_cmdline_help();
	}
	return 0;
}

void print_cmdline_help(void){
	printf("Syntax: \nlogin id mdp\nmodifyacc old_id new_id old_mdp new_mdp\n");
}

int delacc(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	(void *)command;
	(void *)retour;
	int len=0;
	char *send=NULL;
	char *login=strtok(command," ");
	char *mdp=NULL;
	char *to_delete=NULL;
	login=strtok(NULL," ");
	mdp=login;
	mdp=strtok(NULL," ");
	to_delete=mdp;
	to_delete=strtok(NULL," ");
	len=strlen(login)+strlen(mdp)+strlen(to_delete)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"A_D ");
	strcat(send,login);
	strcat(send," ");
	strcat(send,mdp);
	strcat(send," ");
	strcat(send,to_delete);
	strcat(send,"\n");
	printf("%s",send);
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int dircreate(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *recordIndex=strtok(command," ");
	char *username=NULL;	
	recordIndex=strtok(NULL," ");
	username=recordIndex;
	recordIndex=strtok(NULL," ");
	len=strlen(username)+strlen(recordIndex)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"D_C ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,recordIndex);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int dirdelete(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *recordIndex=strtok(command," ");
	char *username=NULL;	
	recordIndex=strtok(NULL," ");
	username=recordIndex;
	recordIndex=strtok(NULL," ");
	len=strlen(username)+strlen(recordIndex)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"D_D ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,recordIndex);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int dirdump(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *recordIndex=strtok(command," ");
	char *username=NULL;	
	recordIndex=strtok(NULL," ");
	username=recordIndex;
	recordIndex=strtok(NULL," ");
	len=strlen(username)+strlen(recordIndex)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"D_P ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,recordIndex);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int diraccess(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *targetedUser=strtok(command," ");
	char *username=NULL;	
	targetedUser=strtok(NULL," ");
	username=targetedUser;
	targetedUser=strtok(NULL," ");
	len=strlen(username)+strlen(targetedUser)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"D_A ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,targetedUser);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int dir_rrights(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *targetedUser=strtok(command," ");
	char *username=NULL;	
	targetedUser=strtok(NULL," ");
	username=targetedUser;
	targetedUser=strtok(NULL," ");
	len=strlen(username)+strlen(targetedUser)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"D_R ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,targetedUser);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int modifyacc(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	(void *)command;
	(void *)retour;
	int len=0;
	char *send=NULL;
	char *old_id=strtok(command," ");
	char *new_id=NULL;
	char *old_mdp=NULL;
	char *new_mdp=NULL;	
	old_id=strtok(NULL," ");
	new_id=old_id;
	new_id=strtok(NULL," ");
	old_mdp=new_id;
	old_mdp=strtok(NULL," ");
	new_mdp=old_mdp;
	new_mdp=strtok(NULL," ");
	len=strlen(old_id)+strlen(new_id)+strlen(old_mdp)+strlen(new_mdp)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"A_M ");
	strcat(send,old_id);
	strcat(send," ");
	strcat(send,new_id);
	strcat(send,"\n");
	strcat(send,old_mdp);
	strcat(send," ");
	strcat(send,new_mdp);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int dir_recordCreate(char *command){
	/*comments not implemented*/
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	(void *)command;
	(void *)retour;
	int len=0;
	char *send=NULL;
	char *username=strtok(command," ");
	char *name=NULL;
	char *firstname=NULL;
	char *phone=NULL;
	char *address=NULL;
	char *email=NULL;
	char *birthdate=NULL;
	/*char *comments=NULL;*/	
	username=strtok(NULL," ");
	name=username;
	name=strtok(NULL," ");
	firstname=name;
	firstname=strtok(NULL," ");
	phone=firstname;
	phone=strtok(NULL," ");
	address=phone;
	address=strtok(NULL," ");
	email=address;
	email=strtok(NULL," ");
	birthdate=email;
	birthdate=strtok(NULL," ");
	/*comments=birthdate;
	comments=strtok(NULL," ");*/
	if(username==NULL || name==NULL || firstname==NULL || phone==NULL || address==NULL || email==NULL || birthdate==NULL){
		printf("[+] Wrong syntax\n");
		free(retour);
		return 1;
	}
	len=strlen(username)+strlen(name)+strlen(firstname)+strlen(phone)+strlen(address)+strlen(email)+strlen(birthdate)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"R_C ");
	strcat(send,username);
	strcat(send," ");
	strcat(send,name);
	strcat(send," ");
	strcat(send,firstname);
	strcat(send," ");
	strcat(send,phone);
	strcat(send,"\n");
	strcat(send,address);
	strcat(send," ");
	strcat(send,email);
	strcat(send," ");
	strcat(send,birthdate);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}

int reg(char *command){
	return 0;
}

int login(char *command){
	char *retour=malloc(LONGUEUR_TAMPON*sizeof(char));
	int len=0;
	char *send=NULL;
	char *mdp=strtok(command," ");
	char *id=NULL;	
	mdp=strtok(NULL," ");
	id=mdp;
	mdp=strtok(NULL," ");
	len=strlen(id)+strlen(mdp)+10;
	send=calloc(1,sizeof(char)*len);
	strcat(send,"AUTH ");
	strcat(send,id);
	strcat(send," ");
	strcat(send,mdp);
	strcat(send,"\n");
	Emission(send);
	retour=Reception();
	/*need to add checks on return value*/
	free(send);
	return 0;
}