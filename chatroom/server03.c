
#include "unp.h"
#include "chatroom.h"

void
sig_int(int arg)
{
    err_quit("\n recving Interrupt\n");
    exit(0);
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

    u_int8_t recvbuf[MAXLINE], sendbuf[MAXLINE];
    struct sockaddr peeraddr;
    for( ; ; ){
        
        socklen_t cliaddr_len;
        u_int32_t nbytes;
        nbytes = Recvfrom(sockfd, recvbuf, MAXLINE, 0, &peeraddr, &cliaddr_len);
        if(fork() == 0){/* child */
            //socket();
            break;
        }
        /* parent */
    }
    
    /* child */
    int peer = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in  childaddr;
    
    bzero(&childaddr, sizeof(struct sockaddr_in));
    childaddr.sin_family      = AF_INET;
    childaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    childaddr.sin_port   = htons(0);/* randmo port */
    
    Bind(peer, (SA *)&childaddr, sizeof(struct sockaddr_in));
    Connect(peer, (SA *)&peeraddr, sizeof(struct sockaddr_in));
    
    while(1){
        Send(peer, recvbuf, MAXLINE, 0);
        /* process */
        Recv(peer, recvbuf, MAXLINE, 0);
    }
    
}

