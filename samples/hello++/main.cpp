#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>

class NumberClass{
private:
  int hiddenValue = 0;
public:
  NumberClass() {hiddenValue = 5;};
  NumberClass(int v) : hiddenValue(v) {};
  int getValue() { return hiddenValue; };
};

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  NumberClass defaultFive;
  NumberClass customSix(6);

  while (true) {
    debugPrint("Value of defaultFive: %i\n", defaultFive.getValue());
    debugPrint("Value of customSix  : %i\n", customSix.getValue());
    Sleep(2000);
  }

  return 0;
}
