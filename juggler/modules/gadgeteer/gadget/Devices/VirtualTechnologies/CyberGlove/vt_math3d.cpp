/*  FILE: vt_math3d.c  --  vector and matrix routines           */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
/*  --  Copyright (C) 1990,91,92 Larry Edwards              --  */
/*  --                                                      --  */
/*  --    Authors: Larry Edwards and William Chapin         --  */
/*  ==========================================================  */

#include <gadget/Devices/DriverConfig.h>
#include <math.h>
#include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*                         VECTOR MATH ROUTINES                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

void
vt_set_vec3(float u, float v, float w, vec3d result)
{
  result[VX] = u; result[VY] = v; result[VZ] = w;
}

void
vt_copy_vec3(vec3d v1, vec3d v2)
{
  v2[VX] = v1[VX];
  v2[VY] = v1[VY];
  v2[VZ] = v1[VZ];
}

void
vt_vec_diff3(vec3d v1, vec3d v2, vec3d result)
{
  result[PX] = v2[PX] - v1[PX];
  result[PY] = v2[PY] - v1[PY];
  result[PZ] = v2[PZ] - v1[PZ];
}

/* NOTE: vec_sub3 subtracts does v1-v2 while above we do v2-v1 */

void
vt_vec_sub3(vec3d v1, vec3d v2, vec3d result)
{
  result[PX] = v1[PX] - v2[PX];
  result[PY] = v1[PY] - v2[PY];
  result[PZ] = v1[PZ] - v2[PZ];
}

void
vt_vec_neg3(vec3d v1, vec3d result)
{
  result[PX] = -v1[PX];
  result[PY] = -v1[PY];
  result[PZ] = -v1[PZ];
}

void
vt_vec_add3(vec3d v1, vec3d v2, vec3d result)
{
  result[PX] = v1[PX] + v2[PX];
  result[PY] = v1[PY] + v2[PY];
  result[PZ] = v1[PZ] + v2[PZ];
}

void
vt_cross_prod3(vec3d v1, vec3d v2, vec3d v1Xv2)
{
  v1Xv2[VX] =    (v1[VY] * v2[VZ]) - (v1[VZ] * v2[VY]);
  v1Xv2[VY] = - ((v1[VX] * v2[VZ]) - (v1[VZ] * v2[VX]));
  v1Xv2[VZ] =    (v1[VX] * v2[VY]) - (v1[VY] * v2[VX]);
}

float
vt_dot_prod3(vec3d v1, vec3d v2)
{
  return ((v1[VX] * v2[VX]) + (v1[VY] * v2[VY]) + (v1[VZ] * v2[VZ]));
}

float
vt_vec_length3(vec3d v1)
{
  return (sqrt((v1[VX] * v1[VX]) + (v1[VY] * v1[VY]) + (v1[VZ] * v1[VZ])));
}

void
vt_normalize3(vec3d v1, vec3d result)
  /* normalize a vector v1, put it in vect result */
{
  float length;
  ;
  length = sqrt((v1[VX] * v1[VX]) + (v1[VY] * v1[VY]) + (v1[VZ] * v1[VZ]));

  if (length > 0.0)     /* just in case */
  {
    result[VX] = v1[VX] / length;
    result[VY] = v1[VY] / length;
    result[VZ] = v1[VZ] / length;
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*                         MATRIX MATH ROUTINES                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/


void
vt_copy_matrix(matrix4x4 A, matrix4x4 B)
{
  B[0][0] = A[0][0];
  B[0][1] = A[0][1];
  B[0][2] = A[0][2];
  B[0][3] = A[0][3];

  B[1][0] = A[1][0];
  B[1][1] = A[1][1];
  B[1][2] = A[1][2];
  B[1][3] = A[1][3];

  B[2][0] = A[2][0];
  B[2][1] = A[2][1];
  B[2][2] = A[2][2];
  B[2][3] = A[2][3];

  B[3][0] = A[3][0];
  B[3][1] = A[3][1];
  B[3][2] = A[3][2];
  B[3][3] = A[3][3];
}

void
vt_zero_matrix_fill(matrix4x4 A)
{
  A[0][0] = 0.0;
  A[0][1] = 0.0;
  A[0][2] = 0.0;
  A[0][3] = 0.0;

  A[1][0] = 0.0;
  A[1][1] = 0.0;
  A[1][2] = 0.0;
  A[1][3] = 0.0;

  A[2][0] = 0.0;
  A[2][1] = 0.0;
  A[2][2] = 0.0;
  A[2][3] = 0.0;

  A[3][0] = 0.0;
  A[3][1] = 0.0;
  A[3][2] = 0.0;
  A[3][3] = 0.0;
}

void
vt_rot_matrix(float theta, char axis, matrix4x4 A)
{               /* NOTE: theta is in radians */
  int i, j;
  float costh, sinth;
  ;
  A[0][0] = 0.0;
  A[0][1] = 0.0;
  A[0][2] = 0.0;
  A[0][3] = 0.0;

  A[1][0] = 0.0;
  A[1][1] = 0.0;
  A[1][2] = 0.0;
  A[1][3] = 0.0;

  A[2][0] = 0.0;
  A[2][1] = 0.0;
  A[2][2] = 0.0;
  A[2][3] = 0.0;

  A[3][0] = 0.0;
  A[3][1] = 0.0;
  A[3][2] = 0.0;
  A[3][3] = 1.0;

  axis -= 'x';          /* WARNING: axis must be lowercase! */
  i = (axis+1) % 3;
  j = (axis+2) % 3;

  costh = cos(theta);
  sinth = sin(theta);

  A[i][i] = costh;
  A[i][j] = sinth;

  A[j][i] = -sinth;
  A[j][j] = costh;

  A[axis][axis] = 1.0;
}

void
vt_trans_matrix(pos3d pnt, matrix4x4 A)
{
  A[0][0] = 1.0;
  A[0][1] = 0.0;
  A[0][2] = 0.0;
  A[0][3] = 0.0;

  A[1][0] = 0.0;
  A[1][1] = 1.0;
  A[1][2] = 0.0;
  A[1][3] = 0.0;

  A[2][0] = 0.0;
  A[2][1] = 0.0;
  A[2][2] = 1.0;
  A[2][3] = 0.0;

  A[3][0] = pnt[PX];
  A[3][1] = pnt[PY];
  A[3][2] = pnt[PZ];
  A[3][3] = 1.0;
}

void
vt_scale_matrix(vec3d scale, matrix4x4 A)
{
  A[0][0] = scale[PX];
  A[0][1] = 0.0;
  A[0][2] = 0.0;
  A[0][3] = 0.0;

  A[1][0] = 0.0;
  A[1][1] = scale[PY];
  A[1][2] = 0.0;
  A[1][3] = 0.0;

  A[2][0] = 0.0;
  A[2][1] = 0.0;
  A[2][2] = scale[PZ];
  A[2][3] = 0.0;

  A[3][0] = 0.0;
  A[3][1] = 0.0;
  A[3][2] = 0.0;
  A[3][3] = 1.0;
}

void
vt_mult_matrix(matrix4x4 A, matrix4x4 B, matrix4x4 C)
{
  int i;
  matrix4x4 temp;
  ;
  if (C == B)
  {
    vt_copy_matrix(B,temp);
    for (i=0; i < 4; i++)
    {
      C[i][0] = A[i][0]*temp[0][0] + A[i][1]*temp[1][0] +
            A[i][2]*temp[2][0] + A[i][3]*temp[3][0];

      C[i][1] = A[i][0]*temp[0][1] + A[i][1]*temp[1][1] +
            A[i][2]*temp[2][1] + A[i][3]*temp[3][1];

      C[i][2] = A[i][0]*temp[0][2] + A[i][1]*temp[1][2] +
            A[i][2]*temp[2][2] + A[i][3]*temp[3][2];

      C[i][3] = A[i][0]*temp[0][3] + A[i][1]*temp[1][3] +
            A[i][2]*temp[2][3] + A[i][3]*temp[3][3];
    }
  }
  else if (C == A)
  {
    vt_copy_matrix(A,temp);
    for (i=0; i < 4; i++)
    {
      C[i][0] = temp[i][0]*B[0][0] + temp[i][1]*B[1][0] +
            temp[i][2]*B[2][0] + temp[i][3]*B[3][0];

      C[i][1] = temp[i][0]*B[0][1] + temp[i][1]*B[1][1] +
            temp[i][2]*B[2][1] + temp[i][3]*B[3][1];

      C[i][2] = temp[i][0]*B[0][2] + temp[i][1]*B[1][2] +
            temp[i][2]*B[2][2] + temp[i][3]*B[3][2];

      C[i][3] = temp[i][0]*B[0][3] + temp[i][1]*B[1][3] +
            temp[i][2]*B[2][3] + temp[i][3]*B[3][3];
    }
  }
  else
  {
    for (i=0; i < 4; i++)
    {
      C[i][0] = A[i][0]*B[0][0] + A[i][1]*B[1][0] +
            A[i][2]*B[2][0] + A[i][3]*B[3][0];

      C[i][1] = A[i][0]*B[0][1] + A[i][1]*B[1][1] +
            A[i][2]*B[2][1] + A[i][3]*B[3][1];

      C[i][2] = A[i][0]*B[0][2] + A[i][1]*B[1][2] +
            A[i][2]*B[2][2] + A[i][3]*B[3][2];

      C[i][3] = A[i][0]*B[0][3] + A[i][1]*B[1][3] +
            A[i][2]*B[2][3] + A[i][3]*B[3][3];
    }
  }
}

void
vt_mult_rot_matrix(float theta, char axis, int order, matrix4x4 A)
{               /* NOTE: theta is in radians */
  int axis1, axis2;
  float costh, sinth;
  vec4d temp;

  axis -= 'x';          /* WARNING: axis must be lowercase! */

  axis1 = (axis+1) % 3;
  axis2 = (axis+2) % 3;
  costh = cos(theta);
  sinth = sin(theta);

  if (order == Premult)
  {
    temp[0] = costh*A[axis1][axis] + sinth*A[axis2][axis];
    temp[1] = costh*A[axis1][axis1] + sinth*A[axis2][axis1];
    temp[2] = costh*A[axis1][axis2] + sinth*A[axis2][axis2];
    temp[3] = costh*A[axis1][3] + sinth*A[axis2][3];

    A[axis2][axis] = -sinth*A[axis1][axis] + costh*A[axis2][axis];
    A[axis2][axis1] = -sinth*A[axis1][axis1] + costh*A[axis2][axis1];
    A[axis2][axis2] = -sinth*A[axis1][axis2] + costh*A[axis2][axis2];
    A[axis2][3] = -sinth*A[axis1][3] + costh*A[axis2][3];

    A[axis1][axis] = temp[0];
    A[axis1][axis1] = temp[1];
    A[axis1][axis2] = temp[2];
    A[axis1][3] = temp[3];
  }
  else
  {
    A[axis][axis1] = costh*A[axis][axis1] - sinth*A[axis][axis2];
    A[axis][axis2] = sinth*A[axis][axis1] + costh*A[axis][axis2];

    A[axis1][axis1] = costh*A[axis1][axis1] - sinth*A[axis1][axis2];
    A[axis1][axis2] = sinth*A[axis1][axis1] + costh*A[axis1][axis2];

    A[axis2][axis1] = costh*A[axis2][axis1] - sinth*A[axis2][axis2];
    A[axis2][axis2] = sinth*A[axis2][axis1] + costh*A[axis2][axis2];

    A[3][axis1] = costh*A[3][axis1] - sinth*A[3][axis2];
    A[3][axis2] = sinth*A[3][axis1] + costh*A[3][axis2];
  }
}

void
vt_mult_trans_matrix(pos3d pnt, int order, matrix4x4 A)
{
  if (order == Premult)
  {
    A[3][0] = A[0][0]*pnt[PX] + A[1][0]*pnt[PY] + A[2][0]*pnt[PZ] + A[3][0];
    A[3][1] = A[0][1]*pnt[PX] + A[1][1]*pnt[PY] + A[2][1]*pnt[PZ] + A[3][1];
    A[3][2] = A[0][2]*pnt[PX] + A[1][2]*pnt[PY] + A[2][2]*pnt[PZ] + A[3][2];
    A[3][3] = A[0][3]*pnt[PX] + A[1][3]*pnt[PY] + A[2][3]*pnt[PZ] + A[3][3];
  }
  else
  {
    A[0][0] = A[0][0] + A[0][3]*pnt[PX];
    A[0][1] = A[0][1] + A[0][3]*pnt[PY];
    A[0][2] = A[0][2] + A[0][3]*pnt[PZ];

    A[1][0] = A[1][0] + A[1][3]*pnt[PX];
    A[1][1] = A[1][1] + A[1][3]*pnt[PY];
    A[1][2] = A[1][2] + A[1][3]*pnt[PZ];

    A[2][0] = A[2][0] + A[2][3]*pnt[PX];
    A[2][1] = A[2][1] + A[2][3]*pnt[PY];
    A[2][2] = A[2][2] + A[2][3]*pnt[PZ];

    A[3][0] = A[3][0] + A[3][3]*pnt[PX];
    A[3][1] = A[3][1] + A[3][3]*pnt[PY];
    A[3][2] = A[3][2] + A[3][3]*pnt[PZ];
  }
}

void
vt_mult_scale_matrix(vec3d scale, int order, matrix4x4 A)
{
  if (order == Premult)
  {
    A[0][0] = A[0][0]*scale[PX];
    A[0][1] = A[0][1]*scale[PX];
    A[0][2] = A[0][2]*scale[PX];
    A[0][3] = A[0][3]*scale[PX];

    A[1][0] = A[1][0]*scale[PY];
    A[1][1] = A[1][1]*scale[PY];
    A[1][2] = A[1][2]*scale[PY];
    A[1][3] = A[1][3]*scale[PY];

    A[2][0] = A[2][0]*scale[PZ];
    A[2][1] = A[2][1]*scale[PZ];
    A[2][2] = A[2][2]*scale[PZ];
    A[2][3] = A[2][3]*scale[PZ];
  }
  else
  {
    A[0][0] = A[0][0]*scale[PX];
    A[0][1] = A[0][1]*scale[PY];
    A[0][2] = A[0][2]*scale[PZ];

    A[1][0] = A[1][0]*scale[PX];
    A[1][1] = A[1][1]*scale[PY];
    A[1][2] = A[1][2]*scale[PZ];

    A[2][0] = A[2][0]*scale[PX];
    A[2][1] = A[2][1]*scale[PY];
    A[2][2] = A[2][2]*scale[PZ];

    A[3][0] = A[3][0]*scale[PX];
    A[3][1] = A[3][1]*scale[PY];
    A[3][2] = A[3][2]*scale[PZ];
  }
}

void
vt_transform3(pos3d pt, matrix4x4 A, pos3d xformedpt)
{
  float temp[3];

  /* NOTE: this assumes A is not a perspective transformation     */
  /* NOTE 2: this assumes pt is a row vector multiplying A on the */
  /*         left!                                                */

  if (xformedpt != pt)
  {
    xformedpt[0] = pt[0]*A[0][0] + pt[1]*A[1][0] + pt[2]*A[2][0] + A[3][0];
    xformedpt[1] = pt[0]*A[0][1] + pt[1]*A[1][1] + pt[2]*A[2][1] + A[3][1];
    xformedpt[2] = pt[0]*A[0][2] + pt[1]*A[1][2] + pt[2]*A[2][2] + A[3][2];
  }
  else
  {
    temp[0] = pt[0]*A[0][0] + pt[1]*A[1][0] + pt[2]*A[2][0] + A[3][0];
    temp[1] = pt[0]*A[0][1] + pt[1]*A[1][1] + pt[2]*A[2][1] + A[3][1];
    temp[2] = pt[0]*A[0][2] + pt[1]*A[1][2] + pt[2]*A[2][2] + A[3][2];

    xformedpt[0] = temp[0];
    xformedpt[1] = temp[1];
    xformedpt[2] = temp[2];
  }
}
