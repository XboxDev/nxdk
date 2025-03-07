// #include <glh/glh_extensions.h>

#include "rc1.0_final.h"
#include "nvparse_errors.h"
#include "nvparse_externs.h"

#include <cstdio>
#include <cassert>

void FinalRgbFunctionStruct::ZeroOut()
{
    RegisterEnum zero;
    zero.word = RCP_ZERO;
    a.Init(zero, MAP_UNSIGNED_IDENTITY);
    b.Init(zero, MAP_UNSIGNED_IDENTITY);
    c.Init(zero, MAP_UNSIGNED_IDENTITY);
    d.Init(zero, MAP_UNSIGNED_IDENTITY);
}

void FinalAlphaFunctionStruct::ZeroOut()
{
    RegisterEnum zero;
    zero.word = RCP_ZERO;
    g.Init(zero, MAP_UNSIGNED_IDENTITY);
}

void FinalProductStruct::ZeroOut()
{
    RegisterEnum zero;
    zero.word = RCP_ZERO;
    e.Init(zero, MAP_UNSIGNED_IDENTITY);
    f.Init(zero, MAP_UNSIGNED_IDENTITY);
}

static void ValidateRgbInputRegister(RegisterEnum& reg, bool isEorF, bool hasProduct) {
    if (RCP_NONE == reg.bits.channel)
        reg.bits.channel = RCP_RGB;

    if (REG_DISCARD == reg.bits.name)
        errors.set("reading from discard", reg.line_number);
    if (RCP_BLUE == reg.bits.channel)
        errors.set("blue component can not be used in rgb portion", reg.line_number);
    if (!isEorF) {
        if (!hasProduct && (REG_E_TIMES_F == reg.bits.name))
            errors.set("final_product used but not set", reg.line_number);
    } else {
        if (REG_E_TIMES_F == reg.bits.name)
            errors.set("final_product can not reference itself", reg.line_number);
        if (REG_SPARE0_PLUS_SECONDARY_COLOR == reg.bits.name)
            errors.set("color_sum can not be used in final_product", reg.line_number);
    }
}

static void ValidateAlphaInputRegister(RegisterEnum& reg) {
    if (RCP_NONE == reg.bits.channel)
        reg.bits.channel = RCP_ALPHA;

    if (REG_E_TIMES_F == reg.bits.name)
        errors.set("final_product can not be used in final-combiner alpha portion", reg.line_number);
    if (REG_SPARE0_PLUS_SECONDARY_COLOR == reg.bits.name)
        errors.set("color_sum can not be used in final-combiner alpha portion", reg.line_number);
    if (REG_DISCARD == reg.bits.name)
        errors.set("reading from discard", reg.line_number);
    if (RCP_RGB == reg.bits.channel)
        errors.set("rgb component can not be used in alpha portion", reg.line_number);
}

void FinalCombinerStruct::Validate()
{
    if (REG_SPARE0_PLUS_SECONDARY_COLOR == rgb.a.reg.bits.name &&
        REG_SPARE0_PLUS_SECONDARY_COLOR != rgb.b.reg.bits.name &&
        REG_ZERO == rgb.c.reg.bits.name && MAP_UNSIGNED_IDENTITY == rgb.c.map)
    {
        MappedRegisterStruct temp;
        temp = rgb.a;
        rgb.a = rgb.b;
        rgb.b = temp;
    }

    if (REG_SPARE0_PLUS_SECONDARY_COLOR == rgb.a.reg.bits.name &&
        REG_ZERO == rgb.b.reg.bits.name && MAP_UNSIGNED_INVERT == rgb.b.map &&
        REG_ZERO == rgb.c.reg.bits.name && MAP_UNSIGNED_IDENTITY == rgb.c.map &&
        REG_SPARE0_PLUS_SECONDARY_COLOR != rgb.d.reg.bits.name)
    {
        MappedRegisterStruct temp;
        temp = rgb.a;
        rgb.a = rgb.d;
        rgb.d = temp;
    }

    if (REG_SPARE0_PLUS_SECONDARY_COLOR == rgb.a.reg.bits.name)
        errors.set("color_sum can not be used in this term of the final-combiner rgb portion", rgb.a.reg.line_number);

    ValidateRgbInputRegister(rgb.a.reg, false, hasProduct);
    ValidateRgbInputRegister(rgb.b.reg, false, hasProduct);
    ValidateRgbInputRegister(rgb.c.reg, false, hasProduct);
    ValidateRgbInputRegister(rgb.d.reg, false, hasProduct);
    ValidateRgbInputRegister(product.e.reg, true, hasProduct);
    ValidateRgbInputRegister(product.f.reg, true, hasProduct);

    ValidateAlphaInputRegister(alpha.g.reg);
}

static void GenerateFinalInput(char var, MappedRegisterStruct reg) {
    int num = (var >= 'E') ? 1 : 0;
    printf("MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW%d_%c_SOURCE, %s)", num, var, GetRegisterNameString(reg.reg.bits.name));
    printf(" | MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW%d_%c_ALPHA, %d)", num, var,
            reg.reg.bits.channel == RCP_ALPHA);
    printf(" | MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW%d_%c_INVERSE, %d)", num, var,
            (reg.map == MAP_UNSIGNED_INVERT));
}

void FinalCombinerStruct::Invoke()
{

    printf("pb_push1(p, NV097_SET_COMBINER_SPECULAR_FOG_CW0,\n");
    printf("    ");
    GenerateFinalInput('A', rgb.a);
    printf("\n    | ");
    GenerateFinalInput('B', rgb.b);
    printf("\n    | ");
    GenerateFinalInput('C', rgb.c);
    printf("\n    | ");
    GenerateFinalInput('D', rgb.d);
    printf(");\n");
    printf("p += 2;\n");

    printf("pb_push1(p, NV097_SET_COMBINER_SPECULAR_FOG_CW1,\n");
    printf("    ");
    GenerateFinalInput('E', product.e);
    printf("\n    | ");
    GenerateFinalInput('F', product.f);
    printf("\n    | ");
    GenerateFinalInput('G', alpha.g);

    printf("\n    | MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW1_SPECULAR_CLAMP, %d)", clamp);

    printf(");\n");
    printf("p += 2;\n");
    // if(clamp)
    //     glCombinerParameteriNV(GL_COLOR_SUM_CLAMP_NV, GL_TRUE);
    // else
    //     glCombinerParameteriNV(GL_COLOR_SUM_CLAMP_NV, GL_FALSE);

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_A_NV,
    //     rgb.a.reg.bits.name,
    //     rgb.a.map,
    //     MAP_CHANNEL(rgb.a.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_B_NV,
    //     rgb.b.reg.bits.name,
    //     rgb.b.map,
    //     MAP_CHANNEL(rgb.b.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_C_NV,
    //     rgb.c.reg.bits.name,
    //     rgb.c.map,
    //     MAP_CHANNEL(rgb.c.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_D_NV,
    //     rgb.d.reg.bits.name,
    //     rgb.d.map,
    //     MAP_CHANNEL(rgb.d.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_E_NV,
    //     product.e.reg.bits.name,
    //     product.e.map,
    //     MAP_CHANNEL(product.e.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_F_NV,
    //     product.f.reg.bits.name,
    //     product.f.map,
    //     MAP_CHANNEL(product.f.reg.bits.channel));

    // glFinalCombinerInputNV(
    //     GL_VARIABLE_G_NV,
    //     alpha.g.reg.bits.name,
    //     alpha.g.map,
    //     MAP_CHANNEL(alpha.g.reg.bits.channel));
    // assert(false);
}
