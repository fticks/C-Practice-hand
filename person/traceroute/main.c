
#include "trace.h"

/* global init */
struct proto    proto_v4 = {
    
    .ficmpcode = icmpcode_v4,
    .fsend = send_v4,
    .frecv = recv_v4,
    .icmpproto = IPPROTO_ICMP,
    .hdrincl_level = IPPROTO_IP,
    .hdrincl_optname = IP_HDRINCL,
};

struct trace_status status = {
    .sendpkt_len = 78,
    .hops_time = 1,
    .buflen = BUFSIZE,    
    .flags_echo = 0,
};

void
sig_term(int arg)
{
       err_quit("\nreceive trem signal````````````````\n");
}

int
main(int argc, char **argv)
{
    int	    opt_c;
    struct addrinfo *host_ai;
    char *host_expr;

    /* global init */
    hops_max = 15;

    //daemon_inetd("traceroute", LOG_USER);

    opterr = 0;
    while((opt_c=getopt(argc, argv,"m:v")) != -1){
        switch(opt_c){
        case 'v':
            verbose++;
            err_msg("option:-v ",verbose);
            break;
        case 'm':
            hops_max = atoi(optarg);
            err_msg("option:-m %d", hops_max);
            break;
        case '?':
            err_quit("unrecognized option \n"
                           "usage: %s [-v -m:] <hostname> \n", argv[0]);
            break;
        }
    }
    if (optind != argc-1)
    	err_quit("usage: %s ping [ -v -m:] <hostname>", argv[0]);
    hostname = argv[optind];
    
    Signal(SIGALRM, sig_alrm);
    Signal(SIGINT, sig_term);
    
    {/* caller ths DNS to analysis host name input, result addrinfo{} */
        struct addrinfo hints;
        int ret;
        const char *serv = NULL;
        
        memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = 0;		/* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = 0;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */
        
        if((ret = getaddrinfo(hostname, serv, &hints, &host_ai)) != 0)
            err_quit("getaddrinfo error for %s, %s: %s",
                 (hostname == NULL) ? "(no hostname)" : hostname,
                 (serv == NULL) ? "(no service name)" : serv,
                 gai_strerror(ret));
    }
    
    char str[128];  /* Unix domain is largest, accodion to AF_xxx.addrlen */
    {/* change value to expression */
        if(AF_INET == host_ai->ai_family){
            struct sockaddr_in  *sin = (struct sockaddr_in *) host_ai->ai_addr;
            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                err_sys("inet_ntop error");
        }
        else if(AF_INET6 == host_ai->ai_family){
            err_quit("dont accept AF_INET6\n");
        }
        else{
            err_quit("inet_ntop: unknown AF_xxx: %d, len %d", host_ai->ai_family, host_ai->ai_addrlen);
        }
    }
    printf("\n\ntraceroute to %s (%s), %d hops max, %d byte packets\n", 
        host_ai->ai_canonname ? host_ai->ai_canonname : hostname,
        str, hops_max, status.sendpkt_len);

    /* According to the protocol, fill the structure */
    if(AF_INET == host_ai->ai_family){
        gs_pr = &proto_v4;
    }
    else if(AF_INET6 == host_ai->ai_family){
        //gs_pr = &proto_v6;
        err_quit("dont accept AF_INET6\n");
    }
    status.pid = getpid() & 0xffff;	/* ICMP ID field is 16 bits */
    gs_pr->salen = host_ai->ai_addrlen;
    gs_pr->sasend = (struct sockaddr  *)host_ai->ai_addr;
    gs_pr->sarecv = (struct sockaddr  *)malloc(sizeof(host_ai->ai_addrlen));
    gs_pr->salast = (struct sockaddr  *)malloc(sizeof(host_ai->ai_addrlen));
    gs_pr->sabind = (struct sockaddr  *)malloc(sizeof(host_ai->ai_addrlen)); 
    
    traceloop();
    
    exit(0);
}
