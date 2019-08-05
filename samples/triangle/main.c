/*
 * This sample provides a very basic demonstration of 3D rendering on the Xbox,
 * using pbkit. Based on the pbkit demo sources.
 */
#include <hal/video.h>
#include <hal/xbox.h>
#include <math.h>
#include <pbkit/pbkit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <xboxkrnl/xboxkrnl.h>
#include <xboxrt/debug.h>
#include <windows.h>

static uint32_t *alloc_vertices;
static uint32_t  num_vertices;
static float     m_viewport[4][4];

typedef struct {
    float pos[3];
    float color[3];
} __attribute__((packed)) ColoredVertex;

static const ColoredVertex verts[] = {
    //  X     Y     Z       R     G     B
    {{-1.0, -1.0,  1.0}, { 0.1,  0.1,  0.6}}, /* Background triangle 1 */
    {{-1.0,  1.0,  1.0}, { 0.0,  0.0,  0.0}},
    {{ 1.0,  1.0,  1.0}, { 0.0,  0.0,  0.0}},
    {{-1.0, -1.0,  1.0}, { 0.1,  0.1,  0.6}}, /* Background triangle 2 */
    {{ 1.0,  1.0,  1.0}, { 0.0,  0.0,  0.0}},
    {{ 1.0, -1.0,  1.0}, { 0.1,  0.1,  0.6}},
    {{-1.0, -1.0,  1.0}, { 1.0,  0.0,  0.0}}, /* Foreground triangle */
    {{ 0.0,  1.0,  1.0}, { 0.0,  1.0,  0.0}},
    {{ 1.0, -1.0,  1.0}, { 0.0,  0.0,  1.0}},
};

#define MASK(mask, val) (((val) << (ffs(mask)-1)) & (mask))

static void matrix_viewport(float out[4][4], float x, float y, float width, float height, float z_min, float z_max);
static void init_shader(void);
static void set_attrib_pointer(unsigned int index, unsigned int format, unsigned int size, unsigned int stride, const void* data);
static void draw_arrays(unsigned int mode, int start, int count);

/* Main program function */
void main(void)
{
    uint32_t *p;
    int       i, status;
    int       width, height;
    int       start, last, now;
    int       fps, frames, frames_total;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    if ((status = pb_init())) {
        debugPrint("pb_init Error %d\n", status);
        Sleep(2000);
        XReboot();
        return;
    }

    pb_show_front_screen();

    /* Basic setup */
    width = pb_back_buffer_width();
    height = pb_back_buffer_height();

    /* Load constant rendering things (shaders, geometry) */
    init_shader();
    alloc_vertices = MmAllocateContiguousMemoryEx(sizeof(verts), 0, 0x3ffb000, 0, 0x404);
    memcpy(alloc_vertices, verts, sizeof(verts));
    num_vertices = sizeof(verts)/sizeof(verts[0]);
    matrix_viewport(m_viewport, 0, 0, width, height, 0, 65536.0f);

    /* Setup to determine frames rendered every second */
    start = now = last = XGetTickCount();
    frames_total = frames = fps = 0;

    while(1) {
        pb_wait_for_vbl();
        pb_reset();
        pb_target_back_buffer();

        /* Clear depth & stencil buffers */
        pb_erase_depth_stencil_buffer(0, 0, width, height);
        pb_fill(0, 0, width, height, 0x00000000);
        pb_erase_text_screen();

        while(pb_busy()) {
            /* Wait for completion... */
        }

        /* Send shader constants
         *
         * WARNING: Changing shader source code may impact constant locations!
         * Check the intermediate file (*.inl) for the expected locations after
         * changing the code.
         */
        p = pb_begin();

        /* Set shader constants cursor at C0 */
        pb_push1(p, NV097_SET_TRANSFORM_CONSTANT_LOAD, 96); p+=2;

        /* Send the transformation matrix */
        pb_push(p++, NV097_SET_TRANSFORM_CONSTANT, 16);
        memcpy(p, m_viewport, 16*4); p+=16;

        pb_end(p);
        p = pb_begin();

        /* Clear all attributes */
        pb_push(p++, NV097_SET_VERTEX_DATA_ARRAY_FORMAT,16);
        for(i = 0; i < 16; i++) {
            *(p++) = NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F;
        }
        pb_end(p);

        /* Set vertex position attribute */
        set_attrib_pointer(0, NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
                           3, sizeof(ColoredVertex), &alloc_vertices[0]);

        /* Set vertex diffuse color attribute */
        set_attrib_pointer(3, NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
                           3, sizeof(ColoredVertex), &alloc_vertices[3]);

        /* Begin drawing triangles */
        draw_arrays(NV097_SET_BEGIN_END_OP_TRIANGLES, 0, num_vertices);

        /* Draw some text on the screen */
        pb_print("Triangle Demo\n");
        pb_print("Frames: %d\n", frames_total);
        if (fps > 0) {
            pb_print("FPS: %d", fps);
        }
        pb_draw_text_screen();

        while(pb_busy()) {
            /* Wait for completion... */
        }

        /* Swap buffers (if we can) */
        while (pb_finished()) {
            /* Not ready to swap yet */
        }

        frames++;
        frames_total++;

        /* Latch FPS counter every second */
        now = XGetTickCount();
        if ((now-last) > 1000) {
            fps = frames;
            frames = 0;
            last = now;
        }
    }

    /* Unreachable cleanup code */
    MmFreeContiguousMemory(alloc_vertices);
    pb_show_debug_screen();
    pb_kill();
    HalReturnToFirmware(HalQuickRebootRoutine);
}

/* Construct a viewport transformation matrix */
static void matrix_viewport(float out[4][4], float x, float y, float width, float height, float z_min, float z_max)
{
    memset(out, 0, 4*4*sizeof(float));
    out[0][0] = width/2.0f;
    out[1][1] = height/-2.0f;
    out[2][2] = z_max - z_min;
    out[3][3] = 1.0f;
    out[3][0] = x + width/2.0f;
    out[3][1] = y + height/2.0f;
    out[3][2] = z_min;
}

/* Load the shader we will render with */
static void init_shader(void)
{
    uint32_t *p;
    int       i;

    /* Setup vertex shader */
    uint32_t vs_program[] = {
        #include "vs.inl"
    };

    p = pb_begin();

    /* Set run address of shader */
    pb_push1(p, NV097_SET_TRANSFORM_PROGRAM_START, 0);
    p += 2;

    /* Set execution mode */
    pb_push1(p, NV097_SET_TRANSFORM_EXECUTION_MODE,
        MASK(NV097_SET_TRANSFORM_EXECUTION_MODE_MODE, NV097_SET_TRANSFORM_EXECUTION_MODE_MODE_PROGRAM)
        | MASK(NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE, NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE_PRIV));
    p += 2;

    pb_push1(p, NV097_SET_TRANSFORM_PROGRAM_CXT_WRITE_EN, 0);
    p += 2;

    /* Set cursor and begin copying program */
    pb_push1(p, NV097_SET_TRANSFORM_PROGRAM_LOAD, 0);
    p += 2;

    for (i=0; i<sizeof(vs_program)/8; i++) {
        pb_push(p++, NV097_SET_TRANSFORM_PROGRAM, 4);
        memcpy(p, &vs_program[i*4], 4*4);
        p+=4;
    }

    pb_end(p);

    /* Setup fragment shader */
    p = pb_begin();
    #include "ps.inl"
    pb_end(p);
}

/* Set an attribute pointer */
static void set_attrib_pointer(unsigned int index, unsigned int format, unsigned int size, unsigned int stride, const void* data)
{
    uint32_t *p = pb_begin();
    pb_push1(p, NV097_SET_VERTEX_DATA_ARRAY_FORMAT + index*4,
        MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE, format) | \
        MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_SIZE, size) | \
        MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_STRIDE, stride));
    p += 2;
    pb_push1(p, NV097_SET_VERTEX_DATA_ARRAY_OFFSET + index*4, (uint32_t)data & 0x03ffffff);
    p += 2;
    pb_end(p);
}

/* Send draw commands for the triangles */
static void draw_arrays(unsigned int mode, int start, int count)
{
    uint32_t *p = pb_begin();
    pb_push1(p, NV097_SET_BEGIN_END, mode); p += 2;

    pb_push(p++,0x40000000|NV097_DRAW_ARRAYS,1); //bit 30 means all params go to same register 0x1810
    *(p++) = MASK(NV097_DRAW_ARRAYS_COUNT, (count-1)) | MASK(NV097_DRAW_ARRAYS_START_INDEX, start);

    pb_push1(p,NV097_SET_BEGIN_END, NV097_SET_BEGIN_END_OP_END); p += 2;
    pb_end(p);
}
