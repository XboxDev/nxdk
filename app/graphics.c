
#include <stdint.h>
#include <string.h>

#include <strings.h>
#include <pbkit/pbkit.h>

#include "graphics.h"

#define MASK(mask, val) (((val) << (ffs(mask)-1)) & (mask))


void draw_arrays(unsigned int mode, int start, int count)
{
    static  int overflow_estimation=0;

    int num_batches=(count+MAX_VERTICES_PER_BATCH-1)/MAX_VERTICES_PER_BATCH;
    int i;
    DWORD offset=0;
    while(num_batches) {
        int n;
        if (num_batches > 1) {
            n = MAX_VERTICES_PER_BATCH;
        } else {
            n = count % MAX_VERTICES_PER_BATCH;
        }


        DWORD *p = pb_begin();
        pb_push1(p, NV097_SET_BEGIN_END, mode); p += 2;

        DWORD size=(n+251)/252;
        pb_push(p++,0x40000000|NV097_DRAW_ARRAYS,size); //bit 30 means all params go to same register 0x1810
        
        while(size) {
            DWORD num_vertices_this_batch;
            if (size>1) 
                num_vertices_this_batch=252; 
            else 
                num_vertices_this_batch=n%252;
            
            *(p++)=((num_vertices_this_batch-1)<<24)|offset;
            
            offset+=252;
            size--;
        }

        pb_push1(p,NV097_SET_BEGIN_END, NV097_SET_BEGIN_END_OP_END); p += 2;
        pb_end(p);

        overflow_estimation+=n;
        if (overflow_estimation>252*8*16000) //depends on push buffer size
        {
            overflow_estimation=0;
            pb_reset(); //jump back to push buffer head to avoid buffer overflow
        }

        num_batches--;
    }
}

void draw_triangle_list_batches(DWORD *vertices, int num_vertices)
{
    unsigned int stride = (3+3+2)*4;
    clear_attribs();
    // position
    set_attrib_pointer(0, NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
                       3, stride, &vertices[0]);
    // normal
    set_attrib_pointer(2, NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
                       3, stride, &vertices[3]);
    // texcoord
    set_attrib_pointer(8, NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
                       2, stride, &vertices[6]);

    draw_arrays(NV097_SET_BEGIN_END_OP_TRIANGLES, 0, num_vertices);
}

#if 0
void draw_triangle_list_batches(DWORD *vertices, int num_vertices)
{
    int n;
    DWORD num_batches=(num_vertices+MAX_VERTICES_PER_BATCH-1)/MAX_VERTICES_PER_BATCH;
    DWORD *ptr_dma_buffer=vertices; //prepared vertices(uv,normal,xyz) data
    while(num_batches) //partial progressive method
    {
        if (num_batches>1)
            n=MAX_VERTICES_PER_BATCH;
        else
            n=num_vertices%MAX_VERTICES_PER_BATCH;

        draw_triangle_list(ptr_dma_buffer,n);

        ptr_dma_buffer+=n*(3+3+2);
        num_batches--;
    }
}

void draw_triangle_list(DWORD *vertices,int num_vertices)
{
static  int overflow_estimation=0;
    DWORD   *p;
    DWORD   size;
#ifdef USE_V_BATCHES
    DWORD   offset;
    DWORD   num_vertices_this_batch;
#endif

#ifdef USE_I_BATCHES
    DWORD   index_upper,index_lower,i;
#endif

    //Useless if normal method is used and necessary for other ones:
    //we set addresses for V0,V3 and v7 feeds
    //Strides (+4*8) have already been set by prepare_shaders()
    p=pb_begin();
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VB_POINTER_ATTR0_POS+0*4,1); //v0 feed address
    *(p++)=((DWORD)(&vertices[0]))&0x03ffffff;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VB_POINTER_ATTR0_POS+2*4,1); //v3 feed address
    *(p++)=((DWORD)(&vertices[3]))&0x03ffffff;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_VB_POINTER_ATTR0_POS+8*4,1); //v7 feed address
    *(p++)=((DWORD)(&vertices[6]))&0x03ffffff;
    pb_end(p);
    
#ifdef USE_V_BATCHES
    //vertex batches method: fast (like dma 'ref' tags on ps2)
    //we enqueue 1 dword that triggers up to 256 vertices transferts
    p=pb_begin();
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=TRIANGLES; //(beginning of list)

    size=(num_vertices+251)/252;
    pb_push(p++,0x40000000|NV20_TCL_PRIMITIVE_3D_VB_VERTEX_BATCH,size); //bit 30 means all params go to same register 0x1810
    offset=0;
    while(size)
    {
        if (size>1) 
            num_vertices_this_batch=252; 
        else 
            num_vertices_this_batch=num_vertices%252;
        
        *(p++)=((num_vertices_this_batch-1)<<24)|offset;
        
        offset+=252;
        size--;
    }

    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=STOP; //triggers the drawing (end of list)

    pb_end(p);

    overflow_estimation+=num_vertices;
    if (overflow_estimation>252*8*16000) //depends on push buffer size
    {
        overflow_estimation=0;
        pb_reset(); //jump back to push buffer head to avoid buffer overflow
    }

#else

#ifdef USE_I_BATCHES
    //index batches method: fast
    //we will enqueue 1 dword for every 2 indices (so we need much less pb_reset calls)

    p=pb_begin();
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=TRIANGLES; //(beginning of list)

    //normally you have to read indices from faces list of mesh
    //but here we will just simulate by incrementing counters
    //for educational (This technic is not compatible with ps2)
    
    //This method speed is the vertex batches method speed
    //multiplied by the ratio num_vertices/num_faces where
    //num_vertices is the number of vertices required by the
    //vertex batches method, depending on the mesh structure.
    //(i.e more than the actual number of vertices in the mesh)
    
    index_upper=1; //upper 16 bits is following index
    index_lower=0;
    if (num_vertices&1) //need to handle special case of odd numbers, since indices are handled by pairs
    {
        pb_push(p++,NV20_TCL_PRIMITIVE_3D_INDEX_DATA+4,size); //no constant yet in nouveau header for 0x1804 ('lonely' index data register)
        *(p++)=index_lower;
        num_vertices--; //make the number even now
        index_lower++;
        index_upper++;
    }
    size=num_vertices/2; //xyz,normal,uv
    pb_push(p++,0x40000000|NV20_TCL_PRIMITIVE_3D_INDEX_DATA,size); //bit 30 means all params go to same register 0x1800
    //let's simulate the flow of face indices
    for(i=0;i<size;i++)
    {
        *(p++)=(index_upper<<16)|index_lower;
        index_upper+=2;
        index_lower+=2;
    }

    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=STOP; //triggers the drawing (end of list)

    pb_end(p);

    overflow_estimation+=num_vertices;
    if (overflow_estimation>16*16000)  //depends on push buffer size
    {
        overflow_estimation=0;
        pb_reset(); //jump back to push buffer head to avoid buffer overflow
    }

#else
    //normal method:  slow (may require to call pb_reset often to avoid push buffer overflow)
    //we will enqueue 3+3+2 dwords for each vertice

    p=pb_begin();
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=TRIANGLES; //(beginning of list)

    size=num_vertices*(3+3+2); //xyz,normal,uv
    pb_push(p++,0x40000000|NV20_TCL_PRIMITIVE_3D_VERTEX_DATA,size); //bit 30 means all params go to same register 0x1818
    memcpy(p,vertices,size*4); p+=size;

    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=STOP; //triggers the drawing (end of list)

    pb_end(p);

    overflow_estimation+=num_vertices;
    if (overflow_estimation>16000) //depends on push buffer size
    {
        overflow_estimation=0;
        pb_reset(); //jump back to push buffer head to avoid buffer overflow
    }

#endif

#endif
}

#endif

void set_vertex_program(const uint32_t* tokens, size_t num_instructions)
{
    DWORD *p = pb_begin();
    pb_push(p++,NV097_SET_TRANSFORM_PROGRAM_START,1); *(p++)=0; //set run address of shader
    pb_push(p++,NV097_SET_TRANSFORM_EXECUTION_MODE,2);
    *(p++)=SHADER_TYPE_EXTERNAL;
    //set shader vertex type (external shader, regular: not allowed to write into constants -faster-)
    *(p++)=SHADER_SUBTYPE_REGULAR; //NV097_SET_TRANSFORM_PROGRAM_CXT_WRITE_EN
    pb_push(p++,NV097_SET_TRANSFORM_PROGRAM_LOAD,1); *(p++)=0; //set cursor in order to load data into program area

    int i;
    for (i=0; i<num_instructions; i++) {
        pb_push(p++,NV097_SET_TRANSFORM_PROGRAM,4);
        memcpy(p, &tokens[i*4], 4*4);
        p+=4;
    }

    pb_end(p);
}

void clear_attribs(void)
{
    int i;
    DWORD *p=pb_begin();
    pb_push(p++,NV097_SET_VERTEX_DATA_ARRAY_FORMAT,16);
    for(i=0;i<16;i++) *(p++)=2; //resets array (nothing goes to v0-v15)
    pb_end(p);
}

void set_attrib_pointer(unsigned int index,
                        unsigned int format,
                        unsigned int size,
                        unsigned int stride,
                        const void* data)
{
    DWORD *p = pb_begin();
    pb_push1(p, NV097_SET_VERTEX_DATA_ARRAY_FORMAT + index*4,
             MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE, format)
              | MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_SIZE, size)
              | MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_STRIDE, stride));
    p += 2;
    pb_push1(p, NV097_SET_VERTEX_DATA_ARRAY_OFFSET + index*4, (DWORD)data & 0x03ffffff);
    p += 2;
    pb_end(p);
}

void prepare_shaders(void)
{
    DWORD           *p;
    int         i,j;

    //Here, we get everything ready for 1 fast texture copy (per frame)
    //from static extra frame buffer, into rotating back buffer.
    //Strong anti-aliasing filters are setup for the texture copy
    //(gaussian cubic)

    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_CULL_FACE_ENABLE,0); p+=2;//CullModeEnable=FALSE
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_DITHER_ENABLE,1); p+=2; //DitherEnable=TRUE
    pb_push2(p,NV20_TCL_PRIMITIVE_3D_POINT_PARAMETERS_ENABLE,0,0); p+=3; //PointScaleEnable=FALSE & PointSpriteEnable=FALSE
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_POINT_SIZE,8); p+=2; //PointSize=1.0f
    pb_end(p);

    //attach extra buffer memory area to texture stage 0 (used in pixel shader)
    //extra buffers and back buffers have same sizes and pitches
    p=pb_begin();
    pb_push2(p,NV20_TCL_PRIMITIVE_3D_TX_OFFSET(0),((DWORD)pb_extra_buffer(0))&0x03FFFFFF,0x0001122a); p+=3; //set stage 0 texture address & format
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_NPOT_PITCH(0),pb_back_buffer_pitch()<<16); p+=2; //set stage 0 texture pitch (pitch<<16)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_NPOT_SIZE(0),(pb_back_buffer_width()<<16)|pb_back_buffer_height()); p+=2; //set stage 0 texture width & height ((witdh<<16)|height)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_WRAP(0),0x00030303); p+=2;//set stage 0 texture modes (0x0W0V0U wrapping: 1=wrap 2=mirror 3=clamp 4=border 5=clamp to edge)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(0),0x4003ffc0); p+=2; //set stage 0 texture enable flags
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(0),0x04074000); p+=2; //set stage 0 texture filters (AA!)
    pb_end(p);

    //Note that 0x02022000 (for TX_FILTER) will disable anti-aliasing.
    //For 2D games, use this non-AA value for 2D direct texture fast copy using same technic
    //or copy with CPU using an assembler routine optimized with MMX insructions (very efficient)

    //EncodedFormat=(log2depth<<28)|    //log2 sizes used if sizes are power of 2 (non swizzled)
    //      (log2height<<24)|
    //      (log2width<<20)|
    //      (MipMapLevels<<16)|
    //      (Format<<8)|
    //      ((2+VolumeFlag)<<4)|    //0x20=2D surface, 0x30=3D volume
    //      ((2+CubeMapFlag)<<2)|   //meaning of bit 3 is "use border color"
    //      (1+Bit0Flag);       //meaning of bit 0 & 1 is unknown

    //EncodedFilters=   (((DWORD)bias)&0x1FFF)|     //bits 12-0
    //          (MagFilter<<24)|        //bits 26-24 4=quincunx
    //          MipMinFilter|           //bits 18-16 7\_gaussian cubic (1 pixel=mean value of 3x3 pixel grid around it)
    //          FiltersType|            //bits 14-13 4/
    //          ColorsSigns;            //bits 31-28
    //for lesser filters: if minfilter=point (mipfilter: 1=none 3=point 5=linear) else (mipfilter: 2=none 4=point 6=linear)
    
    //neutralize other texture stages:
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(1),0x0003ffc0); p+=2;//set stage 1 texture enable flags (bit30 disabled)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(2),0x0003ffc0); p+=2;//set stage 2 texture enable flags (bit30 disabled)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(3),0x0003ffc0); p+=2;//set stage 3 texture enable flags (bit30 disabled)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_WRAP(1),0x00030303); p+=2;//set stage 1 texture modes (0x0W0V0U wrapping: 1=wrap 2=mirror 3=clamp 4=border 5=clamp to edge)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_WRAP(2),0x00030303); p+=2;//set stage 2 texture modes (0x0W0V0U wrapping: 1=wrap 2=mirror 3=clamp 4=border 5=clamp to edge)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_WRAP(3),0x00030303); p+=2;//set stage 3 texture modes (0x0W0V0U wrapping: 1=wrap 2=mirror 3=clamp 4=border 5=clamp to edge)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(1),0x02022000); p+=2;//set stage 1 texture filters (no AA, stage not even used)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(2),0x02022000); p+=2;//set stage 2 texture filters (no AA, stage not even used)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(3),0x02022000); p+=2;//set stage 3 texture filters (no AA, stage not even used)
    pb_end(p);
    
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_LIGHTING_ENABLE,0); p+=2; //(lighting related)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_SEPARATE_SPECULAR_ENABLE,0); p+=2; //(lighting related)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_LIGHT_CONTROL,0x00020001); p+=2; //(specular flags=0x00020000)
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_LIGHT_MODEL_TWO_SIDE_ENABLE,0); p+=2; //TwoSidedLighting=FALSE
    pb_end(p);

    //'final combiner' is an additional invisible (free) stage doing this:
    //final pixel.rgb = A * B + (1 - A) * C + D
    //final pixel.alpha = G.b or G.a (.a modifier must be used if you want .a)
    //Also all values are clamped to 0..1 (negative values become zero)

    //Inner registers NV20_TCL_PRIMITIVE_3D_RC_FINAL0 and following one
    //define inputs and modifiers for the 7 parameters A,B,C,D and E,F,G,? (?=0x80, unknown)
    //Here are a few useful values depending what you want to do:
    //fog on  & specular on  : 0x130e0300,0x00001c80 (means pixel.rgb=fog.a * (r0.rgb + v1.rgb) + (1 - fog.a) * fog.rgb & pixel.a=r0.a)
    //fog on  & specular off : 0x130c0300,0x00001c80 (means pixel.rgb=fog.a * r0.rgb + (1 - fog.a) * fog.rgb & pixel.a=r0.a)
    //fog off & specular on  : 0x0000000e,0x00001c80 (means pixel.rgb=r0.rgb + v1.rgb & pixel.a=r0.a)
    //fog off & specular off : 0x0000000c,0x00001c80 (means D=r0.rgb & G=r0.a, so final pixel.rgb=r0.rgb & pixel.a=r0.a)
        
    //These special read-only registers are also available at final combiner stage (maybe also at any stage?):
    //zero    = 0       (0x0 is the numeric code for this register, modifier is bits 7-4, mapped to C4)
    //fog     = fog     (0x3, fog.rgb returns the fog color inner register value, mapped to pseudocode C5 -fog.a is fog transparency, coming from fog table, I guess-)
    //v1r0sum = r0 + v1 (0xe, I've mapped it to pseudocode C6 in pcode2mcode, useful when specular v1 is to be used)
    //EFprod  = E * F   (0xf, I've mapped it to pseudocode C7 in pcode2mcode, useful for pixel shader optimization, i.e reduce number of stages)

    //Codes for normal registers:
    //C0 => 0x1
    //C1 => 0x2
    //v0 => 0x4
    //v1 => 0x5
    //t0 => 0x8
    //t1 => 0x9
    //t2 => 0xa
    //t3 => 0xb
    //r0 => 0xc
    //r1 => 0xd

    //Modifiers (Or it to code above):
    //default 0x00=|0.rgb| 0x10=x.a
    //0x20=1-|x| 0x40=2*max(0,x)-1("_bx2") 0x60=1-2*max(0,x) 0x80=max(0,x)-0.5f("_bias") 0xa0=0.5f-max(0,x) 0xc0=x 0xf0=-x

    //fog (untested)
    //p=pb_begin();
    //pb_push1(p,NV20_TCL_PRIMITIVE_3D_FOG_ENABLE,1); p+=2; //FogEnable=TRUE
    //pb_push1(p,NV20_TCL_PRIMITIVE_3D_FOG_COORD_DIST,0); p+=2; //FogRange (2=enable, 1=disable)
    //pb_push1(p,NV20_TCL_PRIMITIVE_3D_FOG_MODE,0x2601); p+=2; //FogTable (0x2601=linear 0x800=exp 0x801=exp2)
    //pb_push3(p,NV20_TCL_PRIMITIVE_3D_FOG_EQUATION_CONSTANT,1+e*s,-s,0); p+=4; //FogTableParameters max(scale)=8192.0f scale=1/(end-beginning)
    //pb_push2(p,NV20_TCL_PRIMITIVE_3D_RC_FINAL0,0x130C0300,0x00001c80); p+=3;//PSFinalCombinerIn=ABCD,EFG
    //[NV20_TCL_PRIMITIVE_3D_RC_FINAL0]=0x130E0300 if specular is activated
    //pb_end(p);
    
    //No fog
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_FOG_ENABLE,0); p+=2; //FogEnable=FALSE
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_FOG_COORD_DIST,0); p+=2; //FogRange=disable fog
    pb_push2(p,NV20_TCL_PRIMITIVE_3D_RC_FINAL0,0x0000000c,0x00001c80); p+=3;//PSFinalCombinerIn=abcd,efg
    //[NV20_TCL_PRIMITIVE_3D_RC_FINAL0]=0x0000000e if specular is activated
    pb_end(p);

    //Vertex and pixel shaders initialization:
    //Try to update pixel shaders manually if you change it 
    //often: it's much faster than using function pb_push_mcode()
    //because most of ps registers won't change over time.
    //Also, for pixel shaders that need more than 1 stage
    //pb_pcode2mcode won't help you. You have to set it up manually.
    //(not sure it's even worthy to improve pb_pcode2mcode for ps)
    //On the other hand pb_pcode2mcode is complete for vertex shaders.

    //To get an idea about what constants you need
    //and which vertex shader registers you should target
    //take a look at intermediate assembler file vs.psh

    //set vertex shader constants
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_ID,96); p+=2; //set cursor in order to load data into C0 quaternion (and following ones) (0=C-96)
    pb_push4f(p,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,1.0f,1.0f,1.0f,1.0f); p+=5; //loads constant C0=(Xscale,YScale,Zscale,Wscale) into GPU
    pb_push4f(p,NV20_TCL_PRIMITIVE_3D_VP_UPLOAD_CONST_X,0,0,0,0); p+=5; //loads constant C1=(Xbias,Ybias,Zbias,Wbias) into GPU
    pb_end(p);

    //sets vertex feed configuration
    
    //declares that 3 floats will go into v0 (3D position) (stride for optional batch processing : +8*4)
    //declares that 3 floats will go into v3 (3D normal) (stride for optional batch processing : +8*4)
    //declares that 2 floats will go into v7 (2D texture coordinates) (stride for optional batch processing : +8*4)
    //(check your intermediate assembler file .vsh to see what ATTR:n is expected)
    //stride 0x20=4*8=4*(3+3+2) is the offset that allows batch processing to find next value in a data stream
    // p=pb_begin();
    // pb_push1(p,NV20_TCL_PRIMITIVE_3D_VERTEX_ATTR(0),0x2032); p+=2;
    // pb_push1(p,NV20_TCL_PRIMITIVE_3D_VERTEX_ATTR(3),0x2032); p+=2;
    // pb_push1(p,NV20_TCL_PRIMITIVE_3D_VERTEX_ATTR(7),0x2022); p+=2;
    // pb_end(p);

    uint32_t tokens[] = {
    #include "vs.inl"
    };
    set_vertex_program(tokens, sizeof(tokens)/8);


    p=pb_begin();
    #include "ps.inl"
    pb_end(p);
}


//pixels always drawn (bit 2=0) in texture operations
void set_transparency_off(void)
{
    DWORD       *p;

//main interest of alphakill method is that it's a texture stage dependent method
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(0),0x4003ffc0); p+=2; //set stage 0 texture enable flags
    pb_end(p);


//alternate method : alphafunc, i.e set alphafunc='>=' and alpharef=1 (pixel drawn if alpha>=1)
/*  p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_ALPHA_FUNC_ENABLE,0); p+=2; //Disables Alpha test
    pb_end(p);
*/

//alternate method: blend source and destination colors according to alpha color component
//(allows 256 levels of transparency, 255=opaque)
/*  p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_ENABLE,0); p+=2; //Disables Blending
    pb_end(p);
*/
}

//pixels drawn only if alpha!=0 (bit 2=1) in texture operations
void set_transparency_on(void)
{
    DWORD       *p;
    
//main interest of alphakill method is that it's a texture stage dependent method
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_ENABLE(0),0x4003ffc4); p+=2; //set stage 0 texture enable flags
    pb_end(p);

//alternate method : alphafunc, i.e set alphafunc='>=' and alpharef=1 (pixel drawn if alpha>=1)
/*  p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_ALPHA_FUNC_ENABLE,1); p+=2; //Enables Alpha test
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_ALPHA_FUNC_REF,1); p+=2; //Alpha reference value for condition below
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_ALPHA_FUNC_FUNC,0x206); p+=2; //Pixel draw condition: 0x20n (0:Never 1:< 2:= 3:<= 4:> 5:!= 6:>= 7:Always)
    pb_end(p);
*/

//alternate method: blend source and destination colors according to alpha color component
//(allows 256 levels of transparency, 255=opaque)
/*  p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_ENABLE,1); p+=2; //Enables Blending
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_SRC,0x302); p+=2;//SrcAlpha
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_DST,0x303); p+=2;//1-SrcAlpha
    pb_end(p);
*/
}

//no anti-aliasing filter for texture operations
void set_filters_off(void)
{
    DWORD       *p;
    
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(0),0x02022000); p+=2; //set stage 0 texture filters (No AA!)
    pb_end(p);
}

//anti-aliasing filter (gaussian cubic) for texture operations
void set_filters_on(void)
{
    DWORD       *p;
    
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_TX_FILTER(0),0x04074000); p+=2; //set stage 0 texture filters (AA!)
    pb_end(p);
}



void set_source_extra_buffer(int n)
{
    DWORD       *p;
    
    p=pb_begin();
    pb_push2(p,NV20_TCL_PRIMITIVE_3D_TX_OFFSET(0),((DWORD)pb_extra_buffer(n))&0x03FFFFFF,0x0001122a); p+=3; //set stage 0 texture address & format
    pb_end(p);
}

void image_bitblt(void)
{
    int BytesPerPixel=4;
    DWORD   EncodedBpp=0xa;
    DWORD   SrcPitch,DstPitch;
    DWORD   SrcAddr,DstAddr;
    DWORD   sx,sy,dx,dy,h,w;
    DWORD   *p;
    
    SrcPitch=DstPitch=pb_back_buffer_pitch();
    SrcAddr=(DWORD)pb_extra_buffer(0);
    DstAddr=0; //dest_addr-base_dma_addr (base_dma_addr=last targetted buffer addr)
    
    sx=sy=dx=dy=0;
    w=pb_back_buffer_width();
    h=pb_back_buffer_height();
    
    switch(BytesPerPixel)
    {
        case 1:
            EncodedBpp=1;
            break;
        case 2:
            EncodedBpp=4;
            break;
        case 4:
            EncodedBpp=0xa;
            break;
    }

    p=pb_begin();
    pb_push4to( SUBCH_4,
            p,          
            NV10_CONTEXT_SURFACES_2D_FORMAT,
            EncodedBpp,
            (DstPitch<<16)|(SrcPitch&0xffff),
            SrcAddr,
            DstAddr );
    p+=5;
    pb_end(p);

    p=pb_begin();
    pb_push3to( SUBCH_3,
            p,          
            NV_IMAGE_BLIT_POINT_IN,
            (sx&0xffff)|(sy<<16),
            (dx&0xffff)|(dy<<16),
            (w&0xffff)|(h<<16)  );
    p+=4;
    pb_end(p);
}

void antialiased_texture_copy(void)
{
    float xScale  = 2.0f;
    float yScale  = 2.0f;
    float fLeft   = (float)0;
    float fTop    = (float)0;
    float fRight  = (float)2*pb_back_buffer_width(); //we draw 1 triangle that covers all screen (will be clipped)
    float fBottom = (float)2*pb_back_buffer_height();
    float uAdjust = 0.0f;
    float vAdjust = 0.0f;

    DWORD *p;
    
    int i;
    
    p=pb_begin();
    pb_push1(p,NV20_TCL_PRIMITIVE_3D_BEGIN_END,TRIANGLES); p+=2; //triangle list (beginning of list)
    pb_push(p++,0x40000000|NV20_TCL_PRIMITIVE_3D_VERTEX_DATA,12); //bit 30 means all params go to same register 0x1818
    // Vertex 0
    *((float *)(p++))=fLeft;
    *((float *)(p++))=fTop;
    *((float *)(p++))=uAdjust;
    *((float *)(p++))=vAdjust;
    // Vertex 1
    *((float *)(p++))=fRight;
    *((float *)(p++))=fTop;
    *((float *)(p++))=uAdjust + pb_back_buffer_width()*xScale;
    *((float *)(p++))=vAdjust;
    // Vertex 2
    *((float *)(p++))=fLeft;
    *((float *)(p++))=fBottom;
    *((float *)(p++))=uAdjust;
    *((float *)(p++))=vAdjust + pb_back_buffer_height()*yScale;
    pb_push(p++,NV20_TCL_PRIMITIVE_3D_BEGIN_END,1);
    *(p++)=STOP; //triggers the drawing (end of list)
    pb_end(p);
}




