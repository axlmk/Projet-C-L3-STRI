#ifndef __DATA_STRUCTURES__
#define __DATA_STRUCTURES__

#include "utils.h"

typedef enum boolean {
    TRUE = 1,
    FALSE = 0
} boolean;

typedef enum pdu_code {
    AUTH = 0,
    A_C = 1,
    A_M = 2,
    A_D = 3,
    D_C = 4,
    D_S = 5,
    R_C = 6,
    R_M = 7,
    R_D = 8
} pdu_code;

typedef enum pdu_source {
    SERVER = 0,
    CLIENT = 1
} pdu_source;

typedef struct record {
    char name[15];
    char firstName[15];
    char phone[15];
    char address[30];
    char email[30];
    char birthDate[11];
    char comments[50];
} record;

typedef struct directory {
    record recordList[20];
} directory;

typedef struct account {
    char username[15];
    char password[15];
    directory ownedDirectory;
    directory sharedDirectory[20];
} account;

/* The 'request' field must be terminated by a '\0' character */
typedef struct pdu {
    pdu_source source;
    pdu_code code;
    int requestSize;
    char *request;
} pdu;

#endif