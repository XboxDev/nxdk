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

void InstList::Invoke()
{
    int i;

    printf("pb_push1(p, NV097_SET_SHADER_OTHER_STAGE_INPUT,\n"
           "    MASK(NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE1, 0)\n"
           "    | MASK(NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE2, 1)\n"
           "    | MASK(NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE3, 2));\n");

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
}

void InstList::Validate()
{
    if (size > TSP_NUM_TEXTURE_UNITS)
        errors.set("too many instructions");
    int i;
    for (i = 0; i < size; i++) {
        int stage = list[i].opcode.bits.stage;
        if (stage > i)
            errors.set("prior stage missing");
        if (list[i].opcode.bits.instruction != list[i - stage].opcode.bits.instruction)
            errors.set("stage mismatch");
        if (list[i].opcode.bits.dependent) {
            int previousTexture = (int)list[i].args[0];
            if (previousTexture >= i - stage)
                errors.set("invalid texture reference");
            if (list[previousTexture].opcode.bits.noOutput)
                errors.set("no output on referenced texture");
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
