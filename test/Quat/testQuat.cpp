#include <Math/vjQuat.h>
#include <glquat.h>
#include <iostream.h>


ostream& operator<<(ostream& out, GL_QUAT& quat);
void testMatch(vjQuat& vj_quat, GL_QUAT& gl_quat);


int main(void)
{
   vjMatrix mat;

   GL_QUAT gl_quat;
   vjQuat vj_quat;

   // --- Test 1 --- //
   cout << "Test 1" << endl;
   mat.makeXYZEuler(90.0f, 75.0f, -23.0f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // --- Test 2 --- //
   cout << "Test 2" << endl;
   mat.makeXYZEuler(110.0f, 0.0f, 0.0f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // --- Test 3 --- //
   cout << "Test 3" << endl;
   mat.makeXYZEuler(179.0f, -123.0f, 12.21f);
   gluMatToQuat_EXT((GLfloat (*)[4])mat.getFloatPtr(), &gl_quat);
   vj_quat.makeQuat(mat);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // ---- Test Mat construct --- //
   cout << "Construct Matrix" << endl;
   vjMatrix gl_mat;
   gluQuatToMat_EXT(&gl_quat, (GLfloat (*)[4])gl_mat.getFloatPtr());
   mat.makeQuaternion(vj_quat);

   cout << "gl_mat\n" << gl_mat << endl;
   cout << "vj_mat\n" << mat << endl;
   cout << "Testing match.....";
   if(mat == gl_mat)
      cout << "passed.\n" << flush;
   else
      cout << "FAILED!!!\n" << flush;

   // --- Inverse ---- //
   cout << "Test Invert\n";
   vjQuat vj_inv;
   GL_QUAT gl_inv;
   gl_inv = gl_quat;
   gluQuatInverse_EXT(&gl_inv);
   vj_inv.invert(vj_quat);

   cout << "\tgl_inv: " << gl_inv << endl;
   cout << "\tvj_inv: " << vj_inv << endl;
   testMatch(vj_inv, gl_inv);

   // ---- Multiply ---- //
   cout << "Mult:\n";
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

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // --- Divide --- //
   cout << "Divide:\n";
   gluQuatDiv_EXT(&gl_quat1, &gl_quat2, &gl_quat);
   vj_quat.div(vj_quat1, vj_quat2);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // --- Slerp 1 --- //
   cout << "Slerp 0.5:\n";
   gluQuatSlerp_EXT(&gl_quat1, &gl_quat2, 0.5, &gl_quat);
   vj_quat.slerp(0.5, vj_quat1, vj_quat2);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // --- Slerp 2 --- //
   cout << "Slerp 0.97:\n";
   gluQuatSlerp_EXT(&gl_quat1, &gl_quat2, 0.97, &gl_quat);
   vj_quat.slerp(0.97, vj_quat1, vj_quat2);

   cout << "\tgl_quat: " << gl_quat << endl;
   cout << "\tvj_quat: " << vj_quat << endl;
   testMatch(vj_quat, gl_quat);

   // ---- Test matrix conversion --- //
   vjMatrix mat1, mat2;
   vjQuat quat;
   vjVec3 angles;
   GL_QUAT q;

   cout << "--- Test matrix conversion ---" << endl;
   for(float yAng=-180.0;yAng<180.0;yAng += 2)
   {
      mat1.makeXYZEuler(0.0, yAng, 0.0f);

      mat1.getXYZEuler(angles[0], angles[1], angles[2]);
      cout << "Orig angles:" << angles << endl;

      /*
      gluMatToQuat_EXT((GLfloat (*)[4])mat1.getFloatPtr(), &q);
      gluQuatNormalize_EXT(&q);
      gluQuatToMat_EXT(&q, (GLfloat (*)[4])mat1.getFloatPtr());
      */

      quat.makeQuat(mat1);
      mat1.makeQuaternion(quat);


      mat1.getXYZEuler(angles[0], angles[1], angles[2]);
      cout << "Final angles:" << angles << endl;
   }

}


ostream& operator<<(ostream& out, GL_QUAT& quat)
{
   out << "w:" << quat.w << " x:" << quat.x << " y:" << quat.y << " z:" << quat.z;
   return out;
}

void testMatch(vjQuat& vj_quat, GL_QUAT& gl_quat)
{
   cout << "\tTesting match.........";
   if((vj_quat[VJ_X] == gl_quat.x) &&
      (vj_quat[VJ_Y] == gl_quat.y) &&
      (vj_quat[VJ_Z] == gl_quat.z) &&
      (vj_quat[VJ_W] == gl_quat.w))
   {
      cout << "Passed.\n" << flush;
   } else {
      cout << "FAILED!!!\n" << flush;
   }
}
