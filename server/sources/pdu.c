#include "../headers/pdu.h"

int PDUToMessage(pdu p, char **m) {
    *m = malloc(sizeof(char) * (sizeof(pdu_code) + sizeof(char) * (strlen(p.request) + 1)));
    return sprintf(*m, "%2.d%s\n", p.code, p.request);
}

int messageToPDU(pdu *p, char *m) {
    if(!m)
        return 1;
    p->code = 0;
    if(m[0] >= 48 || m[0] <= 58)
        p->code += m[0] * 10;
    if(m[2] < 48 || m[2] > 58)
        p->code += m[1];
    strcpy(p->request, &m[2]);
    p->request[strlen(p->request) - 2] = '\0';
    return 0;
}
