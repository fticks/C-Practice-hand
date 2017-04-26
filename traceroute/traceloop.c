
#include "trace.h"

void
traceloop(void)
{
    int ret, done = 0;
    struct timeval  tvrecv;
    struct packet pkt;
    int hdrincl = 1;/* customize iphdr optval */
    
    if((status.sendfd = socket(gs_pr->sasend->sa_family, SOCK_RAW, gs_pr->icmpproto)) < 0)
        err_sys("socket err");
    
    if((ret = setsockopt(status.sendfd, gs_pr->hdrincl_level, gs_pr->hdrincl_optname, &hdrincl,
        sizeof(hdrincl))) < 0)
        err_sys("setsockopt err");

    struct sockaddr *sa = gs_pr->sabind ;
    socklen_t salenptr;
    Getsockname(status.sendfd, sa, &salenptr);

    while(status.hops_time < hops_max+1 && done==0)
    {        
        int looptime = 3;//Each jump, trail 3 times
        status.flags_echo = 0;
        printf("%d", status.hops_time);
        fflush(stdout);
        
        for(int curtime = 0; curtime < looptime && done==0; curtime ++)
        {
            Gettimeofday(&status.sendtv,NULL);
            
            gs_pr->fsend(&status, &pkt, gs_pr);

            status.gotalarm = 0;/* init for alarm-signal */
            if((ret = gs_pr->frecv(&status, &pkt, gs_pr)) == -3){
                printf(" *");
            }
            else if(ret == -1){
                Gettimeofday(&status.recvtv,NULL);
                tv_sub(&status.recvtv, &status.sendtv);
                double rtt = status.recvtv.tv_sec * 1000.0 + status.recvtv.tv_usec / 1000.0;
                printf(" %.3f ms", rtt);
            }
            else if(ret == -2){
                printf("```````````The destination route arrives``````````\n");
                done = 1;
            }

            if(curtime == looptime -1){
                
            }
            fflush(stdout);
        }
        status.hops_time ++;
        printf("\n");
        fflush(stdout);
    }
}
