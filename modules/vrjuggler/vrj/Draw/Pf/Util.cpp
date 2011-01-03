/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Draw/Pf/Config.h>

#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Math.h>

#include <vrj/Draw/Pf/Util.h>


namespace vrj
{

namespace pf
{

// Converts Performer matrix to Juggler (GMTL) matrix.
gmtl::Matrix44f GetVjMatrix( const pfMatrix& perfMat )
{
   gmtl::Matrix44f mat;
   gmtl::Vec3f x_axis( 1.0f, 0.0f, 0.0f );
   mat.set( &(perfMat.mat[0][0]) );
   gmtl::postMult(mat, gmtl::makeRot<gmtl::Matrix44f>( gmtl::AxisAnglef( gmtl::Math::deg2Rad(90.0f), x_axis ) ));
   gmtl::preMult(mat, gmtl::makeRot<gmtl::Matrix44f>( gmtl::AxisAnglef( gmtl::Math::deg2Rad(-90.0f), x_axis ) ));

   return mat;
}

// Converts Juggler (GMTL) matrix to Pf Matrix.
pfMatrix GetPfMatrix( const gmtl::Matrix44f& mat )
{
   pfMatrix perf_mat;

   // NOTE: the man page and the pfLinMath.h header file disagree.
   // the man page says const float* and the header says float*
   // the man page is correct, there is no reason for a set func to
   // change the source data (unless you're ref counting or something weird)
   // ...this may change in the future so that this cast can someday be removed.
   float* floatPtr = const_cast<float *>( mat.mData );
   perf_mat.set( floatPtr );

   perf_mat.preRot( -90.0f, 1.0f, 0.0f, 0.0f, perf_mat );
   perf_mat.postRot( perf_mat, 90.0f, 1.0f, 0.0f, 0.0f );

   return perf_mat;
}

// Converts Performer 3-element vector to Juggler (GMTL) vector.
gmtl::Vec3f GetVjVec( const pfVec3& vec )
{
   //     Perf     x       z       -y
   return gmtl::Vec3f( vec[0], vec[2], -vec[1] );
}

// Converts Juggler (GMTL) vector to Pf vector.
pfVec3 GetPfVec( const gmtl::Vec3f& vec )
{
   //   Juggler   x        -z       y
   return pfVec3( vec[0], -vec[2], vec[1] );
}

} // End of pf namespace

} // End of vrj namespace
