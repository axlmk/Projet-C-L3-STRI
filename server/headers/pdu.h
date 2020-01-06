#ifndef __PDU__
#define __PDU__

#include "data_structures.h"

/*
    Convert a pdu structure into a string
    Warning:
        Be careful, the integer format used in the sprintf function would have to change if the number of enumerations becomes greater than 9
    Input:
        p : pdu : the pdu to convert
    Output:
        m : char** : the converted string 
    Return:
        The number of characters well printed by the sprintf function
*/
int PDUToMessage(pdu p, char **m);
int messageToPDU();


#endif