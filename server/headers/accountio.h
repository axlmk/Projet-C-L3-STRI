#ifndef __ACCOUNTIO__
#define __ACCOUNTIO__

#define PATH_ACCOUNT_STORAGE "../storage/account_test"

#include "data_structures.h"
#include "account_verifier.h"

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
            1 : File doesn't exist
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

/*
    Seek for an account inside the storage file.
    Input :
        filename : char * : The name of the storage file
        a : account : The account we are looking for
    Return :
        -2 : The file doesn't exist
        -1 : The account has not been found
        [0, infinite] : The index of the found account
*/
int seekAccount(char *filename, account a);

/*
    Compare two accounts, based on the username / password couple
    Input:
        a : account : account to compare to
        b : account : account to compare to
    Return:
        TRUE or FALSE
*/
boolean areLoginsEquals(account a, account b);

#endif