#ifndef __DIRECTORY__
#define __DIRECTORY__

#include "utils.h"
#include "pdu.h"
#include "account.h"
#include "record.h"

/* ------- DIRECTORY_UTILS ------- */

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
    Determines if the user can has an additional directory
    Input:
        a : account : The user we want to check
    Return
        TRUE or FALSE
*/
boolean isFull(account a);

/*
    Stores a directory to a binary file.
    Input:
        filename : char * : The name of the file that will be written
        t : directory : the directory to write
    Return:
        Exit code:
            0 : OK 
            1 : File could not be opened
            2 : File could not be written

*/
int writeDirectory(char *filename, directory t);

/*
    Read a directory from a binary file.
    Input:
        filename : char * : The name of the file that will be read
        t : directory : the directory to read
    Return:
        Exit code:
            0 : OK 
            1 : File could not be opened
            2 : File could not be read

*/
int readDirectory(char *filename, directory t);

/* ------- RECORD ------- */

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
    Display a requested directory to the client
    Input:
        request : char * : The request sent by the client
    Return:
        The formatted pdu to send
*/
pdu displayDirectory(char *request);

#endif