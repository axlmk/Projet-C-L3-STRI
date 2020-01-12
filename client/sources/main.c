#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/client.h"
int main(int argc,char **argv)
{
    char *current=malloc(500*sizeof(char));
    memset(current,0,500*sizeof(char));
    current[0]='\0';
    if(argc<3){
        print_usage(argv[0]);
        return 1;
    }
    if(init(argv)){
        printf("Incorrect arguments\n");
        return 1;
    }
    while(1){
        printf("#: ");
        fgets(current,sizeof(char)*500,stdin);
        fflush(stdin);
        if(strcmp(current,"quit\n")==0){break;}
        parseCommand(current);
    }
    free(current);
    Terminaison();
    return 0;
}
