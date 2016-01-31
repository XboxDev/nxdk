#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#define USE_V_BATCHES //first speed optimization, enqueue a vertex batch order (like dma-tag 'ref' on ps2)

//#define USE_I_BATCHES //technic simulation, enqueue index batch order (saves a lots of memory, just simulated here) 

#ifdef USE_V_BATCHES
    //65535 is the upper limit of vertex batch technic (will enqueue 255 commands)
    // int         max_vertices_per_batch=65535; //keep it multiple of 3
#define MAX_VERTICES_PER_BATCH 65535
#else
    //Avoid too big pb_begin=>pb_end blocks (but theoretical limit of index batch method is 32M indices)
    // int         max_vertices_per_batch=252; //keep it multiple of 3
#define MAX_VERTICES_PER_BATCH 252
#endif


typedef struct TexturedVertex {
    float pos[3];
    float normal[3];
    float texcoord[2];
} __attribute__((packed)) TexturedVertex;


void draw_triangle_list_batches(DWORD *vertices, int num_vertices);
void draw_triangle_list(DWORD *vertices,int num_vertices);
void prepare_shaders(void);
void antialiased_texture_copy(void);
void image_bitblt(void);
void set_source_extra_buffer(int n);
void set_filters_on(void);
void set_filters_off(void);
void set_transparency_on(void);
void set_transparency_off(void);

void draw_arrays(unsigned int mode, int start, int count);
void clear_attribs(void);
void set_attrib_pointer(unsigned int index,
                        unsigned int format,
                        unsigned int size,
                        unsigned int stride,
                        const void* data);
void set_vertex_program(const uint32_t* tokens, size_t num_instructions);

#endif

