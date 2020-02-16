#ifndef __DATA_STRUCTURES__
#define __DATA_STRUCTURES__

#include "utils.h"

#define NDIRECTORY 20
#define LNAME 15
#define NRECORDS 20
#define LPASS 15
#define LADDRESS 50
#define LCOMMENTS 200

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
    KO = 14
} pdu_code;

typedef enum user_type {
    USER = 0,
    ADMIN = 1
} user_type;

typedef struct record {
    char name[LNAME];
    char firstName[LNAME];
    char phone[11];
    char address[LADDRESS];
    char email[LADDRESS];
    char birthDate[11];
    char comments[LCOMMENTS];
} record;

typedef record directory[NRECORDS];

typedef struct account {
    user_type userT;
    char username[LNAME];
    char password[LPASS];
    char sharedDirectory[NDIRECTORY][LNAME];
    directory ownedDirectory;
} account;

/* The 'request' field must be terminated by a '\n' character and then a '\0' character */
typedef struct pdu {
    pdu_code code;
    char *request;
} pdu;

#endif