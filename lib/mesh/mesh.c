//3D Tutorial: http://www.expertrating.com/courseware/3DCourse/3D-Tutorial.asp
//.max=>.3DS : Select object, File->Export selected (in 3DSMax)
//Add new calls to parse_mesh with new objectnames in mesh.c
//or just gather all your objects in same .3DS file
//"/meshes" subdirectory must be copied in the binary directory
//(.max files are given just to allow texture changes)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <hal/fileio.h>
#include <xboxrt/debug.h>
#include <xboxkrnl/xboxkrnl.h> //for MmAllocateContiguousMemoryEx

#include "mesh.h"

#include "math3d.h"

#include <pbkit/pbkit.h>

#define READ16(p) ((((u16)(*((p)+1)))<<8)|(((u16)(*((p)+0)))<<0))
#define READ32(p) ((((u32)(*((p)+3)))<<24)|(((u32)(*((p)+2)))<<16)|(((u32)(*((p)+1)))<<8)|(((u32)(*((p)+0)))<<0))




// #define BIPLANE

//Retrievable ChunkIDs
#define MESH_VERTICES   0x4110
#define MESH_FACES  0x4120
#define MESH_MATERIAL   0x4130
#define MESH_MAPPING    0x4140
#define MESH_MAPNAME    0xA300


//converts u,v into format expected by GPU (using Jbit's magic constants)
#define TEXTURE_WIDTH   256.0f
#define TEXTURE_HEIGHT  256.0f
#define cv_u(u) ((1.0f*((TEXTURE_WIDTH-0.375f)*(*((float *)&u))+0.5f)))
#define cv_v(v) ((1.0f*((TEXTURE_HEIGHT-0.375f)*(*((float *)&v))+0.5f)))

//converts x,y,z with a scale factor
#define cv(x) ((*((float *)&x))/1.0f)

extern int verbose;

BYTE *polyship;
DWORD size_polyship=0;
BYTE *polysbmp;
DWORD size_polysbmp=0;
#ifdef BIPLANE
BYTE *biplane;
DWORD size_biplane=0;
BYTE *biplabmp;
DWORD size_biplabmp=0;
#endif

BYTE *load_file(char *path,DWORD *pSize)
{
    int     ret;
    int     file_handle;
    BYTE        *buffer;
    int     read;

    *pSize=0;
    //detect size
    ret=XCreateFile(&file_handle,path,
            GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL);
    if (ret!=0) return NULL;
    XGetFileSize(file_handle,(int *)pSize);
    buffer=malloc(*pSize);
    if (buffer==NULL) return NULL;
    ret=XReadFile(file_handle,buffer,*pSize,&read);
    if (ret==0) return NULL;
    if (read!=*pSize) return NULL;
    XCloseHandle(file_handle);

    return buffer;
}

int load_data(void)
{
    polyship=load_file("meshes/polyship.3ds",&size_polyship);
    if (polyship==NULL) return 0;
    polysbmp=load_file("meshes/polyship.bmp",&size_polysbmp);
    if (polysbmp==NULL) return 0;
#ifdef BIPLANE
    biplane=load_file("meshes/biplane.3ds",&size_biplane);
    if (biplane==NULL) return 0;
    biplabmp=load_file("meshes/biplane.bmp",&size_biplabmp);
    if (biplabmp==NULL) return 0;
#endif
    return 1;
}


int get_mesh_texture(char *objectname)
{
    int     i,j,r,g,b,lx,ly;
    DWORD       *p;
    DWORD       pitch;

    unsigned char *bmp_data;
    DWORD       bmp_size;

    if (strcmp(objectname,"SpaceFight")==0)
    {
        bmp_size=size_polysbmp;
        bmp_data=polysbmp;
    }
    else
#ifdef BIPLANE
    if (strcmp(objectname,"Fuselage")==0)
    {
        bmp_size=size_biplabmp;
        bmp_data=biplabmp;
    }
    else
#endif  
        return 0; //wrong object

    bmp_data+=54; //bypass bmp file header

    lx=256;
    ly=256;

    if (size_polysbmp!=54+3*lx*ly) return 0; //wrong format or size

    //bmp file has texture upside down,
    //but GPU expects texture upside down too
    
    //let's draw texture in top left corner of extra buffer #1
    p = pb_extra_buffer(1);
    pitch = pb_back_buffer_pitch()/4;
    for(i=0;i<ly;i++)
        for(j=0;j<lx;j++)
        {
            b=bmp_data[(i*lx+j)*3+0];
            g=bmp_data[(i*lx+j)*3+1];
            r=bmp_data[(i*lx+j)*3+2];
            p[i*pitch+j]=(r<<16)|(g<<8)|b;
        }

    return 1;
}



//find address of a specific chunk for a specific object name (null=first object)
unsigned char *parse_mesh(unsigned char *p,u32 size,char *objectname,u16 chunk)
{
    u32 i;
    int object_found=0;
    int map_type=0;
    
    unsigned char *pEnd;
    
    u16 ChunkID;
    u32 ChunkLen;

    u16 Number;

    ChunkID=READ16(p);
    
    if (ChunkID!=0x4d4d)
    {
        if (verbose)
            pb_print("Not a 3DS file\n");

        return NULL;
    }
    
    ChunkLen=READ32(p+2);
    pEnd=p+ChunkLen-6;

    while (p<pEnd)
    {
        if ((object_found)&&(READ16(p)==chunk)) return p+6;
        
        ChunkID=READ16(p); p+=2;
        ChunkLen=READ32(p); p+=4;

        switch (ChunkID)
        {
            case 0x4d4d: break; //main chunk (nothing + sub chunks)

            case 0x0002: //3DS version

                if (verbose)
                    pb_print("3DS-v%d\n",READ32(p));

                p+=4;
                break;

            case 0x3d3d: break; //3D editor chunk (nothing + sub chunks)

            case 0x3d3e: //mesh version

                if (verbose)
                    pb_print("Mesh-v%d\n",READ32(p));

                p+=4;
                break;

            case 0x0100: //one unit
                if (verbose)
                {
                    if (READ32(p)==0x3f800000)
                        pb_print("Unit=1.0f\n");
                    else
                        pb_print("Unit=%x\n",READ32(p));
                }
                p+=4;
                break;

            case 0x4000: //object block (object name + sub chunks)
                if (verbose)
                    pb_print("Object: '%s'\n",p);

                if (objectname==NULL) 
                    object_found=1;
                else
                {
                    if (stricmp(objectname,p)==0)
                        object_found=1;
                    else
                        object_found=0;
                }
                p+=strlen(p)+1;
                break;

            case 0x4100: break; //triangular mesh (nothing + sub chunks)

            case 0x4110: //vertices list (number + 3 floats per vertex + sub chunks)
                Number=READ16(p); p+=2;
                if (verbose)
                    pb_print("Number of vertices: %d\n",Number);

                for (i=0; i<Number; i++)
                {
                    p+=3*4;
                }
                break;

            case 0x4111: //vertices flags (number + 1 short per flag + sub chunks)
                Number=READ16(p); p+=2;
                //if (verbose)
                //  pb_print("Number of vertices flags: %d\n",Number);

                for (i=0; i<Number; i++)
                {
                    p+=2;
                }
                break;

            case 0x4120: //faces list (number + 4 shorts per face + sub chunks)
                Number=READ16(p); p+=2;              
                if (verbose)
                    pb_print("Number of polygons: %d\n",Number); 

                for (i=0; i<Number; i++)
                {
                    p+=4*2;
                }
                break;

            case 0x4130: //faces material (name + number + faces material list)
                if (verbose)
                    pb_print("Faces material : '%s'\n",p);

                p+=strlen(p)+1;
                Number=READ16(p); p+=2;              
                if (verbose)
                    pb_print("Number of faces material: %d\n",Number);

                for (i=0; i<Number; i++)
                {
                    p+=2;
                }
                break;

            case 0x4140: //mapping points list (number + 2 floats per mapping point)
                Number=READ16(p); p+=2;              
                if (verbose)
                    pb_print("Number of mapping points: %d\n",Number); 

                for (i=0; i<Number; i++)
                {
                    p+=2*4;
                }
                break;

            case 0x4150: //smooth list (1 dword per mapping point)
                p+=ChunkLen-6;
                break;

            case 0x4160: //coordinates (6 floats)
                p+=ChunkLen-6;
                break;

            case 0x4165: //color in editor (1 byte)
                p+=ChunkLen-6;
                break;

            case 0x4600: //light (3 floats x,y,z + sub chunks)
                p+=3*4;
                break;

            case 0x4610: //spotlight (5 floats target(x,y,z) + hotspot + falloff + sub chunks)
                p+=5*4;
                break;

            case 0x4627: //spotlight raytrace bias (nothing + sub chunks)
                break;

            case 0x4630: //spotlight shadowed (nothing + sub chunks)
                break;

            case 0x4641: //spotlight shadow map (10 bytes + sub chunks)
                p+=10;
                break;
                
            case 0x4656: //spotlight roll (1 float)
                p+=4;
                break;

            case 0x4658: //spotlight raytrace bias (1 float)
                p+=4;
                break;

            case 0x4659: //light range start (1 float)
                p+=4;
                break;

            case 0x465a: //light range end (1 float)
                p+=4;
                break;

            case 0x465b: //light multiplier (1 float)
                p+=4;
                break;

            case 0x4700: //camera (8 floats Pos(x,y,z) Target(x,y,z) Bank(degree) Lens + sub chunks)
                p+=32;
                break;

            case 0x4720: //camera ? (2 dwords)
                p+=8;
                break;

            case 0xafff: break; //Material editor chunk (nothing + sub chunks)

            case 0xa000: //Material block (material name + sub chunks)
                if (verbose)
                    pb_print("Material: '%s'\n",p);

                p+=strlen(p)+1;
                break;

            case 0xa010: //ambient color (1 color sub chunk)
                break;

            case 0x0010: //color (3 floats)
                p+=3*4;
                break;
            case 0x0011: //color (3 bytes)
                p+=3;
                break;
            case 0x0012: //gamma corrected color (3 bytes)
                p+=3;
                break;
            case 0x0013: //gamma corrected color (3 float)
                p+=3*4;
                break;
            
            case 0xa020: //diffuse color (1 color sub chunk)
                break;

            case 0xa030: //specular color (1 color sub chunk)
                break;

            case 0xa040: //shininess percent (1 percent sub-chunk)
                break;

            case 0x0030: //percent (1 short)
                p+=2;
                break;
            case 0x0031: //percent (1 float)
                p+=4;
                break;

            case 0xa041: //Shininess strength percent (1 percent sub-chunk)
                break;

            case 0xa050: //transparency percent (1 percent sub-chunk)
                break;

            case 0xa052: //transparency fallout percent (1 percent sub-chunk)
                break;

            case 0xa053: //Reflection blur percent (1 percent sub-chunk)
                break;

            case 0xa084: //Selfilum (1 percent sub-chunk)
                break;
                
            case 0xa087: //Wire thickness (1 float)
                p+=4;
                break;

            case 0xa08a: //Falloff in (nothing)
                break;

            case 0xa100: //Render type (1 word)
                p+=2;
                break;

            case 0xa200: //Texture map 1 (nothing + sub chunks)
            case 0xa204: //Specular map filename (nothing + sub chunks)
            case 0xa210: //Opacity map filename (nothing + sub chunks)
            case 0xa220: //Reflection map filename (nothing + sub chunks)
            case 0xa230: //Bump map filename (nothing + sub chunks)
            case 0xa33a: //Texture map 2 (nothing + sub chunks)
            case 0xa33c: //Shininess map (nothing + sub chunks)
            case 0xa33d: //Selfilum map (nothing + sub chunks)
            case 0xa33e: //Mask for texture map 1 (nothing + sub chunks)
            case 0xa340: //Mask for texture map 2 (nothing + sub chunks)
            case 0xa342: //Mask for opacity map filename (nothing + sub chunks)
            case 0xa344: //Mask for bump map filename (nothing + sub chunks)
            case 0xa346: //Mask for shininess map (nothing + sub chunks)
            case 0xa348: //Mask for specular map filename (nothing + sub chunks)
            case 0xa34a: //Mask for selfilum map (nothing + sub chunks)
            case 0xa34c: //Mask for reflection map filename (nothing + sub chunks)
                map_type=ChunkID;
                break;
    
            case 0xa300: //filename (filename + sub chunks)
                if (verbose)
                    pb_print("%04x-map filename: '%s'\n",map_type,p);

                p+=strlen(p)+1;
                break;

            case 0xa351: //Mapping parameters (1 word, usually 0x0000)
                p+=2;
                break;

            case 0xa353: //Blur percent (1 float)
                p+=4;
                break;
                
            case 0xb000: //Key framer (nothing + sub chunks)
                break;

            case 0xb001: break; //Ambient light information block (nothing + sub chunks)
            case 0xb002: break; //Mesh information block (nothing + sub chunks)
            case 0xb003: break; //Camera information block (nothing + sub chunks)
            case 0xb004: break; //Camera target information block (nothing + sub chunks)
            case 0xb005: break; //Omni light information block (nothing + sub chunks)
            case 0xb006: break; //Spotlight target information block (nothing + sub chunks)
            case 0xb007: break; //Spotlight information block (nothing + sub chunks)

            case 0xb008: //Frames start and end (2 dwords)
                p+=2*4;
                break;

            case 0xb009: //? (1 dword)
                p+=4;
                break;

            case 0xb00a: //Scene ID (word + scene name + dword)
                p+=2;
                //if (verbose)
                //  pb_print("Scene name: '%s'\n",p);

                p+=strlen(p)+1;
                p+=4;
                break;

            case 0xb010: //object name (+3 words 2 flags and 1 parent ptr)
                //if (verbose)
                //  pb_print("Key frame Object name: '%s'\n",p);

                p+=strlen(p)+1;
                p+=3*2;
                break;

            case 0xb013: //pivot point (3 floats x,y,z)
                p+=3*4;
                break;

            case 0xb020: //track position (3 floats x,y,z + 5 floats ?)
                p+=8*4;
                break;

            case 0xb021: //track rotation (1 float angle + 3 floats axis + 5 floats ?)
                p+=9*4;
                break;

            case 0xb022: //track scale (3 floats x,y,z + 5 floats ?)
                p+=8*4;
                break;

            case 0xb023: //track field of view (1 float angle + 5 floats ?)
                p+=6*4;
                break;

            case 0xb024: //track roll (1 float angle + 5 floats ?)
                p+=6*4;
                break;

            case 0xb025: //track color (3 floats r,g,b + 5 floats ?)
                p+=8*4;
                break;
            
            case 0xb027: //track hotspot (1 float angle + 5 floats ?)
                p+=6*4;
                break;

            case 0xb028: //track falloff (1 float angle + 5 floats ?)
                p+=6*4;
                break;

            case 0xb030: //hierarchy position ID (1 word)
                p+=2;
                break;

            default:
                if (verbose)
                    pb_print("[%x?,%d bytes]\n",ChunkID,ChunkLen);
                p+=ChunkLen-6;
                return NULL; //better stop exploring
                break;
        }
    }

    return NULL; //found nothing
}


u32 *insert_batch_header(u32 *p,u32 *pTotal, u32 batch_size)
{
    u32 this_batch_size;

    if (*pTotal>batch_size) this_batch_size=batch_size; else this_batch_size=*pTotal;
    *pTotal-=this_batch_size;

    return p;
}


float *insert_normal(float *p,float *vertices,u32 index1,u32 index2,u32 index3)
{
    VECTOR vA,vB;

    vA[_X]=vertices[index1*3+_X]-vertices[index2*3+_X];
    vA[_Y]=vertices[index1*3+_Y]-vertices[index2*3+_Y];
    vA[_Z]=vertices[index1*3+_Z]-vertices[index2*3+_Z];

    vB[_X]=vertices[index3*3+_X]-vertices[index2*3+_X];
    vB[_Y]=vertices[index3*3+_Y]-vertices[index2*3+_Y];
    vB[_Z]=vertices[index3*3+_Z]-vertices[index2*3+_Z];
    
    vector_outerproduct(vA,vA,vB);

    vector_normalize(vA,vA);

    *(p++)=vA[_X];
    *(p++)=vA[_Y];
    *(p++)=vA[_Z];
    return p;
}

//returns a dma buffer immediately uploadable
//(slice it every batch_size*3 qwords)
u32 get_mesh_batches(char *objectname,u32 **pBuffer,u32 batch_size)
{
    u32     i;
    unsigned char   *p;
    u32     n,index1,index2,index3,flag,tmp;
    u32     nbatches;
    u32     *vertices;
    u32     *mapping_points;
    u32     *ptr;
    u32     num_vertices;
    u32     total;

    if (pBuffer==NULL) return 0;
    if (batch_size==0) return 0;

    p=NULL;
    
    if (p==NULL) p=parse_mesh(polyship,size_polyship,objectname,MESH_FACES);
#ifdef BIPLANE
    if (p==NULL) p=parse_mesh(biplane,size_biplane,objectname,MESH_FACES);
#endif
    
    if (p)
    {
        n=READ16(p); p+=2; //number of faces (3 vertices par face)
        nbatches=(n*3+batch_size-1)/batch_size;

        //we need physical contiguous memory for the vertex batch processing
        *pBuffer=MmAllocateContiguousMemoryEx(n*3*(3+3+2)*4,0,0x3ffb000,0,0x404);

        if (*pBuffer==NULL) return 0;

        vertices=NULL;
        num_vertices=get_mesh_vertices(objectname,(float **)&vertices);
        if ((num_vertices==0)||(vertices==NULL))
        {
            free(*pBuffer); *pBuffer=NULL;
            return 0;
        }
        
        mapping_points=NULL;
        if (num_vertices!=get_mesh_mapping_points(objectname,(float **)&mapping_points))
        {
            if (mapping_points) free(mapping_points);
            free(vertices);
            free(*pBuffer); *pBuffer=NULL;
            return 0;
        }
        if (mapping_points==NULL)
        {
            free(vertices);
            free(*pBuffer); *pBuffer=NULL;
            return 0;
        }

        ptr=(u32 *)(0x00000000|((u32)(*pBuffer)));
        total=n*3;
        for(i=0;i<n;i++)
        {
            index1=READ16(p); p+=2;
            index2=READ16(p); p+=2;
            index3=READ16(p); p+=2;
            flag=READ16(p); p+=2;
            if (((i*3+0)%batch_size)==0) ptr=insert_batch_header(ptr,&total,batch_size);
            *((float *)(ptr++))=cv(vertices[index1*3+0]);
            *((float *)(ptr++))=cv(vertices[index1*3+1]);
            *((float *)(ptr++))=cv(vertices[index1*3+2]);
            ptr=(u32 *)insert_normal((float *)ptr,(float *)vertices,index1,index2,index3);
            *((float *)(ptr++))=cv_u(mapping_points[index1*2+0]);
            *((float *)(ptr++))=cv_v(mapping_points[index1*2+1]);
            if (((i*3+1)%batch_size)==0) ptr=insert_batch_header(ptr,&total,batch_size);
            *((float *)(ptr++))=cv(vertices[index2*3+0]);
            *((float *)(ptr++))=cv(vertices[index2*3+1]);
            *((float *)(ptr++))=cv(vertices[index2*3+2]);
            ptr=(u32 *)insert_normal((float *)ptr,(float *)vertices,index1,index2,index3);
            *((float *)(ptr++))=cv_u(mapping_points[index2*2+0]);
            *((float *)(ptr++))=cv_v(mapping_points[index2*2+1]);
            if (((i*3+2)%batch_size)==0) ptr=insert_batch_header(ptr,&total,batch_size);
            *((float *)(ptr++))=cv(vertices[index3*3+0]);
            *((float *)(ptr++))=cv(vertices[index3*3+1]);
            *((float *)(ptr++))=cv(vertices[index3*3+2]);
            ptr=(u32 *)insert_normal((float *)ptr,(float *)vertices,index1,index2,index3);
            *((float *)(ptr++))=cv_u(mapping_points[index3*2+0]);
            *((float *)(ptr++))=cv_v(mapping_points[index3*2+1]);
        }
        free(mapping_points);
        free(vertices);
        return n*3;
    }
    return 0;
}


u32 get_mesh_faces_vertices(char *objectname,float **pBuffer)
{
    u32     i;
    unsigned char   *p;
    u32     n,index1,index2,index3,flag;
    float       *vertices;
    float       *ptr;
    u32     num_vertices;

    if (pBuffer==NULL) return 0;

    vertices=NULL;
    num_vertices=get_mesh_vertices(objectname,&vertices);
    if (num_vertices==0) return 0;
    if (vertices==NULL) return 0;
    
    p=NULL;
    
    if (p==NULL) p=parse_mesh(polyship,size_polyship,objectname,MESH_FACES);
#ifdef BIPLANE
    if (p==NULL) p=parse_mesh(biplane,size_biplane,objectname,MESH_FACES);
#endif
    
    if (p)
    {
        n=READ16(p); p+=2;
        *pBuffer=malloc(n*3*3*sizeof(float)); //4 bytes aligned by default
        if (*pBuffer==NULL) return 0;
        ptr=*pBuffer;
        for(i=0;i<n;i++)
        {
            index1=READ16(p); p+=2;
            *(ptr++)=vertices[index1*3+0];
            *(ptr++)=vertices[index1*3+1];
            *(ptr++)=vertices[index1*3+2];
            index2=READ16(p); p+=2;
            *(ptr++)=vertices[index2*3+0];
            *(ptr++)=vertices[index2*3+1];
            *(ptr++)=vertices[index2*3+2];
            index3=READ16(p); p+=2;
            *(ptr++)=vertices[index3*3+0];
            *(ptr++)=vertices[index3*3+1];
            *(ptr++)=vertices[index3*3+2];
            flag=READ16(p); p+=2;
        }
        free(vertices);
        return n*3;
    }
    free(vertices);
    return 0;
}



u32 get_mesh_vertices(char *objectname,float **pBuffer)
{
    unsigned char   *p;
    u32     n;
    
    p=NULL;
    
    if (p==NULL) p=parse_mesh(polyship,size_polyship,objectname,MESH_VERTICES);
#ifdef BIPLANE
    if (p==NULL) p=parse_mesh(biplane,size_biplane,objectname,MESH_VERTICES);
#endif

    if (pBuffer==NULL) return 0;
    
    if (p)
    {
        n=READ16(p); p+=2;
        *pBuffer=malloc(n*3*sizeof(float)); //4 bytes aligned by default
        if (*pBuffer==NULL) return 0;
        memcpy(*pBuffer,p,n*3*sizeof(float));
        return n;
    }

    return 0;
}

u32 get_mesh_mapping_points(char *objectname,float **pBuffer)
{
    unsigned char   *p;
    u32     n;
    
    p=NULL;
    
    if (p==NULL) p=parse_mesh(polyship,size_polyship,objectname,MESH_MAPPING);
#ifdef BIPLANE
    if (p==NULL) p=parse_mesh(biplane,size_biplane,objectname,MESH_MAPPING);
#endif
    if (pBuffer==NULL) return 0;
    
    if (p)
    {
        n=READ16(p); p+=2;
        *pBuffer=malloc(n*2*sizeof(float)); //4 bytes aligned by default
        if (*pBuffer==NULL) return 0;
        memcpy(*pBuffer,p,n*2*sizeof(float));
        return n;
    }

    return 0;
}

u32 get_mesh_faces(char *objectname,short **pBuffer)
{
    unsigned char   *p;
    u32     n;

    p=NULL;
    
    if (p==NULL) p=parse_mesh(polyship,size_polyship,objectname,MESH_FACES);
#ifdef BIPLANE
    if (p==NULL) p=parse_mesh(biplane,size_biplane,objectname,MESH_FACES);
#endif

    if (pBuffer==NULL) return 0;
    
    if (p)
    {
        n=READ16(p); p+=2;
        *pBuffer=malloc(n*4*sizeof(short)); //4 bytes aligned by default
        if (*pBuffer==NULL) return 0;
        memcpy(*pBuffer,p,n*4*sizeof(short));
        return n;
    }

    return 0;
}

