#ifndef __RECORD__
#define __RECORD__

#include "data_structures.h"
#include "utils.h"
#include "pdu.h"
#include "account.h"
#include <regex.h>

/* ------- RECORDIO ------- */


/*
    Stores a record to a binary file.
    Input:
        filename : char * : The name of the file that will be written
        cur : int : A cursor to navigate within the file. The cursor must be used to find
a record, not a precise byte. The function working only with record structures, it will
be able to find the cursor by itself. So to go the third record in the file, the cur value must
be 3. The cursor value start at 0.
    Output:
        r : record : The record structure to store
    Return:
        Exit code:
            0 : OK 
            1 : File could not be opened
            2 : File could not be written

*/
int writeRecord(char *filename, record r, int cur);

/*
    Gets a record from a binary file.
    Input:
        filename : char * : The name of the file from where the record will be read
        cur : int : A cursor to navigate within the file. The cursor must be used to find
a record, not a precise byte. The function working only with record structures, it will
be able to find the cursor by itself. So to go the third record in the file, the cur value must
be 3. The cursor value start at 0.
    Output:
        a : record * : The record structure get from the file
    Return:
        Exit code:
            0 : OK 
            1 : File doesn't exist
            2 : File could not be read

*/
int readRecord(char *filename, record *a, int cur);

/* ------- RECORD_UTILS ------- */

/* 
    Checks if the user requesting the display of a record is allow to do this operation
    Input:
        user : char * : The user requesting the display
        requestedUser : char * : The user's desired directory
    Return
        TRUE or FALSE
*/
boolean R_DAuthorization(char *user, char *requestedUser);

/*
    Parses the request string into the settings array according to the R_D rules
    Input:
        request : char * : The request to parse
    Output:
        settints : char *** : The array containing the parsed elements
*/
void getR_DParameters(char *request, char ***data);

/*
    Parses the request string into the settings array according to the R_M rules
    Input:
        request : char * : The request to parse
    Output:
        settints : char *** : The array containing the parsed elements
*/
void getR_MParameters(char *request, char ***settings, int N);

/*
    Clear the strings composing the record structure
    Output:
        r: record * : The record to clear
*/
int getSettingsNumber(char *request);

/*
    TO FILL
*/
void clearRecord(record *r);

/*
    TO FILL
*/
boolean userCanRead(account host, char *guest);

/*
    TO FILL
*/
boolean matchField(char *field, char *str_regex);

/* ------- RECORD ------- */

/*
    TO FILL
*/
pdu createRecord(char *request);

/*
    TO FILL
*/
pdu modifyRecord(char *request);

/*
    TO FILL
*/
pdu deleteRecord(char *request);

/*
    TO FILL
*/
pdu displayRecord(char *request);



#endif