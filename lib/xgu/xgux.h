#ifndef __XGUX_H
#define __XGUX_H

#include "xgu.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX_BATCH 120

inline
void xgux_draw_arrays(XguPrimitiveType mode, unsigned int start, unsigned int count) {
    uint32_t *p = pb_begin();
    p = xgu_begin(p, mode);
    p = xgu_draw_arrays(p, start, count);
    p = xgu_end(p);
    pb_end(p);
}

inline
void xgux_draw_elements16(XguPrimitiveType mode, const uint16_t* elements, unsigned int count) {
    uint32_t *p = pb_begin();
    p = xgu_begin(p, mode);

    /* Submit elements in pairs if possible */
    unsigned int pair_count = count / 2;
    for (unsigned int i = 0; i < pair_count; ) {

        if (i > 0) {
            /* Start next batch */
            pb_end(p);
            p = pb_begin();
        }

        unsigned int batch_pair_count = MIN(MAX_BATCH, pair_count - i);
        p = xgu_element16(p, &elements[i * 2], batch_pair_count * 2);

        i += batch_pair_count;
    }

    /* Submit final index if necessary */
    if (count % 2) {
        uint32_t index = elements[count - 1];
        p = xgu_element32(p, &index, 1);
    }

    p = xgu_end(p);
    pb_end(p);
}

inline
void xgux_draw_elements32(XguPrimitiveType mode, const uint32_t* elements, unsigned int count) {
    uint32_t *p = pb_begin();
    p = xgu_begin(p, mode);

    /* Submit elements */
    for (unsigned int i = 0; i < count; ) {

        if (i > 0) {
            /* Start next batch */
            pb_end(p);
            p = pb_begin();
        }

        unsigned int batch_count = MIN(MAX_BATCH, count - i);
        p = xgu_element32(p, elements, batch_count);

        i += batch_count;
    }

    p = xgu_end(p);
    pb_end(p);
}

inline
void xgux_set_clear_rect(unsigned int x, unsigned int y,
                         unsigned int width, unsigned int height) {
    uint32_t *p = pb_begin();
    p = xgu_set_clear_rect_horizontal(p, x, x+width);
    p = xgu_set_clear_rect_vertical(p, y, y+height);
    pb_end(p);
}

inline
void xgux_set_attrib_pointer(XguVertexArray index, XguVertexArrayType format, unsigned int size, unsigned int stride, const void* data) {
    uint32_t *p = pb_begin();
    p = xgu_set_vertex_data_array_format(p, index, format, size, stride);
    p = xgu_set_vertex_data_array_offset(p, index, (uint32_t)data & 0x03ffffff);
    pb_end(p);
}


inline
void xgux_set_transform_constant_vec4(int location, unsigned int count, const XguVec4* v) {
    uint32_t *p = pb_begin();
    p = xgu_set_transform_constant_load(p, 96 + location);
    p = xgu_set_transform_constant(p, v, count);
    pb_end(p);
}

inline
void xgux_set_transform_constant_matrix4x4(unsigned int location, unsigned int count, bool transpose, const XguMatrix4x4* m) {
    for(unsigned int i = 0; i < count; i++) {
        if (transpose) {
            XguMatrix4x4 t;
            for(unsigned int row = 0; row < 4; row++) {
                for(unsigned int col = 0; col < 4; col++) {
                    t.col[row].f[col] = m[i].col[col].f[row];
                }
            }
            xgux_set_transform_constant_vec4(location + i*4, 4, &t.col);
        } else {
            xgux_set_transform_constant_vec4(location + i*4, 4, &m[i].col);
        }
    }
}

#endif
