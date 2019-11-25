/*
 * This sample provides a very basic demonstration of 3D rendering on the Xbox,
 * using pbkit. Based on the pbkit demo sources.
 */
#include <hal/video.h>
#include <hal/xbox.h>
#include <math.h>
#include <pbkit/pbkit.h>
#include <xgu/xgu.h>
#include <xgu/xgux.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <xboxkrnl/xboxkrnl.h>
#include <hal/debug.h>
#include <windows.h>

static float     m_viewport[4][4];

#define MASK(mask, val) (((val) << (ffs(mask)-1)) & (mask))

static void matrix_viewport(float out[4][4], float x, float y, float width, float height, float z_min, float z_max);
static void init_shader(void);

/* Main program function */
int main(void)
{
    uint32_t *p;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    int status = pb_init();
    if (status) {
        debugPrint("pb_init Error %d\n", status);
        Sleep(2000);
        XReboot();
        return 1;
    }

    //FIXME: Hack for XQEMU
    Sleep(1000);

    pb_show_front_screen();

    /* Basic setup */
    int width = pb_back_buffer_width();
    int height = pb_back_buffer_height();

    /* Load constant rendering things (shaders, geometry) */
    init_shader();
    matrix_viewport(m_viewport, 0, 0, width, height, 0, 65536.0f);

    /* Setup to determine frames rendered every second */
    int       start, last, now;
    int       fps, frames, frames_total;
    start = now = last = GetTickCount();
    frames_total = frames = fps = 0;

    while(1) {
        pb_wait_for_vbl();
        pb_reset();
        pb_target_back_buffer();

        /* Clear depth & stencil buffers */
        pb_erase_depth_stencil_buffer(0, 0, width, height); //FIXME: Do in XGU
        pb_fill(0, 0, width, height, 0x00000000); //FIXME: Do in XGU
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
        p = xgu_set_transform_constant_load(p, 96);

        /* Send the transformation matrix */
        p = xgu_set_transform_constant(p, m_viewport, 4);

        pb_end(p);
        p = pb_begin();

        p = xgu_begin(p, XGU_TRIANGLES);

        /* Background triangle 1 */
        p = xgux_set_color3f(p, 0.1f, 0.1f, 0.6f); p = xgu_vertex3f(p, -1.0f, -1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.0f, 0.0f, 0.0f); p = xgu_vertex3f(p, -1.0f,  1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.0f, 0.0f, 0.0f); p = xgu_vertex3f(p,  1.0f,  1.0f,  1.0f);

        /* Background triangle 2 */
        p = xgux_set_color3f(p, 0.1f, 0.1f, 0.6f); p = xgu_vertex3f(p, -1.0f, -1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.0f, 0.0f, 0.0f); p = xgu_vertex3f(p,  1.0f,  1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.1f, 0.1f, 0.6f); p = xgu_vertex3f(p,  1.0f, -1.0f,  1.0f);

        /* Foreground triangle */
        p = xgux_set_color3f(p, 1.0f, 0.0f, 0.0f); p = xgu_vertex3f(p, -1.0f, -1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.0f, 1.0f, 0.0f); p = xgu_vertex3f(p,  0.0f,  1.0f,  1.0f);
        p = xgux_set_color3f(p, 0.0f, 0.0f, 1.0f); p = xgu_vertex3f(p,  1.0f, -1.0f,  1.0f);

        p = xgu_end(p);

        pb_end(p);

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
        now = GetTickCount();
        if ((now-last) > 1000) {
            fps = frames;
            frames = 0;
            last = now;
        }
    }

    /* Unreachable cleanup code */
    pb_show_debug_screen();
    pb_kill();
    
    return 0;
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
    p = xgu_set_transform_program_start(p, 0);

    /* Set execution mode */
    p = xgu_set_transform_execution_mode(p, XGU_PROGRAM, XGU_RANGE_MODE_PRIVATE);
    p = xgu_set_transform_program_cxt_write_enable(p, false);

    /* Set cursor and begin copying program */
    p = xgu_set_transform_program_load(p, 0);
    p = xgu_set_transform_program(p, vs_program, sizeof(vs_program)/16);

    pb_end(p);

    /* Setup fragment shader */
    p = pb_begin();
    #include "ps.inl"
    pb_end(p);
}
