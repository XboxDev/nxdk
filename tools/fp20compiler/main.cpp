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

    FILE* fh = fopen(argv[1], "rb");
    if (!fh) {
        fprintf(stderr, "unable to open %s\n", argv[1]);
        exit(1);
    }

    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    rewind(fh);
    char* buffer = (char*)malloc(size+1);
    if (buffer == NULL) {
        fprintf(stderr, "failed to allocate buffer\n");
        exit(1);
    }

    fread(buffer, size, 1, fh);
    buffer[size] = '\0';

    translate(buffer);

    fclose(fh);
    free(buffer);

    return 0;
}
