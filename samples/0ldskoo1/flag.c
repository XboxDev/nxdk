
#include <string.h>
#include <math.h>

#include <xboxkrnl/xboxkrnl.H>
#include <pbkit/pbkit.h>

#include "graphics.h"

#include "math3D.h"

#include "flag_tris.h"
#include "flag_tex.h"

static DWORD *vertices;
static DWORD num_vertices;

static MATRIX local_world;
static MATRIX local_light;
static MATRIX world_view;
// static MATRIX view_screen;
static MATRIX local_screen;

static VECTOR object_scale = {100.0, 100.0, 100.0, 1.00};
    // VECTOR object_position = { 0.00f, 0.00f, 0.00f, 1.00f };
    // VECTOR object_rotation = { 0.00f, 0.00f, 0.00f, 1.00f };
static VECTOR object_position = { -8.50f, 0.00f, 0.00f, 1.00f };
static VECTOR object_rotation = { 0.00f, 0.00f, 0.00f, 1.00f };

static VECTOR camera_position = { 0.00f, 0.00f, 100.00f, 1.00f };
static VECTOR camera_rotation = { 0.00f, 0.00f, 0.00f,   1.00f };

    // VECTOR light_direction = { 0.577f,0.577f,-0.577f, 0.00f };
static VECTOR light_direction = { 0.577f,0.577f,0.577f, 0.00f };
static VECTOR light_color     = { 1.00f, 1.00f, 1.00f,   0.00f };

static int cnt = 0;

static void load_texture(void)
{
    DWORD *p = pb_extra_buffer(1);
    DWORD pitch = pb_back_buffer_pitch()/4;

    int y;
    for (y=0; y<256; y++) {
        // memset(p+pitch*y, 0xff, pitch);
        memcpy(p+pitch*y, &flag_tex[y*256*4], 256*4);
    }
}

void flag_init(void)
{
    load_texture();

    vertices = MmAllocateContiguousMemoryEx(sizeof(flag_tris),0,0x3ffb000,0,0x404);
    memcpy(vertices, flag_tris, sizeof(flag_tris));

    num_vertices = sizeof(flag_tris)/sizeof(flag_tris[0]);
}

// static void filld(int x, int y, int w, int h, DWORD color, DWORD depth)
// {
//     DWORD       *p;

//     int     x1,y1,x2,y2;

//     x1=x;
//     y1=y;
//     x2=x+w;
//     y2=y+h;
    
//     //if you supply 32 bits color and res is 16 bits, apply function below
//     //color=((color>>8)&0xF800)|((color>>5)&0x07E0)|((color>>3)&0x001F);

//     p=pb_begin();
//     pb_push(p++,NV097_SET_CLEAR_RECT_HORIZONTAL,2);     //sets rectangle coordinates
//     *(p++)=((x2-1)<<16)|x1;
//     *(p++)=((y2-1)<<16)|y1;  //NV097_SET_CLEAR_RECT_VERTICAL
//     pb_push(p++,NV097_SET_ZSTENCIL_CLEAR_VALUE,3);     //sets data used to fill in rectangle
//     *(p++)=depth<<8;           //(depth<<8)|stencil
//     *(p++)=color;           //color NV097_SET_COLOR_CLEAR_VALUE
//     *(p++)=0xF0;            //triggers the HW rectangle fill (0x03 for D&S)  NV097_CLEAR_SURFACE
//     pb_end(p);
// }


void flag_update(MATRIX view_screen)
{
    // int             x=320,y=240,z=0;
    int             x=320,y=240-120,z=0;

    cnt++;

    //per object stuff:

    object_rotation[1]=(x-640/2)/80.0f;
    while (object_rotation[1] > M_PI) { object_rotation[1] -= 2*M_PI; }
    while (object_rotation[1] < -M_PI) { object_rotation[1] += 2*M_PI; }

    object_rotation[0]=(y-480/2)/80.0f;
    while (object_rotation[0] > M_PI) { object_rotation[0] -= 2*M_PI; }
    while (object_rotation[0] < -M_PI) { object_rotation[0] += 2*M_PI; }

    camera_position[_X]=0.0f+0*(y-480/2)/1.0f;
    camera_position[_Y]=0.0f+0*(y-480/2)/1.0f;
    // camera_position[_Z]=500.0f+z*1.0f;
    // camera_position[_Z]=340.0f+z*1.0f;
    camera_position[_Z]=100.0f+z*1.0f;


    float lp = 2*M_PI*(cnt % (10*60) ) / (10.0*60.0);
    light_direction[_X] = 0.577f * sinf(lp);
    // light_direction[_Y] = 0.577f + 0.2* cosf(lp);

    object_position[_X] = -8.50 + 8.5 * cosf(lp);

    //create the local_world matrix.
    // MATRIX local_world2;
    create_local_world(local_world, object_position, object_rotation);
    // VECTOR ts = (VECTOR){100.0, 100.0, 100.0, 1.0};
    matrix_scale(local_world, local_world, object_scale);

    //create the local_light matrix.
    create_local_light(local_light, object_rotation);
    //rotate light_dirs with local_light

    //create the world_view matrix.
    create_world_view(world_view, camera_position, camera_rotation);

    //create the local_screen matrix.
    create_local_screen(local_screen, local_world, world_view, view_screen);

}

void flag_draw(void)
{
    DWORD *p;

    set_source_extra_buffer(1); //so we read texture from extra buffer #1
    set_filters_off();
    set_transparency_off();

    //Uploads qwords C0-C13 (per object stuff : matrices, lights, etc...)
    //See vs.vsh for the mapping of constant parameter names to constants indices
    //Note that 'def' instructions will generate automatically upload commands
    //in the sequence created by pb_pcode2mcode, thus, you don't need to upload them
    //Caution, if you no longer use a named constant parameter, it won't
    //be mapped any longer and all incices will be lifted accordingly
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_ID,96); p+=2; //set shader constants cursor at C0
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,16);    //loads C0-C3
    memcpy(p,local_screen,16*4); p+=16;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,16);    //loads C4-C7
    memcpy(p,local_light,16*4); p+=16;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,4);     //loads C8
    memcpy(p,light_direction,4*4); p+=4;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,4);     //loads C9
    memcpy(p,light_color,4*4); p+=4;            
    
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,8);     //loads C10-C11

    *((float *)(p++))=500.0f; //final 2D scales (better apply this at the very end, in order to not disrupt clipping)
    *((float *)(p++))=500.0f;
    *((float *)(p++))=65536.0f; //actually we can occupy 24 bits of the depth field, but don't overflow
    *((float *)(p++))=1.0f; //don't change it (or unused if you only scale xyz)

    *((float *)(p++))=640.0f/2; //final 2D decals
    *((float *)(p++))=480.0f/2;
    *((float *)(p++))=65536.0f; //getting away from (0,0,0) is good for Z-buffering
    *((float *)(p++))=0.0f; //don't change it (or unused if you only decal xyz)

    pb_end(p);

    draw_triangle_list_batches(vertices, num_vertices);

    if (object_position[_X] < -2) pb_fill(0,125,640,150,0x00ff0000);

}