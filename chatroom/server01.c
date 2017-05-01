
#include "unp.h"
#include "chatroom.h"

void
sig_int(int arg)
{
    err_quit("\nsever recv Interrupt\n");
    exit(0);
}

struct client_list *list = NULL;

/* dg_echo */
void 
dg_broadcast(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int	    n;
    socklen_t   len;
    char    mesg[MAXLINE];

    for ( ; ; ) {
        len = clilen;
        memset(mesg,0,sizeof(mesg));/* It is best to be able to initialize */
        n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

        char str[128] = {0};
        if(0 == memcmp(mesg, sig_handshake, sizeof(sig_handshake))){/* can't do that, for sin_len */
            struct client_list *tmp = list;
            int a = 0;
            while(NULL != tmp){
                tmp = tmp->next;
                a++;
                printf("inter %d\n", a);
            }
            tmp = (struct client_list *)malloc(sizeof(struct client_list));
            tmp->next = NULL;
            memcpy(&tmp->cliaddr, pcliaddr, len);
            
            printf("hand shake, client addr:%s\n", Inet_ntop(AF_INET, &tmp->cliaddr, str, sizeof(str)));
        }
        else if(0 == memcmp(mesg, sig_disconnect, sizeof(sig_disconnect))){
            printf("disconnect, client addr:%s\n", Inet_ntop(AF_INET, pcliaddr, str, sizeof(str)));
        }
        else{
            printf("recvfrom addr:%s, datagram: %s",Inet_ntop(AF_INET, pcliaddr, str, sizeof(str)), mesg);
            
            Sendto(sockfd, mesg, n, 0, pcliaddr, len);
        }
        fflush(stdout);
    }
}

int
main(int argc, char **argv)
{
    int	    sockfd;
    struct sockaddr_in  servaddr, cliaddr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Signal(SIGINT,sig_int);

    Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    dg_broadcast(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
