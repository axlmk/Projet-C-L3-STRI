#include <stdlib.h>
#include <stdio.h>
#include "../headers/client.h"
int main(int argc,char **argv)
{
    if(argc<3){
        print_usage(argv[0]);
        return 1;
    }
    if(args_check(argv)){
        printf("Incorrect arguments\n");
        return 1;
    }
    InitialisationAvecService("localhost","3355");
    return 0;
}
