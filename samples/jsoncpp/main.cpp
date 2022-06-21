#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include <windows.h>

#include <hal/debug.h>
#include <hal/video.h>

void LoadJson(std::string filename, Json::Value *data) {
    std::ifstream configFile(filename);
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    parseFromStream(builder, configFile, data, &errs);
    configFile.close();
}


int main(int argc, char *argv[]) {
	XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

	Json::Value data;
	LoadJson("D:\\message.json", &data);
    while(true) {
        debugPrint(data.get("message", "Could not load json message!\n").asString().c_str());
        Sleep(2000);
    }
	return 0;
}
