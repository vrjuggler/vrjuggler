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

#include <vrj/vrjConfig.h>
#include <vrj/Draw/Pf/PfUtil.h>
#include <vrj/Math/Vec3.h>

namespace vrj
{

//: Convert Performer matrix to Juggler matrix
Matrix GetVjMatrix( const pfMatrix& perfMat )
{
   Matrix mat;
   Vec3 x_axis( 1,0,0 );
   mat.set( &(perfMat.mat[0][0]) );
   mat.postRot( mat, 90, x_axis );
   mat.preRot( -90, x_axis, mat );
   
   return mat;
}

//: Convert Juggler Matrix to Pf Matrix
pfMatrix GetPfMatrix( const Matrix& mat )
{
   pfMatrix perf_mat;

   // NOTE: the man page and the pfLinMath.h header file disagree.
   // the man page says const float* and the header says float*
   // the man page is correct, there is no reason for a set func to
   // change the source data (unless you're ref counting or something weird)
   // ...this may change in the future so that this cast can someday be removed.
   float* floatPtr = const_cast<float *>( mat.getFloatPtr() );
   perf_mat.set( floatPtr );

   perf_mat.preRot( -90, 1, 0, 0, perf_mat );
   perf_mat.postRot( perf_mat, 90, 1, 0, 0 );

   return perf_mat;
}



Vec3 GetVjVec( const pfVec3& vec )
{
   //     Perf     x       z       -y
   return Vec3( vec[0], vec[2], -vec[1] );
}

pfVec3 GetPfVec( const Vec3& vec )
{
   //   Juggler   x        -z       y
   return pfVec3( vec[0], -vec[2], vec[1] );
}

};
