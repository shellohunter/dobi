

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


#define SOCK_PAIR "[%08u] %4s %16s : %-5u --> %-16s:%-5u  %6u bytes\n"



typedef struct
{
    uint8_t pattern[4];
    uint8_t offset;
    // uint8_t size;
    uint8_t type;      /* 0 = c2s, 1 = s2c */
} pattern;


typedef struct __udp_pair
{
    uint32_t dstip;
    uint16_t dstport;
    uint32_t srcip;
    uint16_t srcport;
    uint32_t bytes;
    time_t ttl;
    time_t ctime;
    time_t mtime;
    struct __udp_pair * next;
} udp_pair;

uint8_t * qqservers[] =
{
    "qq.com",
    "tencent.com",
};



int __debug__ = 0;
int __verbose__ = 0; /* for hexdump */
udp_pair * __wzry_udp_pairs__ = NULL;


int dns_parser(uint8_t * data, size_t len)
{
    // HEXDUMP(data, len, data);
    dnshdr * dns = (dnshdr *)data;
    if (dns->qr == 0 && dns->opcode == 0)
    {
        uint8_t * p = NULL;
        uint8_t cname[128] = {0};
        int i;

        p = data+sizeof(dnshdr);
        // HEXDUMP(p, len-sizeof(dnshdr), NULL);
        while(1)
        {
            strncat(cname, p+1, *p);
            p += (*p + 1);
            if (*p != 0)
                strncat(cname, ".", 1);
            else
                break;
        }
        BLUE("dns query %s\n", cname);

        /* check if current dns is from qq */
        for (i = 0; i<sizeof(qqservers)/sizeof(uint8_t*); i++)
        {
            // if (0 == strncasecmp(qqservers[i], cname, strlen(qqservers[i])))
            if (strstr(cname, qqservers[i]))
            {
                RED("# dns query: %s\n", cname);
                break;
            }
        }
    }
    if (dns->qr == 1)
    {
        // HEXDUMP(data+sizeof(dnshdr), len-sizeof(dnshdr), NULL);
        // YELLOW("dns answer %u\n", ntohs(dns->an_num));
    }
}



int wzry_udp(uint8_t * data, int len)
{
    const static pattern wzry[2][2] =
    {
        {{{0,1,0,0}, 0, 1}, {{0,0,0,0}, 6, 1}}, ////{0,0,0,0,1,0,0x33,0x66,0,9,0,9}
        {{{1,2,0,0}, 0, 0}, {{0xde,0xf0,0,1}, 6, 0}},
    };

    int i, j, match;

    for (j = 0; j < 2; ++j)
    {
        for (i = 0, match=0; i < 2; ++i)
        {
            if (len < (wzry[j][i].offset + 4)) break;
            if (0 == memcmp(data+wzry[j][i].offset, wzry[j][i].pattern, 4))
            {
                match++;
            }
        }
        if (match == 2)
        {
            RED("pattern %d match\n", j);
            HEXDUMP(data, len, data);
            return 1;
        }
    }

    return 0;
}


uint8_t * memsearch(uint8_t * mem, size_t len, uint8_t * mem2, size_t len2)
{
    uint8_t * p = mem;

    if(!mem2 || 0 == len2) return mem;

    do
    {
        if (0 == memcmp(p, mem2, len2))
            return p;
        p++;
    }
    while (len - (p - mem) >= len2);

    return NULL;
}

#if 0
uint8_t * memsearch(uint8_t * mem, size_t len, uint8_t * mem2, size_t len2)
{
    uint8_t c, sc;

    if(!mem2 || 0 == len2) return mem;
    if(len2 > len) return NULL;

    c = *mem2++;
    do
    {
        do
        {
            sc = *mem++;
            len--;
            //if (!sc) return NULL;
            if(len2 > len || len <= 0) return NULL;
        }
        while (sc != c);
        if(len2 > len) return NULL;
    }
    while (memcmp(mem, mem2, len2-1) != 0);

    return (uint8_t *) (mem - 1);
}
#endif

udp_pair * known_wzry_udp_pair(uint32_t srcip, uint16_t srcport, uint32_t dstip, uint16_t dstport)
{
    udp_pair * pair = __wzry_udp_pairs__;
    while (pair)
    {
        if ((pair->dstip == dstip && pair->srcip == srcip) ||
            (pair->dstip == srcip && pair->srcip == dstip))
        {
            return pair;
        }
        if (time(NULL) > pair->ttl)
        {
            /* expired */
            // TODO
        }
        pair = pair->next;
    }

    return NULL;
}

void update_wzry_udp_pair(uint32_t srcip, uint16_t srcport, uint32_t dstip, uint16_t dstport, size_t bytes)
{
    udp_pair * pair = known_wzry_udp_pair(srcip, srcport, dstip, dstport);
    if (pair)
    {
        pair->ttl = time(NULL) + 600; // 10 minutes
        pair->bytes += bytes;
        pair->mtime = time(NULL); // 10 minutes
        return;
    }
    else
    {
        pair = calloc(1, sizeof(udp_pair));
        pair->srcip = srcip;
        pair->dstip = dstip;
        pair->srcport = srcport;
        pair->dstport = dstport;
        pair->bytes = bytes;
        pair->ctime = time(NULL); // 10 minutes
        pair->ttl = pair->ctime + 600; // 10 minutes
        pair->mtime = pair->ctime; // 10 minutes
        pair->next = __wzry_udp_pairs__;
        __wzry_udp_pairs__ = pair;
    }

    return;
}


void dump_wzry_udp_pair()
{
    udp_pair * pair = __wzry_udp_pairs__;
    int counter = 0;
    char * srcip = NULL;
    char * dstip = NULL;
    printf(ANSI_COLOR_RESET"\n========= Captured WZRY Data ============\n");
    while (pair)
    {
        srcip = strdup(i2ip(ntohl(pair->srcip)));
        dstip = strdup(i2ip(ntohl(pair->dstip)));
        printf("<%02d> %16s : %-5u --> %-16s:%-5u  %6u bytes, %4lu s, rate %3lu KB/s\n",
               counter, srcip, pair->srcport, dstip, pair->dstport,
               pair->bytes, pair->mtime - pair->ctime, pair->bytes/1024/(pair->mtime - pair->ctime));
        pair = pair->next;
        counter++;
        free(srcip);
        free(dstip);
    }
    printf(ANSI_COLOR_RESET"=========================================\n");
}


void ctrl_c_handler(int dummy)
{
    dump_wzry_udp_pair();
    exit(0);
}


void ctrl_backslash_handler(int dummy)
{
    dump_wzry_udp_pair();
}


int main(int argc, char *argv[])
{
    int rawsock;
    int i;
    uint8_t buffer[1024];
    struct sockaddr_in destaddr;
    socklen_t len = sizeof(destaddr);
    iphdr * ip = NULL;
    struct ifreq ifopts;
    uint32_t counter = 0;
    char * ifname = NULL;
    int32_t proto = IPPROTO_RAW;
    struct ifreq ifr;
    struct sockaddr_ll sa;
    char * srcip;
    char * dstip;

    while((i = getopt(argc, argv, "d:i:p:v")) != -1)
    {
        switch (i)
        {
            case 'v':
                __verbose__  = 1;
                break;
            case 'd':
                __debug__ = atoi(optarg);
                break;
            case 'i':
                ifname = optarg;
                break;
            case 'p':
                if (0 == strncasecmp(optarg, "udp", 3))
                    proto = IPPROTO_UDP;
                else if (0 == strncasecmp(optarg, "tcp", 3))
                    proto = IPPROTO_TCP;
                else if (0 == strncasecmp(optarg, "icmp", 4))
                    proto = IPPROTO_ICMP;
                else
                {
                    fprintf(stderr, "%s\n", optarg);
                    exit(-1);
                }
                break;
            case '?':
            default:
                exit(-1);
        }
    }

    signal(SIGINT, ctrl_c_handler);
    signal(SIGQUIT, ctrl_backslash_handler);

    /* ETH_P_IP captures incoming packets, without ethhdr.
       ETH_P_ALL capture both incoming and outgoing packets, with ethhdr.
    */
    rawsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    ASSERT(rawsock > 0);

    /* bind socket to interface */
    if (ifname)
    {
        strcpy(ifr.ifr_name, ifname);
        i = ioctl(rawsock, SIOCGIFINDEX, &ifr);
        if (i)
        {
            perror("get ifindex error ");
            return errno;
        }

        sa.sll_family = PF_PACKET;
        sa.sll_protocol = htons(ETH_P_ALL);
        sa.sll_ifindex = ifr.ifr_ifindex;
        i = bind(rawsock, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll));
        if (i != 0)
        {
            perror("bind error ");
            return errno;
        }
    }

    /* set interface to promiscous mode */
    ioctl(rawsock, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(rawsock, SIOCSIFFLAGS, &ifopts);

    /* capturing */
    while (1)
    {
        counter++;

        i = recvfrom(rawsock, buffer, sizeof(buffer), 0, (struct sockaddr *)&destaddr, &len);
        // ethhdr * eth = (ethhdr *)buffer;
        ip = (iphdr *)(buffer + sizeof(ethhdr));
        srcip = strdup(i2ip(ntohl(ip->srcip)));
        dstip = strdup(i2ip(ntohl(ip->dstip)));

        print_iphdr(ip);

        if (ip->proto == IPPROTO_TCP && (proto == IPPROTO_TCP || proto == IPPROTO_RAW))
        {
            tcphdr * tcp = (tcphdr*)(buffer + sizeof(ethhdr) + ip->headlen*4);

            if (22 == ntohs(tcp->dst) || 22 == ntohs(tcp->src))
            {
                BLUE(SOCK_PAIR, counter, "SSH", srcip, ntohs(tcp->src),
                     dstip, ntohs(tcp->dst), ntohs(ip->totlen) - ip->headlen*4 - tcp->headlen*4);
            }
            else if (80 == ntohs(tcp->dst) || 80 == ntohs(tcp->src))
            {
                GREEN(SOCK_PAIR, counter, "HTTP", srcip, ntohs(tcp->src),
                      dstip, ntohs(tcp->dst), ntohs(ip->totlen) - ip->headlen*4 - tcp->headlen*4);
            }
            else if (445 == ntohs(tcp->dst) || 445 == ntohs(tcp->src))
            {
                BLUE(SOCK_PAIR, counter, "SMB", srcip, ntohs(tcp->src),
                     dstip, ntohs(tcp->dst), ntohs(ip->totlen) - ip->headlen*4 - tcp->headlen*4);
            }
            else
            {
                GREEN(SOCK_PAIR, counter, "TCP", srcip, ntohs(tcp->src),
                      dstip, ntohs(tcp->dst), ntohs(ip->totlen) - ip->headlen*4 - tcp->headlen*4);
            }
            // MAGE("\t\tflags: %s %s %s %s %s %s\n", tcp->f_syn?"SYN":"", tcp->f_ack?"ACK":"",
            //     tcp->f_psh?"PSH":"", tcp->f_urg?"URG":"", tcp->f_rst?"RST":"", tcp->f_fin?"FIN":"");
        }
        else if (ip->proto == IPPROTO_UDP && (proto == IPPROTO_UDP || proto == IPPROTO_RAW))
        {
            udphdr * udp = (udphdr*)(buffer + sizeof(ethhdr) + ip->headlen*4);
            print_udphdr(udp);
            if (53 == ntohs(udp->dst) || 53 == ntohs(udp->src))  /* DNS */
            {
                GREEN(SOCK_PAIR, counter, "DNS", srcip, ntohs(udp->src),
                      dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
                // HEXDUMP(buffer + sizeof(ethhdr) + ip->headlen*4 + sizeof(udphdr), ntohs(udp->len) - sizeof(udphdr), NULL);
                dns_parser(buffer + sizeof(ethhdr) + ip->headlen*4 + sizeof(udphdr),
                           ntohs(udp->len) - sizeof(udphdr));
            }
            else if (1900 == ntohs(udp->dst) || 1900 == ntohs(udp->src)) /* SSDP */
            {
                BLUE(SOCK_PAIR, counter, "SSDP", srcip, ntohs(udp->src),
                     dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
            else if (8235 == ntohs(udp->dst) || 8235 == ntohs(udp->src)) /* IP Guard */
            {
                BLUE(SOCK_PAIR, counter, "IPGD", srcip, ntohs(udp->src),
                     dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
            else if ((137 == ntohs(udp->dst) || 137 == ntohs(udp->src)) || /* NetBIOS NameService */
                     (138 == ntohs(udp->dst) || 138 == ntohs(udp->src)) || /* NetBIOS Datagram */
                     (139 == ntohs(udp->dst) || 139 == ntohs(udp->src)))   /* NetBIOS Session */
            {
                BLUE(SOCK_PAIR, counter, "NTBS", srcip, ntohs(udp->src),
                     dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
            else if ((67 == ntohs(udp->dst) || 67 == ntohs(udp->src)) || /* DHCP Client */
                     (68 == ntohs(udp->dst) || 68 == ntohs(udp->src)))   /* DHCP Server */
            {
                BLUE(SOCK_PAIR, counter, "DHCP", srcip, ntohs(udp->src),
                     dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
            else if (known_wzry_udp_pair(ip->srcip, udp->src, ip->dstip, udp->dst) ||
                     wzry_udp(buffer + sizeof(ethhdr) + ip->headlen*4 + sizeof(udphdr), ntohs(udp->len) - sizeof(udphdr)))
            {
                /* record ip and port */
                update_wzry_udp_pair(ip->srcip, udp->src, ip->dstip, udp->dst, ntohs(udp->len) - sizeof(udphdr));
                RED(SOCK_PAIR, counter, "UDP", srcip, ntohs(udp->src),
                    dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
            else
            {
                GREEN(SOCK_PAIR, counter, "UDP", srcip, ntohs(udp->src),
                      dstip, ntohs(udp->dst), ntohs(udp->len) - sizeof(udphdr));
            }
        }
        else if (ip->proto == IPPROTO_ICMP && (proto == IPPROTO_ICMP || proto == IPPROTO_RAW))
        {
            GREEN("[%08u] ICMP %16s         --> %-16s        %6u bytes\n", counter, srcip,
                  dstip, ntohs(ip->totlen));
        }

        HEXDUMP(buffer, sizeof(buffer), buffer);
        free(srcip);
        free(dstip);
    }

    return 0;
}
