
#include "unp.h"
#include "chatroom.h"

void
sig_int(int arg)
{
    err_quit("\n recving Interrupt\n");
    exit(0);
}

void
mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
    int			n;
    socklen_t	len;
    char		mesg[MAXLINE];

    for ( ; ; ) {
        len = clilen;
        n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

        Sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}


int
main(int argc, char **argv)
{
    int	    sockfd;
    struct sockaddr_in  listenaddr, cliaddr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&listenaddr, sizeof(listenaddr));
    listenaddr.sin_family      = AF_INET;
    listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    listenaddr.sin_port        = htons(SERV_PORT);

    Signal(SIGINT,sig_int);

    Bind(sockfd, (SA *) &listenaddr, sizeof(listenaddr));

    mydg_echo(sockfd, (SA *)&listenaddr, sizeof(listenaddr));
}

