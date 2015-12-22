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
void FinalCombinerStruct::Validate()
{
    if (hasProduct &&
        (REG_E_TIMES_F == product.e.reg.bits.name ||
         REG_SPARE0_PLUS_SECONDARY_COLOR == product.e.reg.bits.name ||
         REG_DISCARD == product.e.reg.bits.name ||
         REG_E_TIMES_F == product.f.reg.bits.name ||
         REG_SPARE0_PLUS_SECONDARY_COLOR == product.f.reg.bits.name ||
         REG_DISCARD == product.f.reg.bits.name))
        errors.set("invalid input register for final_product");

    if (hasProduct &&
        (RCP_BLUE == product.e.reg.bits.channel ||
         RCP_BLUE == product.f.reg.bits.channel))
        errors.set("blue register used in final_product");

    if (REG_E_TIMES_F == alpha.g.reg.bits.name ||
        REG_SPARE0_PLUS_SECONDARY_COLOR == alpha.g.reg.bits.name ||
        REG_DISCARD == alpha.g.reg.bits.name)
        errors.set("invalid input register for final alpha");

    if (RCP_RGB == alpha.g.reg.bits.channel)
        errors.set("rgb register used in final alpha");

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

    if (REG_SPARE0_PLUS_SECONDARY_COLOR == rgb.a.reg.bits.name ||
        REG_DISCARD == rgb.a.reg.bits.name ||
        REG_DISCARD == rgb.b.reg.bits.name ||
        REG_DISCARD == rgb.c.reg.bits.name ||
        REG_DISCARD == rgb.d.reg.bits.name)
        errors.set("invalid input register for final rgb");

    if (RCP_BLUE == rgb.a.reg.bits.channel ||
        RCP_BLUE == rgb.b.reg.bits.channel ||
        RCP_BLUE == rgb.c.reg.bits.channel ||
        RCP_BLUE == rgb.d.reg.bits.channel)
        errors.set("blue register used in final rgb");

    if ((REG_E_TIMES_F == rgb.a.reg.bits.name ||
        REG_E_TIMES_F == rgb.b.reg.bits.name ||
        REG_E_TIMES_F == rgb.c.reg.bits.name ||
        REG_E_TIMES_F == rgb.d.reg.bits.name) && !hasProduct)
        errors.set("final_product used but not set");

    if (RCP_NONE == rgb.a.reg.bits.channel)
        rgb.a.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == rgb.b.reg.bits.channel)
        rgb.b.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == rgb.c.reg.bits.channel)
        rgb.c.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == rgb.d.reg.bits.channel)
        rgb.d.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == product.e.reg.bits.channel)
        product.e.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == product.f.reg.bits.channel)
        product.f.reg.bits.channel = RCP_RGB;
    if (RCP_NONE == alpha.g.reg.bits.channel)
        alpha.g.reg.bits.channel = RCP_ALPHA;
}

static void GenerateFinalInput(char var, MappedRegisterStruct reg) {
    int num = (var >= 'E') ? 1 : 0;
    printf("MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW%d_%c_SOURCE, 0x%x)", num, var, reg.reg.bits.name);
    printf(" | MASK(NV097_SET_COMBINER_SPECULAR_FOG_CW%d_%c_ALPHA, %d)", num, var,
            reg.reg.bits.channel == RCP_ALPHA); //TODO: Blue
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
