#ifndef __INSTALL__
#define __INSTALL__

#include "utils.h"
#include "data_structures.h"
#include "accountio.h"

/*
    Check if the server was previously installed or not. Create the storage file and the admin user if it wasn't
    Return :
        0 : The server was previously installed
        1 : The server wasn't previously installed
*/
int initiateServer(void);

#endif