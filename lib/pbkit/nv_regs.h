#ifndef _NV_REGS_H_
#define _NV_REGS_H_

// clang-format off

// Indicates that all values being pushed in a pb_push* invocation should be
// sent to the same command.
//
// The default behavior is for pgraph commands that do not operate on fixed size
// arrays to treat multiple parameters as invocations of subsequent pgraph
// commands.
// E.g., NV097_INLINE_ARRAY expects a single parameter which is added to the
// inline array being built up in graphics memory. Rather than invoke pb_push1
// many times over and waste time/space re-sending the NV097_INLINE_ARRAY
// command, it is desirable to send multiple components (e.g., all 3 coordinates
// for a vertex) in a single push. Setting bit 30 (e.g., via
// NV2A_SUPPRESS_COMMAND_INCREMENT(NV097_INLINE_ARRAY)) instructs the pushbuffer
// to treat all the parameters in the command packet as part of the
// NV097_INLINE_ARRAY command.
// Failure to set bit 30 would utilize the default behavior, so the first
// parameter would go to NV097_INLINE_ARRAY, but the next would go to
// NV097_SET_EYE_VECTOR (0x0000181C), which itself will expect 3 parameters and
// will at best lead to unexpected behavior and potentially an exception being
// raised by the hardware depending on how many parameters are sent.
#define NV2A_SUPPRESS_COMMAND_INCREMENT(cmd) (0x40000000 | (cmd))

#define NV_KELVIN_PRIMITIVE                              0x0097
#   define NV097_NO_OPERATION                                 0x00000100
#   define NV097_WAIT_FOR_IDLE                                0x00000110
#   define NV097_SET_FLIP_READ                                0x00000120
#   define NV097_SET_FLIP_WRITE                               0x00000124
#   define NV097_SET_FLIP_MODULO                              0x00000128
#   define NV097_FLIP_INCREMENT_WRITE                         0x0000012C
#   define NV097_FLIP_STALL                                   0x00000130
#   define NV097_SET_CONTEXT_DMA_NOTIFIES                     0x00000180
#   define NV097_SET_CONTEXT_DMA_A                            0x00000184
#   define NV097_SET_CONTEXT_DMA_B                            0x00000188
#   define NV097_SET_CONTEXT_DMA_STATE                        0x00000190
#   define NV097_SET_CONTEXT_DMA_COLOR                        0x00000194
#   define NV097_SET_CONTEXT_DMA_ZETA                         0x00000198
#   define NV097_SET_CONTEXT_DMA_VERTEX_A                     0x0000019C
#   define NV097_SET_CONTEXT_DMA_VERTEX_B                     0x000001A0
#   define NV097_SET_CONTEXT_DMA_SEMAPHORE                    0x000001A4
#   define NV097_SET_CONTEXT_DMA_REPORT                       0x000001A8
#   define NV097_SET_SURFACE_CLIP_HORIZONTAL                  0x00000200
#       define NV097_SET_SURFACE_CLIP_HORIZONTAL_X                0x0000FFFF
#       define NV097_SET_SURFACE_CLIP_HORIZONTAL_WIDTH            0xFFFF0000
#   define NV097_SET_SURFACE_CLIP_VERTICAL                    0x00000204
#       define NV097_SET_SURFACE_CLIP_VERTICAL_Y                  0x0000FFFF
#       define NV097_SET_SURFACE_CLIP_VERTICAL_HEIGHT             0xFFFF0000
#   define NV097_SET_SURFACE_FORMAT                           0x00000208
#       define NV097_SET_SURFACE_FORMAT_COLOR                     0x0000000F
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X1R5G5B5_Z1R5G5B5     0x01
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X1R5G5B5_O1R5G5B5     0x02
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_R5G6B5                0x03
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X8R8G8B8_Z8R8G8B8     0x04
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X8R8G8B8_O8R8G8B8     0x05
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X1A7R8G8B8_Z1A7R8G8B8 0x06
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_X1A7R8G8B8_O1A7R8G8B8 0x07
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_A8R8G8B8              0x08
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_B8                    0x09
#           define NV097_SET_SURFACE_FORMAT_COLOR_LE_G8B8                  0x0A
#       define NV097_SET_SURFACE_FORMAT_ZETA                      0x000000F0
#           define NV097_SET_SURFACE_FORMAT_ZETA_Z16                       1
#           define NV097_SET_SURFACE_FORMAT_ZETA_Z24S8                     2
#       define NV097_SET_SURFACE_FORMAT_TYPE                      0x00000F00
#           define NV097_SET_SURFACE_FORMAT_TYPE_PITCH                     0x1
#           define NV097_SET_SURFACE_FORMAT_TYPE_SWIZZLE                   0x2
#       define NV097_SET_SURFACE_FORMAT_ANTI_ALIASING             0x0000F000
#           define NV097_SET_SURFACE_FORMAT_ANTI_ALIASING_CENTER_1         0
#           define NV097_SET_SURFACE_FORMAT_ANTI_ALIASING_CENTER_CORNER_2  1
#           define NV097_SET_SURFACE_FORMAT_ANTI_ALIASING_SQUARE_OFFSET_4  2
#       define NV097_SET_SURFACE_FORMAT_WIDTH                     0x00FF0000
#       define NV097_SET_SURFACE_FORMAT_HEIGHT                    0xFF000000
#   define NV097_SET_SURFACE_PITCH                            0x0000020C
#       define NV097_SET_SURFACE_PITCH_COLOR                      0x0000FFFF
#       define NV097_SET_SURFACE_PITCH_ZETA                       0xFFFF0000
#   define NV097_SET_SURFACE_COLOR_OFFSET                     0x00000210
#   define NV097_SET_SURFACE_ZETA_OFFSET                      0x00000214
#   define NV097_SET_COMBINER_ALPHA_ICW                       0x00000260
#       define NV097_SET_COMBINER_ALPHA_ICW_A_MAP                 0xE0000000
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_UNSIGNED_IDENTITY 0
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_UNSIGNED_INVERT   1
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_EXPAND_NORMAL     2
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_EXPAND_NEGATE     3
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_HALFBIAS_NORMAL   4
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_HALFBIAS_NEGATE   5
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_SIGNED_IDENTITY   6
#           define NV097_SET_COMBINER_ALPHA_ICW_A_MAP_SIGNED_NEGATE     7
#       define NV097_SET_COMBINER_ALPHA_ICW_A_ALPHA               (1<<28)
#       define NV097_SET_COMBINER_ALPHA_ICW_A_SOURCE              0x0F000000
#       define NV097_SET_COMBINER_ALPHA_ICW_B_MAP                 0x00E00000
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_UNSIGNED_IDENTITY 0
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_UNSIGNED_INVERT   1
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_EXPAND_NORMAL     2
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_EXPAND_NEGATE     3
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_HALFBIAS_NORMAL   4
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_HALFBIAS_NEGATE   5
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_SIGNED_IDENTITY   6
#           define NV097_SET_COMBINER_ALPHA_ICW_B_MAP_SIGNED_NEGATE     7
#       define NV097_SET_COMBINER_ALPHA_ICW_B_ALPHA               (1<<20)
#       define NV097_SET_COMBINER_ALPHA_ICW_B_SOURCE              0x000F0000
#       define NV097_SET_COMBINER_ALPHA_ICW_C_MAP                 0x0000E000
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_UNSIGNED_IDENTITY 0
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_UNSIGNED_INVERT   1
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_EXPAND_NORMAL     2
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_EXPAND_NEGATE     3
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_HALFBIAS_NORMAL   4
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_HALFBIAS_NEGATE   5
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_SIGNED_IDENTITY   6
#           define NV097_SET_COMBINER_ALPHA_ICW_C_MAP_SIGNED_NEGATE     7
#       define NV097_SET_COMBINER_ALPHA_ICW_C_ALPHA               (1<<12)
#       define NV097_SET_COMBINER_ALPHA_ICW_C_SOURCE              0x00000F00
#       define NV097_SET_COMBINER_ALPHA_ICW_D_MAP                 0x000000E0
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_UNSIGNED_IDENTITY 0
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_UNSIGNED_INVERT   1
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_EXPAND_NORMAL     2
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_EXPAND_NEGATE     3
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_HALFBIAS_NORMAL   4
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_HALFBIAS_NEGATE   5
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_SIGNED_IDENTITY   6
#           define NV097_SET_COMBINER_ALPHA_ICW_D_MAP_SIGNED_NEGATE     7
#       define NV097_SET_COMBINER_ALPHA_ICW_D_ALPHA               (1<<4)
#       define NV097_SET_COMBINER_ALPHA_ICW_D_SOURCE              0x0000000F
#   define NV097_SET_COMBINER_SPECULAR_FOG_CW0                0x00000288
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_A_INVERSE      0xE0000000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_A_ALPHA        (1<<28)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_A_SOURCE       0x0F000000
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_A_SOURCE_REG_SPECLIT 0xE
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_A_SOURCE_REG_EF_PROD 0xF
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_B_INVERSE      0x00E00000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_B_ALPHA        (1<<20)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_B_SOURCE       0x000F0000
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_B_SOURCE_REG_SPECLIT 0xE
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_B_SOURCE_REG_EF_PROD 0xF
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_C_INVERSE      0x0000E000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_C_ALPHA        (1<<12)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_C_SOURCE       0x00000F00
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_C_SOURCE_REG_SPECLIT 0xE
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_C_SOURCE_REG_EF_PROD 0xF
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_D_INVERSE      0x000000E0
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_D_ALPHA        (1<<4)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW0_D_SOURCE       0x0000000F
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_D_SOURCE_REG_SPECLIT 0xE
#           define NV097_SET_COMBINER_SPECULAR_FOG_CW0_D_SOURCE_REG_EF_PROD 0xF
#   define NV097_SET_COMBINER_SPECULAR_FOG_CW1                0x0000028C
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_E_INVERSE      0xE0000000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_E_ALPHA        (1<<28)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_E_SOURCE       0x0F000000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_F_INVERSE      0x00E00000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_F_ALPHA        (1<<20)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_F_SOURCE       0x000F0000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_G_INVERSE      0x0000E000
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_G_ALPHA        (1<<12)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_G_SOURCE       0x00000F00
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_SPECULAR_CLAMP (1<<7)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_SPECULAR_ADD_INVERT_R5 (1<<6)
#       define NV097_SET_COMBINER_SPECULAR_FOG_CW1_SPECULAR_ADD_INVERT_R12 0x0000003F
#   define NV097_SET_CONTROL0                                 0x00000290
#       define NV097_SET_CONTROL0_STENCIL_WRITE_ENABLE            (1 << 0)
#       define NV097_SET_CONTROL0_Z_FORMAT                        (1 << 12)
#       define NV097_SET_CONTROL0_Z_FORMAT_FIXED                  0
#       define NV097_SET_CONTROL0_Z_FORMAT_FLOAT                  NV097_SET_CONTROL0_Z_FORMAT
#       define NV097_SET_CONTROL0_Z_PERSPECTIVE_ENABLE            (1 << 16)
#       define NV097_SET_CONTROL0_COLOR_SPACE_CONVERT             0xF0000000
#       define NV097_SET_CONTROL0_COLOR_SPACE_CONVERT_CRYCB_TO_RGB (0x10 << 24)
#   define NV097_SET_LIGHT_CONTROL                            0x00000294
#       define NV097_SET_LIGHT_CONTROL_V_INCLUDE_SPECULAR         0
#       define NV097_SET_LIGHT_CONTROL_V_SEPARATE_SPECULAR        1
#       define NV097_SET_LIGHT_CONTROL_V_LOCALEYE                 (1 << 16)
#       define NV097_SET_LIGHT_CONTROL_V_ALPHA_OPAQUE             0
#       define NV097_SET_LIGHT_CONTROL_V_ALPHA_FROM_MATERIAL_SPECULAR (1 << 17)
#   define NV097_SET_COLOR_MATERIAL 					      0x00000298
#       define NV097_SET_COLOR_MATERIAL_ALL_FROM_MATERIAL                  0
#       define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_MATERIAL             0
#       define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_VERTEX_DIFFUSE       (1 << 0)
#       define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_VERTEX_SPECULAR      (2 << 0)
#       define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_MATERIAL              0
#       define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_VERTEX_DIFFUSE        (1 << 2)
#       define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_VERTEX_SPECULAR       (2 << 2)
#       define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_MATERIAL              0
#       define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_VERTEX_DIFFUSE        (1 << 4)
#       define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_VERTEX_SPECULAR       (2 << 4)
#       define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_MATERIAL             0
#       define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_VERTEX_DIFFUSE       (1 << 6)
#       define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_VERTEX_SPECULAR      (2 << 6)
#       define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_MATERIAL        0
#       define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_VERTEX_DIFFUSE  (1 << 8)
#       define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_VERTEX_SPECULAR (2 << 8)
#       define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_MATERIAL         0
#       define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_VERTEX_DIFFUSE   (1 << 10)
#       define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_VERTEX_SPECULAR  (2 << 10)
#       define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_MATERIAL         0
#       define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_VERTEX_DIFFUSE   (1 << 12)
#       define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_VERTEX_SPECULAR  (2 << 12)
#       define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_MATERIAL        0
#       define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_VERTEX_DIFFUSE  (1 << 14)
#       define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_VERTEX_SPECULAR (2 << 14)
#   define NV097_SET_FOG_MODE                                 0x0000029C
#       define NV097_SET_FOG_MODE_V_LINEAR                        0x2601
#       define NV097_SET_FOG_MODE_V_EXP                           0x800
#       define NV097_SET_FOG_MODE_V_EXP2                          0x801
#       define NV097_SET_FOG_MODE_V_EXP_ABS                       0x802
#       define NV097_SET_FOG_MODE_V_EXP2_ABS                      0x803
#       define NV097_SET_FOG_MODE_V_LINEAR_ABS                    0x804
#   define NV097_SET_FOG_GEN_MODE                             0x000002A0
#       define NV097_SET_FOG_GEN_MODE_V_SPEC_ALPHA                0
#       define NV097_SET_FOG_GEN_MODE_V_RADIAL                    1
#       define NV097_SET_FOG_GEN_MODE_V_PLANAR                    2
#       define NV097_SET_FOG_GEN_MODE_V_ABS_PLANAR                3
#       define NV097_SET_FOG_GEN_MODE_V_FOG_X                     6
#   define NV097_SET_FOG_ENABLE                               0x000002A4
#   define NV097_SET_FOG_COLOR                                0x000002A8
#       define NV097_SET_FOG_COLOR_RED                            0x000000FF
#       define NV097_SET_FOG_COLOR_GREEN                          0x0000FF00
#       define NV097_SET_FOG_COLOR_BLUE                           0x00FF0000
#       define NV097_SET_FOG_COLOR_ALPHA                          0xFF000000
#   define NV097_SET_WINDOW_CLIP_TYPE                         0x000002B4
#   define NV097_SET_WINDOW_CLIP_HORIZONTAL                   0x000002C0
#   define NV097_SET_WINDOW_CLIP_VERTICAL                     0x000002E0
#   define NV097_SET_ALPHA_TEST_ENABLE                        0x00000300
#   define NV097_SET_BLEND_ENABLE                             0x00000304
#   define NV097_SET_CULL_FACE_ENABLE                         0x00000308
#   define NV097_SET_DEPTH_TEST_ENABLE                        0x0000030C
#   define NV097_SET_DITHER_ENABLE                            0x00000310
#   define NV097_SET_LIGHTING_ENABLE                          0x00000314
#   define NV097_SET_POINT_PARAMS_ENABLE                      0x00000318
#   define NV097_SET_POINT_SMOOTH_ENABLE                      0x0000031C
#   define NV097_SET_LINE_SMOOTH_ENABLE                       0x00000320
#   define NV097_SET_POLY_SMOOTH_ENABLE                       0x00000324
#   define NV097_SET_SKIN_MODE                                0x00000328
#       define NV097_SET_SKIN_MODE_OFF                            0
#       define NV097_SET_SKIN_MODE_2G                             1
#       define NV097_SET_SKIN_MODE_2                              2
#       define NV097_SET_SKIN_MODE_3G                             3
#       define NV097_SET_SKIN_MODE_3                              4
#       define NV097_SET_SKIN_MODE_4G                             5
#       define NV097_SET_SKIN_MODE_4                              6
#   define NV097_SET_STENCIL_TEST_ENABLE                      0x0000032C
#   define NV097_SET_POLY_OFFSET_POINT_ENABLE                 0x00000330
#   define NV097_SET_POLY_OFFSET_LINE_ENABLE                  0x00000334
#   define NV097_SET_POLY_OFFSET_FILL_ENABLE                  0x00000338
#   define NV097_SET_ALPHA_FUNC                               0x0000033C
#       define NV097_SET_ALPHA_FUNC_V_NEVER                       0x0200
#       define NV097_SET_ALPHA_FUNC_V_LESS                        0x0201
#       define NV097_SET_ALPHA_FUNC_V_EQUAL                       0x0202
#       define NV097_SET_ALPHA_FUNC_V_LEQUAL                      0x0203
#       define NV097_SET_ALPHA_FUNC_V_GREATER                     0x0204
#       define NV097_SET_ALPHA_FUNC_V_NOTEQUAL                    0x0205
#       define NV097_SET_ALPHA_FUNC_V_GEQUAL                      0x0206
#       define NV097_SET_ALPHA_FUNC_V_ALWAYS                      0x0207
#   define NV097_SET_ALPHA_REF                                0x00000340
#   define NV097_SET_BLEND_FUNC_SFACTOR                       0x00000344
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ZERO                0x0000
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE                 0x0001
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_COLOR           0x0300
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_SRC_COLOR 0x0301
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_ALPHA           0x0302
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_SRC_ALPHA 0x0303
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_DST_ALPHA           0x0304
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_DST_ALPHA 0x0305
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_DST_COLOR           0x0306
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_DST_COLOR 0x0307
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_SRC_ALPHA_SATURATE  0x0308
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_CONSTANT_COLOR      0x8001
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_CONSTANT_COLOR 0x8002
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_CONSTANT_ALPHA      0x8003
#       define NV097_SET_BLEND_FUNC_SFACTOR_V_ONE_MINUS_CONSTANT_ALPHA 0x8004
#   define NV097_SET_BLEND_FUNC_DFACTOR                       0x00000348
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ZERO                0x0000
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE                 0x0001
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_SRC_COLOR           0x0300
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_SRC_COLOR 0x0301
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_SRC_ALPHA           0x0302
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_SRC_ALPHA 0x0303
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_DST_ALPHA           0x0304
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_DST_ALPHA 0x0305
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_DST_COLOR           0x0306
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_DST_COLOR 0x0307
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_SRC_ALPHA_SATURATE  0x0308
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_CONSTANT_COLOR      0x8001
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_CONSTANT_COLOR 0x8002
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_CONSTANT_ALPHA      0x8003
#       define NV097_SET_BLEND_FUNC_DFACTOR_V_ONE_MINUS_CONSTANT_ALPHA 0x8004
#   define NV097_SET_BLEND_COLOR                              0x0000034C
#   define NV097_SET_BLEND_EQUATION                           0x00000350
#       define NV097_SET_BLEND_EQUATION_V_FUNC_SUBTRACT           0x800A
#       define NV097_SET_BLEND_EQUATION_V_FUNC_REVERSE_SUBTRACT   0x800B
#       define NV097_SET_BLEND_EQUATION_V_FUNC_ADD                0x8006
#       define NV097_SET_BLEND_EQUATION_V_MIN                     0x8007
#       define NV097_SET_BLEND_EQUATION_V_MAX                     0x8008
#       define NV097_SET_BLEND_EQUATION_V_FUNC_REVERSE_SUBTRACT_SIGNED 0xF005
#       define NV097_SET_BLEND_EQUATION_V_FUNC_ADD_SIGNED         0xF006
#   define NV097_SET_DEPTH_FUNC                               0x00000354
#       define NV097_SET_DEPTH_FUNC_V_NEVER                       0x00000200
#       define NV097_SET_DEPTH_FUNC_V_LESS                        0x00000201
#       define NV097_SET_DEPTH_FUNC_V_EQUAL                       0x00000202
#       define NV097_SET_DEPTH_FUNC_V_LEQUAL                      0x00000203
#       define NV097_SET_DEPTH_FUNC_V_GREATER                     0x00000204
#       define NV097_SET_DEPTH_FUNC_V_NOTEQUAL                    0x00000205
#       define NV097_SET_DEPTH_FUNC_V_GEQUAL                      0x00000206
#       define NV097_SET_DEPTH_FUNC_V_ALWAYS                      0x00000207
#   define NV097_SET_COLOR_MASK                               0x00000358
#       define NV097_SET_COLOR_MASK_BLUE_WRITE_ENABLE             (1 << 0)
#       define NV097_SET_COLOR_MASK_GREEN_WRITE_ENABLE            (1 << 8)
#       define NV097_SET_COLOR_MASK_RED_WRITE_ENABLE              (1 << 16)
#       define NV097_SET_COLOR_MASK_ALPHA_WRITE_ENABLE            (1 << 24)
#   define NV097_SET_DEPTH_MASK                               0x0000035C
#   define NV097_SET_STENCIL_MASK                             0x00000360
#   define NV097_SET_STENCIL_FUNC                             0x00000364
#   define NV097_SET_STENCIL_FUNC_REF                         0x00000368
#   define NV097_SET_STENCIL_FUNC_MASK                        0x0000036C
#   define NV097_SET_STENCIL_OP_FAIL                          0x00000370
#   define NV097_SET_STENCIL_OP_ZFAIL                         0x00000374
#   define NV097_SET_STENCIL_OP_ZPASS                         0x00000378
#       define NV097_SET_STENCIL_OP_V_KEEP                        0x1E00
#       define NV097_SET_STENCIL_OP_V_ZERO                        0x0000
#       define NV097_SET_STENCIL_OP_V_REPLACE                     0x1E01
#       define NV097_SET_STENCIL_OP_V_INCRSAT                     0x1E02
#       define NV097_SET_STENCIL_OP_V_DECRSAT                     0x1E03
#       define NV097_SET_STENCIL_OP_V_INVERT                      0x150A
#       define NV097_SET_STENCIL_OP_V_INCR                        0x8507
#       define NV097_SET_STENCIL_OP_V_DECR                        0x8508
#   define NV097_SET_SHADE_MODEL                              0x0000037C
#       define NV097_SET_SHADE_MODEL_FLAT                         0x1D00
#       define NV097_SET_SHADE_MODEL_SMOOTH                       0x1D01
#   define NV097_SET_LINE_WIDTH                               0x00000380
#       define NV097_SET_LINE_WIDTH_MAX_VALUE                     (63 << 3)
#       define NV097_SET_LINE_WIDTH_MASK                          ((64 << 3) - 1)
#   define NV097_SET_POLYGON_OFFSET_SCALE_FACTOR              0x00000384
#   define NV097_SET_POLYGON_OFFSET_BIAS                      0x00000388
#   define NV097_SET_FRONT_POLYGON_MODE                       0x0000038C
#       define NV097_SET_FRONT_POLYGON_MODE_V_POINT               0x1B00
#       define NV097_SET_FRONT_POLYGON_MODE_V_LINE                0x1B01
#       define NV097_SET_FRONT_POLYGON_MODE_V_FILL                0x1B02
#   define NV097_SET_BACK_POLYGON_MODE                        0x00000390
#   define NV097_SET_CLIP_MIN                                 0x00000394
#   define NV097_SET_CLIP_MAX                                 0x00000398
#   define NV097_SET_CULL_FACE                                0x0000039C
#       define NV097_SET_CULL_FACE_V_FRONT                         0x404
#       define NV097_SET_CULL_FACE_V_BACK                          0x405
#       define NV097_SET_CULL_FACE_V_FRONT_AND_BACK                0x408
#   define NV097_SET_FRONT_FACE                               0x000003A0
#       define NV097_SET_FRONT_FACE_V_CW                           0x900
#       define NV097_SET_FRONT_FACE_V_CCW                          0x901
#   define NV097_SET_NORMALIZATION_ENABLE                     0x000003A4
#   define NV097_SET_MATERIAL_EMISSION                        0x000003A8
#   define NV097_SET_MATERIAL_ALPHA                           0x000003B4
#   define NV097_SET_SPECULAR_ENABLE                          0x000003B8
#   define NV097_SET_LIGHT_ENABLE_MASK                        0x000003BC
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_OFF           0
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_INFINITE      1
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_LOCAL         2
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT0_SPOT          3
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_OFF           0
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_INFINITE      (1 << 2)
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_LOCAL         (2 << 2)
#           define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_SPOT          (3 << 2)
#   define NV097_SET_TEXGEN_S                                 0x000003C0
#       define NV097_SET_TEXGEN_S_DISABLE                         0x0000
#       define NV097_SET_TEXGEN_S_EYE_LINEAR                      0x2400
#       define NV097_SET_TEXGEN_S_OBJECT_LINEAR                   0x2401
#       define NV097_SET_TEXGEN_S_SPHERE_MAP                      0x2402
#       define NV097_SET_TEXGEN_S_REFLECTION_MAP                  0x8512
#       define NV097_SET_TEXGEN_S_NORMAL_MAP                      0x8511
#   define NV097_SET_TEXGEN_T                                 0x000003C4
#   define NV097_SET_TEXGEN_R                                 0x000003C8
#   define NV097_SET_TEXGEN_Q                                 0x000003CC
#   define NV097_SET_TEXTURE_MATRIX_ENABLE                    0x00000420
#   define NV097_SET_POINT_SIZE                               0x0000043C
#   define NV097_SET_PROJECTION_MATRIX                        0x00000440
#   define NV097_SET_MODEL_VIEW_MATRIX                        0x00000480
#   define NV097_SET_INVERSE_MODEL_VIEW_MATRIX                0x00000580
#   define NV097_SET_COMPOSITE_MATRIX                         0x00000680
#   define NV097_SET_TEXTURE_MATRIX                           0x000006C0
#   define NV097_SET_FOG_PARAMS                               0x000009C0
#   define NV097_SET_TEXGEN_PLANE_S                           0x00000840
#   define NV097_SET_TEXGEN_PLANE_T                           0x00000850
#   define NV097_SET_TEXGEN_PLANE_R                           0x00000860
#   define NV097_SET_TEXGEN_PLANE_Q                           0x00000870
#   define NV097_SET_TEXGEN_VIEW_MODEL                        0x000009CC
#       define NV097_SET_TEXGEN_VIEW_MODEL_LOCAL_VIEWER           0
#       define NV097_SET_TEXGEN_VIEW_MODEL_INFINITE_VIEWER        1
#   define NV097_SET_FOG_PLANE                                0x000009D0
#   define NV097_SET_FLAT_SHADE_OP                            0x000009FC
#       define NV097_SET_FLAT_SHADE_OP_VERTEX_LAST                0
#       define NV097_SET_FLAT_SHADE_OP_VERTEX_FIRST               1
#   define NV097_SET_SCENE_AMBIENT_COLOR                      0x00000A10
#   define NV097_SET_VIEWPORT_OFFSET                          0x00000A20
#   define NV097_SET_POINT_PARAMS                             0x00000A30
#   define NV097_SET_POINT_PARAMS_SCALE_FACTOR_A              0x00000A30
#   define NV097_SET_POINT_PARAMS_SCALE_FACTOR_B              0x00000A34
#   define NV097_SET_POINT_PARAMS_SCALE_FACTOR_C              0x00000A38
#   define NV097_SET_POINT_PARAMS_SIZE_RANGE                  0x00000A3C
#   define NV097_SET_POINT_PARAMS_SIZE_RANGE_DUP_1            0x00000A40
#   define NV097_SET_POINT_PARAMS_SIZE_RANGE_DUP_2            0x00000A44
#   define NV097_SET_POINT_PARAMS_SCALE_BIAS                  0x00000A48
#   define NV097_SET_POINT_PARAMS_MIN_SIZE                    0x00000A4C
#   define NV097_SET_EYE_POSITION                             0x00000A50
#   define NV097_SET_COMBINER_FACTOR0                         0x00000A60
#   define NV097_SET_COMBINER_FACTOR1                         0x00000A80
#   define NV097_SET_COMBINER_ALPHA_OCW                       0x00000AA0
#       define NV097_SET_COMBINER_ALPHA_OCW_OP                    0xFFFF8000
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_NOSHIFT           0
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_NOSHIFT_BIAS      1
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_SHIFTLEFTBY1      2
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_SHIFTLEFTBY1_BIAS 3
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_SHIFTLEFTBY2      4
#           define NV097_SET_COMBINER_ALPHA_OCW_OP_SHIFTRIGHTBY1     6
#       define NV097_SET_COMBINER_ALPHA_OCW_MUX_ENABLE            (1<<14)
#       define NV097_SET_COMBINER_ALPHA_OCW_SUM_DST               0x00000F00
#       define NV097_SET_COMBINER_ALPHA_OCW_AB_DST                0x000000F0
#       define NV097_SET_COMBINER_ALPHA_OCW_CD_DST                0x0000000F
#   define NV097_SET_COMBINER_COLOR_ICW                       0x00000AC0
#       define NV097_SET_COMBINER_COLOR_ICW_A_MAP                 0xE0000000
#       define NV097_SET_COMBINER_COLOR_ICW_A_ALPHA               (1<<28)
#       define NV097_SET_COMBINER_COLOR_ICW_A_SOURCE              0x0F000000
#       define NV097_SET_COMBINER_COLOR_ICW_B_MAP                 0x00E00000
#       define NV097_SET_COMBINER_COLOR_ICW_B_ALPHA               (1<<20)
#       define NV097_SET_COMBINER_COLOR_ICW_B_SOURCE              0x000F0000
#       define NV097_SET_COMBINER_COLOR_ICW_C_MAP                 0x0000E000
#       define NV097_SET_COMBINER_COLOR_ICW_C_ALPHA               (1<<12)
#       define NV097_SET_COMBINER_COLOR_ICW_C_SOURCE              0x00000F00
#       define NV097_SET_COMBINER_COLOR_ICW_D_MAP                 0x000000E0
#       define NV097_SET_COMBINER_COLOR_ICW_D_ALPHA               (1<<4)
#       define NV097_SET_COMBINER_COLOR_ICW_D_SOURCE              0x0000000F
#   define NV097_SET_COLOR_KEY_COLOR                          0x00000AE0
#   define NV097_SET_VIEWPORT_SCALE                           0x00000AF0
#   define NV097_SET_TRANSFORM_PROGRAM                        0x00000B00
#   define NV097_SET_TRANSFORM_CONSTANT                       0x00000B80
#   define NV097_SET_VERTEX3F                                 0x00001500
#   define NV097_SET_BACK_LIGHT_AMBIENT_COLOR                 0x00000C00
#   define NV097_SET_BACK_LIGHT_DIFFUSE_COLOR                 0x00000C0C
#   define NV097_SET_BACK_LIGHT_SPECULAR_COLOR                0x00000C18
#   define NV097_SET_LIGHT_AMBIENT_COLOR                      0x00001000
#   define NV097_SET_LIGHT_DIFFUSE_COLOR                      0x0000100C
#   define NV097_SET_LIGHT_SPECULAR_COLOR                     0x00001018
#   define NV097_SET_LIGHT_LOCAL_RANGE                        0x00001024
#   define NV097_SET_LIGHT_INFINITE_HALF_VECTOR               0x00001028
#   define NV097_SET_LIGHT_INFINITE_DIRECTION                 0x00001034
#   define NV097_SET_LIGHT_SPOT_FALLOFF                       0x00001040
#   define NV097_SET_LIGHT_SPOT_DIRECTION                     0x0000104C
#   define NV097_SET_LIGHT_LOCAL_POSITION                     0x0000105C
#   define NV097_SET_LIGHT_LOCAL_ATTENUATION                  0x00001068
#   define NV097_SET_STIPPLE_ENABLE                           0x0000147C
#   define NV097_SET_STIPPLE_PATERN_0                         0x00001480
#       define NV097_SET_STIPPLE_PATERN_SIZE                      32
#   define NV097_SET_VERTEX4F                                 0x00001518
#   define NV097_SET_WEIGHT1F                                 0x0000169C
#   define NV097_SET_EDGE_FLAG                                0x000016BC
#   define NV097_SET_WEIGHT4F                                 0x000016C0
#   define NV097_SET_NORMAL3F                                 0x00001530
#   define NV097_SET_NORMAL3S                                 0x00001540
#   define NV097_SET_DIFFUSE_COLOR4F                          0x00001550
#   define NV097_SET_DIFFUSE_COLOR3F                          0x00001560
#   define NV097_SET_DIFFUSE_COLOR4I                          0x0000156C
#   define NV097_SET_SPECULAR_COLOR4F                         0x00001570
#   define NV097_SET_SPECULAR_COLOR3F                         0x00001580
#   define NV097_SET_SPECULAR_COLOR4I                         0x0000158C
#   define NV097_SET_FOG_COORD                                0x00001698
#   define NV097_SET_TEXCOORD0_2F                             0x00001590
#   define NV097_SET_TEXCOORD0_4F                             0x000015A0
#   define NV097_SET_TEXCOORD0_2S                             0x00001598
#   define NV097_SET_TEXCOORD0_4S                             0x000015B0
#   define NV097_SET_TEXCOORD1_2F                             0x000015B8
#   define NV097_SET_TEXCOORD1_4F                             0x000015C8
#   define NV097_SET_TEXCOORD1_2S                             0x000015C0
#   define NV097_SET_TEXCOORD1_4S                             0x000015D8
#   define NV097_SET_TEXCOORD2_2F                             0x000015E0
#   define NV097_SET_TEXCOORD2_4F                             0x000015F0
#   define NV097_SET_TEXCOORD2_2S                             0x000015E8
#   define NV097_SET_TEXCOORD2_4S                             0x00001600
#   define NV097_SET_TEXCOORD3_2F                             0x00001608
#   define NV097_SET_TEXCOORD3_4F                             0x00001620
#   define NV097_SET_TEXCOORD3_2S                             0x00001610
#   define NV097_SET_TEXCOORD3_4S                             0x00001630
#   define NV097_BREAK_VERTEX_BUFFER_CACHE                    0x00001710
#   define NV097_SET_VERTEX_DATA_ARRAY_OFFSET                 0x00001720
#   define NV097_SET_VERTEX_DATA_ARRAY_FORMAT                 0x00001760
#       define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE            0x0000000F
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_UB_D3D     0
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_S1         1
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_F          2
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_UB_OGL     4
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_S32K       5
#           define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_TYPE_CMP        6
#       define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_SIZE            0x000000F0
#       define NV097_SET_VERTEX_DATA_ARRAY_FORMAT_STRIDE          0xFFFFFF00
#   define NV097_SET_BACK_MATERIAL_ALPHA                      0x000017AC
#   define NV097_SET_LOGIC_OP_ENABLE                          0x000017BC
#   define NV097_SET_LOGIC_OP                                 0x000017C0
#   define NV097_SET_LIGHT_TWO_SIDE_ENABLE                    0x000017C4
#   define NV097_CLEAR_REPORT_VALUE                           0x000017C8
#       define NV097_CLEAR_REPORT_VALUE_TYPE                      0xFFFFFFFF
#           define NV097_CLEAR_REPORT_VALUE_TYPE_ZPASS_PIXEL_CNT      1
#   define NV097_SET_ZPASS_PIXEL_COUNT_ENABLE                 0x000017CC
#   define NV097_GET_REPORT                                   0x000017D0
#       define NV097_GET_REPORT_OFFSET                            0x00FFFFFF
#       define NV097_GET_REPORT_TYPE                              0xFF000000
#           define NV097_GET_REPORT_TYPE_ZPASS_PIXEL_CNT              1
#   define NV097_SET_EYE_DIRECTION                            0x000017E0
#   define NV097_SET_SHADER_CLIP_PLANE_MODE                   0x000017F8
#   define NV097_SET_BEGIN_END                                0x000017FC
#       define NV097_SET_BEGIN_END_OP_END                         0x00
#       define NV097_SET_BEGIN_END_OP_POINTS                      0x01
#       define NV097_SET_BEGIN_END_OP_LINES                       0x02
#       define NV097_SET_BEGIN_END_OP_LINE_LOOP                   0x03
#       define NV097_SET_BEGIN_END_OP_LINE_STRIP                  0x04
#       define NV097_SET_BEGIN_END_OP_TRIANGLES                   0x05
#       define NV097_SET_BEGIN_END_OP_TRIANGLE_STRIP              0x06
#       define NV097_SET_BEGIN_END_OP_TRIANGLE_FAN                0x07
#       define NV097_SET_BEGIN_END_OP_QUADS                       0x08
#       define NV097_SET_BEGIN_END_OP_QUAD_STRIP                  0x09
#       define NV097_SET_BEGIN_END_OP_POLYGON                     0x0A
#   define NV097_ARRAY_ELEMENT16                              0x00001800
#   define NV097_ARRAY_ELEMENT32                              0x00001808
#   define NV097_DRAW_ARRAYS                                  0x00001810
#       define NV097_DRAW_ARRAYS_COUNT                            0xFF000000
#       define NV097_DRAW_ARRAYS_START_INDEX                      0x00FFFFFF
#   define NV097_INLINE_ARRAY                                 0x00001818
#   define NV097_SET_EYE_VECTOR                               0x0000181C
#   define NV097_SET_VERTEX_DATA2F_M                          0x00001880
#   define NV097_SET_VERTEX_DATA4F_M                          0x00001A00
#   define NV097_SET_VERTEX_DATA2S                            0x00001900
#   define NV097_SET_VERTEX_DATA4UB                           0x00001940
#   define NV097_SET_VERTEX_DATA4S_M                          0x00001980
#   define NV097_SET_TEXTURE_OFFSET                           0x00001B00
#   define NV097_SET_TEXTURE_FORMAT                           0x00001B04
#       define NV097_SET_TEXTURE_FORMAT_CONTEXT_DMA               0x00000003
#       define NV097_SET_TEXTURE_FORMAT_CUBEMAP_ENABLE            (1 << 2)
#       define NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE             (1 << 3)
#           define NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE_TEXTURE   0
#           define NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE_COLOR     1
#       define NV097_SET_TEXTURE_FORMAT_DIMENSIONALITY            0x000000F0
#       define NV097_SET_TEXTURE_FORMAT_COLOR                     0x0000FF00
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_Y8             0x00
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_AY8            0x01
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A1R5G5B5       0x02
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_X1R5G5B5       0x03
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A4R4G4B4       0x04
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R5G6B5         0x05
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8R8G8B8       0x06
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_X8R8G8B8       0x07
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_I8_A8R8G8B8    0x0B
#           define NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT1_A1R5G5B5   0x0C
#           define NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT23_A8R8G8B8  0x0E
#           define NV097_SET_TEXTURE_FORMAT_COLOR_L_DXT45_A8R8G8B8  0x0F
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A1R5G5B5 0x10
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_R5G6B5   0x11
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8R8G8B8 0x12
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_Y8       0x13
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_R8B8     0x16
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_G8B8     0x17
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8             0x19
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8Y8           0x1A
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_AY8      0x1B
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_X1R5G5B5 0x1C
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A4R4G4B4 0x1D
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_X8R8G8B8 0x1E
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8       0x1F
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8Y8     0x20
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8 0x24
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_YB8CR8YA8CB8 0x25
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R6G5B5         0x27
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_G8B8           0x28
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R8B8           0x29
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_DEPTH_Y16_FIXED 0x2C
# define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_DEPTH_X8_Y24_FIXED 0x2E
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_DEPTH_Y16_FIXED 0x30
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_DEPTH_Y16_FLOAT 0x31
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_Y16      0x35
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8B8G8R8       0x3A
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_B8G8R8A8       0x3B
#           define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_R8G8B8A8       0x3C
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_A8B8G8R8 0x3F
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_B8G8R8A8 0x40
#           define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_R8G8B8A8 0x41
#       define NV097_SET_TEXTURE_FORMAT_MIPMAP_LEVELS             0x000F0000
#       define NV097_SET_TEXTURE_FORMAT_BASE_SIZE_U               0x00F00000
#       define NV097_SET_TEXTURE_FORMAT_BASE_SIZE_V               0x0F000000
#       define NV097_SET_TEXTURE_FORMAT_BASE_SIZE_P               0xF0000000
#   define NV097_SET_TEXTURE_ADDRESS                          0x00001B08
#       define NV097_SET_TEXTURE_ADDRESS_U                        0x0000000F
#       define NV097_SET_TEXTURE_ADDRESS_CYLINDERWRAP_U           0x000000F0
#       define NV097_SET_TEXTURE_ADDRESS_V                        0x00000F00
#       define NV097_SET_TEXTURE_ADDRESS_CYLINDERWRAP_V           0x0000F000
#       define NV097_SET_TEXTURE_ADDRESS_P                        0x000F0000
#       define NV097_SET_TEXTURE_ADDRESS_CYLINDERWRAP_P           0x00F00000
#       define NV097_SET_TEXTURE_ADDRESS_CYLINDERWRAP_Q           0x0F000000
#   define NV097_SET_TEXTURE_CONTROL0                         0x00001B0C
#       define NV097_SET_TEXTURE_CONTROL0_ALPHA_KILL_ENABLE      (1 << 2)
#       define NV097_SET_TEXTURE_CONTROL0_COLOR_KEY_MODE         0x3
#       define NV097_SET_TEXTURE_CONTROL0_ENABLE                 (1 << 30)
#       define NV097_SET_TEXTURE_CONTROL0_MIN_LOD_CLAMP           0x3FFC0000
#       define NV097_SET_TEXTURE_CONTROL0_MAX_LOD_CLAMP           0x0003FFC0
#   define NV097_SET_TEXTURE_CONTROL1                         0x00001B10
#       define NV097_SET_TEXTURE_CONTROL1_IMAGE_PITCH             0xFFFF0000
#   define NV097_SET_TEXTURE_FILTER                           0x00001B14
#       define NV097_SET_TEXTURE_FILTER_MIPMAP_LOD_BIAS           0x00001FFF
#       define NV097_SET_TEXTURE_FILTER_MIN                       0x00FF0000
#       define NV097_SET_TEXTURE_FILTER_MAG                       0x0F000000
#       define NV097_SET_TEXTURE_FILTER_ASIGNED                   (1 << 28)
#       define NV097_SET_TEXTURE_FILTER_RSIGNED                   (1 << 29)
#       define NV097_SET_TEXTURE_FILTER_GSIGNED                   (1 << 30)
#       define NV097_SET_TEXTURE_FILTER_BSIGNED                   (1 << 31)
#   define NV097_SET_TEXTURE_IMAGE_RECT                       0x00001B1C
#       define NV097_SET_TEXTURE_IMAGE_RECT_WIDTH                 0xFFFF0000
#       define NV097_SET_TEXTURE_IMAGE_RECT_HEIGHT                0x0000FFFF
#   define NV097_SET_TEXTURE_PALETTE                          0x00001B20
#       define NV097_SET_TEXTURE_PALETTE_CONTEXT_DMA              (1 << 0)
#       define NV097_SET_TEXTURE_PALETTE_LENGTH                   0x0000000C
#         define NV097_SET_TEXTURE_PALETTE_LENGTH_256               0
#         define NV097_SET_TEXTURE_PALETTE_LENGTH_128               1
#         define NV097_SET_TEXTURE_PALETTE_LENGTH_64                2
#         define NV097_SET_TEXTURE_PALETTE_LENGTH_32                3
#       define NV097_SET_TEXTURE_PALETTE_OFFSET                   0xFFFFFFC0
#   define NV097_SET_TEXTURE_BORDER_COLOR                     0x00001B24
#   define NV097_SET_TEXTURE_SET_BUMP_ENV_MAT                 0x00001B28
#   define NV097_SET_TEXTURE_SET_BUMP_ENV_SCALE               0x00001B38
#   define NV097_SET_TEXTURE_SET_BUMP_ENV_OFFSET              0x00001B3C
#   define NV097_SET_SEMAPHORE_OFFSET                         0x00001D6C
#   define NV097_BACK_END_WRITE_SEMAPHORE_RELEASE             0x00001D70
#   define NV097_SET_ZMIN_MAX_CONTROL                         0x00001D78
#       define NV097_SET_ZMIN_MAX_CONTROL_CULL_NEAR_FAR           1
#       define NV097_SET_ZMIN_MAX_CONTROL_ZCLAMP_CULL             0
#       define NV097_SET_ZMIN_MAX_CONTROL_ZCLAMP_CLAMP            (1 << 4)
#       define NV097_SET_ZMIN_MAX_CONTROL_CULL_IGNORE_W           (1 << 8)
#   define NV097_SET_COMPRESS_ZBUFFER_EN                      0x00001D80
#   define NV097_SET_ZSTENCIL_CLEAR_VALUE                     0x00001D8C
#   define NV097_SET_COLOR_CLEAR_VALUE                        0x00001D90
#   define NV097_CLEAR_SURFACE                                0x00001D94
#       define NV097_CLEAR_SURFACE_Z                              (1 << 0)
#       define NV097_CLEAR_SURFACE_STENCIL                        (1 << 1)
#       define NV097_CLEAR_SURFACE_COLOR                          0x000000F0
#       define NV097_CLEAR_SURFACE_R                                (1 << 4)
#       define NV097_CLEAR_SURFACE_G                                (1 << 5)
#       define NV097_CLEAR_SURFACE_B                                (1 << 6)
#       define NV097_CLEAR_SURFACE_A                                (1 << 7)
#   define NV097_SET_CLEAR_RECT_HORIZONTAL                    0x00001D98
#   define NV097_SET_CLEAR_RECT_VERTICAL                      0x00001D9C
#   define NV097_SET_SPECULAR_FOG_FACTOR                      0x00001E20
#   define NV097_SET_COMBINER_COLOR_OCW                       0x00001E40
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_AB        0xFFF80000
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_AB_DISABLE  0
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_AB_AB_DST_ENABLE 1
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_CD        (1<<18)
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_CD_DISABLE  0
#       define NV097_SET_COMBINER_COLOR_OCW_BLUETOALPHA_CD_CD_DST_ENABLE 1
#       define NV097_SET_COMBINER_COLOR_OCW_OP                    0x00038000
#           define NV097_SET_COMBINER_COLOR_OCW_OP_NOSHIFT          0
#           define NV097_SET_COMBINER_COLOR_OCW_OP_NOSHIFT_BIAS     1
#           define NV097_SET_COMBINER_COLOR_OCW_OP_SHIFTLEFTBY1     2
#           define NV097_SET_COMBINER_COLOR_OCW_OP_SHIFTLEFTBY1_BIAS 3
#           define NV097_SET_COMBINER_COLOR_OCW_OP_SHIFTLEFTBY2     4
#           define NV097_SET_COMBINER_COLOR_OCW_OP_SHIFTRIGHTBY1    6
#       define NV097_SET_COMBINER_COLOR_OCW_MUX_ENABLE            (1 << 14)
#       define NV097_SET_COMBINER_COLOR_OCW_AB_DOT_ENABLE         (1 << 13)
#       define NV097_SET_COMBINER_COLOR_OCW_CD_DOT_ENABLE         (1<<12)
#       define NV097_SET_COMBINER_COLOR_OCW_SUM_DST               0x00000F00
#       define NV097_SET_COMBINER_COLOR_OCW_AB_DST                0x000000F0
#       define NV097_SET_COMBINER_COLOR_OCW_CD_DST                0x0000000F
#   define NV097_SET_COMBINER_CONTROL                         0x00001E60
#       define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT         0x000000FF
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_ONE   1
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_TWO   2
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_THREE 3
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_FOUR  4
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_FIVE  5
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_SIX   6
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_SEVEN 7
#           define NV097_SET_COMBINER_CONTROL_ITERATION_COUNT_EIGHT 8
#       define NV097_SET_COMBINER_CONTROL_MUX_SELECT              0x00000F00
#           define NV097_SET_COMBINER_CONTROL_MUX_SELECT_LSB        0
#           define NV097_SET_COMBINER_CONTROL_MUX_SELECT_MSB        1
#       define NV097_SET_COMBINER_CONTROL_FACTOR0                 0x0000F000
#           define NV097_SET_COMBINER_CONTROL_FACTOR0_SAME_FACTOR_ALL 0
#           define NV097_SET_COMBINER_CONTROL_FACTOR0_EACH_STAGE    1
#       define NV097_SET_COMBINER_CONTROL_FACTOR1                 0xFFFF0000
#           define NV097_SET_COMBINER_CONTROL_FACTOR1_SAME_FACTOR_ALL 0
#           define NV097_SET_COMBINER_CONTROL_FACTOR1_EACH_STAGE    1
#   define NV097_SET_SHADOW_COMPARE_FUNC                      0x00001E6C
#       define NV097_SET_SHADOW_COMPARE_FUNC_NEVER                0
#       define NV097_SET_SHADOW_COMPARE_FUNC_GREATER              1
#       define NV097_SET_SHADOW_COMPARE_FUNC_EQUAL                2
#       define NV097_SET_SHADOW_COMPARE_FUNC_GEQUAL               3
#       define NV097_SET_SHADOW_COMPARE_FUNC_LESS                 4
#       define NV097_SET_SHADOW_COMPARE_FUNC_NOTEQUAL             5
#       define NV097_SET_SHADOW_COMPARE_FUNC_LEQUAL               6
#       define NV097_SET_SHADOW_COMPARE_FUNC_ALWAYS               7
#   define NV097_SET_SHADOW_ZSLOPE_THRESHOLD                  0x00001E68
#   define NV097_SET_SHADER_STAGE_PROGRAM                     0x00001E70
#       define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0              0x0000001F
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_PROGRAM_NONE   0
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_2D_PROJECTIVE  1
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_3D_PROJECTIVE  2
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_CUBE_MAP       3
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_PASS_THROUGH   4
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE0_CLIP_PLANE     5
#       define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1              0x000003E0
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_PROGRAM_NONE   0x00
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_2D_PROJECTIVE  0x01
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_3D_PROJECTIVE  0x02
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_CUBE_MAP       0x03
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_PASS_THROUGH   0x04
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_CLIP_PLANE     0x05
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_BUMPENVMAP     0x06
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_BUMPENVMAP_LUMINANCE 0x07
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_DEPENDENT_AR   0x0F
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_DEPENDENT_GB   0x10
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE1_DOT_PRODUCT    0x11
#       define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2              0x00007C00
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_PROGRAM_NONE   0x00
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_2D_PROJECTIVE  0x01
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_3D_PROJECTIVE  0x02
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_CUBE_MAP       0x03
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_PASS_THROUGH   0x04
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_CLIP_PLANE     0x05
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_BUMPENVMAP     0x06
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_BUMPENVMAP_LUMINANCE 0x07
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_BRDF           0x08
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DOT_ST         0x09
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DOT_ZW         0x0A
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DOT_REFLECT_DIFFUSE 0x0B
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DEPENDENT_AR   0x0F
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DEPENDENT_GB   0x10
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE2_DOT_PRODUCT    0x11
#       define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3              0x000F8000
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_PROGRAM_NONE   0x00
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_2D_PROJECTIVE  0x01
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_3D_PROJECTIVE  0x02
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_CUBE_MAP       0x03
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_PASS_THROUGH   0x04
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_CLIP_PLANE     0x05
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_BUMPENVMAP     0x06
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_BUMPENVMAP_LUMINANCE 0x07
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_BRDF           0x08
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_ST         0x09
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_ZW         0x0A
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_REFLECT_SPECULAR 0x0C
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_STR_3D     0x0D
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_STR_CUBE   0x0E
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DEPENDENT_AR   0x0F
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DEPENDENT_GB   0x10
#           define NV097_SET_SHADER_STAGE_PROGRAM_STAGE3_DOT_REFLECT_SPECULAR_CONST 0x12
#   define NV097_SET_DOT_RGBMAPPING                           0X00001E74
#   define NV097_SET_SHADER_OTHER_STAGE_INPUT                 0x00001E78
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE1          0x0000FFFF
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE1_INSTAGE_0  0
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE2          0x000F0000
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE2_INSTAGE_0  0
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE2_INSTAGE_1  1
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE3          0x00F00000
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE3_INSTAGE_0  0
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE3_INSTAGE_1  1
#       define NV097_SET_SHADER_OTHER_STAGE_INPUT_STAGE3_INSTAGE_2  2
#   define NV097_SET_TRANSFORM_DATA                           0x00001E80
#   define NV097_LAUNCH_TRANSFORM_PROGRAM                     0x00001E90
#   define NV097_SET_TRANSFORM_EXECUTION_MODE                 0x00001E94
#       define NV097_SET_TRANSFORM_EXECUTION_MODE_MODE            0x00000003
#           define NV097_SET_TRANSFORM_EXECUTION_MODE_MODE_FIXED    0
#           define NV097_SET_TRANSFORM_EXECUTION_MODE_MODE_PROGRAM  2
#       define NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE      0xFFFFFFFC
#           define NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE_USER 0
#           define NV097_SET_TRANSFORM_EXECUTION_MODE_RANGE_MODE_PRIV 1
#   define NV097_SET_TRANSFORM_PROGRAM_CXT_WRITE_EN           0x00001E98
#   define NV097_SET_TRANSFORM_PROGRAM_LOAD                   0x00001E9C
#   define NV097_SET_TRANSFORM_PROGRAM_START                  0x00001EA0
#   define NV097_SET_TRANSFORM_CONSTANT_LOAD                  0x00001EA4

#endif // _NV_REGS_H_
