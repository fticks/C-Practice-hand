
#include "unp.h"
#include "chatroom.h"

void
sig_int(int arg)
{
    err_quit("\nrecving Interrupt\n");
    exit(0);
}

void
mydg_cli(FILE *fp, int sockfd, SA *pservaddr, socklen_t servlen)
{
    int	n;
    char	sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t len = servlen;

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Send(sockfd, sendline, strlen(sendline), 0);
        n = Recv(sockfd, recvline, MAXLINE, 0);
        recvline[n] = 0;	/* null terminate */
        Fputs(recvline, stdout);
    }
}

int
main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in  servaddr;

    //if (argc != 2)
    	//err_quit("usage: udpcli <IPaddress>");

    Signal(SIGINT,sig_int);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    
    mydg_cli(stdin, sockfd, NULL, 0);/* not to set addr */
}
