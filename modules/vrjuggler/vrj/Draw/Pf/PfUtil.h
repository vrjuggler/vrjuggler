#ifndef _VJ_PF_UTIL_H_
#define _VJ_PF_UTIL_H_

/**************************************************
* Performer helper routines                       *
***************************************************/

vjMatrix vjGetVJMatrix(pfMatrix& perfMat);
pfMatrix getPfMatrix(vjMatrix& mat);


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


#endif
