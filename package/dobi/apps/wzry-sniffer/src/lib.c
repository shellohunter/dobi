/****************************************************************************
 *
 * Copyright (c) Hua Shao <nossiac@163.com>
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>

#include "lib.h"



void hexdump(uint8_t * p, size_t s, uint8_t * base)
{
    int i, c;
    if ((size_t)s > (size_t)p)
    {
        s = (size_t)s - (size_t)p;
    }
    while ((int)s > 0)
    {
        if (base)
        {
            printf("%08zX: ", (size_t)p - (size_t)base);
        }
        else
        {
            printf("%08zX: ", (size_t)p);
        }
        for (i = 0;  i < 16;  i++)
        {
            if (i < (int)s)
            {
                printf("%02X ", p[i] & 0xFF);
            }
            else
            {
                printf("   ");
            }
            if (i == 7) printf(" ");
        }
        printf(" |");
        for (i = 0;  i < 16;  i++)
        {
            if (i < (int)s)
            {
                c = p[i] & 0xFF;
                if ((c < 0x20) || (c >= 0x7F)) c = '.';
            }
            else
            {
                c = ' ';
            }
            printf("%c", c);
        }
        printf("|\n");
        s -= 16;
        p += 16;
    }
}


char * i2ip(uint32_t ip)
{
    static char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", ip>>24, (ip&0xFFFFFF)>>16, (ip&0xFFFF)>>8, ip&0xFF);
    return buffer;
}


uint32_t ip2i(char * ip)
{
    int i1 = 0,i2 = 0,i3 = 0,i4 = 0;
    int n = sscanf(ip, "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
    return (i1<<24)+(i2<<16)+(i3<<8)+(i4);
}


void print_tcphdr(tcphdr * header)
{
    DBG("<TCP> src port %d", ntohs(header->src));
    DBG("<TCP> dst port %d", ntohs(header->dst));
    DBG("<TCP> seq 0x%02X %u", header->seq, ntohl(header->seq));
    DBG("<TCP> ack 0x%02X %u", header->ack, ntohl(header->ack));
    DBG("<TCP> headlen %d", header->headlen);
    DBG("<TCP> f_urg %d", header->f_urg);
    DBG("<TCP> f_ack %d", header->f_ack);
    DBG("<TCP> f_psh %d", header->f_psh);
    DBG("<TCP> f_rst %d", header->f_rst);
    DBG("<TCP> f_syn %d", header->f_syn);
    DBG("<TCP> f_fin %d", header->f_fin);
    DBG("<TCP> window %u", ntohs(header->window));
    DBG("<TCP> chksum 0x%04X", ntohs(header->chksum));
    DBG("<TCP> urgptr %u", ntohs(header->urgptr));
}

void print_iphdr(iphdr * ip)
{
    DBG("<IP> ver     : %d", ip->ver);
    DBG("<IP> headlen : %d bytes", ip->headlen*4);
    // DBG("<IP> tos     : min_deley %d, max_throu %d, max_relia %d, min_fee %d",
    //        ip->tos&8, ip->tos&4, ip->tos&2, ip->tos&1);
    DBG("<IP> totlen  : %d bytes", ntohs(ip->totlen));
    // DBG("<IP> seq     : %d", ip->seq);
    // DBG("<IP> segoff  : %d", ip->segoff>>13);
    // DBG("<IP> offset  : %d bytes\n", ip->segoff&0x1FFF);
    // DBG("<IP> ttl     : %d", ip->ttl);
    DBG("<IP> srcip   : %s", i2ip(ntohl(ip->srcip)));
    DBG("<IP> dstip   : %s", i2ip(ntohl(ip->dstip)));
    DBG("<IP> proto   : %d", ip->proto);
}

void print_udphdr(udphdr * udp)
{
    DBG("<UDP> src    : %u", ntohs(udp->src));
    DBG("<UDP> dst    : %u", ntohs(udp->dst));
    DBG("<UDP> len    : %d", ntohs(udp->len));
    DBG("<UDP> chksum : %04x", udp->chksum);
}
