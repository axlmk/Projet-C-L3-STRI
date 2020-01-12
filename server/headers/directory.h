#ifndef __DIRECTORY__
#define __DIRECTORY__

#include "utils.h"
#include "pdu.h"
#include "accountio.h"
#include "record.h"

/*
    Parses the request string into the settings array according to D_A rules
    Input:
        request : char * : The request to parse
    Output:
        settings : char *** : The array containing the parsed elements
*/
void getD_AParameters(char *request, char ***settings);

/*
    Seeks if a user has already a directory in its shared directories
    Input:
        a : account : The user
        directoryName : char * : The directory
    Return:
        -1 : The user doesn't have this directory
        [0:infinite] : The user has this directory and its index is

*/
int userHasDirectory(account a, char *directoryName);

/*
    Allows a user to have a read only access on the directory of the requesting user
    Input:
        request : char * : The request sent by the client
    Return:
        The formatted pdu to send
*/
pdu addReader(char *request);

/*
    Removes the read only access of a user on the directory of the requesting user
    Input:
        request : char * : The request sent by the client
    Return:
        The formatted pdu to send
*/
pdu rmReader(char *request);

/*
    Determines if the user can has an additional directory
    Input:
        a : account : The user we want to check
    Return
        TRUE or FALSE
*/
boolean isFull(account a);

int writeDirectory(char *filename, directory t);

#endif