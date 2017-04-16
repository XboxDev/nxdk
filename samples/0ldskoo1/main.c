//Demo 04-Initial Fantasy

#include <hal/input.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <xboxrt/debug.h>
#include <xboxkrnl/xboxkrnl.h> //for MmFreeContiguousMemory

#include <pbkit/pbkit.h>


#include "string.h"
#include "stdio.h"
#include <stdlib.h>
#include <math.h>

#include "math3d.h"

#include "graphics.h"

// #include "mesh.h"
#include "flag.h"


#define WAIT    //force waitings to allow precise loop performance time calculation (remove that to gain more speed later)

//under that displacement, a stick will be considered in neutral position
#define DEADZONE_JOYPAD     16

int model=0;

unsigned int cpu_ticks(void) 
{
    unsigned int v;
    __asm__ __volatile__ ("rdtsc":"=a" (v));
    //edx:eax holds cpu ticks count after 'rdtsc'
    return v;
}

int verbose=0;


int test(void)
{

    int         i,dx,dy,dz;
    DWORD       *p;
    int         perf; //performance bar length (time spent rendering in a frame)
    //float           aspect=1.78f; //16/9, 4/3=1.33f
    float aspect=1.33f;

    
    int         frames=0;

    int         done=0;

    DWORD       frame_start_ticks;
    DWORD       frame_end_ticks;
    DWORD       loop_start_ticks;
    DWORD       frame_ticks;

    int         antialiasing=0;
    int         debug_screen=0;
    int         depth_screen=0;

    // int         texture;
    

    //clear screen
    pb_wait_for_vbl();
    frame_start_ticks=cpu_ticks(); //remember when vblank events occured
    pb_reset(); //do it at beginning of the frame
    pb_fill(0,0,640,480,0);
    while(pb_finished());
    pb_wait_for_vbl();
    frame_ticks=cpu_ticks()-frame_start_ticks; //around 12 Millions cpu ticks
    pb_reset(); //do it at beginning of the frame
    pb_fill(0,0,640,480,0);
    while(pb_finished());
    

    //ooPo's math3D lib, ported from ps2sdk
    
    //create the view_screen matrix.
    MATRIX view_screen;
    create_view_screen(view_screen, aspect, -3.00f, 3.00f, -3.00f, 3.00f, 1.00f, 2000.00f);

    // int max = num_vertices;

    flag_init();

    perf=0;

    while(done==0)
    {
        //per frame stuff:
        
        pb_wait_for_vbl(); //because we want to count ticks from start of frame (remove it when trying triple buffering technic)

        frame_start_ticks=cpu_ticks(); //remember when vblank events occured
        frames++;

        pb_reset(); //new frame, let's start from push buffer head again

        //we draw in non anti-aliased static extra frame buffer now
        pb_target_extra_buffer(0); //set target for pixel rendering AND image bitblt
        pb_erase_depth_stencil_buffer(0,0,640,480); //clear depth stencil buffer (MANDATORY)
        // pb_fill(0,0,640,480,0x0000ff); //clear frame (optional, also, this call and previous one can be merged: flag=0xf3)
        pb_fill(0,0,640,480,0x00005500);
        pb_erase_text_screen();
        
        // set_source_extra_buffer(1); //so we read texture from extra buffer #1
        // set_filters_off();
        // set_transparency_off();

        // if (perf) pb_fill(0,50,perf,1,0xffff00);

        flag_update(view_screen);


// #ifdef WAIT
//         while(pb_busy());
//         //we have finished the per frame/object stuff here
// #endif
        //per batch stuff:
        loop_start_ticks=cpu_ticks();

        flag_draw();

#ifdef WAIT
        while(pb_busy());
        //We have finished rendering here
#endif
        frame_end_ticks=cpu_ticks();

        //let's calculate next performance bar length
        perf=(int)(640.0f*(frame_end_ticks-frame_start_ticks)/frame_ticks);

        // pb_print("Z=%d %d verts (prep:%d%% loop:%d%% %f v/f max)\n",
        //     (int)camera_position[_Z],
        //     max,
        //     (int)(100.0f*(loop_start_ticks-frame_start_ticks)/frame_ticks),
        //     (int)(100.0f*(frame_end_ticks-loop_start_ticks)/frame_ticks),
        //     (max*((float)frame_ticks)/(frame_end_ticks-loop_start_ticks))
        //     );

/*
pb_print("%f %f %f %f\n",m[4*0+0],m[4*0+1],m[4*0+2],m[4*0+3]);
pb_print("%f %f %f %f\n",m[4*1+0],m[4*1+1],m[4*1+2],m[4*1+3]);
pb_print("%f %f %f %f\n",m[4*2+0],m[4*2+1],m[4*2+2],m[4*2+3]);
pb_print("%f %f %f %f\n",m[4*3+0],m[4*3+1],m[4*3+2],m[4*3+3]);
*/

//verbose=0;
//if (verbose) get_mesh_vertices("x",NULL);
//verbose=0;    
        pb_draw_text_screen();

        XInput_GetEvents();

        if (g_Pads[0].PressedButtons.ucAnalogButtons[XPAD_Y]) done=1;

        // if (g_Pads[0].CurrentButtons.ucAnalogButtons[XPAD_X]) max=((max/252)-1)*252;
        // if (g_Pads[0].CurrentButtons.ucAnalogButtons[XPAD_B]) max=((max/252)+1)*252;

        //Doing antialiasing would require to switch shaders
        //if (g_Pads[0].PressedButtons.ucAnalogButtons[XPAD_X]) antialiasing=0;
        //if (g_Pads[0].PressedButtons.ucAnalogButtons[XPAD_B]) antialiasing=1;

        // if (g_Pads[0].PressedButtons.usDigitalButtons&XPAD_START) 
        // {
        //     if (depth_screen)
        //     {
        //         depth_screen=0;
        //         debug_screen=0;
        //         pb_show_front_screen();
        //     }
        //     else
        //     {
        //         depth_screen=1;
        //         debug_screen=0;
        //         pb_show_depth_screen();
        //     }
        // }
        
        // if (g_Pads[0].PressedButtons.usDigitalButtons&XPAD_BACK)
        // {
        //     if (debug_screen)
        //     {
        //         depth_screen=0;
        //         debug_screen=0;
        //         pb_show_front_screen();
        //     }
        //     else
        //     {
        //         depth_screen=0;
        //         debug_screen=1;
        //         pb_show_debug_screen();
        //     }
        // }

        // if (max<252) max=252;
        // if (max>num_vertices) max=num_vertices;

        // dx=g_Pads[0].sRThumbX/256;
        // dy=-g_Pads[0].sRThumbY/256;
        // dz=-g_Pads[0].sLThumbY/256;

        // if (dy<-DEADZONE_JOYPAD) y+=(dy+DEADZONE_JOYPAD)/8;
        // if (dy>DEADZONE_JOYPAD) y+=(dy-DEADZONE_JOYPAD)/8;
        // if (dx<-DEADZONE_JOYPAD) x+=(dx+DEADZONE_JOYPAD)/8;
        // if (dx>DEADZONE_JOYPAD) x+=(dx-DEADZONE_JOYPAD)/8;
        // if (dz<-DEADZONE_JOYPAD) z+=(dz+DEADZONE_JOYPAD)/8;
        // if (dz>DEADZONE_JOYPAD) z+=(dz-DEADZONE_JOYPAD)/8;
        // if (g_Pads[0].CurrentButtons.usDigitalButtons & XPAD_DPAD_UP) z += 4;
        // if (g_Pads[0].CurrentButtons.usDigitalButtons & XPAD_DPAD_DOWN) z -= 4;


        // if (x<0) x=0;
        // if (y<0) y=0;
        // if (x>640) x=640;
        // if (y>480) y=480;
    
    
        set_source_extra_buffer(0); //for incoming antialiasing texture copy
        set_filters_on();
        set_transparency_off(); 

        //now, we draw in usual rotating back buffer an AntiAliased copy
        //of our non-AntiAliased static extra frame buffer (we just drew in it)
        pb_target_back_buffer(); //set target for pixel rendering AND image bitblt
        if (antialiasing)
            antialiased_texture_copy();
        else
            image_bitblt(); //just do an hardware bitblt!
        //you may eventually draw here something (non anti-aliased HUD for example)
        
        //we declare this frame finished (next VBLank will swap screens automatically)
        while (pb_finished()) //no free buffer available, wait and retry later
        {
            //Should never happen if you use pb_wait_for_vbl for each frame
            //Otherwise, it gives an information : you are too much in advance
            //so, you can try drawing more details in each frame. Functions
            //pb_get_vbl_counter() can be used to survey your advance.
        }
    }

    // if (vertices) MmFreeContiguousMemory((PVOID)vertices);

    return 0;
}





void main(void)
{
    int     i;

    XInput_Init();

    pb_extra_buffers(2);
    //we want 1 extra frame buffer (#0) that will be the non-AntiAliased back buffer
    //(an AntiAliased texture copy can be done from static non-AA back buffer towards AA rotating back buffer)
    //we want 1 extra frame buffer (#1) that will hold texture
    
    switch(pb_init())
    {
        case 0: break; //no error

        // case -4:        
        //     debugPrint("IRQ3 already handled. Can't install GPU interrupt.\n");
        //     debugPrint("You must apply the patch and compile again OpenXDK.\n");
        //     debugPrint("Also be sure to call pb_init() before any other call.\n");
        //     XSleep(2000);
        //     XReboot();
        //     return;

        // case -5:        
        //     debugPrint("Unexpected PLL configuration. Report this issue please.\n");
        //     XSleep(2000);
        //     XReboot();
        //     return;
        
        default:
            // debugPrint("pb_init Error\n");
            XSleep(2000);
            XReboot();
            return;
    }

    prepare_shaders();

    //Because openxdk frame buffer is not allocated in memory, third frame buffer
    //overlaps openxdk frame buffer. To avoid graphic interference, skip upper lines.
    //Note that scrolling up will produce an interference, but won't prevent reading.
    for(i=0;i<22;i++) debugPrint("\n"); //if you use double font size, skip 11 only
    //need to call pb_show_debug_screen() to see it, revert with pb_show_front_buffer()
    // debugPrint("Debug screen (Press BACK to come here or leave from here):\n\n");
    //debugPrint("Debug screen (Press X and B to turn antialiasing off/on):\n\n");

    // pb_show_debug_screen();

    // debugPrint("\nChoose the model :\n\n");
    // debugPrint("A - SpaceFighter (500 faces, 1500 vertices)\n\n");
    // debugPrint("B - Fuselage (30000 faces, 90000 vertices)\n\n");

    // while(model==0)
    // {
    //  XInput_GetEvents();
    //  if (g_Pads[0].PressedButtons.ucAnalogButtons[XPAD_A]) model=1;
    //  if (g_Pads[0].PressedButtons.ucAnalogButtons[XPAD_B]) model=2;
    // }
    model = 1;

    pb_show_front_screen();

    while(test());

    pb_show_debug_screen();
    // while(1) {
    //     XSleep(2000);
    //     debugPrint("ping\n");
    // }
    // while(1) debugPrint("done\n");

    pb_kill();

    XInput_Quit();

    XReboot();
}
