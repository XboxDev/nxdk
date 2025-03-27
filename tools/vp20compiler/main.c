#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "nvvertparse.h"
#include "prog_instruction.h"
#include "mtypes.h"


#define VSH_TOKEN_SIZE 4

typedef enum {
    FLD_ILU = 0,
    FLD_MAC,
    FLD_CONST,
    FLD_V,
    // Input A
    FLD_A_NEG,
    FLD_A_SWZ_X,
    FLD_A_SWZ_Y,
    FLD_A_SWZ_Z,
    FLD_A_SWZ_W,
    FLD_A_R,
    FLD_A_MUX,
    // Input B
    FLD_B_NEG,
    FLD_B_SWZ_X,
    FLD_B_SWZ_Y,
    FLD_B_SWZ_Z,
    FLD_B_SWZ_W,
    FLD_B_R,
    FLD_B_MUX,
    // Input C
    FLD_C_NEG,
    FLD_C_SWZ_X,
    FLD_C_SWZ_Y,
    FLD_C_SWZ_Z,
    FLD_C_SWZ_W,
    FLD_C_R_HIGH,
    FLD_C_R_LOW,
    FLD_C_MUX,
    // Output
    FLD_OUT_MAC_MASK,
    FLD_OUT_R,
    FLD_OUT_ILU_MASK,
    FLD_OUT_O_MASK,
    FLD_OUT_ORB,
    FLD_OUT_ADDRESS,
    FLD_OUT_MUX,
    // Relative addressing
    FLD_A0X,
    // Final instruction
    FLD_FINAL,

    FLD_C_R, //hax
} VshFieldName;




typedef enum {
    PARAM_UNKNOWN = 0,
    PARAM_R,
    PARAM_V,
    PARAM_C
} VshParameterType;

typedef enum {
    OUTPUT_C = 0,
    OUTPUT_O
} VshOutputType;

typedef enum {
    OMUX_MAC = 0,
    OMUX_ILU
} VshOutputMux;

typedef enum {
    ILU_NOP = 0,
    ILU_MOV,
    ILU_RCP,
    ILU_RCC,
    ILU_RSQ,
    ILU_EXP,
    ILU_LOG,
    ILU_LIT
} VshILU;

typedef enum {
    MAC_NOP,
    MAC_MOV,
    MAC_MUL,
    MAC_ADD,
    MAC_MAD,
    MAC_DP3,
    MAC_DPH,
    MAC_DP4,
    MAC_DST,
    MAC_MIN,
    MAC_MAX,
    MAC_SLT,
    MAC_SGE,
    MAC_ARL
} VshMAC;

// typedef enum {
//     SWIZZLE_X = 0,
//     SWIZZLE_Y,
//     SWIZZLE_Z,
//     SWIZZLE_W
// } VshSwizzle;


typedef struct VshFieldMapping {
    VshFieldName field_name;
    uint8_t subtoken;
    uint8_t start_bit;
    uint8_t bit_length;
} VshFieldMapping;

static const VshFieldMapping field_mapping[] = {
    // Field Name         DWORD BitPos BitSize
    {  FLD_ILU,              1,   25,     3 },
    {  FLD_MAC,              1,   21,     4 },
    {  FLD_CONST,            1,   13,     8 },
    {  FLD_V,                1,    9,     4 },
    // INPUT A
    {  FLD_A_NEG,            1,    8,     1 },
    {  FLD_A_SWZ_X,          1,    6,     2 },
    {  FLD_A_SWZ_Y,          1,    4,     2 },
    {  FLD_A_SWZ_Z,          1,    2,     2 },
    {  FLD_A_SWZ_W,          1,    0,     2 },
    {  FLD_A_R,              2,   28,     4 },
    {  FLD_A_MUX,            2,   26,     2 },
    // INPUT B
    {  FLD_B_NEG,            2,   25,     1 },
    {  FLD_B_SWZ_X,          2,   23,     2 },
    {  FLD_B_SWZ_Y,          2,   21,     2 },
    {  FLD_B_SWZ_Z,          2,   19,     2 },
    {  FLD_B_SWZ_W,          2,   17,     2 },
    {  FLD_B_R,              2,   13,     4 },
    {  FLD_B_MUX,            2,   11,     2 },
    // INPUT C
    {  FLD_C_NEG,            2,   10,     1 },
    {  FLD_C_SWZ_X,          2,    8,     2 },
    {  FLD_C_SWZ_Y,          2,    6,     2 },
    {  FLD_C_SWZ_Z,          2,    4,     2 },
    {  FLD_C_SWZ_W,          2,    2,     2 },
    {  FLD_C_R_HIGH,         2,    0,     2 },
    {  FLD_C_R_LOW,          3,   30,     2 },
    {  FLD_C_MUX,            3,   28,     2 },
    // Output
    {  FLD_OUT_MAC_MASK,     3,   24,     4 },
    {  FLD_OUT_R,            3,   20,     4 },
    {  FLD_OUT_ILU_MASK,     3,   16,     4 },
    {  FLD_OUT_O_MASK,       3,   12,     4 },
    {  FLD_OUT_ORB,          3,   11,     1 },
    {  FLD_OUT_ADDRESS,      3,    3,     8 },
    {  FLD_OUT_MUX,          3,    2,     1 },
    // Other
    {  FLD_A0X,              3,    1,     1 },
    {  FLD_FINAL,            3,    0,     1 }
};

typedef enum {
    MASK_W = 1,   
    MASK_Z,   
    MASK_ZW,  
    MASK_Y,   
    MASK_YW,  
    MASK_YZ,  
    MASK_YZW, 
    MASK_X,   
    MASK_XW,  
    MASK_XZ,  
    MASK_XZW, 
    MASK_XY,  
    MASK_XYW, 
    MASK_XYZ, 
    MASK_XYZW,
} VshMask;


void vsh_set_field(uint32_t *shader_token, VshFieldName field_name, uint8_t v)
{
    if (field_name == FLD_C_R) {
        vsh_set_field(shader_token, FLD_C_R_LOW, v & 3);
        vsh_set_field(shader_token, FLD_C_R_HIGH, (v >> 2));
        return;
    }
    VshFieldMapping f = field_mapping[field_name];
    uint32_t f_bits = (1<<f.bit_length)-1;
    shader_token[f.subtoken] &= ~(f_bits << f.start_bit);
    shader_token[f.subtoken] |= (uint32_t)(v & f_bits) << f.start_bit;
}


static uint8_t vsh_mask(unsigned int write_mask) {
    switch (write_mask) {
    case WRITEMASK_X: return MASK_X;
    case WRITEMASK_Y: return MASK_Y;
    case WRITEMASK_XY: return MASK_XY;
    case WRITEMASK_Z: return MASK_Z;
    case WRITEMASK_XZ: return MASK_XZ;
    case WRITEMASK_YZ: return MASK_YZ;
    case WRITEMASK_XYZ: return MASK_XYZ;
    case WRITEMASK_W: return MASK_W;
    case WRITEMASK_XW: return MASK_XW;
    case WRITEMASK_YW: return MASK_YW;
    case WRITEMASK_XYW: return MASK_XYW;
    case WRITEMASK_ZW: return MASK_ZW;
    case WRITEMASK_XZW: return MASK_XZW;
    case WRITEMASK_YZW: return MASK_YZW;
    case WRITEMASK_XYZW: return MASK_XYZW;
    default:
        assert(false);
        return 0;
    }
}

void translate(const char* str)
{
    struct prog_instruction *instructions = NULL;
    unsigned int num_instructions = 0;

    int r = parse_nv_vertex_program(str, &instructions, &num_instructions);

//     int r = parse_nv_vertex_program(
// "!!VP1.1\n"
// "# NV_vertex_program generated by NVIDIA Cg compiler\n"
// "# cgc version 1.3.0001, build date Jan  7 2005 14:01:35\n"
// "# command line args: -profile vp20\n"
// "# source file: /Users/niel/wutv.cg\n"
// "# nv30vp backend compiling 'main' program\n"
// "#vendor NVIDIA Corporation\n"
// "#version 1.0.02\n"
// "#profile vp20\n"
// "#program main\n"
// "#semantic main.LocalScreen\n"
// "#semantic main.LocalLight\n"
// "#semantic main.LightDirection\n"
// "#semantic main.LightColor\n"
// "#semantic main.Scales\n"
// "#semantic main.Decals\n"
// "#var float4x4 LocalScreen :  : c[0], 4 : 1 : 1\n"
// "#var float4x4 LocalLight :  : c[4], 4 : 2 : 1\n"
// "#var float4 LightDirection :  : c[8] : 3 : 1\n"
// "#var float4 LightColor :  : c[9] : 4 : 1\n"
// "#var float4 Scales :  : c[10] : 5 : 1\n"
// "#var float4 Decals :  : c[11] : 6 : 1\n"
// "#var float4 I.texcoord : $vin.TEXCOORD : ATTR8 : 0 : 1\n"
// "#var float4 I.normalvec : $vin.NORMAL : ATTR2 : 0 : 1\n"
// "#var float4 I.position : $vin.POSITION : ATTR0 : 0 : 1\n"
// "#var float4 main.tex : $vout.TEXCOORD : TEX0 : -1 : 1\n"
// "#var float4 main.col : $vout.COLOR : COL0 : -1 : 1\n"
// "#var float4 main.pos : $vout.POSITION : HPOS : -1 : 1\n"
// "#const c[12] = 0 1 0 0\n"
// "    MOV o[TEX0], v[8];\n"
// "    MUL R0, v[2].y, c[5];\n"
// "    MAD R0, v[2].x, c[4], R0;\n"
// "    MAD R0, v[2].z, c[6], R0;\n"
// "    MAD R0, v[2].w, c[7], R0;\n"
// "    DP4 R0.x, R0, c[8];\n"
// "    MAX R0.x, R0.x, c[12].x;\n"
// "    MUL o[COL0], c[9], R0.x;\n"
// "    MUL R0, v[0].y, c[1];\n"
// "    MAD R0, v[0].x, c[0], R0;\n"
// "    MAD R0, v[0].z, c[2], R0;\n"
// "    ADD R1, R0, c[3];\n"
// "    RCP R0.x, R1.w;\n"
// "    MUL R1.xyz, R1.xyzx, R0.x;\n"
// "    MOV R0, c[10];\n"
// "    MAD o[HPOS], R1, R0, c[11];\n"
// "END\n"
// "# 16 instructions\n"
// "# 2 temp registers\n",
//     &instructions,
//     &num_instructions);

//     printf("num_instructions: %u\n", num_instructions);

    const char* cur = str;
    const char* end = &str[strlen(str) - 1];
    while(cur < end) {

        const char* lf = strchr(cur + 1, '\n');
        const char* cr = strchr(cur + 1, '\r');
        if (lf == NULL) { lf = end; }
        if (cr == NULL) { cr = end; }
        const char* line_end = (lf < cr ? lf : cr) + 1;

        if (*cur == '#') {
            printf("//%.*s\n", line_end - &cur[1] - 1, &cur[1]);
        }

        cur = line_end;

    }

    uint32_t vsh_buf[136*4];
    memset(vsh_buf, 0, sizeof(vsh_buf));

    uint32_t* vsh_ins = vsh_buf;

    int i;
    for (i=0; i<num_instructions; i++) {
        struct prog_instruction ins = instructions[i];

        if (ins.Opcode == OPCODE_END) {
            break;
        }

        // printf("%s\n", _mesa_opcode_string(ins.Opcode));

        vsh_set_field(vsh_ins, FLD_ILU, ILU_NOP);
        vsh_set_field(vsh_ins, FLD_MAC, MAC_NOP);
        vsh_set_field(vsh_ins, FLD_A_SWZ_X, SWIZZLE_X);
        vsh_set_field(vsh_ins, FLD_A_SWZ_Y, SWIZZLE_Y);
        vsh_set_field(vsh_ins, FLD_A_SWZ_Z, SWIZZLE_Z);
        vsh_set_field(vsh_ins, FLD_A_SWZ_W, SWIZZLE_W);
        vsh_set_field(vsh_ins, FLD_A_MUX, PARAM_V);
        vsh_set_field(vsh_ins, FLD_B_SWZ_X, SWIZZLE_X);
        vsh_set_field(vsh_ins, FLD_B_SWZ_Y, SWIZZLE_Y);
        vsh_set_field(vsh_ins, FLD_B_SWZ_Z, SWIZZLE_Z);
        vsh_set_field(vsh_ins, FLD_B_SWZ_W, SWIZZLE_W);
        vsh_set_field(vsh_ins, FLD_B_MUX, PARAM_V);
        vsh_set_field(vsh_ins, FLD_C_SWZ_X, SWIZZLE_X);
        vsh_set_field(vsh_ins, FLD_C_SWZ_Y, SWIZZLE_Y);
        vsh_set_field(vsh_ins, FLD_C_SWZ_Z, SWIZZLE_Z);
        vsh_set_field(vsh_ins, FLD_C_SWZ_W, SWIZZLE_W);
        vsh_set_field(vsh_ins, FLD_C_MUX, PARAM_V);
        vsh_set_field(vsh_ins, FLD_OUT_R, 7);
        vsh_set_field(vsh_ins, FLD_OUT_ADDRESS, 0xff);
        vsh_set_field(vsh_ins, FLD_OUT_MUX, OMUX_MAC);
        // vsh_set_field(vsh_ins, )


        bool ilu = false;
        bool mac = false;

        switch(ins.Opcode) {
        case OPCODE_MOV:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_MOV);
            mac = true;
            break;
        case OPCODE_ADD:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_ADD);
            mac = true;
            break;
        case OPCODE_SUB:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_ADD);
            vsh_set_field(vsh_ins, FLD_C_NEG, 1);
            assert(false); //TODO: xor negated args
            mac = true;
            break;
        case OPCODE_MAD:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_MAD);
            mac = true;
            break;
        case OPCODE_MUL:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_MUL);
            mac = true;
            break;
        case OPCODE_MAX:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_MAX);
            mac = true;
            break;
        case OPCODE_MIN:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_MIN);
            mac = true;
            break;
        case OPCODE_SGE:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_SGE);
            mac = true;
            break;
        case OPCODE_SLT:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_SLT);
            mac = true;
            break;
        case OPCODE_DP3:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_DP3);
            mac = true;
            break;
        case OPCODE_DP4:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_DP4);
            mac = true;
            break;
        case OPCODE_DPH:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_DPH);
            mac = true;
            break;
        case OPCODE_DST:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_DST);
            mac = true;
            break;

        case OPCODE_RCP:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_RCP);
            ilu = true;
            break;
        case OPCODE_RCC:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_RCC);
            ilu = true;
            break;
        case OPCODE_RSQ:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_RSQ);
            ilu = true;
            break;
        case OPCODE_EXP:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_EXP);
            ilu = true;
            break;
        case OPCODE_LOG:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_LOG);
            ilu = true;
            break;
        case OPCODE_LIT:
            vsh_set_field(vsh_ins, FLD_ILU, ILU_LIT);
            ilu = true;
            break;
        case OPCODE_ARL:
            vsh_set_field(vsh_ins, FLD_MAC, MAC_ARL);
            mac = true;
            break;
        default:
            assert(false);
        }


        if (ins.DstReg.File != PROGRAM_UNDEFINED) {
            if (ins.DstReg.File == PROGRAM_TEMPORARY) {
                vsh_set_field(vsh_ins, FLD_OUT_R, ins.DstReg.Index);
                if (mac) {
                    vsh_set_field(vsh_ins, FLD_OUT_MAC_MASK, vsh_mask(ins.DstReg.WriteMask));
                } else if (ilu) {
                    vsh_set_field(vsh_ins, FLD_OUT_ILU_MASK, vsh_mask(ins.DstReg.WriteMask));
                }
            } else if (ins.DstReg.File == PROGRAM_OUTPUT) {
                vsh_set_field(vsh_ins, FLD_OUT_O_MASK, vsh_mask(ins.DstReg.WriteMask));
                if (mac) {
                    vsh_set_field(vsh_ins, FLD_OUT_MUX, OMUX_MAC);
                } else if (ilu) {
                    vsh_set_field(vsh_ins, FLD_OUT_MUX, OMUX_ILU);
                }
                int out_reg;
                const char* name = _mesa_nv_vertex_output_register_name(ins.DstReg.Index);
                for (out_reg = 0; _mesa_nv_vertex_hw_output_register_name(out_reg); out_reg++) {
                    if (strcmp(name, _mesa_nv_vertex_hw_output_register_name(out_reg)) == 0) {
                        break;
                    }
                }
                if (!_mesa_nv_vertex_hw_output_register_name(out_reg)) {
                    assert(false);
                }
                vsh_set_field(vsh_ins, FLD_OUT_ORB, OUTPUT_O);
                vsh_set_field(vsh_ins, FLD_OUT_ADDRESS, out_reg);
            } else if (ins.DstReg.File == PROGRAM_ENV_PARAM) {
                vsh_set_field(vsh_ins, FLD_OUT_O_MASK, vsh_mask(ins.DstReg.WriteMask));
                if (mac) {
                    vsh_set_field(vsh_ins, FLD_OUT_MUX, OMUX_MAC);
                } else if (ilu) {
                    vsh_set_field(vsh_ins, FLD_OUT_MUX, OMUX_ILU);
                }
                vsh_set_field(vsh_ins, FLD_OUT_ORB, OUTPUT_C);
                // TODO: the index needs ajustment?
                vsh_set_field(vsh_ins, FLD_OUT_ADDRESS, ins.DstReg.Index);
            } else if (ins.DstReg.File == PROGRAM_ADDRESS) {
                // No need to do anything, setting the MAC_ARL is all that's necessary
            } else {
                assert(false);
            }
        }

        VshFieldName mux_field[3] = {FLD_A_MUX, FLD_B_MUX, FLD_C_MUX};
        VshFieldName swizzle_field[3][4] = {
            {FLD_A_SWZ_X, FLD_A_SWZ_Y, FLD_A_SWZ_Z, FLD_A_SWZ_W},
            {FLD_B_SWZ_X, FLD_B_SWZ_Y, FLD_B_SWZ_Z, FLD_B_SWZ_W},
            {FLD_C_SWZ_X, FLD_C_SWZ_Y, FLD_C_SWZ_Z, FLD_C_SWZ_W},
        };
        VshFieldName reg_field[3] = {FLD_A_R, FLD_B_R, FLD_C_R};
        VshFieldName neg_field[3] = {FLD_A_NEG, FLD_B_NEG, FLD_C_NEG};

        if (ilu) {
            // ILU instructions only use input C. Swap src reg 0 and 2.
            assert(ins.SrcReg[1].File == PROGRAM_UNDEFINED);
            assert(ins.SrcReg[2].File == PROGRAM_UNDEFINED);
            struct prog_src_register unused_reg = ins.SrcReg[2];
            ins.SrcReg[2] = ins.SrcReg[0];
            ins.SrcReg[0] = unused_reg;
        }

        if (ins.Opcode == OPCODE_ADD || ins.Opcode == OPCODE_SUB) {
            // hax. ADD uses A and C. Swap src reg 1 and 2
            assert(ins.SrcReg[2].File == PROGRAM_UNDEFINED);
            struct prog_src_register unused_reg = ins.SrcReg[2];
            ins.SrcReg[2] = ins.SrcReg[1];
            ins.SrcReg[1] = unused_reg;
        }

        int j;
        for (j=0; j<3; j++) {
            struct prog_src_register reg = ins.SrcReg[j];
            if (reg.File != PROGRAM_UNDEFINED) {
                // printf(" - in %d\n", j);
                if (reg.RelAddr) {
                    vsh_set_field(vsh_ins, FLD_A0X, 1);
                }
                if (reg.File == PROGRAM_TEMPORARY) {
                    vsh_set_field(vsh_ins, mux_field[j], PARAM_R);
                    vsh_set_field(vsh_ins, reg_field[j], reg.Index);
                } else if (reg.File == PROGRAM_ENV_PARAM) {
                    vsh_set_field(vsh_ins, mux_field[j], PARAM_C);
                    // TODO: the index needs ajustment?
                    vsh_set_field(vsh_ins, FLD_CONST, reg.Index+96);
                } else if (reg.File == PROGRAM_INPUT) {
                    int in_reg;
                    const char* name = _mesa_nv_vertex_input_register_name(reg.Index);
                    for (in_reg = 0; _mesa_nv_vertex_hw_input_register_name(in_reg); in_reg++) {
                        if (strcmp(name, _mesa_nv_vertex_hw_input_register_name(in_reg)) == 0) {
                            break;
                        }
                    }
                    if (!_mesa_nv_vertex_hw_input_register_name(in_reg)) {
                        assert(false);
                    }
                    vsh_set_field(vsh_ins, mux_field[j], PARAM_V);
                    vsh_set_field(vsh_ins, FLD_V, in_reg);
                } else {
                    assert(false);
                }

                if (reg.Negate == NEGATE_XYZW) {
                    vsh_set_field(vsh_ins, neg_field[j], 1);
                }

                int k;
                for (k=0; k<4; k++) {
                    vsh_set_field(vsh_ins, swizzle_field[j][k], GET_SWZ(reg.Swizzle, k));
                }
            }
        }

        vsh_ins += 4;
    }

    if (num_instructions) {
        vsh_set_field(vsh_ins-4, FLD_FINAL, 1);
    }

    for (uint32_t* pvsh = vsh_buf; pvsh < vsh_ins; pvsh += 4) {
        printf("0x%08x, 0x%08x, 0x%08x, 0x%08x,\n", pvsh[0], pvsh[1], pvsh[2], pvsh[3]);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s vpfile\n", argv[0]);
        exit(1);
    }

    FILE* fh = fopen(argv[1], "r");
    if (!fh) {
        fprintf(stderr, "unable to open %s\n", argv[1]);
        exit(1);
    }

    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    rewind(fh);
    char* buffer = (char*)malloc(size+1);
    if (buffer == NULL) {
        fprintf(stderr, "failed to allocate buffer\n");
        exit(1);
    }
    memset(buffer, 0, size+1);

    fread(buffer, size, 1, fh);

    translate(buffer);

    free(buffer);
    fclose(fh);

    return 0;
}