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

static char* copy_string(const char* str, size_t length) {
    char* buffer = (char*)malloc(length + 1);
    memcpy(buffer, str, length);
    buffer[length] = '\0';
    return buffer;
}

static char* find_at_line_start(const char* haystack, const char* cursor,
                                const char* needle) {
    cursor = strstr(cursor, needle);
    while (cursor != NULL) {

        // Accept if candidate is at beginning of file or line
        if ((cursor == haystack) || (cursor[-1] == '\n')) {
            return (char*)cursor;
        }

        cursor = strstr(cursor + 1, needle);
    }
    return NULL;
}

void translate(const char* s) {

    // Keep a cursor for line-counting
    const char* line_cursor = s;
    unsigned int shader_line_number = 1;

    // Look for the first shader magic
    const char* shader_magic = find_at_line_start(s, s, "!!");

    // Warn the user if we couldn't find any shader at all
    if (shader_magic == NULL) {
        fprintf(stderr, "no shaders found\n");
    }

    // Loop until we can't find a shader anymore
    while (shader_magic != NULL) {

        // Move line cursor until we found the line of the shader magic
        while (line_cursor < shader_magic) {
            line_cursor = strchr(line_cursor, '\n');
            assert(line_cursor != NULL);
            line_cursor++;
            shader_line_number++;
        }

        // Find end of shader magic line (whitespace or comment)
        const char* shader_magic_end = shader_magic+2;
        while (1) {
            shader_magic_end += strcspn(shader_magic_end, " \t\n\r#;/*");

            // Check for start of C style comment or single-line comment
            if (shader_magic_end[0] == '/') {
                if ((shader_magic_end[1] != '*') &&
                    (shader_magic_end[1] != '/')) {
                    shader_magic_end++;
                    continue;
                }
            }

            // Check for end of C style comment
            if (shader_magic_end[0] == '*') {
                if (shader_magic_end[1] != '/') {
                    shader_magic_end++;
                    continue;
                }
            }

            break;
        }

        // Copy the magic
        size_t shader_magic_len = shader_magic_end - shader_magic;
        char* shader_magic_str = copy_string(shader_magic, shader_magic_len);

        // Add information about shader section to output
        printf("/* %s (line %u) */\n", shader_magic_str, shader_line_number);

        // Shader magic marks shader start
        const char* shader = shader_magic;

        // The next shader magic will mark the end of current shader; find it
        const char* next_shader_magic;
        if (*shader_magic_end != '\0') {
            next_shader_magic = find_at_line_start(s, shader_magic_end+1, "!!");
        } else {
            next_shader_magic = NULL;
        }

        // Find shader end; also respect case where no other shader follows
        const char* shader_end;
        if (next_shader_magic != NULL) {
            shader_end = next_shader_magic;
        } else {
            shader_end = &shader_magic_end[strlen(shader_magic_end)];
        }

        // Copy the shader
        size_t shader_len = shader_end - shader;
        char* shader_str = copy_string(shader, shader_len);

        // Prepare error reporting
        errors.set_line_number_offset(shader_line_number-1);

        // Process shader
        if (is_ts10(shader_str)) {
            ts10_init(shader_str);
            ts10_parse();
        } else if (is_rc10(shader_str)) {
            rc10_init(shader_str);
            rc10_parse();
        } else {
            fprintf(stderr, "unknown shader type \"%s\"\n", shader_magic_str);
        }

        // Show all errors
        int num_errors = errors.get_num_errors();
        if (num_errors > 0) {
            fprintf(stderr, "errors in shader \"%s\" (line %d):\n",
                    shader_magic_str, shader_line_number);
            for(int i = 0; i < num_errors; i++) {
                fprintf(stderr, "error: %s\n", errors.get_errors()[i]);
            }
        }

        // Free temporary string copies
        free(shader_str);
        free(shader_magic_str);

        // Continue with next shader by jumping to its magic
        shader_magic = next_shader_magic;
    }
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
