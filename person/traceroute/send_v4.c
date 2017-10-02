
#include "trace.h"

void
send_v4(struct trace_status *statu, struct packet *pkt, struct proto *pro)
{
    memset(statu->sendbuf, 0, sizeof(statu->buflen));
    
    /* ip_hdr */
    pkt->send.iphdr.ip_v = IPVERSION;
    pkt->send.iphdr.ip_hl = 5;
    pkt->send.iphdr.ip_tos = 0;
    pkt->send.iphdr.ip_len = statu->sendpkt_len;
    
    pkt->send.iphdr.ip_id = statu->pid;
    pkt->send.iphdr.ip_off = 0;
    
    pkt->send.iphdr.ip_ttl = statu->hops_time;
    pkt->send.iphdr.ip_p = pro->icmpproto;
    pkt->send.iphdr.ip_sum = 0;

    struct sockaddr_in *dst = (struct sockaddr_in *)pro->sasend;
    pkt->send.iphdr.ip_dst = dst->sin_addr;
    struct sockaddr_in *src = (struct sockaddr_in *)pro->sabind;
    pkt->send.iphdr.ip_src = src->sin_addr;
    
    /* icmp_hdr */
    pkt->send.icmphdr.icmp_type = ICMP_ECHO;
    pkt->send.icmphdr.icmp_code = 0;
    pkt->send.icmphdr.icmp_cksum = 0;
    pkt->send.icmphdr.icmp_id = statu->pid;
    pkt->send.icmphdr.icmp_seq = statu->hops_time;

    /* icmp_data */ 
    int ip_icmp_hdrlen = sizeof(struct ip) + sizeof(struct icmp);
    memcpy(statu->sendbuf, &pkt->send.iphdr, sizeof(struct ip));
    memcpy(statu->sendbuf+sizeof(struct ip), &pkt->send.icmphdr, sizeof(struct icmp));
    memset(statu->sendbuf+ip_icmp_hdrlen, 0, statu->sendpkt_len-ip_icmp_hdrlen);
    pkt->send.icmphdr.icmp_cksum = in_cksum((u_int16_t *)((u_int8_t *)statu->sendbuf+sizeof(struct ip)), 
        sizeof(struct icmp)+statu->sendpkt_len-ip_icmp_hdrlen);
    memcpy(statu->sendbuf+sizeof(struct ip), &pkt->send.icmphdr, sizeof(struct icmp));
    
    /* sendto */ 
    sendto(statu->sendfd, statu->sendbuf, statu->sendpkt_len, 0, pro->sasend, pro->salen);
}
