#ifndef VT_MATH3D_H
#define VT_MATH3D_H
/*  FILE: vt_math3d.h --                                        */
/*                                                              */
/*  Header file for vt_math3d.c: 3D Matrix and Vector           */
/*  operations                                                  */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
/*  --  Copyright (C) 1990,91,92 Larry Edwards              --  */
/*  --                                                      --  */
/*  --    Authors:  Larry Edwards, William L. Chapin        --  */
/*  ==========================================================  */

#include <vrj/vrjConfig.h>
#include <math.h>

#define epsilon 0.000001
#define EPSILON 0.01

#ifndef RAD2DEG
#define RAD2DEG (180.0 / M_PI)
#endif
#ifndef DEG2RAD
#define DEG2RAD (M_PI / 180.0)
#endif

enum { XAXIS = 0, YAXIS, ZAXIS };
enum { VX = 0, VY, VZ, VW, AZ = 3, EL, ROLL };
enum { PX = 0, PY, PZ, PW };
enum { Premult, Postmult };

typedef float vec2d[2];
typedef float vec3d[3];
typedef float vec4d[4];
typedef float vec5d[5];
typedef float vec6d[6];
typedef vec2d pos2d;
typedef vec3d pos3d;
typedef vec4d pos4d;
typedef vec5d pos5d;
typedef vec6d pos6d;
typedef float matrix2x2[2][2];
typedef float matrix3x3[3][3];
typedef float matrix4x4[4][4];

extern matrix4x4 identity_matrix,zero_matrix;

extern float vt_atan2(float x, float y);
extern float vt_coord_plane_dist(pos3d pnt, int plane_normal_axis);

extern void vt_set_vec3(float u, float v, float w, vec3d result);
extern void vt_copy_vec3(vec3d v1, vec3d v2);
extern void vt_copy_vec6(vec6d v1, vec6d v2);
extern void vt_vec_diff3(vec3d pnt1, vec3d pnt2, vec3d result);
extern void vt_vec_sub3(vec3d v1, vec3d v2, vec3d result);
extern void vt_vec_neg3(vec3d v1, vec3d result);
extern void vt_vec_add3(vec3d v1, vec3d v2, vec3d result);
extern void vt_cross_prod3(vec3d v1, vec3d v2, vec3d v1Xv2);
extern float vt_dot_prod3(vec3d v1, vec3d v2);
extern float vt_vec_length3(vec3d v1);
extern void vt_normalize3(vec3d v1, vec3d v2);

extern void vt_copy_matrix(matrix4x4 A, matrix4x4 B);
extern void vt_identity_matrix_fill(matrix4x4 A);
extern void vt_zero_matrix_fill(matrix4x4 A);
extern void vt_rot_matrix(float theta, char axis, matrix4x4 A);
extern void vt_trans_matrix(vec3d pnt, matrix4x4 A);
extern void vt_scale_matrix(vec3d scale, matrix4x4 A);
extern void vt_axis_rot_matrix(vec3d rot_pt, vec3d rot_axis, float rot_angle,
            matrix4x4 rot_matrix);
extern void vt_mult_matrix(matrix4x4 A, matrix4x4 B, matrix4x4 result);
extern void vt_mult_rot_matrix(float theta, char axis, int order, matrix4x4 A);
extern void vt_mult_trans_matrix(pos3d pnt, int order, matrix4x4 A);
extern void vt_mult_scale_matrix(vec3d scale, int order, matrix4x4 A);
extern void vt_transform3(vec3d pt, matrix4x4 A, vec3d xformedpt);
extern void vt_transform4(vec4d pt, matrix4x4 A, vec4d xformedpt);
extern void vt_matrix_to_euler_angles(matrix4x4 rotm, vec3d euler_angle);
extern void vt_gen_view_matrix(vec3d camera, vec3d gaze_vec,
                   matrix4x4 viewmatrix);

#endif              /* VT_MATH3D_H */
