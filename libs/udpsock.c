/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
*/

#include "udpsock.h"

static int
_bind_dgramsocket(int fd, const char* host, uint16_t port)
{
  struct sockaddr_in localaddr;

  // set local address
  localaddr.sin_family = AF_INET;
  localaddr.sin_addr.s_addr = inet_addr(host);
  localaddr.sin_port = htons(port);

  return bind(fd, (struct sockaddr *) &localaddr, sizeof(struct sockaddr));
}

static int
_make_dgramsocket(int sockbufsize)
{
  int fd=-1;
  int set=1;
  int flags;

  // Create endpoint
  if ((fd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    return -1;
  }
  // Set socket option
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int)) < 0) {
	  goto fail_return;
  }
  // set receive buffer
  if (sockbufsize > 0){
    if( setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &sockbufsize, sizeof(int)) < 0 ) {
	    goto fail_return;
    }
    // set send buffer
    if( setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &sockbufsize, sizeof(int)) < 0 ) {
	    goto fail_return;
    }
  }

  // set non blocking
  if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
	  goto fail_return;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
	  goto fail_return;
  }

  return fd;

fail_return:
  if (fd != -1) close(fd);
  return -1;
}


int
udpsock_init(udpsock_t* udpsock, char *host, uint16_t port, int flags, int bcast)
{
	if (udpsock == NULL) return -1;
	memset(udpsock, 0, sizeof(udpsock_t));

	udpsock->sock = _make_dgramsocket(0);
	_bind_dgramsocket(udpsock->sock, host, port);

	if (udpsock->sock == -1) return -1;
	return 0;
}


int
udpsock_free(udpsock_t* udpsock)
{
	udpsock->lastError = UDPSOCK_ERR_CLOSE;
	if (udpsock == NULL) return 0;
	if (udpsock->sock != -1) close(udpsock->sock);
	memset(udpsock, 0, sizeof(udpsock_t));
	return 0;
}

ssize_t
udpsock_fetch(udpsock_t* udpsock, void *packet, int psize)
{
  if (udpsock == NULL) return -1;
	ssize_t recvLen;
	struct sockaddr_in srcAddr;
	socklen_t sockAddrLen = sizeof(srcAddr);

	udpsock->lastError = UDPSOCK_ERR_RECVFROM; //if fail
	recvLen = recvfrom(udpsock->sock, packet, psize, MSG_DONTWAIT, (struct sockaddr *) &srcAddr, &sockAddrLen);

	//sementara
	udpsock->srcAddr = srcAddr;
	udpsock->sockAddrLen = sockAddrLen;

	return recvLen;
}

ssize_t
udpsock_fetch_block(udpsock_t* udpsock, void *packet, int psize)
{
  if (udpsock == NULL) return -1;
	ssize_t recvLen;
	struct sockaddr_in srcAddr;
	socklen_t sockAddrLen = sizeof(srcAddr);

	udpsock->lastError = UDPSOCK_ERR_RECVFROM; //if fail
	recvLen = recvfrom(udpsock->sock, packet, psize, MSG_WAITALL, (struct sockaddr *) &srcAddr, &sockAddrLen);

	//sementara
	udpsock->srcAddr = srcAddr;
	udpsock->sockAddrLen = sockAddrLen;

	return recvLen;
}

int
udpsock_send(udpsock_t* udpsock, void *packet, int psize)
{
  if (udpsock == NULL) return -1;
	udpsock->lastError = UDPSOCK_ERR_SEND;
	return (sendto(udpsock->sock, packet, psize, 0, &udpsock->sockAddr, udpsock->sockAddrLen) == psize);
}

int
udpsock_sendto(udpsock_t* udpsock, struct sockaddr_in* addr, void *packet, int psize)
{
	if (udpsock == NULL || addr == NULL) return -1;
	return (sendto(udpsock->sock, packet, psize, 0, (struct sockaddr*)addr, sizeof(struct sockaddr)) == psize);
}

int
udpsock_reply(udpsock_t* udpsock, void *packet, int psize)
{
	udpsock->lastError = UDPSOCK_ERR_SEND;
	return (sendto(udpsock->sock, packet, psize, 0, (struct sockaddr *)&udpsock->srcAddr, udpsock->sockAddrLen) == psize);
}

int
udpsock_getfd(udpsock_t* udpsock)
{
	return udpsock->sock;
}
