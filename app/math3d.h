//port of ooPo's ps2sdk math3d library

#ifndef _MATH3D_H_
#define _MATH3D_H_

typedef float VECTOR[4];
typedef float MATRIX[16];

//vector indices
#define	_X					0
#define	_Y					1
#define	_Z					2
#define	_W					3

//4x4 matrices indices
#define	_11					0
#define	_12					1
#define	_13					2
#define	_14					3
#define	_21					4
#define	_22					5
#define	_23					6
#define	_24					7
#define	_31					8
#define	_32					9
#define	_33					10
#define	_34					11
#define	_41					12
#define	_42					13
#define	_43					14
#define	_44					15


//vector functions

void vector_apply(VECTOR output, VECTOR input0, MATRIX input1);
// Multiply a vector by a matrix, returning a vector.

void vector_clamp(VECTOR output, VECTOR input0, float min, float max);
// Clamp a vector's values by cutting them off at a minimum and maximum value.

void vector_copy(VECTOR output, VECTOR input0);
// Copy a vector.

float vector_innerproduct(VECTOR input0, VECTOR input1);
// Calculate the inner product of two vectors. Returns a scalar value.

void vector_multiply(VECTOR output, VECTOR input0, VECTOR input1);
// Multiply two vectors together.

void vector_normalize(VECTOR output, VECTOR input0);
// Normalize a vector by determining its length and dividing its values by this value.

void vector_outerproduct(VECTOR output, VECTOR input0, VECTOR input1);
// Calculate the outer product of two vectors.

//matrices functions

void matrix_copy(MATRIX output, MATRIX input0);
// Copy a matrix.

void matrix_inverse(MATRIX output, MATRIX input0);
// Calculate the inverse of a matrix.

void matrix_multiply(MATRIX output, MATRIX input0, MATRIX input1);
// Multiply two matrices together.

void matrix_rotate(MATRIX output, MATRIX input0, VECTOR input1);
// Create a rotation matrix and apply it to the specified input matrix.

void matrix_scale(MATRIX output, MATRIX input0, VECTOR input1);
// Create a scaling matrix and apply it to the specified input matrix.

void matrix_translate(MATRIX output, MATRIX input0, VECTOR input1);
// Create a translation matrix and apply it to the specified input matrix.

void matrix_transpose(MATRIX output, MATRIX input0);
// Transpose a matrix.

void matrix_unit(MATRIX output);
// Create a unit matrix.

//creation functions

void create_local_world(MATRIX local_world, VECTOR translation, VECTOR rotation);
// Create a local_world matrix given a translation and rotation.
// Commonly used to describe an object's position and orientation.

void create_local_light(MATRIX local_light, VECTOR rotation);
// Create a local_light matrix given a rotation.
// Commonly used to transform an object's normals for lighting calculations.

void create_world_view(MATRIX world_view, VECTOR translation, VECTOR rotation);
// Create a world_view matrix given a translation and rotation.
// Commonly used to describe a camera's position and rotation.

void create_view_screen(MATRIX view_screen, float aspect, float left, float right, float bottom, float top, float near, float far);
// Create a view_screen matrix given an aspect and clipping plane values.
// Functionally similar to the opengl function: glFrustum()

void create_local_screen(MATRIX local_screen, MATRIX local_world, MATRIX world_view, MATRIX view_screen);
// Create a local_screen matrix given a local_world, world_view and view_screen matrix.
// Commonly used with vector_apply() to transform vertices for rendering.

 
#endif
