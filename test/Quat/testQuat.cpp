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


#include <iostream>
#include <Math/vjQuat.h>
#include <glquat.h>


std::ostream& operator<<(std::ostream& out, GL_QUAT& quat);
void testMatch(vjQuat& vj_quat, GL_QUAT& gl_quat);

int number_failed = 0;

int main(void)
{
   vjMatrix mat;

   GL_QUAT gl_quat;
   vjQuat vj_quat;

   // --- Test 1 --- //
   std::cout << "Test 1" << std::endl;
   mat.makeXYZEuler(90.0f, 75.0f, -23.0f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // --- Test 2 --- //
   std::cout << "Test 2" << std::endl;
   mat.makeXYZEuler(110.0f, 0.0f, 0.0f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // --- Test 3 --- //
   std::cout << "Test 3" << std::endl;
   mat.makeXYZEuler(179.0f, -123.0f, 12.21f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // ---- Test Mat construct --- //
   std::cout << "Construct Matrix" << std::endl;
   vjMatrix gl_mat;
   gluQuatToMat_EXT(&gl_quat, (GLfloat (*)[4])gl_mat.getFloatPtr());
   mat.makeQuaternion(vj_quat);

   std::cout << "gl_mat\n" << gl_mat << std::endl;
   std::cout << "vj_mat\n" << mat << std::endl;
   std::cout << "Testing match.....";
   if(mat == gl_mat)
      std::cout << "passed.\n" << std::flush;
   else
   {
      ++number_failed;
      std::cout << "FAILED!!!\n" << std::flush;
   }
      
   // --- Inverse ---- //
   std::cout << "Test Invert\n";
   vjQuat vj_inv;
   GL_QUAT gl_inv;
   gl_inv = gl_quat;
   gluQuatInverse_EXT(&gl_inv);
   vj_inv.invert(vj_quat);

   std::cout << "\tgl_inv: " << gl_inv << std::endl;
   std::cout << "\tvj_inv: " << vj_inv << std::endl;
   testMatch(vj_inv, gl_inv);

   // ---- Multiply ---- //
   std::cout << "Mult:\n";
   vjQuat vj_quat1, vj_quat2;
   GL_QUAT gl_quat1, gl_quat2;
   mat.makeXYZEuler(123.0f, -15.0f, 12.21f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat1);
   vj_quat1.makeQuat(mat);
   mat.makeXYZEuler(-63.0f, 79.0f, 75.1221f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat2);
   vj_quat2.makeQuat(mat);

   gluQuatMul_EXT(&gl_quat1, &gl_quat2, &gl_quat);
   vj_quat.mult(vj_quat1, vj_quat2);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // --- Divide --- //
   std::cout << "Divide:\n";
   gluQuatDiv_EXT(&gl_quat1, &gl_quat2, &gl_quat);
   vj_quat.div(vj_quat1, vj_quat2);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // --- Slerp 1 --- //
   std::cout << "Slerp 0.5:\n";
   gluQuatSlerp_EXT(&gl_quat1, &gl_quat2, 0.5, &gl_quat);
   vj_quat.slerp(0.5, vj_quat1, vj_quat2);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // --- Slerp 2 --- //
   std::cout << "Slerp 0.97:\n";
   gluQuatSlerp_EXT(&gl_quat1, &gl_quat2, 0.97, &gl_quat);
   vj_quat.slerp(0.97, vj_quat1, vj_quat2);

   std::cout << "\tgl_quat: " << gl_quat << std::endl;
   std::cout << "\tvj_quat: " << vj_quat << std::endl;
   testMatch(vj_quat, gl_quat);

   // ---- Test matrix conversion --- //
   vjMatrix mat1, mat2;
   vjQuat quat;
   vjVec3 angles;
   GL_QUAT q;

   std::cout << "--- Test matrix conversion ---" << std::endl;
   for(float yAng=-180.0;yAng<180.0;yAng += 2)
   {
      mat1.makeXYZEuler(0.0, yAng, 0.0f);

      mat1.getXYZEuler(angles[0], angles[1], angles[2]);
      std::cout << "Orig angles:" << angles << std::endl;

      quat.makeQuat(mat1);
      mat1.makeQuaternion(quat);


      mat1.getXYZEuler(angles[0], angles[1], angles[2]);
      std::cout << "Final angles:" << angles << std::endl;
   }

   std::cout<<"\n\n\n -->>> RESULTS::: "<<number_failed<<" tests failed (scroll up to see)\n\n."<<std::flush;
}


std::ostream& operator<<(std::ostream& out, GL_QUAT& quat)
{
   out << "w:" << quat.w << " x:" << quat.x << " y:" << quat.y << " z:" << quat.z;
   return out;
}

void testMatch(vjQuat& vj_quat, GL_QUAT& gl_quat)
{
   std::cout << "\tTesting match.........";
   if((vj_quat[VJ_X] == gl_quat.x) &&
      (vj_quat[VJ_Y] == gl_quat.y) &&
      (vj_quat[VJ_Z] == gl_quat.z) &&
      (vj_quat[VJ_W] == gl_quat.w))
   {
      std::cout << "Passed.\n" << std::flush;
   } else 
   {
      ++number_failed;
      std::cout << "FAILED!!!\n" << std::flush;
   }
}
