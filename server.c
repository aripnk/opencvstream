#include "libs/stcam.h"

int main(int argc, char*argv[])
{
  stcam_t stcam;
  stcam_init(&stcam, "0.0.0.0", 8088, SERVERMODE);
  stcam_srv_start(&stcam);
  stcam_free(&stcam);
  return 0;
}
