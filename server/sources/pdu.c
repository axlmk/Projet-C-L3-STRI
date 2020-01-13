#include "../headers/pdu.h"

int PDUToMessage(pdu p, char **m) {
    *m = malloc(sizeof(char) * (sizeof(pdu_code) + sizeof(char) * (strlen(p.request) + 1)));
    return sprintf(*m, "%2.d%s\n", p.code, p.request);
}

int messageToPDU(pdu *p, char *m) {
    char *ccode=malloc(sizeof(char));
    if(!m)
        return 1;
    p->code = 0;
    ccode[0]=m[0];
    p->code=atoi(ccode);
    strcpy(p->request, &m[2]);
    p->request[strlen(p->request) - 1] = '\0';
    return 0;
}

pdu generateReturnedPdu(pdu_code code, char *request) {
    pdu res;
    res.code = code;
    res.request = malloc(sizeof(char) * (strlen(request) + 1));
    strcpy(res.request, request);
    return res;
}