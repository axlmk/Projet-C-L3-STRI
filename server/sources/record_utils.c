#include "../headers/record.h"

boolean userCanRead(account host, char *guest) {
    int i;
    for(i=0;i<NDIRECTORY;i++) {
        if(!strcmp(host.sharedDirectory[i], guest))
            return TRUE;}
    return FALSE;
}

boolean R_DAuthorization(char *user, char *requestedUser) {
    account t;
    strcpy(t.username, user);
    int index;
    if((index = seekAccount(PATH_ACCOUNT_STORAGE, t)) < 0)
        return FALSE;
    if(readAccount(PATH_ACCOUNT_STORAGE, &t, index))
        return FALSE;
    if(requestedUser) {
        if(!userCanRead(t, requestedUser))
            return FALSE;}
    return TRUE;
    
}

void getR_DParameters(char *request, char ***data) {
    int i=0;
    /*Parsage de la requète contenue dans la PDU en entrée*/
    (*data)[i] = strtok(request, " ");
    i++;
    while(i<3){
        (*data)[i] = strtok(NULL, " ");
        i++;
    }
}

int getSettingsNumber(char *request) {
    int count = 1, i = 0;
    while(request[i++])
        if(request[i] == ' ')
            count++;
    return count;
}

void getR_MParameters(char *request, char ***settings, int N) {
    int i = 0;
    (*settings)[i] = strtok(request, " ");
    for(i=1;i<N;i++)
        (*settings)[i] = strtok(NULL, " ");
}

boolean matchField(char *field, char *str_regex) {
   int err;
   regex_t preg;

   err = regcomp(&preg, str_regex, REG_NOSUB | REG_EXTENDED);
   if (!err) {
        int match;

        match = regexec (&preg, field, 0, NULL, 0);
        regfree (&preg);

        if(!match)
            return TRUE;
        else if (match == REG_NOMATCH)
            return FALSE;
        else {
            char *text;
            size_t size;

            size = regerror (err, &preg, NULL, 0);
            text = malloc (sizeof (*text) * size);
            if (text) {
                regerror (err, &preg, text, size);
                fprintf (stderr, "%s\n", text);
                free (text);
            }
            else {
                fprintf (stderr, "Not enough memory\n");
            }
            return FALSE;
        }
   }
   fprintf(stderr, "Error compiling the regexp\n");
   return FALSE;
}

void clearRecord(record *r) {
    memset(r->address, 0, LADDRESS);
    memset(r->phone, 0, 11);
    memset(r->birthDate, 0, 11);
    memset(r->firstName, 0, LNAME);
    memset(r->name, 0, LNAME);
    memset(r->email, 0, LADDRESS);
    memset(r->comments, 0, LCOMMENTS);
}
