#ifndef __XGU_H
#define __XGU_H

#include <assert.h>
#include <pbkit/pbkit.h>
// Hack until we fix pbkit/outer.h (or stop including it)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"
#include "nv2a_regs.h"
#pragma GCC diagnostic pop

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    //FIXME: define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_UB_D3D     0
    //FIXME: define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_S1         1
    XGU_FLOAT = NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F,
    //FIXME: define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_UB_OGL     4
    //FIXME: define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_S32K       5
    //FIXME: define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_CMP        6
} XguVertexArrayType;

typedef enum {
    XGU_END = NV097_SET_BEGIN_END_OP_END, // FIXME: Disallow this one?
    XGU_POINTS = NV097_SET_BEGIN_END_OP_POINTS,
    XGU_LINES = NV097_SET_BEGIN_END_OP_LINES,
    XGU_LINE_LOOP = NV097_SET_BEGIN_END_OP_LINE_LOOP,
    XGU_LINE_STRIP = NV097_SET_BEGIN_END_OP_LINE_STRIP,
    XGU_TRIANGLES = NV097_SET_BEGIN_END_OP_TRIANGLES,
    XGU_TRIANGLE_STRIP = NV097_SET_BEGIN_END_OP_TRIANGLE_STRIP,
    XGU_TRIANGLE_FAN = NV097_SET_BEGIN_END_OP_TRIANGLE_FAN,
    XGU_QUADS = NV097_SET_BEGIN_END_OP_QUADS,
    XGU_QUAD_STRIP = NV097_SET_BEGIN_END_OP_QUAD_STRIP,
    XGU_POLYGON = NV097_SET_BEGIN_END_OP_POLYGON
} XguPrimitiveType;

typedef enum {
    XGU_FUNC_NEVER = 0x200,
    XGU_FUNC_LESS,
    XGU_FUNC_EQUAL,
    XGU_FUNC_LESS_OR_EQUAL,
    XGU_FUNC_GREATER,
    XGU_FUNC_NOT_EQUAL,
    XGU_FUNC_GREATER_OR_EQUAL,
    XGU_FUNC_ALWAYS
} XguFuncType;

typedef enum {
    XGU_VERTEX_ARRAY = NV2A_VERTEX_ATTR_POSITION,
    XGU_NORMAL_ARRAY = NV2A_VERTEX_ATTR_NORMAL,
    XGU_COLOR_ARRAY = NV2A_VERTEX_ATTR_DIFFUSE,
    XGU_SECONDARY_COLOR_ARRAY = NV2A_VERTEX_ATTR_SPECULAR,
    XGU_FOG_ARRAY = NV2A_VERTEX_ATTR_FOG,
    XGU_TEXCOORD0_ARRAY = NV2A_VERTEX_ATTR_TEXTURE0,
    XGU_TEXCOORD1_ARRAY = NV2A_VERTEX_ATTR_TEXTURE1,
    XGU_TEXCOORD2_ARRAY = NV2A_VERTEX_ATTR_TEXTURE2,
    XGU_TEXCOORD3_ARRAY = NV2A_VERTEX_ATTR_TEXTURE3
} XguVertexArray;

typedef enum {
    XGU_FIXED = NV097_SET_TRANSFORM_EXECUTION_MODE_MODE_FIXED,
    XGU_PROGRAM = NV097_SET_TRANSFORM_EXECUTION_MODE_MODE_PROGRAM
} XguExecMode;

typedef enum {
    XGU_RANGE_MODE_USER = 0 /*NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE_USER*/,
    XGU_RANGE_MODE_PRIVATE = 1 /*NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE_PRIVATE */
} XguExecRange;

typedef enum {
    XGU_CLEAR_Z = NV097_CLEAR_SURFACE_Z,
    XGU_CLEAR_STENCIL = NV097_CLEAR_SURFACE_STENCIL,
    XGU_CLEAR_COLOR = NV097_CLEAR_SURFACE_COLOR,
    XGU_CLEAR_R = NV097_CLEAR_SURFACE_R,
    XGU_CLEAR_G = NV097_CLEAR_SURFACE_G,
    XGU_CLEAR_B = NV097_CLEAR_SURFACE_B,
    XGU_CLEAR_A = NV097_CLEAR_SURFACE_A
} XguClearSurface;

/*
 * SFACTOR and DFACTOR share the same values for their respective defines.
 * Thus no duplicate table was created.
 */
typedef enum {
    XGU_FACTOR_ZERO = NV097_SET_BLEND_FUNC_SFACTOR_V_ZERO,
    XGU_FACTOR_ONE = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE,
    XGU_FACTOR_SRC_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_COLOR,
    XGU_FACTOR_ONE_MINUS_SRC_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_SRC_COLOR,
    XGU_FACTOR_SRC_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_ALPHA,
    XGU_FACTOR_ONE_MINUS_SRC_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_SRC_ALPHA,
    XGU_FACTOR_DST_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_DST_ALPHA,
    XGU_FACTOR_ONE_MINUS_DST_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_DST_ALPHA,
    XGU_FACTOR_DST_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_DST_COLOR,
    XGU_FACTOR_ONE_MINUS_DST_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_DST_COLOR,
    XGU_FACTOR_SRC_ALPHA_SATURATE = NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_ALPHA_SATURATE,
    XGU_FACTOR_CONSTANT_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_CONSTANT_COLOR,
    XGU_FACTOR_ONE_MINUS_CONSTANT_COLOR = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_CONSTANT_COLOR,
    XGU_FACTOR_CONSTANT_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_CONSTANT_ALPHA,
    XGU_FACTOR_ONE_MINUS_CONSTANT_ALPHA = NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_CONSTANT_ALPHA
} XguBlendFactor;

typedef enum {
    XGU_BLUE = NV097_SET_COLOR_MASK_BLUE_WRITE_ENABLE,
    XGU_GREEN = NV097_SET_COLOR_MASK_GREEN_WRITE_ENABLE,
    XGU_RED = NV097_SET_COLOR_MASK_RED_WRITE_ENABLE,
    XGU_ALPHA = NV097_SET_COLOR_MASK_ALPHA_WRITE_ENABLE
} XguColorMask;

typedef enum {
    XGU_POINT = NV097_SET_FRONT_POLYGON_MODE_V_POINT,
    XGU_LINE = NV097_SET_FRONT_POLYGON_MODE_V_LINE,
    XGU_FILL = NV097_SET_FRONT_POLYGON_MODE_V_FILL
} XguPolygonMode;

typedef enum {
    XGU_CULL_FRONT = NV097_SET_CULL_FACE_V_FRONT,
    XGU_CULL_BACK = NV097_SET_CULL_FACE_V_BACK,
    XGU_CULL_FRONT_AND_BACK = NV097_SET_CULL_FACE_V_FRONT_AND_BACK
} XguCullFace;

typedef enum {
    XGU_TEXGEN_DISABLE = NV097_SET_TEXGEN_S_DISABLE,
    XGU_TEXGEN_EYE_LINEAR = NV097_SET_TEXGEN_S_EYE_LINEAR,
    XGU_TEXGEN_OBJECT_LINEAR = NV097_SET_TEXGEN_S_OBJECT_LINEAR,
    XGU_TEXGEN_SPHERE_MAP = NV097_SET_TEXGEN_S_SPHERE_MAP,
    XGU_TEXGEN_REFLECTION_MAP = NV097_SET_TEXGEN_S_REFLECTION_MAP,
    XGU_TEXGEN_NORMAL_MAP = NV097_SET_TEXGEN_S_NORMAL_MAP
} XguTexgen;

typedef enum {
    XGU_SOURCE_TEXTURE = NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE_TEXTURE,
    XGU_SOURCE_COLOR = NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE_COLOR
} XguBorderSrc;

typedef enum {
    XGU_TEXTURE_FORMAT_Y8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_Y8,
    XGU_TEXTURE_FORMAT_AY8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_AY8,
    XGU_TEXTURE_FORMAT_A1R5G5B5_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A1R5G5B5,
    XGU_TEXTURE_FORMAT_X1R5G5B5_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_X1R5G5B5,
    XGU_TEXTURE_FORMAT_A4R4G4B4_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A4R4G4B4,
    XGU_TEXTURE_FORMAT_R5G6B5_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R5G6B5,
    XGU_TEXTURE_FORMAT_A8R8G8B8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8R8G8B8,
    XGU_TEXTURE_FORMAT_X8R8G8B8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_X8R8G8B8,
    //FIXME: XGU_TEXTURE_FORMAT_I8_A8R8G8B8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_I8_A8R8G8B8,
    XGU_TEXTURE_FORMAT_DXT1 = NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT1_A1R5G5B5,
    XGU_TEXTURE_FORMAT_DXT3 = NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT23_A8R8G8B8,
    XGU_TEXTURE_FORMAT_DXT5 = NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT45_A8R8G8B8,
    XGU_TEXTURE_FORMAT_A1R5G5B5 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A1R5G5B5,
    XGU_TEXTURE_FORMAT_R5G6B5 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_R5G6B5,
    XGU_TEXTURE_FORMAT_A8R8G8B8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8R8G8B8,
    XGU_TEXTURE_FORMAT_Y8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_Y8,
    XGU_TEXTURE_FORMAT_A8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8,
    XGU_TEXTURE_FORMAT_A8Y8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8Y8,
    XGU_TEXTURE_FORMAT_AY8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_AY8,
    XGU_TEXTURE_FORMAT_X1R5G5B5 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_X1R5G5B5,
    XGU_TEXTURE_FORMAT_A4R4G4B4 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A4R4G4B4,
    XGU_TEXTURE_FORMAT_X8R8G8B8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_X8R8G8B8,
    XGU_TEXTURE_FORMAT_A8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8,
    XGU_TEXTURE_FORMAT_A8Y8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8Y8,
    //FIXME: XGU_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8 = NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8,
    XGU_TEXTURE_FORMAT_R6G5B5_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R6G5B5,
    XGU_TEXTURE_FORMAT_G8B8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_G8B8,
    XGU_TEXTURE_FORMAT_R8B8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R8B8,
    //FIXME: XGU_TEXTURE_FORMAT_DEPTH_X8_Y24_FIXED = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_DEPTH_X8_Y24_FIXED,
    //FIXME: XGU_TEXTURE_FORMAT_DEPTH_Y16_FIXED = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_DEPTH_Y16_FIXED,
    XGU_TEXTURE_FORMAT_Y16 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_Y16,
    XGU_TEXTURE_FORMAT_A8B8G8R8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8B8G8R8,
    XGU_TEXTURE_FORMAT_R8G8B8A8_SWIZZLED = NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R8G8B8A8,
    XGU_TEXTURE_FORMAT_A8B8G8R8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8B8G8R8,
    XGU_TEXTURE_FORMAT_B8G8R8A8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_B8G8R8A8,
    XGU_TEXTURE_FORMAT_R8G8B8A8 = NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_R8G8B8A8
} XguTexFormatColor;

typedef enum {
    XGU_PALETTE_LENGTH_256 = NV097_SET_TEXTURE_PALETTE_LENGTH_256,
    XGU_PALETTE_LENGTH_128 = NV097_SET_TEXTURE_PALETTE_LENGTH_128,
    XGU_PALETTE_LENGTH_64 = NV097_SET_TEXTURE_PALETTE_LENGTH_64,
    XGU_PALETTE_LENGTH_32 = NV097_SET_TEXTURE_PALETTE_LENGTH_32,    
} XguPaletteLen;

typedef enum {
    XGU_STENCIL_OP_KEEP = NV097_SET_STENCIL_OP_V_KEEP,
    XGU_STENCIL_OP_ZERO = NV097_SET_STENCIL_OP_V_ZERO,
    XGU_STENCIL_OP_REPLACE = NV097_SET_STENCIL_OP_V_REPLACE,
    XGU_STENCIL_OP_INCRSAT = NV097_SET_STENCIL_OP_V_INCRSAT,
    XGU_STENCIL_OP_DECRSAT = NV097_SET_STENCIL_OP_V_DECRSAT,
    XGU_STENCIL_OP_INVERT = NV097_SET_STENCIL_OP_V_INVERT,
    XGU_STENCIL_OP_INCR = NV097_SET_STENCIL_OP_V_INCR,
    XGU_STENCIL_OP_DECR = NV097_SET_STENCIL_OP_V_DECR
} XguStencilOp;

typedef enum {
    XGU_FRONT_CW = NV097_SET_FRONT_FACE_V_CW,
    XGU_FRONT_CCW = NV097_SET_FRONT_FACE_V_CCW
} XguFrontFace;

typedef enum {
    XGU_LMASK_OFF = NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_OFF,
    XGU_LMASK_INFINITE = NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_INFINITE,
    XGU_LMASK_LOCAL = NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_LOCAL,
    XGU_LMASK_SPOT = NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_SPOT
} XguLightMask;

#define XGU_MASK(mask, val) (((val) << (__builtin_ffs(mask)-1)) & (mask))

#define XGU_ATTRIBUTE_COUNT 16
#define XGU_TEXTURE_COUNT 4
#define XGU_WEIGHT_COUNT 4
#define XGU_LIGHT_COUNT 8

typedef union {
    float f[3];
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
} XguVec3;

typedef union {
    float f[4];
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
} XguVec4;

typedef union {
  float f[4*4];
  XguVec4 col[4];
} XguMatrix4x4;


/* ========================= *
 *  BEGIN PUSH HELPER BLOCK  *
 * ========================= */

static inline
uint32_t* push_command(uint32_t* p, uint32_t command, unsigned int parameter_count) {
    int nparams = parameter_count;
    assert(nparams > 0);
    pb_push(p++, command, nparams);
    return p;
}

static inline
uint32_t* push_parameter(uint32_t* p, uint32_t parameter) {
    *p++ = parameter;
    return p;
}

static inline
uint32_t* push_parameters(uint32_t* p, const uint32_t* parameters, unsigned int count) {
    for(unsigned int i = 0; i < count; i++) {
        p = push_parameter(p, parameters[i]);
    }
    return p;
}

static inline
uint32_t* push_boolean(uint32_t* p, bool enabled) {
    return push_parameter(p, enabled ? 1 : 0);
}

static inline
uint32_t* push_command_boolean(uint32_t* p, uint32_t command, bool enabled) {
    p = push_command(p, command, 1);
    p = push_boolean(p, enabled);
    return p;
}

static inline
uint32_t* push_float(uint32_t* p, float f) {
    return push_parameter(p, *(uint32_t*)&f);
}

static inline
uint32_t* push_floats(uint32_t* p, const float* f, unsigned int count) {
    return push_parameters(p, (const uint32_t*)f, count);
}

static inline
uint32_t* push_matrix2x2(uint32_t* p, const float m[2*2]) {
    return push_floats(p, m, 2*2);
}

static inline
uint32_t* push_matrix4x4(uint32_t* p, const float m[4*4]) {
    return push_floats(p, m, 4*4);
}

static inline
uint32_t* push_command_matrix2x2(uint32_t* p, uint32_t command, const float m[2*2]) {
    p = push_command(p, command, 2*2);
    p = push_matrix2x2(p, m);
    return p;
}

static inline
uint32_t* push_command_matrix4x4(uint32_t* p, uint32_t command, const float m[4*4]) {
    p = push_command(p, command, 4*4);
    p = push_matrix4x4(p, m);
    return p;
}

static inline
uint32_t* push_command_parameter(uint32_t* p, uint32_t command, uint32_t parameter) {
    p = push_command(p, command, 1);
    p = push_parameter(p, parameter);
    return p;
}

static inline
uint32_t* push_command_float(uint32_t* p, uint32_t command, float parameter) {
    p = push_command(p, command, 1);
    p = push_float(p, parameter);
    return p;
}


/* ========================= *
 * BEGIN XGU FUNCTIONS BLOCK *
 * ========================= */

inline
uint32_t* xgu_begin(uint32_t* p, XguPrimitiveType type) {

    // Force people to use xgu_end instead
    assert(type != NV097_SET_BEGIN_END_OP_END);

    return push_command_parameter(p, NV097_SET_BEGIN_END, type);
}

inline
uint32_t* xgu_end(uint32_t* p) {
    return push_command_parameter(p, NV097_SET_BEGIN_END, NV097_SET_BEGIN_END_OP_END);
}

inline
uint32_t* xgu_no_operation(uint32_t* p, uint32_t param) {
    /* param is typically 0 */
    return push_command_parameter(p, NV097_NO_OPERATION, param);
}

inline
uint32_t* xgu_wait_for_idle(uint32_t* p) {
    return push_command(p, NV097_WAIT_FOR_IDLE, 0);
}

inline
uint32_t* xgu_set_viewport_offset(uint32_t* p, float x, float y, float z, float w) {
    p = push_command(p, NV097_SET_VIEWPORT_OFFSET, 4);
    p = push_float(p, x);
    p = push_float(p, y);
    p = push_float(p, z);
    p = push_float(p, w);
    return p;
}

inline
uint32_t* xgu_set_viewport_scale(uint32_t* p, float x, float y, float z, float w) {
    p = push_command(p, NV097_SET_VIEWPORT_SCALE, 4);
    p = push_float(p, x);
    p = push_float(p, y);
    p = push_float(p, z);
    p = push_float(p, w);
    return p;
}

inline
uint32_t* xgu_set_clip_min(uint32_t* p, float znear) {
    return push_command_float(p, NV097_SET_CLIP_MIN, znear);
}

inline
uint32_t* xgu_set_clip_max(uint32_t* p, float zfar) {
    return push_command_float(p, NV097_SET_CLIP_MAX, zfar);
}

inline
uint32_t* xgu_set_zstencil_clear_value(uint32_t* p, uint32_t value) {
    return push_command_parameter(p, NV097_SET_ZSTENCIL_CLEAR_VALUE, value);
}

inline
uint32_t* xgu_set_color_clear_value(uint32_t* p, uint32_t color) {
    return push_command_parameter(p, NV097_SET_COLOR_CLEAR_VALUE, color);
}

inline
uint32_t* xgu_clear_surface(uint32_t* p, XguClearSurface flags) {
    /*
     * flags should probably be a combination of values -
     * not sure if using an enum allows such magic and/or
     * if it hinders out-of-bounds values.
     */
    return push_command_parameter(p, NV097_CLEAR_SURFACE, flags);
}

inline
uint32_t* xgu_set_clear_rect_horizontal(uint32_t* p, uint32_t x1, uint32_t x2) {
    return push_command_parameter(p, NV097_SET_CLEAR_RECT_HORIZONTAL,
                                  ((x2-1)<<16)|x1);
}

inline
uint32_t* xgu_set_clear_rect_vertical(uint32_t* p, uint32_t y1, uint32_t y2) {
    return push_command_parameter(p, NV097_SET_CLEAR_RECT_VERTICAL,
                                  ((y2-1)<<16)|y1);
}

inline
uint32_t* xgu_set_object(uint32_t* p, uint32_t instance) {
    return push_command_parameter(p, NV097_SET_OBJECT, instance);
}

inline
uint32_t* xgu_set_texgen_s(uint32_t* p, unsigned int texture_index, XguTexgen tg) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_parameter(p, NV097_SET_TEXGEN_S, tg);
}

inline
uint32_t* xgu_set_texgen_t(uint32_t* p, unsigned int texture_index, XguTexgen tg) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_parameter(p, NV097_SET_TEXGEN_T, tg);
}

inline
uint32_t* xgu_set_texgen_r(uint32_t* p, unsigned int texture_index, XguTexgen tg) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_parameter(p, NV097_SET_TEXGEN_R, tg);
}

inline
uint32_t* xgu_set_texgen_q(uint32_t* p, unsigned int texture_index, XguTexgen tg) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_parameter(p, NV097_SET_TEXGEN_Q, tg);
}

inline
uint32_t* xgu_set_texture_matrix_enable(uint32_t* p, unsigned int texture_index, bool enabled) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_boolean(p, NV097_SET_TEXTURE_MATRIX_ENABLE, enabled);
}

inline
uint32_t* xgu_set_projection_matrix(uint32_t* p, const float m[4*4]) {
    return push_command_matrix4x4(p, NV097_SET_PROJECTION_MATRIX, m);
}

inline
uint32_t* xgu_set_model_view_matrix(uint32_t* p, uint32_t bone_index, const float m[4*4]) {
    return push_command_matrix4x4(p, NV097_SET_MODEL_VIEW_MATRIX + bone_index*(4*4)*4, m);
}

inline
uint32_t* xgu_set_inverse_model_view_matrix(uint32_t* p, uint32_t bone_index, const float m[4*4]) {
    return push_command_matrix4x4(p, NV097_SET_INVERSE_MODEL_VIEW_MATRIX + bone_index*(4*4)*4, m);
}

inline
uint32_t* xgu_set_composite_matrix(uint32_t* p, const float m[4*4]) {
    return push_command_matrix4x4(p, NV097_SET_COMPOSITE_MATRIX, m);
}

inline
uint32_t* xgu_set_texture_matrix(uint32_t* p, unsigned int texture_index, const float m[4*4]) {
    assert(texture_index == 0); //FIXME: Support up to 4
    return push_command_matrix4x4(p, NV097_SET_TEXTURE_MATRIX, m);
}

/* ==== Stencil OP ==== */

inline
uint32_t* xgu_set_stencil_op_fail(uint32_t* p, XguStencilOp so) {
    return push_command_parameter(p, NV097_SET_STENCIL_OP_FAIL, so);
}

inline
uint32_t* xgu_set_stencil_op_zfail(uint32_t* p, XguStencilOp so) {
    return push_command_parameter(p, NV097_SET_STENCIL_OP_ZFAIL, so);
}

inline
uint32_t* xgu_set_stencil_op_zpass(uint32_t* p, XguStencilOp so) {
    return push_command_parameter(p, NV097_SET_STENCIL_OP_ZPASS, so);
}

/* ==== Vertex Data Array ==== */

inline
uint32_t* xgu_set_vertex_data_array_format(uint32_t* p, XguVertexArray index, XguVertexArrayType format,
                                        uint32_t size, uint32_t stride) {
    return push_command_parameter(p, NV097_SET_VERTEX_DATA_ARRAY_FORMAT + index*4,
                                  XGU_MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE, format) |
                                  XGU_MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_SIZE, size) |
                                  XGU_MASK(NV097_SET_VERTEX_DATA_ARRAY_FORMAT_STRIDE, stride));
}

inline
uint32_t* xgu_set_vertex_data_array_offset(uint32_t* p, XguVertexArray index, const void* data) {
    return push_command_parameter(p, NV097_SET_VERTEX_DATA_ARRAY_OFFSET + index*4,
                                  (uint32_t)data/* & 0x03fffff*/);
}

inline
uint32_t* xgu_element16(uint32_t* p, const uint16_t* elements, unsigned int count) {
    assert(count % 2 == 0);
    p = push_command(p, 0x40000000|NV097_ARRAY_ELEMENT16, count/2);
    memcpy(p, elements, count * sizeof(uint16_t));
    p += count / 2;
    return p;
}

inline
uint32_t* xgu_element32(uint32_t* p, const uint32_t* elements, unsigned int count) {
    p = push_command(p, 0x40000000|NV097_ARRAY_ELEMENT32, count);
    memcpy(p, elements, count * sizeof(uint32_t));
    p += count;
    return p;
}

inline
uint32_t* xgu_draw_arrays(uint32_t* p, unsigned int start, unsigned int count) {
    assert(count>=1);
    assert(count<=256);
    return push_command_parameter(p, 0x40000000|NV097_DRAW_ARRAYS,
                                  XGU_MASK(NV097_DRAW_ARRAYS_COUNT, (count-1)) |
                                  XGU_MASK(NV097_DRAW_ARRAYS_START_INDEX, start));
}

/* ==== Alpha/Blend/Cull ==== */
inline
uint32_t* xgu_set_alpha_test_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_ALPHA_TEST_ENABLE, enabled);
}

inline
uint32_t* xgu_set_blend_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_BLEND_ENABLE, enabled);
}

inline
uint32_t* xgu_set_cull_face_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_CULL_FACE_ENABLE, enabled);
}

inline
uint32_t* xgu_set_depth_test_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_DEPTH_TEST_ENABLE, enabled);
}

inline
uint32_t* xgu_set_dither_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_DITHER_ENABLE, enabled);
}

inline
uint32_t* xgu_set_lighting_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_LIGHTING_ENABLE, enabled);
}

inline
uint32_t* xgu_set_stencil_test_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_STENCIL_TEST_ENABLE, enabled);
}

inline
uint32_t* xgu_set_alpha_func(uint32_t* p, XguFuncType func) {
    return push_command_parameter(p, NV097_SET_ALPHA_FUNC, func);
}

inline
uint32_t* xgu_set_alpha_ref(uint32_t* p, uint8_t ref) {
    return push_command_parameter(p, NV097_SET_ALPHA_REF, ref);
}

inline
uint32_t* xgu_set_blend_func_sfactor(uint32_t* p, XguBlendFactor sf) {
    return push_command_parameter(p, NV097_SET_BLEND_FUNC_SFACTOR, sf);
}

inline
uint32_t* xgu_set_blend_func_dfactor(uint32_t* p, XguBlendFactor df) {
    return push_command_parameter(p, NV097_SET_BLEND_FUNC_DFACTOR, df);
}

inline
uint32_t* xgu_set_depth_func(uint32_t* p, XguFuncType func) {
    return push_command_parameter(p, NV097_SET_DEPTH_FUNC, func);
}

inline
uint32_t* xgu_set_color_mask(uint32_t* p, XguColorMask cm) {
    return push_command_parameter(p, NV097_SET_COLOR_MASK, cm);
}

inline
uint32_t* xgu_set_front_polygon_mode(uint32_t* p, XguPolygonMode pm) {
    return push_command_parameter(p, NV097_SET_FRONT_POLYGON_MODE, pm);
}

inline
uint32_t* xgu_set_cull_face(uint32_t* p, XguCullFace cf) {
    return push_command_parameter(p, NV097_SET_CULL_FACE, cf);
}

inline
uint32_t* xgu_set_front_face(uint32_t* p, XguFrontFace ff) {
    return push_command_parameter(p, NV097_SET_FRONT_FACE, ff);
}

/* ==== Transform ==== */
inline
uint32_t* xgu_set_transform_execution_mode(uint32_t* p, XguExecMode mode, XguExecRange range) {
    return push_command_parameter(p,
                                  NV097_SET_TRANSFORM_EXECUTION_MODE,
                                  XGU_MASK(NV097_SET_TRANSFORM_EXECUTION_MODE_MODE, mode) |
                                  XGU_MASK(NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE, range));
}

inline
uint32_t* xgu_set_transform_constant(uint32_t* p, const XguVec4 *v, unsigned int count) {
    p = push_command(p, NV097_SET_TRANSFORM_CONSTANT, count*4);
    for (uint32_t i = 0; i < count; ++i) {
        p = push_floats(p, v[i].f, 4);
    }
    return p;
}

inline
uint32_t* xgu_set_transform_constant_load(uint32_t* p, uint32_t offset) {
    return push_command_parameter(p, NV097_SET_TRANSFORM_CONSTANT_LOAD, offset);
}


typedef union {
    uint32_t i[4];
    struct {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
    };
} XguTransformProgramInstruction;

inline
uint32_t* xgu_set_transform_program(uint32_t* p, const XguTransformProgramInstruction* instructions, unsigned int count) {
    p = push_command(p, NV097_SET_TRANSFORM_PROGRAM, count*4);
    for (uint32_t i = 0; i < count; ++i) {
        p = push_parameters(p, &instructions[i].i[0], 4);
    }
    return p;
}

inline
uint32_t* xgu_set_transform_program_start(uint32_t* p, uint32_t offset) {
    return push_command_parameter(p, NV097_SET_TRANSFORM_PROGRAM_START, offset);
}

inline
uint32_t* xgu_set_transform_program_load(uint32_t* p, uint32_t offset) {
    return push_command_parameter(p, NV097_SET_TRANSFORM_PROGRAM_LOAD, offset);
}

inline
uint32_t* xgu_set_transform_program_cxt_write_enable(uint32_t* p, bool enabled) {
    return push_command_boolean(p, NV097_SET_TRANSFORM_PROGRAM_CXT_WRITE_EN, enabled);
}

/* ==== Lights ==== */
inline
uint32_t* xgu_set_light_enable_mask(uint32_t* p, XguLightMask light0, XguLightMask light1, XguLightMask light2, XguLightMask light3, XguLightMask light4, XguLightMask light5,XguLightMask light6, XguLightMask light7) {
    return push_command_parameter(p, NV097_SET_LIGHT_ENABLE_MASK, 0
                                  | (light0 << 0)
                                  | (light1 << 2)
                                  | (light2 << 4)
                                  | (light3 << 6)
                                  | (light4 << 8)
                                  | (light5 << 10)
                                  | (light6 << 12)
                                  | (light7 << 14));
}

inline
uint32_t* xgu_set_back_light_ambient_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_BACK_LIGHT_AMBIENT_COLOR + light_index*64, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_back_light_diffuse_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_BACK_LIGHT_DIFFUSE_COLOR + light_index*64, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_back_light_specular_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_BACK_LIGHT_SPECULAR_COLOR + light_index*64, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_light_ambient_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_AMBIENT_COLOR + light_index*128, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_light_diffuse_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_DIFFUSE_COLOR + light_index*128, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_light_specular_color(uint32_t* p, unsigned int light_index, float r, float g, float b) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_SPECULAR_COLOR + light_index*128, 3);
    p = push_float(p, r);
    p = push_float(p, g);
    p = push_float(p, b);
    return p;
}

inline
uint32_t* xgu_set_light_local_range(uint32_t* p, unsigned int light_index, float range) {
    assert(light_index < XGU_LIGHT_COUNT);
    return push_command_float(p, NV097_SET_LIGHT_LOCAL_RANGE + light_index*128, range);
}

inline
uint32_t* xgu_set_light_infinite_half_vector(uint32_t* p, unsigned int light_index, XguVec3 v) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_INFINITE_HALF_VECTOR + light_index*128, 3);
    return push_floats(p, v.f, 3);
}

inline
uint32_t* xgu_set_light_infinite_direction(uint32_t* p, unsigned int light_index, XguVec3 v) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_INFINITE_DIRECTION + light_index*128, 3);
    return push_floats(p, v.f, 3);
}

inline
uint32_t* xgu_set_light_spot_falloff(uint32_t* p, unsigned int light_index, XguVec3 v) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_SPOT_FALLOFF + light_index*128, 3);
    return push_floats(p, v.f, 3);
}

inline
uint32_t* xgu_set_light_spot_direction(uint32_t* p, unsigned int light_index, XguVec4 v) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_SPOT_DIRECTION + light_index*128, 4);
    return push_floats(p, v.f, 4);
}

inline
uint32_t* xgu_set_light_local_position(uint32_t* p, unsigned int light_index, XguVec3 v) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_LOCAL_POSITION + light_index*128, 3);
    return push_floats(p, v.f, 3);
}

inline
uint32_t* xgu_set_light_local_attenuation(uint32_t* p, unsigned int light_index, float constant_factor, float linear_factor, float quadratic_factor) {
    assert(light_index < XGU_LIGHT_COUNT);
    p = push_command(p, NV097_SET_LIGHT_LOCAL_ATTENUATION + light_index*128, 3);
    p = push_float(p, constant_factor);
    p = push_float(p, linear_factor);
    p = push_float(p, quadratic_factor);
    return p;
}

/* ==== Texture stuff ==== */

inline
uint32_t* xgu_set_texture_offset(uint32_t* p, unsigned int texture_index, const void* offset) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_OFFSET + texture_index*64, (uint32_t)offset);
}

inline
uint32_t* xgu_set_texture_format(uint32_t* p, unsigned int texture_index, uint8_t context_dma, bool cubemap_enable,
                                 XguBorderSrc border_src, uint8_t dimensionality,
                                 XguTexFormatColor format, uint8_t mipmap_levels,
                                 uint8_t u_size, uint8_t v_size, uint8_t p_size) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_FORMAT + texture_index*64,
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_CONTEXT_DMA, context_dma) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_CUBEMAP_ENABLE, cubemap_enable) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE, border_src) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_DIMENSIONALITY, dimensionality) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_COLOR, format) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_MIPMAP_LEVELS, mipmap_levels) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_U, u_size) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_V, v_size) |
                                  XGU_MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_P, p_size));
}

inline
uint32_t* xgu_set_texture_address(uint32_t* p, unsigned int texture_index, uint32_t address) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_ADDRESS + texture_index*64, address);
}

inline
uint32_t* xgu_set_texture_control0(uint32_t* p, unsigned int texture_index, bool enable, uint16_t min_lod, uint16_t max_lod) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_CONTROL0 + texture_index*64,
                                  (enable ? NV097_SET_TEXTURE_CONTROL0_ENABLE : 0) |
                                  XGU_MASK(NV097_SET_TEXTURE_CONTROL0_MIN_LOD_CLAMP, min_lod) |
                                  XGU_MASK(NV097_SET_TEXTURE_CONTROL0_MAX_LOD_CLAMP, max_lod));
}

inline
uint32_t* xgu_set_texture_control1(uint32_t* p, unsigned int texture_index, uint16_t pitch) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_CONTROL1 + texture_index*64,
                                  XGU_MASK(NV097_SET_TEXTURE_CONTROL1_IMAGE_PITCH, pitch));
}

inline
uint32_t* xgu_set_texture_filter(uint32_t* p, unsigned int texture_index, uint16_t lod_bias, uint8_t filter_min, uint8_t filter_mag,
                                 bool r_signed, bool g_signed, bool b_signed, bool a_signed) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_FILTER + texture_index*64,
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_MIPMAP_LOD_BIAS, lod_bias) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_MIN, filter_min) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_MAG, filter_mag) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_ASIGNED, a_signed) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_RSIGNED, r_signed) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_GSIGNED, g_signed) |
                                  XGU_MASK(NV097_SET_TEXTURE_FILTER_BSIGNED, b_signed));
}

inline
uint32_t* xgu_set_texture_image_rect(uint32_t* p, unsigned int texture_index, uint16_t width, uint16_t height) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_IMAGE_RECT + texture_index*64,
                                  XGU_MASK(NV097_SET_TEXTURE_IMAGE_RECT_WIDTH, width) |
                                  XGU_MASK(NV097_SET_TEXTURE_IMAGE_RECT_HEIGHT, height));
}

inline
uint32_t* xgu_set_texture_palette(uint32_t* p, unsigned int texture_index, bool context_dma, XguPaletteLen palette_length,
                                  const void* offset) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_PALETTE + texture_index*64,
                                  XGU_MASK(NV097_SET_TEXTURE_PALETTE_CONTEXT_DMA, context_dma) |
                                  XGU_MASK(NV097_SET_TEXTURE_PALETTE_LENGTH, palette_length) |
                                  XGU_MASK(NV097_SET_TEXTURE_PALETTE_OFFSET, (uint32_t)offset)); //FIXME: Probably bad offset? I don't think this has to be shifted?
}

inline
uint32_t* xgu_set_texture_border_color(uint32_t* p, unsigned int texture_index, uint32_t color) {
    assert(texture_index < 4);
    return push_command_parameter(p, NV097_SET_TEXTURE_BORDER_COLOR + texture_index*64, color);
}

inline
uint32_t* xgu_set_texture_set_bump_env_mat(uint32_t* p, unsigned int texture_index, const float m[2*2]) {
    assert(texture_index < 4);
    return push_command_matrix2x2(p, NV097_SET_TEXTURE_SET_BUMP_ENV_MAT + texture_index*64, m);
}

inline
uint32_t* xgu_set_texture_set_bump_env_scale(uint32_t* p, unsigned int texture_index, float scale) {
    assert(texture_index < 4);
    return push_command_float(p, NV097_SET_TEXTURE_SET_BUMP_ENV_SCALE+ texture_index*64, scale);
}

inline
uint32_t* xgu_set_texture_set_bump_env_offset(uint32_t* p, unsigned int texture_index, float offset) {
    assert(texture_index < 4);
    return push_command_float(p, NV097_SET_TEXTURE_SET_BUMP_ENV_OFFSET+texture_index*64, offset);
}


/* ==== Immediate Mode ==== */

inline
uint32_t* xgu_vertex3f(uint32_t* p, float x, float y, float z) {
    p = push_command(p, NV097_SET_VERTEX3F, 3);
    p = push_float(p, x);
    p = push_float(p, y);
    p = push_float(p, z);
    return p;
}

inline
uint32_t* xgu_vertex4f(uint32_t* p, float x, float y, float z, float w) {
    p = push_command(p, NV097_SET_VERTEX4F, 4);
    p = push_float(p, x);
    p = push_float(p, y);
    p = push_float(p, z);
    p = push_float(p, w);
    return p;
}

inline
uint32_t* xgu_set_vertex_data2f(uint32_t* p, XguVertexArray index, float x, float y) {
    //FIXME: Why does this even have to exist? Can't we use parts of the 4F variant?
    p = push_command(p, NV097_SET_VERTEX_DATA2F_M + index * 2*4, 2);
    p = push_float(p, x);
    p = push_float(p, y);
    return p;
}

inline
uint32_t* xgu_set_vertex_data4f(uint32_t* p, XguVertexArray index, float x, float y, float z, float w) {
    p = push_command(p, NV097_SET_VERTEX_DATA4F_M + index * 4*4, 4);
    p = push_float(p, x);
    p = push_float(p, y);
    p = push_float(p, z);
    p = push_float(p, w);
    return p;
}

inline
uint32_t* xgu_set_vertex_data2s(uint32_t* p, XguVertexArray index, int16_t x, int16_t y) {
    p = push_command(p, NV097_SET_VERTEX_DATA2S + index * 1*4, 1);
    p = push_parameter(p, (uint16_t)y << 16 | (uint16_t)x);
    return p;
}

inline
uint32_t* xgu_set_vertex_data4ub(uint32_t* p, XguVertexArray index, uint8_t x, uint8_t y, uint8_t z, uint8_t w) {
    p = push_command(p, NV097_SET_VERTEX_DATA4UB + index * 1*4, 1);
    p = push_parameter(p, w << 24 | z << 16 | y << 8 | x);
    return p;
}

inline
uint32_t* xgu_set_vertex_data4s(uint32_t* p, XguVertexArray index, int16_t x, int16_t y, int16_t z, int16_t w) {
    p = push_command(p, NV097_SET_VERTEX_DATA4S_M + index * 2*4, 2);
    p = push_parameter(p, (uint16_t)y << 16 | (uint16_t)x);
    p = push_parameter(p, (uint16_t)w << 16 | (uint16_t)z);
    return p;
}

#endif
