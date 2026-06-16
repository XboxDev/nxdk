#include <hal/video.h>

BOOL verify_c_file_builds() {
  return XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
}
