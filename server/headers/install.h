#ifndef __INSTALL__
#define __INSTALL__

#include "utils.h"
#include "data_structures.h"
#include "account.h"
#include <dirent.h>
#include <errno.h>

/*
    Checks if the server was previously installed or not. Create the storage file and the admin user if it wasn't
    Return :
        0 : The server was previously installed
        1 : The server wasn't previously installed
*/
int initiateServer(void);


/*
    Creates a file
    Input:
        filename : char * : The name of the file
    Return:
        0 : An error occured while opening the file
        1 : The file was successfully created
*/
int createFile(char *filename);

#endif
