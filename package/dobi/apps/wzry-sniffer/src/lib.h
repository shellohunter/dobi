/****************************************************************************
 *
 * Copyright (c) Hua Shao <nossiac@163.com>
 *
 ****************************************************************************/
#ifndef __LIB_H__
#define __LIB_H__



enum
{
    DBG_LVL_OFF = 0,
    DBG_LVL_RED,
    DBG_LVL_GREEN,
    DBG_LVL_YELLOW,
    DBG_LVL_MAGENTA,
    DBG_LVL_CYAN,
    DBG_LVL_BLUE,
    DBG_LVL_DBG,
    DBG_LVL_ALL,
};


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern int __debug__;

#define DBG(...) if(__debug__ >= DBG_LVL_DBG) do { \
        fprintf(stderr, "<dbg> "__VA_ARGS__); \
        fprintf(stderr," %s() L%d.\n", __FUNCTION__, __LINE__); \
    } while(0)

#define ASSERT(cond) do { \
        if(!(cond)) { \
            fprintf(stderr, "<assert> [%s] FAIL, %s L%d\n", #cond, __FUNCTION__, __LINE__); \
            exit(-1); \
        } \
    } while(0)

#define RED(...) if (__debug__ >= DBG_LVL_RED) do { \
        fprintf(stderr, ANSI_COLOR_RED __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)

#define YELLOW(...) if (__debug__ >= DBG_LVL_YELLOW) do { \
        fprintf(stderr, ANSI_COLOR_YELLOW __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)

#define GREEN(...) if (__debug__ >= DBG_LVL_GREEN) do { \
        fprintf(stderr, ANSI_COLOR_GREEN __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)

#define MAGE(...) if (__debug__ >= DBG_LVL_MAGENTA) do { \
        fprintf(stderr, ANSI_COLOR_CYAN __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)

#define CYAN(...) if (__debug__ >= DBG_LVL_CYAN) do { \
        fprintf(stderr, ANSI_COLOR_CYAN __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)

#define BLUE(...) if (__debug__ >= DBG_LVL_BLUE) do { \
        fprintf(stderr, ANSI_COLOR_BLUE __VA_ARGS__); \
        fprintf(stderr, ANSI_COLOR_RESET); \
    } while(0)


#define HEXDUMP(...) if (__verbose__) do { \
        hexdump(__VA_ARGS__); \
    } while(0)




typedef struct
{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t headlen:4;
    uint8_t ver:4;
    #elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t ver:4;
    uint8_t headlen:4;
    #endif
    uint8_t tos;
    uint16_t totlen;
    uint16_t seq;
    uint16_t segoff;
    uint8_t ttl;
    uint8_t proto;
    uint16_t chksum;
    unsigned int srcip;
    unsigned int dstip;
} __attribute__ ((__packed__)) iphdr;


typedef struct
{
    uint16_t src;
    uint16_t dst;
    uint16_t len;
    uint16_t chksum;
} __attribute__ ((__packed__)) udphdr;


typedef struct
{
    uint16_t src;
    uint16_t dst;
    uint32_t seq;
    uint32_t ack;
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t res1:4;
    uint16_t headlen:4;
    uint16_t f_fin:1;
    uint16_t f_syn:1;
    uint16_t f_rst:1;
    uint16_t f_psh:1;
    uint16_t f_ack:1;
    uint16_t f_urg:1;
    uint16_t res2:2;
    #elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t headlen:4;
    uint16_t res1:4;
    uint16_t res2:2;
    uint16_t f_urg:1;
    uint16_t f_ack:1;
    uint16_t f_psh:1;
    uint16_t f_rst:1;
    uint16_t f_syn:1;
    uint16_t f_fin:1;
    #endif
    uint16_t window;  
    uint16_t chksum;
    uint16_t urgptr;
} __attribute__ ((__packed__)) tcphdr;


typedef struct
{
    uint8_t src[6];
    uint8_t dst[6];
    uint16_t type;
} __attribute__ ((__packed__)) ethhdr;

/*
    +---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+
*/

typedef struct
{
    uint16_t id; // identification number

    #if __BYTE_ORDER == __BIG_ENDIAN
    uint8_t qr:1;      // Query or Response
    uint8_t opcode:4;  // Query Type
    uint8_t aa:1;      // Authoritative Answer
    uint8_t tc:1;      // TrunCation
    uint8_t rd:1;      // Recursion Desired

    uint8_t ra:1;      // Recursion Available
    uint8_t z:3;       // Reserved
    uint8_t rcode:4;   // Response Code
    #elif __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t rd:1;      // Recursion Desired
    uint8_t tc:1;      // TrunCation
    uint8_t aa:1;      // Authoritative Answer
    uint8_t opcode:4;  // Query Type
    uint8_t qr:1;      // Query or Response

    uint8_t rcode:4;   // Response Code
    uint8_t z:3;       // Reserved
    uint8_t ra:1;      // Recursion Available
   #endif
    uint16_t qd_num; // number of question entries
    uint16_t an_num; // number of answer entries
    uint16_t ns_num; // number of authority entries
    uint16_t ar_num; // number of resource entries

} __attribute__ ((__packed__)) dnshdr;


extern void hexdump(uint8_t * p, size_t s, uint8_t * base);
extern void print_iphdr(iphdr * ip);
extern void print_tcphdr(tcphdr * header);
extern void print_udphdr(udphdr * udp);
extern char * i2ip(uint32_t ip);
extern uint32_t ip2i(char * ip);

#endif /* __LIB_H__ */
