/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
*/

#ifndef __STCAM_H__
#define __STCAM_H__

#include <event.h>
#include "udpsock.h"
#include "common.h"
#include "b64.h"
#include <cv.h> // Include the OpenCV library
#include <highgui.h> // Include interfaces for video capturing

#define SERVERMODE  0
#define CLIENTMODE  1
#define WAITDELAY   5

struct stcam_t{
  int nread;
  size_t elen;
  struct event* ev;
	struct event_base* ev_base;
  struct sockaddr_in srvaddr;
  udpsock_t udpsock;
  IplImage* frame;
  unsigned char b64_jpg[65535];
  char data[2048 + 921600];
  IplImage* DecodedImage;
  CvMat cvmat;
  CvMat *mat;
};
typedef struct stcam_t stcam_t;

void
stcam_add_server(stcam_t* stcam, const char * host, uint16_t port);

void
stcam_init(stcam_t* stcam, char *host, uint16_t port, int mode);

void
stcam_free(stcam_t* stcam);

static void
stcam_read_cb(evutil_socket_t evfd, short evwhat, void * arg);

void
stcam_srv_start(stcam_t* stcam);

void
stcam_parse_payload(stcam_t* stcam);

struct sockaddr_in*
stcam_get_srv(stcam_t* stcam);

void
stcam_capture(stcam_t* stcam, int cam);

#endif
