
#include "trace.h"

int
recv_v4(struct trace_status *statu, struct packet *pkt, struct proto *pro)
{
    int ret;
    memset(statu->recvbuf, 0, sizeof(statu->buflen));
    statu->gotalarm = 0;
    alarm(1);
    
    for( ; ; ){
        if(1 == statu->gotalarm)
            return -3;
        else{
            if((ret = recvfrom(statu->sendfd, statu->recvbuf, statu->buflen, 0, NULL, NULL)) <= 0){
                if (errno == EINTR)
                    continue;
                else
                    err_sys("recvfrom error");
            }
            
            if(ret < sizeof(struct ip))
                continue;
            u_int8_t *buf = (u_int8_t *)statu->recvbuf;
            memcpy(&pkt->recv.iphdr, buf, sizeof(struct ip));
            
            if(pro->icmpproto != pkt->recv.iphdr.ip_p)
                continue;
            if(pkt->recv.iphdr.ip_len < sizeof(struct ip) + sizeof(struct icmp))
                continue;
            memcpy(&pkt->recv.icmphdr, buf+sizeof(struct ip), sizeof(struct icmp));
            
            int icmp_datalen = pkt->recv.iphdr.ip_len - sizeof(struct ip) + sizeof(struct icmp);
            //printf("````");
            
            alarm(0);
            if(ICMP_TIME_EXCEEDED == pkt->recv.icmphdr.icmp_type){
                if(0 == statu->flags_echo){
                    struct sockaddr_in *recv = (struct sockaddr_in *)pro->sarecv;
                    memcpy(&recv->sin_addr, &pkt->recv.iphdr.ip_src, sizeof(struct in_addr));
                    pro->sarecv = (struct sockaddr  *)recv;
                    statu->flags_echo = 1;
                    char str[128];
                    inet_ntop(AF_INET, &recv->sin_addr, str, 128);
                    printf(" %s  ", str);
                }
                return -1;
            }
            else if(ICMP_ECHOREPLY == pkt->recv.icmphdr.icmp_type){
                struct sockaddr_in *recv = (struct sockaddr_in *)pro->sarecv;
                memcpy(&recv->sin_addr, &pkt->recv.iphdr.ip_src, sizeof(struct in_addr));
                pro->sarecv = (struct sockaddr  *)recv;
                statu->flags_echo = 1;
                char str[128];
                inet_ntop(AF_INET, &recv->sin_addr, str, 128);
                printf(" %s  \n", str);
                return -2;
            }
            else{
                return -3;
            }
        }
    }
    alarm(0);
    
    return 0;
}
