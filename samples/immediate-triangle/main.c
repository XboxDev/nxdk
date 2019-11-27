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

    /* A generic identity matrix */
    const float m_identity[4*4] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    /* A viewport matrix; this maps:
     *  - X from [-1, 1] to [0, width]
     *  - Y from [-1, 1] to [height, 0]
     *  - Z from [ 0, 1] to [0, 0xFFFF]
     *  - W to 1
     * This scales: X from [-1, 1] to [0, width]
     */
    float m_viewport[4*4] = {
        width/2.0f, 0.0f,         0.0f,          width/2.0f,
        0.0f,       height/-2.0f, 0.0f,          height/2.0f,
        0.0f,       0.0f,         (float)0xFFFF, 0.0f,
        0.0f,       0.0f,         0.0f,          1.0f
    };

    /* Setup fragment shader */
    p = pb_begin();
    //FIXME: The MASK macro should be contained within ps.inl
    //FIXME: Ideally ps.inl would contain a struct, not this weird garbage
    #pragma push_macro("MASK")
    #undef MASK //FIXME: Maybe not necessary?
    #define MASK(mask, val) (((val) << (__builtin_ffs(mask)-1)) & (mask))
    #include "ps.inl"
    #undef MASK //FIXME: Maybe not necessary?
    #pragma pop_macro("MASK")
    pb_end(p);

    /* Set up some default GPU state (should be done in xgux_init maybe? currently partially done in pb_init) */
    {
        p = pb_begin();

        //FIXME: p = xgu_set_skinning(p, XGU_SKINNING_OFF);
        //FIXME: p = xgu_set_normalization(p, false);
        p = xgu_set_lighting_enable(p, false);

        for(int i = 0; i < XGU_TEXTURE_COUNT; i++) {
            //FIXME: p = xgu_set_texgen(p, XGU_TEXGEN_OFF);
            //p = xgu_set_texture_matrix_enable(p, i, false);
        }

        for(int i = 0; i < XGU_WEIGHT_COUNT; i++) {
            p = xgu_set_model_view_matrix(p, i, m_identity); //FIXME: Not sure when used?
            p = xgu_set_inverse_model_view_matrix(p, i, m_identity); //FIXME: Not sure when used?
        }

        pb_end(p);
    }

    /* Set up all states for hardware vertex pipeline */
    p = pb_begin();
    p = xgu_set_transform_execution_mode(p, XGU_FIXED, XGU_RANGE_MODE_USER);
    //FIXME: p = xgu_set_fog_enable(p, false);
    p = xgu_set_projection_matrix(p, m_identity); //FIXME: Unused in XQEMU
    p = xgu_set_composite_matrix(p, m_viewport); //FIXME: Always used in XQEMU?
    p = xgu_set_viewport_offset(p, 0.0f, 0.0f, 0.0f, 0.0f);
    p = xgu_set_viewport_scale(p, 1.0f / width, 1.0f / height, 1.0f / (float)0xFFFF, 1.0f); //FIXME: Ignored?!
    pb_end(p);

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
