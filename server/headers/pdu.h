#ifndef __PDU__
#define __PDU__

#include "data_structures.h"

/*
    Convert a pdu structure into a string
    Input:
        p : pdu : the pdu to convert
    Output:
        m : char** : the converted string 
    Return:
        The number of characters well printed by the sprintf function
*/
int PDUToMessage(pdu p, char **m);

/*
    Convert a char array into a pdu structure
    Warning: m must be terminated by a '\n' character and then a '\0'. This information is only useful if tests are running on this function. 
    Input:
        m : char* : the string to convert
    Output:
        p : pdu* : the created pdu
    Return
        0 : OK
        1 : m is NULL;
*/
int messageToPDU(pdu *p, char *m);


/*
    Generates the PDU that will be send back to the client according to the calling function
    Input:
        code : pdu_code : The code to send
        returnedRequest : char * : The request to send
    Return:
        The newly created PDU
*/
pdu generateReturnedPdu(pdu_code code, char *returnedRequest);


#endif