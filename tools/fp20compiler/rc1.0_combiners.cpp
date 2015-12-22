// #include <glh/glh_extensions.h>

#include "rc1.0_combiners.h"
#include "nvparse_errors.h"
#include "nvparse_externs.h"

#include <cstdio>
#include <cstring>
#include <cassert>

void CombinersStruct::Validate()
{
    if (2 == numConsts &&
        cc[0].reg.bits.name == cc[1].reg.bits.name)
        errors.set("global constant set twice");

    generals.Validate(numConsts, &cc[0]);

    final.Validate();
}

void CombinersStruct::Invoke()
{
    // for (int i = 0; i < numConsts; i++)
    //     glCombinerParameterfvNV(cc[i].reg.bits.name, &(cc[i].v[0]));
    assert(numConsts <= 2);

    printf("pb_push1(p, NV097_SET_COMBINER_CONTROL,");
    printf("\n    MASK(NV097_SET_COMBINER_CONTROL_FACTOR0, %s)",
        numConsts >= 1 ? "NV097_SET_COMBINER_CONTROL_FACTOR1_SAME_FACTOR_ALL"
            : "NV097_SET_COMBINER_CONTROL_FACTOR0_EACH_STAGE");
    printf("\n    | MASK(NV097_SET_COMBINER_CONTROL_FACTOR1, %s)",
        numConsts >= 2 ? "NV097_SET_COMBINER_CONTROL_FACTOR1_SAME_FACTOR_ALL"
            : "NV097_SET_COMBINER_CONTROL_FACTOR1_EACH_STAGE");
    printf("\n    | MASK(NV097_SET_COMBINER_CONTROL_ITERATION_COUNT, %d)", generals.num);
    printf(");\n");
    printf("p += 2;\n");

    for (int i = 0; i < numConsts; i++) {
        const char* cmd = NULL;
        switch (cc[i].reg.bits.name) {
        case REG_CONSTANT_COLOR0:
            cmd = "NV097_SET_SPECULAR_FOG_FACTOR + 0";
            break;
        case REG_CONSTANT_COLOR1:
            cmd = "NV_097_SET_SPECULAR_FOG_FACTOR + 4";
            break;
        default:
            assert(false);
            break;
        }
        printf("pb_push4f(p, %s, %f, %f, %f, %f);\n", cmd,
            cc[i].v[0], cc[i].v[1], cc[i].v[2], cc[i].v[3]);
        printf("p += 5;\n");
    }


    generals.Invoke();

    final.Invoke();
}

bool is_rc10(const char * s)
{
    return ! strncmp(s, "!!RC1.0", 7);
}


bool rc10_init_more()
{
    
    errors.reset();
    line_number = 1;

    return true;
}
