
#include "unp.h"
#include "chatroom.h"

int flags_disconnnet = 0;

void
sig_int(int arg)
{
    flags_disconnnet = 1;
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
    
    //dg_chatroom(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    err_quit("\nsever recv Interrupt\n");
    exit(0);
}
