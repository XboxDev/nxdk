#include <vector>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>

#include "nvparse_errors.h"



// RC1.0  -- register combiners 1.0 configuration
bool rc10_init(char *);
int  rc10_parse();
bool is_rc10(const char *);

// TS1.0  -- texture shader 1.0 configuration
bool ts10_init(char *);
int  ts10_parse();
bool is_ts10(const char *);

// DX8 stuff

// PS1.0  -- DX8 Pixel Shader 1.0 configuration
bool ps10_init(char *);
int  ps10_parse();
bool ps10_set_map(const std::vector<int>& argv);
bool is_ps10(const char *);
const int* ps10_get_info(int* pcount);



nvparse_errors errors;
int line_number;
char * myin = 0;

void translate(const char* s) {
    char* ns = strdup(s);

    // printf("-- %s\b", s);

    char* ts_start = strstr(ns, "!!TS1.0");
    if (ts_start) {
        char* ts_end = strstr(ts_start, "// End of program");
        assert(ts_end);
        *ts_end = 0;

        ts10_init(ts_start);
        ts10_parse();

        *ts_end = '/';
    }

    char* rc_start = strstr(ns, "!!RC1.0");
    if (rc_start) {
        rc10_init(rc_start);
        rc10_parse();
    }

    free(ns);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s fpfile\n", argv[0]);
        exit(1);
    }

    FILE* fh = fopen(argv[1], "r");
    if (!fh) {
        fprintf(stderr, "unable to open %s\n", argv[1]);
        exit(1);
    }

    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    rewind(fh);
    char* buffer = (char*)malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "failed to allocate buffer\n");
        exit(1);
    }

    fread(buffer, size, 1, fh);

    translate(buffer);

    fclose(fh);
    free(buffer);

//     translate("!!TS1.0\n"
// "texture_2d();\n"
// "texture_2d();\n");

//     translate("!!RC1.0\n"
// "{\n"
// "  rgb\n"
// "  {\n"
// "    discard = tex0.rgb * col0.rgb;\n"
// "    discard = tex1.rgb * col0.rgb;\n"
// "    col0 = sum();\n"
// "  }\n"
// "  alpha\n"
// "  {\n"
// "    discard = tex0.a * col0.a;\n"
// "    discard = tex1.a * col0.a;\n"
// "    col0 = sum();\n"
// "  }\n"
// "}\n"
// "out.rgb = unsigned(col0.rgb);\n"
// "out.a = unsigned(col0.a);\n");

//     translate("!!RC1.0\n"
// "const0 = ( 1.000000, 0.000000, 0.000000, 0.000000 );\n"
// "out.rgb = unsigned(const0.rgb);\n"
// "out.a = unsigned_invert(zero.a);\n");



// translate("!!TS1.0\n"
// "texture_2d();");

// translate("!!RC1.0\n"
// "{\n"
// "  rgb\n"
// "  {\n"
// "    col0 = tex0.rgb * col0.rgb;\n"
// "  }\n"
// "  alpha\n"
// "  {\n"
// "    col0 = tex0.a * col0.a;\n"
// "  }\n"
// "}\n"
// "out.rgb = unsigned(col0.rgb);\n"
// "out.a = unsigned(col0.a);\n");

    return 0;
}
