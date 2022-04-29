#include <string>
#include <vector>

#include <hal/video.h>
#include <nxdk/log_console.h>
#include <windows.h>

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  nxLogConsoleRegister();

  std::vector<std::string> words;
  words.emplace_back("Hello");
  words.emplace_back(" ");
  words.emplace_back("nxdk!");
  words.emplace_back("\n");

  while (true)
  {
    for (auto& word : words) {
      nxLogPrint(word.c_str());
    }

    Sleep(2000);
  }

  return 0;
}
