#ifndef __mesh_h__
#define __mesh_h__

typedef unsigned short u16;
typedef unsigned int u32;

int load_data(void);
int get_mesh_texture(char *objectname);
u32 get_mesh_batches(char *objectname,u32 **pBuffer,u32 batch_size); //8 dwords per batch + 12 floats per vertex (uv,normal,xyz)
u32 get_mesh_faces_vertices(char *objectname,float **pBuffer); //3 floats per vertex (x, y, z)
u32 get_mesh_vertices(char *objectname,float **pBuffer); //3 floats per vertex (x, y, z)
u32 get_mesh_mapping_points(char *objectname,float **pBuffer); //2 floats per vertex (u, v)
u32 get_mesh_faces(char *objectname,short **pBuffer); //4 shorts per face (3 indices and 1 flag)

#endif
