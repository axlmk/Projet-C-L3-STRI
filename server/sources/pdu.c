#include "../headers/pdu.h"

int PDUToMessage(pdu p, char **m) {
    *m = malloc(sizeof(char) * (sizeof(pdu_source) + sizeof(pdu_code) + sizeof(int) + sizeof(char) * p.requestSize));
    return sprintf(*m, "%d%d%3.d%s", p.source, p.code, p.requestSize, p.request);
}

int messageToPDU(pdu *p, char *m) {
    return 0;
}
