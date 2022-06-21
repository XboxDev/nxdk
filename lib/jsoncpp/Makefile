JSONCPP_DIR = $(NXDK_DIR)/lib/jsoncpp/jsoncpp
JSONCPP_SRCS = $(JSONCPP_DIR)/src/lib_json/json_reader.cpp \
            $(JSONCPP_DIR)/src/lib_json/json_value.cpp \
            $(JSONCPP_DIR)/src/lib_json/json_writer.cpp

JSONCPP_OBJS = $(addsuffix .obj, $(basename $(JSONCPP_SRCS)))

NXDK_CFLAGS += -I $(JSONCPP_DIR)/src/lib_json -I $(JSONCPP_DIR)/include -DJSON_USE_EXCEPTION=0
NXDK_CXXFLAGS += -I $(JSONCPP_DIR)/src/lib_json -I $(JSONCPP_DIR)/include -DJSON_USE_EXCEPTION=0

NXDK_CXX = y

$(NXDK_DIR)/lib/libjsoncpp.lib: $(JSONCPP_OBJS)

main.exe: $(NXDK_DIR)/lib/libjsoncpp.lib

CLEANRULES += clean-jsoncpp

.PHONY: clean-jsoncpp
clean-jsoncpp:
	$(VE)rm -f $(JSONCPP_OBJS) $(NXDK_DIR)/lib/libjsoncpp.lib
