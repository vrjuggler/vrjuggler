/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vjConfig.h>
#include <Kernel/Pf/vjPfUtil.h>
#include <Math/vjVec3.h>


//: Convert Performer matrix to Juggler matrix
vjMatrix vjGetVjMatrix(pfMatrix& perfMat)
{
   vjMatrix mat;
   vjVec3 x_axis(1,0,0);
   mat.set(&(perfMat.mat[0][0]));
   mat.postRot( mat, 90, x_axis );
   mat.preRot( -90, x_axis, mat );
   
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



vjVec3 vjGetVjVec(pfVec3& vec)
{
   //     Perf     x       z       -y
   return vjVec3(vec[0], vec[2], -vec[1]);
}

pfVec3 vjGetPfVec(vjVec3& vec)
{
   //   Juggler   x        -z       y
   return pfVec3(vec[0], -vec[2], vec[1]);
}
