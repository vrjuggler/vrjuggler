/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Kernel/Pf/vjPfUtil.h>

//: Convert Performer matrix to Juggler matrix
vjMatrix vjGetVJMatrix(pfMatrix& perfMat)
{
   vjMatrix mat;
   vjVec3 x_axis(1,0,0);
   mat.set(&(perfMat.mat[0][0]));
   mat.preRot(90, x_axis, mat);
   mat.postRot(mat, -90, x_axis);

   return mat;
}

//: Convert Juggler Matrix to Pf Matrix
pfMatrix vjGetPfMatrix(vjMatrix& mat)
{
   pfMatrix perf_mat;

   perf_mat.set(mat.getFloatPtr());
   perf_mat.preRot(-90, 1, 0, 0, perf_mat);
   perf_mat.postRot(perf_mat, 90, 1, 0, 0);

   return perf_mat;
}
