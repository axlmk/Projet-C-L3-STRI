#include "../headers/pdu.h"

int PDUToMessage(pdu p, char **m) {
    *m = malloc(sizeof(char) * (sizeof(pdu_code) + sizeof(char) * (strlen(p.request) + 1)));
    return sprintf(*m, "%2.d%s\n", p.code, p.request);
}

int messageToPDU(pdu *p, char *m) {
    if(!m)
        return 1;
    p->code = 0;

    if(m[0] >= 48 && m[0] < 58)
        p->code += (m[0] - 48) * 10;
    if(m[1] >= 48 && m[1] < 58)
        p->code += (m[1] - 48);
    free(p->request);
    p->request = malloc(sizeof(char) * (strlen(&m[2]) + 1));
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