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


#include <assert.h>
#include <Math/vjQuat.h>


void vjQuat::makeRot(const vjMatrix& mat)
{
   float tr, s;
   //static int nxt[3] = {VJ_Y, VJ_Z, VJ_X};

   tr = mat.mat[0][0] + mat.mat[1][1] + mat.mat[2][2];

   // Check the diagonal
   if (tr > 0.0)
   {
      s = sqrt(tr + 1.0);
      vec[VJ_W] = s/2.0;
      s = 0.5/s;

      vec[VJ_X] = (mat.mat[1][2] - mat.mat[2][1])*s;
      vec[VJ_Y] = (mat.mat[2][0] - mat.mat[0][2])*s;
      vec[VJ_Z] = (mat.mat[0][1] - mat.mat[1][0])*s;
   }
   else     // Diagonal is negative
   {
      int   i, j, k;
      int   nxt[3] = {VJ_Y, VJ_Z, VJ_X};

      i = VJ_X;

      if (mat.mat[VJ_Y][VJ_Y] > mat.mat[VJ_X][VJ_X])
         i = VJ_Y;
      if (mat.mat[VJ_Z][VJ_Z] > mat.mat[i][i])
         i = VJ_Z;

      j = nxt[i];
      k = nxt[j];

      s = sqrt ((mat.mat[i][i] - (mat.mat[j][j] + mat.mat[k][k])) + 1.0);

      vec[i] = s * 0.5;

      if (s != 0.0)
         s = (0.5 / s);

      vec[VJ_W] = (mat.mat[j][k] - mat.mat[k][j]) * s;
      vec[j] = (mat.mat[i][j] + mat.mat[j][i]) * s;
      vec[k] = (mat.mat[i][k] + mat.mat[k][i]) * s;
   }

}

// Multiply the two quaternions
// From gdmag
void vjQuat::mult(const vjQuat& q1, const vjQuat& q2)
{
   vjQuat tempQuat;
   tempQuat[VJ_X] = q1[VJ_W]*q2[VJ_X] + q1[VJ_X]*q2[VJ_W] + q1[VJ_Y]*q2[VJ_Z] - q1[VJ_Z]*q2[VJ_Y];
   tempQuat[VJ_Y] = q1[VJ_W]*q2[VJ_Y] + q1[VJ_Y]*q2[VJ_W] + q1[VJ_Z]*q2[VJ_X] - q1[VJ_X]*q2[VJ_Z];
   tempQuat[VJ_Z] = q1[VJ_W]*q2[VJ_Z] + q1[VJ_Z]*q2[VJ_W] + q1[VJ_X]*q2[VJ_Y] - q1[VJ_Y]*q2[VJ_X];
   tempQuat[VJ_W] = q1[VJ_W]*q2[VJ_W] - q1[VJ_X]*q2[VJ_X] - q1[VJ_Y]*q2[VJ_Y] - q1[VJ_Z]*q2[VJ_Z];

   vec[VJ_X] = tempQuat[VJ_X];
   vec[VJ_Y] = tempQuat[VJ_Y];
   vec[VJ_Z] = tempQuat[VJ_Z];
   vec[VJ_W] = tempQuat[VJ_W];
   this->normalize();               // Make sure it is a unit quaternion
}

// Invert
void vjQuat::invert(const vjQuat& q1)
{
   float temp_norm = q1.norm();
   float inv_norm;

   if (temp_norm >= VJ_QUAT_EPSILON)
   {
      inv_norm = 1.0f/temp_norm;
      vec[VJ_X] = q1.vec[VJ_X]*(-inv_norm);
      vec[VJ_Y] = q1.vec[VJ_Y]*(-inv_norm);
      vec[VJ_Z] = q1.vec[VJ_Z]*(-inv_norm);
      vec[VJ_W] =  q1.vec[VJ_W]*inv_norm;
   }
   else
      set(0.0f, 0.0f, 0.0f, 0.0f);
}

// From Adv Anim and Rendering Tech. Pg 364
//
void vjQuat::slerp(float t, const vjQuat& p, const vjQuat& q)
{
   vjQuat to;
   double omega, cosom, sinom, sclp, sclq;
   //int i;

   // calc cosine
   cosom = p[VJ_X]*q[VJ_X] + p[VJ_Y]*q[VJ_Y] + p[VJ_Z]*q[VJ_Z]
           + p[VJ_W]*q[VJ_W];


   // adjust signs (if necessary)
   if ( cosom < 0.0 )
   {
      cosom = -cosom;
      to.vec[0] = -q.vec[0];   // Reverse all signs
      to.vec[1] = -q.vec[1];
      to.vec[2] = -q.vec[2];
      to.vec[3] = -q.vec[3];
   } else  {
      to = q;
   }

   // Calculate coefficients

   if ((1.0 - cosom) > VJ_QUAT_EPSILON)
   {
         // Standard case (slerp)
      omega = acos(cosom);
      sinom = sin(omega);
      sclp  = sin((1.0 - t)*omega)/sinom;
      sclq = sin(t*omega)/sinom;
   }
   else
   {
         // Very close, do linear interp
      sclp = 1.0 - t;
      sclq = t;
   }

   vec[VJ_X] = sclp*p.vec[VJ_X] + sclq*to.vec[VJ_X];
   vec[VJ_Y] = sclp*p.vec[VJ_Y] + sclq*to.vec[VJ_Y];
   vec[VJ_Z] = sclp*p.vec[VJ_Z] + sclq*to.vec[VJ_Z];
   vec[VJ_W] = sclp*p.vec[VJ_W] + sclq*to.vec[VJ_W];

}

void vjQuat::squad(float _t, const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _a, const vjQuat& _b)
{ 
   assert( false && "not implemented" ); 
}

