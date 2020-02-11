#ifndef __RECORD__
#define __RECORD__

#include "data_structures.h"
#include "utils.h"
#include "pdu.h"
#include "accountio.h"
#include <regex.h>

int writeRecord(char *filename, record r, int cur);
int readRecord(char *filename, record *a, int cur);
pdu modifyRecord(char *request);
boolean matchField(char *field, char *str_regex);
pdu deleteRecord(char *request);
pdu createRecord(char *request);
void clearRecord(record *record);

#endif