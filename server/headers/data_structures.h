#ifndef __DATA_STRUCTURES__
#define __DATA_STRUCTURES__


#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

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