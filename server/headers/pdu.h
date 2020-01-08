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
    Input:
        m : char* : the string to convert
    Output:
        p : pdu* : the created pdu
    Return
        0 : OK
        1 : m is NULL;
*/
int messageToPDU(pdu *p, char *m);


#endif