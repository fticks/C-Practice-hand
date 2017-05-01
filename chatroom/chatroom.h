
#include "unp.h"

typedef struct client_list{
    struct sockaddr_in cliaddr;
    struct client_list *next;
}Client_List;

extern struct client_list *list;

const char sig_handshake[128]="signal: hand shake\n";
const char sig_disconnect[128]="signal: disconnect\n";