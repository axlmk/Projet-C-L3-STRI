#include "../headers/tools.h"
#include "../headers/data_structures.h"

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