#ifndef MESA_CONFIG_H_INCLUDED
#define MESA_CONFIG_H_INCLUDED

#define MAX_VARYING                    0    /**< number of float[4] vectors */


/** For GL_NV_vertex_program (as implemented in NV2A) */
/*@{*/
#define MAX_NV_VERTEX_PROGRAM_INSTRUCTIONS 136
#define MAX_NV_VERTEX_PROGRAM_TEMPS         13
#define MAX_NV_VERTEX_PROGRAM_PARAMS       192
#define MAX_NV_VERTEX_PROGRAM_INPUTS        16
/*
 Actually Xbox has 11 outputs, however, the parser is using the GL spec and
 expects 8 units. From the GL spec (NV_vertex_program, section 2.14.1.5):

   "If the number of texture units supported is less than eight, the values of
    vertex result registers that do not correspond to existent texture units
    are ignored."

 So despite only having 4 texture units, we provide 8 texture units here.
 = 11 actual outputs (including 4 texture units) + 4 fake texture-units.
*/
#define MAX_NV_VERTEX_PROGRAM_OUTPUTS       (11+4)

#endif
