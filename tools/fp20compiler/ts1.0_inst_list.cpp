#include "ts1.0_inst_list.h"
#include <stdlib.h>
#include <stdio.h>

#include "nvparse_errors.h"
#include "nvparse_externs.h"

#include <cassert>
#include <cstring>

const int instListInc = 4;

InstList::InstList()
{
    size = 0;
    max = instListInc;
    list = (InstPtr)malloc(sizeof(Inst) * max);
}

InstList::~InstList()
{
    free(list);
}

int InstList::Size()
{
    return size;
}

InstList& InstList::operator+=(InstPtr t)
{
    if (size == max) {
        /* Extend list size by instListInc amount */
        max += instListInc;
        list = (InstPtr)realloc(list, sizeof(Inst) * max);
    }
    list[size++] = *t;
    return *this;
}

static unsigned int FloatToRaw(float value)
{
    assert(sizeof(float) == 4);
    assert(sizeof(unsigned int) == 4);

    return *(unsigned int*)&value;
}

void InstList::Invoke()
{
    int i;

    printf("#pragma push_macro(\"MASK\")\n");
    printf("#undef MASK\n");
    printf("#define MASK(mask, val) (((val) << (__builtin_ffs(mask)-1)) & (mask))\n");
    printf("\n");

    assert(size > 1);
    printf("pb_push1(p, NV097_SET_SHADER_OTHER_STAGE_INPUT,\n    ");
    for (i=1; i<size; i++) {
        if (i != 1) printf("    | ");
        int previousTexture = 0;
        if (list[i].opcode.bits.dependent)
            previousTexture = (int)list[i].args[0];
        printf("MASK(NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE%d, %d)",
            i, previousTexture);
        if (i != size-1) printf("\n");
    }
    printf(");\n");
    printf("p += 2;\n");

    printf("pb_push1(p, NV097_SET_SHADER_STAGE_PROGRAM,\n    ");
    for (i=0; i<size; i++) {
        const char* op = NULL;
        switch(list[i].opcode.word) {
        case TSP_NOP:
            op = "PROGRAM_NONE";
            break;
        case TSP_TEXTURE_1D:
        case TSP_TEXTURE_2D:
            op = "2D_PROJECTIVE";
            break;
        case TSP_TEXTURE_3D:
            op = "3D_PROJECTIVE";
        case TSP_TEXTURE_CUBE_MAP:
            op = "CUBE_MAP";
            break;
        case TSP_CULL_FRAGMENT:
            op = "CLIP_PLANE";
            assert(false);
            break;
        case TSP_PASS_THROUGH:
            op = "PASS_THROUGH";
            break;
        case TSP_OFFSET_2D_SCALE:
            assert(i >= 1);
            op = "BUMPENVMAP_LUMINANCE";
            assert(false); /* Untested */
            break;
        case TSP_OFFSET_2D:
            assert(i >= 1);
            op = "BUMPENVMAP";
            break;
        case TSP_DEPENDENT_AR:
            assert(i >= 1);
            op = "DEPENDENT_AR";
            assert(false); /* Untested */
            break;
        case TSP_DEPENDENT_GB:
            assert(i >= 1);
            op = "DEPENDENT_GB";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_2D_1_OF_2:
            assert(i == 1 || i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_2D_2_OF_2:
            assert(i >= 2);
            op = "DOT_ST";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_DEPTH_REPLACE_1_OF_2:
            assert(i == 1 || i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_DEPTH_REPLACE_2_OF_2:
            assert(i >= 2);
            op = "DOT_ZW";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_3D_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_3D_2_OF_3:
            assert(i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_3D_3_OF_3:
            assert(i == 3);
            op = "DOT_STR_3D";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_CUBE_MAP_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_2_OF_3:
            assert(i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_3_OF_3:
            assert(i == 3);
            op = "DOT_STR_CUBE";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_EYE_FROM_QS_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_EYE_FROM_QS_2_OF_3:
            assert(i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_EYE_FROM_QS_3_OF_3:
            assert(i == 3);
            op = "DOT_REFLECT_SPECULAR";
            break;

        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_CONST_EYE_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_CONST_EYE_2_OF_3:
            assert(i == 2);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_CONST_EYE_3_OF_3:
            assert(i == 3);
            op = "DOT_REFLECT_SPECULAR_CONST";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_EYE_FROM_QS_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_EYE_FROM_QS_2_OF_3:
            assert(i == 2);
            op = "DOT_REFLECT_DIFFUSE";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_EYE_FROM_QS_3_OF_3:
            assert(i == 3);
            op = "DOT_REFLECT_SPECULAR";
            assert(false); /* Untested */
            break;

        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_CONST_EYE_1_OF_3:
            assert(i == 1);
            op = "DOT_PRODUCT";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_CONST_EYE_2_OF_3:
            assert(i == 2);
            op = "DOT_REFLECT_DIFFUSE";
            break;
        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_CONST_EYE_3_OF_3:
            assert(i == 3);
            op = "DOT_REFLECT_SPECULAR_CONST";
            assert(false); /* Untested */
            break;

        default:
            assert(false);
            break;
        }
        if (i != 0) printf("    | ");
        printf("MASK(NV097_SET_SHADER_STAGE_PROGRAM_STAGE%d, NV097_SET_SHADER_STAGE_PROGRAM_STAGE%d_%s)",
               i, i, op);
        if (i != size-1) printf("\n");
    }
    printf(");\n");
    printf("p += 2;\n");

    // Process texture stage mode arguments
    for (i=0; i<size; i++) {
        switch(list[i].opcode.word) {
        case TSP_CULL_FRAGMENT: {
            assert(false); /* Unimplemented */
            break;
        }
        case TSP_OFFSET_2D_SCALE: {
            float matrix[2*2] = {
                list[i].args[1], list[i].args[2],
                list[i].args[3], list[i].args[4]
            };
            float offset = list[i].args[5];
            float scale = list[i].args[6];

            printf("pb_push(p++, NV097_SET_TEXTURE_SET_BUMP_ENV_MAT + %d * 64, 6);\n", i);
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[0] */\n", FloatToRaw(matrix[0]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[1] */\n", FloatToRaw(matrix[1]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[2] */\n", FloatToRaw(matrix[2]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[3] */\n", FloatToRaw(matrix[3]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_SCALE */\n", FloatToRaw(scale));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_OFFSET */\n", FloatToRaw(offset));
            assert(false); /* Untested */
            break;
        }
        case TSP_OFFSET_2D: {
            float matrix[2*2] = {
                list[i].args[1], list[i].args[2],
                list[i].args[3], list[i].args[4]
            };

            printf("/* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT */\n");
            printf("pb_push(p++, NV097_SET_TEXTURE_SET_BUMP_ENV_MAT + %d * 64, 4);\n", i);
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[0] */\n", FloatToRaw(matrix[0]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[1] */\n", FloatToRaw(matrix[1]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[2] */\n", FloatToRaw(matrix[2]));
            printf("*p++ = 0x%08x; /* NV097_SET_TEXTURE_SET_BUMP_ENV_MAT m[3] */\n", FloatToRaw(matrix[3]));
            break;
        }
        case TSP_DOT_PRODUCT_REFLECT_CUBE_MAP_CONST_EYE_1_OF_3:
        case TSP_DOT_PRODUCT_CUBE_MAP_AND_REFLECT_CUBE_MAP_CONST_EYE_1_OF_3: {
            assert(i == 1);

            float eye_vector_x = list[i].args[1];
            float eye_vector_y = list[i].args[2];
            float eye_vector_z = list[i].args[3];
            float eye_vector_w = 1.0f;

            printf("pb_push(p++, NV097_SET_EYE_VECTOR, 4);\n");
            printf("*p++ = 0x%08x; /* NV097_SET_EYE_VECTOR x */\n", FloatToRaw(eye_vector_x));
            printf("*p++ = 0x%08x; /* NV097_SET_EYE_VECTOR y */\n", FloatToRaw(eye_vector_y));
            printf("*p++ = 0x%08x; /* NV097_SET_EYE_VECTOR z */\n", FloatToRaw(eye_vector_z));
            printf("*p++ = 0x%08x; /* NV097_SET_EYE_VECTOR w */\n", FloatToRaw(eye_vector_w));
            assert(false); /* Untested */
            break;
        }
        default:
            break;
        }
    }

    printf("\n");
    printf("#pragma pop_macro(\"MASK\")\n");
}

void InstList::Validate()
{
    int i;
    for (i = 0; i < size; i++) {
        if (i >= TSP_NUM_TEXTURE_UNITS) {
            errors.set("too many instructions", list[i].line_number);
            continue;
        }
        int stage = list[i].opcode.bits.stage;
        int instruction = list[i].opcode.bits.instruction;
        if (stage > 0) {
            if (i <= 0)
                errors.set("previous stage is missing", list[i].line_number);
            else if (list[i-1].opcode.bits.instruction != instruction ||
                     list[i-1].opcode.bits.stage != stage-1)
                errors.set("previous stage does not match", list[i].line_number);
        }
        if (list[i].opcode.bits.dependent) {
            int previousTexture = (int)list[i].args[0];
            if (previousTexture >= i)
                errors.set("texture reference not ready", list[i].line_number);
            else {
                if (previousTexture >= i - stage)
                    errors.set("texture references a previous stage of itself", list[i].line_number);
                if (list[previousTexture].opcode.bits.noOutput)
                    errors.set("no output on referenced texture", list[i].line_number);
            }
        }
        if (stage < stageCounts[instruction]-1) {
            if (i+1 >= size)
                errors.set("next stage is missing", list[i].line_number);
            else if (list[i+1].opcode.bits.instruction != instruction ||
                     list[i+1].opcode.bits.stage != stage+1)
                errors.set("next stage does not match", list[i].line_number);
        }
    }

    // Assign remaining undesignated texture units to nop
    for (; i < TSP_NUM_TEXTURE_UNITS; i++) {
        InstPtr nopInst = new Inst(TSP_NOP);
        *this += nopInst;
        delete nopInst;
    }
}

bool is_ts10(const char * s)
{
    return ! strncmp(s, "!!TS1.0", 7);
}

bool ts10_init_more()
{
    // static bool tsinit = false;
    // if (tsinit == false )
    // {
    //     if(! glh_init_extensions( "GL_NV_texture_shader " "GL_ARB_multitexture " ))
    //     {
    //         errors.set("unable to initialize GL_NV_texture_shader\n");
    //         return false;
    //     }
    //     else
    //     {
    //         tsinit = true;
    //     }
    // }
    errors.reset();
    line_number = 1;
    return true;
}

/*

      else if(!strncmp(instring, "!!TS1.0", 7))
    {
        if (tsinit == 0 )
        {
            if(! glh_init_extensions( "GL_NV_texture_shader " "GL_ARB_multitexture " ))
            {
                errors.set("unable to initialize GL_NV_texture_shader\n");
                free(instring);
                return;
            }
            else
            {
                tsinit = 1;
            }
        }
        errors.reset();
        line_number = 1;
        ts10_init(instring+7);
        ts10_parse();
    }
    

  */
