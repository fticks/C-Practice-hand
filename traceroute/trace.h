
#include "unp.h"//unpv13e/lib-API

//To reinforce the memory, repeat the header file and annotate it
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>//ushort
#include <sys/socket.h>//socket() connect() bind() listen() accpet()
#include <syslog.h>//openlog() syslog() closelog()

#include <arpa/inet.h>//htons
#include <netinet/in.h>//INADDR_ANY IPPROTO_IP in_addr{} sockaddr_in{} ntohl()
#include <netinet/ip.h>//iphdr{]
#include <netinet/ip_icmp.h>//icmphdr{}
#include <netdb.h>//addrinfo{} gai_strerror() getaddrinfo()

#define	BUFSIZE		1500

/* packet struct */
struct sendpkt_v4 {
    struct ip iphdr;
    struct icmp icmphdr;
 };

struct recvpkt_v4 {
    struct ip iphdr;
    struct icmp icmphdr;
    struct ip iphdr_echo;
    struct icmp icmphdr_echo;
 };

struct pkt_v4{
    struct sendpkt_v4 send;
    struct recvpkt_v4 recv;
};

struct packet{
    struct sendpkt_v4 send;
    struct recvpkt_v4 recv;
};

struct trace_status {
    int	    sendfd, recvfd;	/* send/recv on raw ICMP sock */
    pid_t pid;
    int hops_time;
    int gotalarm;
    struct timeval sendtv;
    struct timeval recvtv;
    char    sendbuf[BUFSIZE];
    int sendpkt_len;
    char    recvbuf[BUFSIZE];
    int buflen;
    int flags_echo;
};

/* trace struct for different ip_protocol */
struct proto {
    const char	*(*ficmpcode)(int);
    void  (*fproc)(void);
    void  (*fsend)(struct trace_status *statu, struct packet *pkt, struct proto *pro);
    int   (*frecv)(struct trace_status *statu, struct packet *pkt, struct proto *pro);
    void  (*finit)(void);
    struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
    struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
    struct sockaddr  *salast;	/* last sockaddr{} for receiving */
    struct sockaddr  *sabind;	/* sockaddr{} for binding source ICMP */
    socklen_t	    salen;		/* length of sockaddr{} */
    int icmpproto;	                /* IPPROTO_xxx value for ICMP */
    int hdrincl_level;
    int hdrincl_optname;
} *gs_pr;

/* globals */
int sendpkt_len;		/* # bytes of data following ICMP header */
char    *hostname;
int verbose;                     /* Visualization option */
int hops_max;   //defualt hops max : 30
extern struct trace_status status;

/* function prototypes */
const char *icmpcode_v4(int code);
void	 proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void    send_v4(struct trace_status *statu, struct packet *pkt, struct proto *pro);
int	 recv_v4(struct trace_status *statu, struct packet *pkt, struct proto *pro);
void	 traceloop(void);
void	 sig_alrm(int);
float mytv_sub(struct timeval tv_a, struct timeval tv_b);
u_short in_checknum(u_short *data, u_int len);

