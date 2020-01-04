#ifndef __DATA_STRUCTURES__
#define __DATA_STRUCTURES__

#include <string.h>

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

typedef enum boolean {
    TRUE = 0,
    FALSE = 1
} boolean;



#endif