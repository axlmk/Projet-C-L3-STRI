#ifndef __TOOLS__
#define __TOOLS__

#include "utils.h"
#include "data_structures.h"
#include <regex.h>

/*
    Checks if a string correspond to a regular expression
    Input:
        field : char * : The string to compare
        str_regex : char * : The regex to match
    Return:
        TRUE or FALSE
*/
boolean matchField(char *field, char *str_regex);

#endif