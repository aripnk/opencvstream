/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
*/
#ifndef __UDPSOCK_H__
#define __UDPSOCK_H__

#include "common.h"

// list of error
#define UDPSOCK_ERR_GETADDRINFO 	1
#define UDPSOCK_ERR_CREATESOCKET	2
#define UDPSOCK_ERR_SOCKBIND			3
#define UDPSOCK_ERR_RECVFROM	 		4
#define UDPSOCK_ERR_GETNAMEINFO 	5
#define UDPSOCK_ERR_RECVSIZE			6
#define UDPSOCK_ERR_READ					11
#define UDPSOCK_ERR_WRITE	 				12
#define UDPSOCK_ERR_SEND	 				13
#define UDPSOCK_ERR_CLOSE					14

#ifdef __cplusplus
extern "C" {
#endif

struct udpsock_t{
	int sock;
	int lastError;
	struct sockaddr_in srcAddr;
	struct sockaddr sockAddr;
	struct sockaddr *srcAddress;
	socklen_t sockAddrLen;
};
typedef struct udpsock_t udpsock_t;


int
udpsock_init(	udpsock_t* udpsock,
							char *host, uint16_t port,
							int flags,
							int bcast);

int
udpsock_free(udpsock_t* udpsock);

ssize_t
udpsock_fetch(udpsock_t* udpsock,
						 	void *packet,
							int psize);

ssize_t
udpsock_fetch_block(udpsock_t* udpsock,
									 	void *packet,
										int psize);

int
udpsock_send( udpsock_t* udpsock,
							void *packet,
							int psize);

int
udpsock_sendto(	udpsock_t* udpsock,
								struct sockaddr_in* addr,
								void *packet, int psize);

int
udpsock_reply(udpsock_t* udpsock,
							void *packet,
							int psize);

int
udpsock_getfd(udpsock_t* udpsock);


#ifdef __cplusplus
}
#endif

#endif /* __RAFT_UDPSOCK_H__ */
