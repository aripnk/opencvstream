/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
*/

#include "stcam.h"

void
stcam_add_server(stcam_t* stcam, const char * host, uint16_t port)
{
  if (stcam == NULL) return; /*always check*/
  struct sockaddr_in* addr = &(stcam->srvaddr);
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(host);
  addr->sin_port = htons(port);
}

void
stcam_init(stcam_t* stcam, char *host, uint16_t port, int mode)
{
  if (stcam == NULL ) return;
  memset(stcam, 0, sizeof(stcam_t));
  udpsock_init(&(stcam->udpsock), host, port, 1, 0);
  if (mode == SERVERMODE) {
    stcam->ev_base = event_base_new();
    CvSize size = cvSize(1080, 720);
    int depth  = IPL_DEPTH_32F;
    int channels = 3;
    stcam->frame =  cvCreateImageHeader(size, depth, channels);
    stcam->DecodedImage = cvCreateImage(cvSize(1080, 720), IPL_DEPTH_8U, 3);
    cvNamedWindow("Window", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Window", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
  }
}

void
stcam_free(stcam_t* stcam)
{
  if (stcam == NULL) return; /*always check*/
  memset(stcam, 0, sizeof(stcam_t));
}

static void
stcam_read_cb(evutil_socket_t evfd, short evwhat, void * arg)
{
  stcam_t* stcam = (stcam_t*)arg;
  if (stcam == NULL) return; /*always check*/
  if (evwhat&EV_READ) {
    stcam_parse_payload(stcam);
  }
  return;
}

void
stcam_srv_start(stcam_t* stcam)
{
  if (stcam == NULL) return; /*always check*/
  uint32_t fd = udpsock_getfd(&(stcam->udpsock));
  if (fd == -1) return;
  stcam->ev = event_new(stcam->ev_base,
                           fd,
                           EV_READ | EV_PERSIST | EV_TIMEOUT,
                           stcam_read_cb,
                           (void*)stcam);

  event_add(stcam->ev, NULL);
  event_base_dispatch(stcam->ev_base);
  return;
}

void
stcam_parse_payload(stcam_t* stcam)
{
  if (stcam == NULL) return; /*always check*/
  memset(stcam->data, 0, sizeof(stcam->data));
  stcam->nread = udpsock_fetch(&stcam->udpsock, stcam->data, sizeof(stcam->data));
  char * data = base64_decode(stcam->data, stcam->nread, &stcam->elen);
  stcam->cvmat = cvMat(720, 1080, CV_8UC3, (void*)data);
  stcam->DecodedImage = cvDecodeImage(&stcam->cvmat, 1);
  cvShowImage("Window",stcam->DecodedImage);
  cvWaitKey(WAITDELAY);
  cvReleaseImage(&stcam->DecodedImage);
  free(data);
}

struct sockaddr_in*
stcam_get_srv(stcam_t* stcam)
{
  if (stcam == NULL) return NULL; /*always check*/
  return &(stcam->srvaddr);
}

void
stcam_capture(stcam_t* stcam, int cam)
{
  if (stcam == NULL) return; /*always check*/
  CvCapture* capture =cvCreateCameraCapture(cam);
  // cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1080 );
  // cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 720 );
  if (!capture){
    printf("Error. Cannot capture.");
  }
  else{
    struct sockaddr_in* addr;
    const static int encodeParams[] = { CV_IMWRITE_JPEG_QUALITY, 80 };
    addr = stcam_get_srv(stcam);
    while (1){
      stcam->frame = cvQueryFrame(capture);
      stcam->mat = cvEncodeImage(".jpg", stcam->frame, encodeParams);
      memset(stcam->b64_jpg, 0, sizeof(stcam->b64_jpg));
      b64_encode(stcam->b64_jpg, stcam->mat->data.ptr, stcam->mat->step);

      udpsock_sendto(	&stcam->udpsock,
      								addr,
      								stcam->b64_jpg, strlen((const char*)stcam->b64_jpg));

      cvWaitKey(WAITDELAY);
      cvReleaseMat(&stcam->mat);
    }
    cvReleaseCapture(&capture);
  }
}
