#include "../headers/install.h"


int initiateServer(void) {
    if(!folderExists(PATH_STORAGE))
        if(!createFolder(PATH_STORAGE))
            exit(2);
    
    account t;
    strcpy(t.username, "admin");
    t.userT = ADMIN;
    int res = seekAccount(PATH_ACCOUNT_STORAGE, t);

    if(res == -2) {
        printf("[\033[0;31m!\033[0m] Storage file not detected.\n[\033[0;32m*\033[0m] Installation of the server.\n");
        strcpy(t.password, "admin");
        if(!createFile(PATH_ACCOUNT_STORAGE)){
            fprintf(stderr, "[" RED "*" RESET "] Error while creating a file.\n");
        }
        if(writeAccount(PATH_ACCOUNT_STORAGE, t, 0)!=0){
          printf("[\033[0;31m!\033[0m] Admin account could not be written\n");
          return 1;
        }
        printf("[\033[0;32m*\033[0m] Storage file created and admin account registered.\n[\033[0;32m*\033[0m] Default password for the admin account : " YELLOW "admin" RESET "\n");
        return 1;
    } else if(res == -1) {
        if(writeAccount(PATH_ACCOUNT_STORAGE, t, 0)!=0){
          printf("[\033[0;31m!\033[0m] Admin account could not be written\n");
          return 1;
        }
        printf("[\033[0;31m!\033[0m] Storage file detected but admin account not detected.\n[\033[0;32m*\033[0m] Check your storage file.\n[\033[0;32m*\033[0m] Shutdown of the server.\n");
        exit(2);

    } else{
        printf("[\033[0;32m*\033[0m] Admin account detected.\n[\033[0;32m*\033[0m] Start of the server.\n");
        return 0;
    }
}

int createFile(char *filename) {
    FILE *f = fopen(filename, "w");
    DIR* dir = opendir(PATH_STORAGE);
    if (!dir && ENOENT==errno) {
      printf("[" RED "!" RESET "] Directory %s doesn't exist\n",PATH_STORAGE);
      closedir(dir);
      return 0;
    }
    if(!f){
      return 0;
    }
    fclose(f);
    return 1;
}

int createFolder(char *filename) {
    if(mkdir(filename, 0777)) {
        printf("[\033[0;31m!\033[0m] Folder %s could not be created because : " RED "%s" RESET "\n",filename, strerror(errno));
        return 0;
    }
    return 1;
}

boolean folderExists(char *filename) {
    DIR* dir = opendir(filename);
    if (!dir && ENOENT==errno) {
        closedir(dir);
        return FALSE;
    }
    closedir(dir);
    return TRUE;
    
}
