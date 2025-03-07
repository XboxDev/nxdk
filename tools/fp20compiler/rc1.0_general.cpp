// #include <glh/glh_extensions.h>

#include "rc1.0_general.h"
#include "nvparse_errors.h"
#include "nvparse_externs.h"

#include <cstdio>
#include <cassert>

void GeneralCombinersStruct::Validate(int numConsts, ConstColorStruct *pcc)
{
    // GLint maxGCs;
    // glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &maxGCs);
    // if (num > maxGCs) {
    //     char buffer[256];
    //     sprintf(buffer, "%d general combiners specified, only %d supported", num, (int)maxGCs);
    //     errors.set(buffer);
    //     num = maxGCs;
    // }
    int maxGCs = 8;

    if (0 == num) {
        // Setup a "fake" general combiner 0
        general[0].ZeroOut();
        num = 1;
    }

    localConsts = 0;
    int i;
    for (i = 0; i < num; i++) {
        general[i].Validate(i);
        localConsts += general[i].numConsts;
    }

    if (localConsts > 0)
        // if (NULL == glCombinerStageParameterfvNV)
        //     errors.set("local constant(s) specified, but not supported -- ignored");
        // else
        for (i = 0; i < num; i++)
            general[i].SetUnusedLocalConsts(numConsts, pcc);


    for (; i < maxGCs; i++)
        general[i].ZeroOut();
}

void GeneralCombinersStruct::Invoke()
{
    // glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, num);

    int i;
    for (i = 0; i < num; i++)
        general[i].Invoke(i);
    
    // if (NULL != glCombinerStageParameterfvNV) {
    //     if (localConsts > 0)
    //         glEnable(GL_PER_STAGE_CONSTANTS_NV);
    //     else
    //         glDisable(GL_PER_STAGE_CONSTANTS_NV);
    // }
    // assert(false);

    printf("pb_push1(p, NV097_SET_COMBINER_CONTROL,");
    printf("\n    MASK(NV097_SET_COMBINER_CONTROL_FACTOR0, %s)",
        localConsts > 0 ? "NV097_SET_COMBINER_CONTROL_FACTOR0_EACH_STAGE"
                : "NV097_SET_COMBINER_CONTROL_FACTOR0_SAME_FACTOR_ALL");
    printf("\n    | MASK(NV097_SET_COMBINER_CONTROL_FACTOR1, %s)",
        localConsts > 0 ? "NV097_SET_COMBINER_CONTROL_FACTOR1_EACH_STAGE"
                : "NV097_SET_COMBINER_CONTROL_FACTOR1_SAME_FACTOR_ALL");
    printf("\n    | MASK(NV097_SET_COMBINER_CONTROL_ITERATION_COUNT, %d)", num);
    printf(");\n");
    printf("p += 2;\n");
}

void GeneralCombinerStruct::ZeroOut()
{
        numPortions = 2;
        numConsts = 0;

        portion[0].ZeroOut();
        portion[0].designator = RCP_RGB;
        portion[1].ZeroOut();
        portion[1].designator = RCP_ALPHA;
}


void GeneralCombinerStruct::SetUnusedLocalConsts(int numGlobalConsts, ConstColorStruct *globalCCs)
{
        int i;
    for (i = 0; i < numGlobalConsts; i++) {
        bool constUsed = false;
        int j;
        for (j = 0; j < numConsts; j++)
            constUsed |= (cc[j].reg.bits.name == globalCCs[i].reg.bits.name);
        if (!constUsed) {
            assert(numConsts < 2);
            cc[numConsts++] = globalCCs[i];
        }
    }
}


void GeneralCombinerStruct::Validate(int stage)
{
    if (2 == numConsts &&
        cc[0].reg.bits.name == cc[1].reg.bits.name) {
        errors.set("local constant set twice", cc[1].line_number);
        cc[0] = cc[1];
        numConsts = 1;
    }

    switch (numPortions)
    {
    case 0:
        portion[0].designator = RCP_RGB;
        // Fallthru
    case 1:
        portion[1].designator = ((RCP_RGB == portion[0].designator) ? RCP_ALPHA : RCP_RGB);
        // Fallthru
    case 2:
        if (portion[0].designator == portion[1].designator)
            errors.set("portion declared twice", portion[1].line_number);
        break;
    }
    int i;
    for (i = 0; i < numPortions; i++)
        portion[i].Validate(stage);

    for (; i < 2; i++)
        portion[i].ZeroOut();

}

void GeneralCombinerStruct::Invoke(int stage)
{
    int i;

    // if (NULL != glCombinerStageParameterfvNV)
    //     for (i = 0; i < numConsts; i++)
    //         glCombinerStageParameterfvNV(GL_COMBINER0_NV + stage, cc[i].reg.bits.name, &(cc[i].v[0]));
    assert(numConsts <= 2);
    for (i = 0; i < numConsts; i++) {
        const char* cmd = NULL;
        switch(cc[i].reg.bits.name) {
        case REG_CONSTANT_COLOR0:
            cmd = "NV097_SET_COMBINER_FACTOR0";
            break;
        case REG_CONSTANT_COLOR1:
            cmd = "NV097_SET_COMBINER_FACTOR1";
            break;
        default:
            assert(false);
            break;
        }

        assert(cc[i].v[0] >= 0.0f && cc[i].v[0] <= 1.0f);
        assert(cc[i].v[1] >= 0.0f && cc[i].v[1] <= 1.0f);
        assert(cc[i].v[2] >= 0.0f && cc[i].v[2] <= 1.0f);
        assert(cc[i].v[3] >= 0.0f && cc[i].v[3] <= 1.0f);

        printf("pb_push1(p, %s + %d * 4,", cmd, stage);
        printf("\n    MASK(0xFF000000, 0x%02X)", (unsigned char)(cc[i].v[3] * 0xFF));
        printf("\n    | MASK(0x00FF0000, 0x%02X)", (unsigned char)(cc[i].v[0] * 0xFF));
        printf("\n    | MASK(0x0000FF00, 0x%02X)", (unsigned char)(cc[i].v[1] * 0xFF));
        printf("\n    | MASK(0x000000FF, 0x%02X)", (unsigned char)(cc[i].v[2] * 0xFF));
        printf(");\n");
        printf("p += 2;\n");
    }

    for (i = 0; i < 2; i++)
        portion[i].Invoke(stage);
}

void GeneralPortionStruct::Validate(int stage)
{
    gf.Validate(stage, designator);
}

void GeneralPortionStruct::Invoke(int stage)
{
    gf.Invoke(stage, designator, bs);
}

void GeneralPortionStruct::ZeroOut()
{
    gf.ZeroOut();
    bs.word = RCP_SCALE_BY_ONE;
}

void GeneralFunctionStruct::ZeroOut()
{
    // Create mapped registers for zero and discard
    MappedRegisterStruct unsignedZero;
    RegisterEnum zero;
    zero.word = RCP_ZERO_ALPHA;
    unsignedZero.Init(zero);

    MappedRegisterStruct unsignedDiscard;
    RegisterEnum discard;
    discard.word = RCP_DISCARD;
    unsignedDiscard.Init(discard);

    numOps = 3;

    op[0].op = RCP_MUL;
    op[0].reg[0] = unsignedDiscard;
    op[0].reg[1] = unsignedZero;
    op[0].reg[2] = unsignedZero;

    op[1].op = RCP_MUL;
    op[1].reg[0] = unsignedDiscard;
    op[1].reg[1] = unsignedZero;
    op[1].reg[2] = unsignedZero;

    op[2].op = RCP_SUM;
    op[2].reg[0] = unsignedDiscard;

}

void GeneralFunctionStruct::Validate(int stage, int portion)
{
        int i;
    for (i = 0; i < numOps; i++)
        op[i].Validate(stage, portion);
    // Check if multiple ops are writing to same register (and it's not DISCARD)
    if (numOps > 1 &&
        op[0].reg[0].reg.bits.name == op[1].reg[0].reg.bits.name &&
        REG_DISCARD != op[1].reg[0].reg.bits.name)
        errors.set("writing to same register twice", op[1].reg[0].reg.line_number);
    if (numOps > 2 &&
        (op[0].reg[0].reg.bits.name == op[2].reg[0].reg.bits.name ||
         op[1].reg[0].reg.bits.name == op[2].reg[0].reg.bits.name) &&
        REG_DISCARD != op[2].reg[0].reg.bits.name)
        errors.set("writing to same register twice", op[2].reg[0].reg.line_number);

    // Set unused outputs to discard, unused inputs to zero/unsigned_identity
    if (numOps < 2) {
        // Set C input to zero
        op[1].reg[1].reg.bits.name = REG_ZERO;
        op[1].reg[1].map = MAP_UNSIGNED_IDENTITY;
        op[1].reg[1].reg.bits.channel = portion;

        // Set D input to zero
        op[1].reg[2].reg.bits.name = REG_ZERO;
        op[1].reg[2].map = MAP_UNSIGNED_IDENTITY;
        op[1].reg[2].reg.bits.channel = portion;

        // Discard CD output
        op[1].op = false;
        op[1].reg[0].reg.bits.name = REG_DISCARD;
    }

    if (numOps < 3) {
        // Discard muxSum output
        op[2].reg[0].reg.bits.name = REG_DISCARD;
        op[2].op = RCP_SUM;
    }
}

static void GenerateInput(int portion, char variable, MappedRegisterStruct reg) {
    const char* portion_s = portion == RCP_RGB ? "COLOR" : "ALPHA";
    printf("MASK(NV097_SET_COMBINER_%s_ICW_%c_SOURCE, %s)", portion_s, variable, GetRegisterNameString(reg.reg.bits.name));
    printf(" | MASK(NV097_SET_COMBINER_%s_ICW_%c_ALPHA, %d)", portion_s, variable,
            reg.reg.bits.channel == RCP_ALPHA);
    printf(" | MASK(NV097_SET_COMBINER_%s_ICW_%c_MAP, 0x%x)", portion_s, variable, reg.map);
}

void GeneralFunctionStruct::Invoke(int stage, int portion, BiasScaleEnum bs)
{
    // GLenum portionEnum = (RCP_RGB == portion) ? GL_RGB : GL_ALPHA;
    const char* portion_s = (portion == RCP_RGB ? "COLOR" : "ALPHA");

    printf("pb_push1(p, NV097_SET_COMBINER_%s_ICW + %d * 4,", portion_s, stage);
    printf("\n    ");
    GenerateInput(portion, 'A', op[0].reg[1]);
    printf("\n    | ");
    GenerateInput(portion, 'B', op[0].reg[2]);
    printf("\n    | ");
    GenerateInput(portion, 'C', op[1].reg[1]);
    printf("\n    | ");
    GenerateInput(portion, 'D', op[1].reg[2]);
    printf(");\n");
    printf("p += 2;\n");

    // glCombinerInputNV(GL_COMBINER0_NV + stage,
    //     portionEnum,
    //     GL_VARIABLE_A_NV,
    //     op[0].reg[1].reg.bits.name,
    //     op[0].reg[1].map,
    //     MAP_CHANNEL(op[0].reg[1].reg.bits.channel));

    // glCombinerInputNV(GL_COMBINER0_NV + stage,
    //     portionEnum,
    //     GL_VARIABLE_B_NV,
    //     op[0].reg[2].reg.bits.name,
    //     op[0].reg[2].map,
    //     MAP_CHANNEL(op[0].reg[2].reg.bits.channel));

    // glCombinerInputNV(GL_COMBINER0_NV + stage,
    //     portionEnum,
    //     GL_VARIABLE_C_NV,
    //     op[1].reg[1].reg.bits.name,
    //     op[1].reg[1].map,
    //     MAP_CHANNEL(op[1].reg[1].reg.bits.channel));

    // glCombinerInputNV(GL_COMBINER0_NV + stage,
    //     portionEnum,
    //     GL_VARIABLE_D_NV,
    //     op[1].reg[2].reg.bits.name,
    //     op[1].reg[2].map,
    //     MAP_CHANNEL(op[1].reg[2].reg.bits.channel));

    printf("pb_push1(p, NV097_SET_COMBINER_%s_OCW + %d * 4,\n", portion_s, stage);
    printf("    MASK(NV097_SET_COMBINER_%s_OCW_AB_DST, %s)\n", portion_s, GetRegisterNameString(op[0].reg[0].reg.bits.name));
    printf("    | MASK(NV097_SET_COMBINER_%s_OCW_CD_DST, %s)\n", portion_s, GetRegisterNameString(op[1].reg[0].reg.bits.name));
    printf("    | MASK(NV097_SET_COMBINER_%s_OCW_SUM_DST, %s)\n", portion_s, GetRegisterNameString(op[2].reg[0].reg.bits.name));
    printf("    | MASK(NV097_SET_COMBINER_%s_OCW_MUX_ENABLE, %d)\n", portion_s, (op[2].op == RCP_MUX));

    const char* scale_s = NULL;
    switch(bs.bits.scale) {
    case SCALE_NONE: scale_s = "NOSHIFT"; break;
    case SCALE_BY_TWO: scale_s = "SHIFTLEFTBY1"; break;
    case SCALE_BY_FOUR: scale_s = "SHIFTLEFTBY2"; break;
    case SCALE_BY_ONE_HALF: scale_s = "SHIFTRIGHTBY1"; break;
    default:
        assert(false);
        break;
    }

    if (portion == RCP_RGB) {
        printf("    | MASK(NV097_SET_COMBINER_%s_OCW_AB_DOT_ENABLE, %d)\n", portion_s, op[0].op);
        printf("    | MASK(NV097_SET_COMBINER_%s_OCW_CD_DOT_ENABLE, %d)\n", portion_s, op[1].op);
    }

    printf("    | MASK(NV097_SET_COMBINER_%s_OCW_OP, NV097_SET_COMBINER_%s_OCW_OP_%s%s)",
            portion_s, portion_s, scale_s,
            (bs.bits.bias == BIAS_BY_NEGATIVE_ONE_HALF) ? "_BIAS" : "");

    printf(");\n");
    printf("p += 2;\n");

    // glCombinerOutputNV(GL_COMBINER0_NV + stage,
    //     portionEnum,
    //     op[0].reg[0].reg.bits.name,
    //     op[1].reg[0].reg.bits.name,
    //     op[2].reg[0].reg.bits.name,
    //     bs.bits.scale,
    //     bs.bits.bias,
    //     op[0].op,
    //     op[1].op,
    //     (op[2].op == RCP_MUX) ? true : false);
    // assert(false);
}

// This helper function assigns a channel to an undesignated input register
static void ConvertRegister(RegisterEnum& reg, int portion)
{
    if (RCP_NONE == reg.bits.channel) {
        reg.bits.channel = portion;
        if (REG_FOG == reg.bits.name && RCP_ALPHA == portion)
            // Special case where fog alpha is final only, but RGB is not
            reg.bits.finalOnly = true;
    }
}


void OpStruct::Validate(int stage, int portion)
{
    int args = 1;

    if (RCP_DOT == op || RCP_MUL == op)
        args = 3;
    else
        args = 1;

    if (reg[0].reg.bits.readOnly)
        errors.set("writing to a read-only register", reg[0].reg.line_number);

    if (RCP_ALPHA == portion &&
        RCP_DOT == op)
        errors.set("dot can not be used in alpha portion", reg[0].reg.line_number);
    int i;
    for (i = 0; i < args; i++) {
        ConvertRegister(reg[i].reg, portion);
        if (reg[i].reg.bits.finalOnly)
            errors.set("final-combiner register can not be used in general-combiner", reg[i].reg.line_number);
        if (RCP_RGB == portion &&
            RCP_BLUE == reg[i].reg.bits.channel)
            errors.set("blue component can not be used in rgb portion", reg[i].reg.line_number);
        if (RCP_ALPHA == portion &&
            RCP_RGB == reg[i].reg.bits.channel)
            errors.set("rgb component can not be used in alpha portion", reg[i].reg.line_number);
        if (i > 0 &&
            REG_DISCARD == reg[i].reg.bits.name)
            errors.set("reading from discard", reg[i].reg.line_number);
    }
}
