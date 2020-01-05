#ifndef __ACCOUNTIO__
#define __ACCOUNTIO__

#include <stdio.h>
#include <stdlib.h>
#include "../headers/data_structures.h"

/*
    Get an account from a binary file.
    Input:
        filename : char * : The name of the file from where the account will be read
        cur : int : A cursor to navigate within the file. The cursor must be used to find
an account, not a precise byte. The function working only with account structures, it will
be able to find the cursor by itself. So to go the third account in the file, the cur value must
be 3. The cursor value start at 0.
    Output:
        a : account * : The account structure get from the file
    Return:
        Exit code:
            0 : OK 
            1 : File could not be opened
            2 : File could not be read

*/
int readAccount(char *filename, account *a, int cur);

/*
    Store an account to a binary file.
    Input:
        filename : char * : The name of the file that will be written
        cur : int : A cursor to navigate within the file. The cursor must be used to find
an account, not a precise byte. The function working only with account structures, it will
be able to find the cursor by itself. So to go the third account in the file, the cur value must
be 3. The cursor value start at 0.
    Output:
        a : account : The account structure to store
    Return:
        Exit code:
            0 : OK 
            1 : File could not be opened
            2 : File could not be written

*/
int writeAccount(char *filename, account a, int cur);

#endif