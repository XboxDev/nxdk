#include <hal/debug.h>
#include <hal/video.h>
#include <string>
#include <vector>
#include <windows.h>

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  std::vector<std::string> words;
  words.emplace_back("Hello");
  words.emplace_back(" ");
  words.emplace_back("nxdk!");
  words.emplace_back("\n");

  while (true) {
    for (auto& word : words) {
      debugPrint("%s", word.c_str());
    }
    Sleep(2000);
  }

  return 0;
}
