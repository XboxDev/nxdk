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

static char* find_at_line_start(char* haystack, char* cursor, const char* needle) {
    cursor = strstr(cursor, needle);
    while (cursor != NULL) {

        // Accept if candidate is at beginning of file or line
        if ((cursor == haystack) || (cursor[-1] == '\n')) {
            return cursor;
        }

        cursor = strstr(cursor + 1, needle);
    }
    return NULL;
}

void translate(const char* s) {
    char* ns = strdup(s);

    // Look for the first shader magic
    char* shader_start = find_at_line_start(ns, ns, "!!");

    // Warn the user if we couldn't find any shader at all
    if (shader_start == NULL) {
        fprintf(stderr, "no shaders found\n");
    }

    // Loop until we can't find a shader anymore
    while (shader_start != NULL) {

        // Find end of shader magic line
        char* line_end = strchr(shader_start, '\n');

        // The next shader magic marks the end of the current shader; find it
        char* next_shader_start;
        if (line_end == NULL) {
            next_shader_start = NULL;
        } else {
            next_shader_start = find_at_line_start(ns, line_end + 1, "!!");
        }

        // If another shader exists, modify it to end current shader string
        if (next_shader_start) { *next_shader_start = '\0'; }

        // Process shader
        if (is_ts10(shader_start)) {
            ts10_init(shader_start);
            ts10_parse();
        } else if (is_rc10(shader_start)) {
            rc10_init(shader_start);
            rc10_parse();
        } else {
            if (line_end) { *line_end = '\0'; }
            fprintf(stderr, "unknown shader type \"%s\"\n", shader_start);
            if (line_end) { *line_end = '\n'; }
        }

        // Recover next magic (was modified to mark end of current shader)
        if (next_shader_start) { *next_shader_start = '!'; }


        // Continue with next shader
        shader_start = next_shader_start;

    }

    free(ns);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <fpfile>\n", argv[0]);
        exit(1);
    }

    FILE* fh = fopen(argv[1], "rb");
    if (!fh) {
        fprintf(stderr, "unable to open \"%s\"\n", argv[1]);
        exit(1);
    }

    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    rewind(fh);
    char* buffer = (char*)malloc(size+1);
    if (buffer == NULL) {
        fprintf(stderr, "failed to allocate buffer for file\n");
        exit(1);
    }

    fread(buffer, size, 1, fh);
    buffer[size] = '\0';

    translate(buffer);

    fclose(fh);
    free(buffer);

    return 0;
}
