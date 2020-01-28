#include "../headers/install.h"

int initiateServer(void) {
    account t;
    strcpy(t.username, "admin");
    int res = seekAccount(PATH_ACCOUNT_STORAGE, t);
    if(res == -2) {
        printf("[*] Storage file not detected.\n[*] Installation of the server.\n");
        strcpy(t.password, "admin");
        createFile(PATH_ACCOUNT_STORAGE);
        writeAccount(PATH_ACCOUNT_STORAGE, t, 0);
        printf("[*] Storage file created and admin account registered.\n[*] Default password for the admin account : " YELLOW "admin" RESET "\n");
        return 1;
    } else if(res == -1) {
        printf("[*] Storage file detected but admin account not detected.\n[*] Check your storage file.\n[*] Shutdown of the server.\n");
        exit(2);

    } else{
        printf("[*] Admin account detected.\n[*] Start of the server.\n");
        return 0;
    }
}

int createFile(char *filename) {
    FILE *f = fopen(filename, "w");

    if(!f)
        return 0;
    fclose(f);
    return 1;
}
