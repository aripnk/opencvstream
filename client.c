#include "libs/stcam.h"

int main(int argc, char*argv[])
{
  if (argc != 3){
    printf("%s\n", "usage : ./client <serverhost> <cameraindex>");
    return -1;
  }
  stcam_t stcam;
  stcam_init(&stcam, "0.0.0.0", 8089, CLIENTMODE);
  stcam_add_server(&stcam, argv[1], 8088);
  stcam_capture(&stcam, atoi(argv[2]));
  stcam_free(&stcam);
  return 0;
}
