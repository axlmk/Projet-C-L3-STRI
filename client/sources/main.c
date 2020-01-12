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
    while(strcmp(current,"quit\n")!=0){
        fgets(current,sizeof(char)*500,stdin);
        printf("%s",current);
    }
    free(current);
    return 0;
}
