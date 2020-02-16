#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/client.h"


int main(int argc,char **argv) {
    char *current=malloc(500*sizeof(char));
    memset(current,0,500*sizeof(char));

    if(argc<2){
        print_usage(argv[0]);
        return 2;
    }

    if(init(argv)) {
        printf("[" RED "!" RESET "] Incorrect arguments\n");
        return 1;
    }

    account cur;
    memset(cur.username, 0, LNAME);

    while(1){
        if(cur.userT == USER)
            printf(CYAN "%s" RESET "#: ", cur.username);
        else
            printf(YELLOW "%s" RESET "#: ", cur.username);
        fgets(current,sizeof(char)*500,stdin);
        fflush(stdin);
        if(strcmp(current,"quit\n")==0) {
            break;
        }
        parseCommand(current, &cur);
    }

    free(current);
    Terminaison();
    return 0;
}
