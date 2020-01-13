#ifndef __DATA_STRUCTURES__
#define __DATA_STRUCTURES__

#include "utils.h"

#define NDIRECTORY 20
#define LNAME 15
#define NRECORDS 20
#define LPASS 15

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
    D_D = 5,
    D_P = 6,
    D_A = 7,
    D_R = 8,
    R_C = 9,
    R_M = 10,
    R_D = 11,
    R_P = 12,
    OK = 13,
    KO = 14,
} pdu_code;

typedef struct record {
    char name[15];
    char firstName[15];
    char phone[10];
    char address[30];
    char email[30];
    char birthDate[10];
    char comments[50];
} record;

typedef record directory[NRECORDS];

typedef struct account {
    char username[LNAME];
    char password[LPASS];
    directory ownedDirectory;
    char sharedDirectory[NDIRECTORY][LNAME];
} account;

/* The 'request' field must be terminated by a '\n' character and then a '\0' character */
typedef struct pdu {
    pdu_code code;
    char *request;
} pdu;

#endif