#include <iostream.h>
#include <C2Matrix.h>
#include <C2Vec3.h>

void testMakeXYZ(float x, float y, float z);
void testMakeZYX(float z, float y, float x);
void compareMats(C2Matrix mat1, C2Matrix mat2);


int main(void)
{
   C2Matrix testMat;
   C2Matrix testMat2;

   // ---- Test Matrix set ---- //
   cout << "\n---- Testing Set Matrix ---" << endl;
   
   testMat.set(0.0f, 0.1f, 0.2f, 0.3f,
               1.0f, 1.1f, 1.2f, 1.3f,
               2.0f, 2.1f, 2.2f, 2.3f,
               3.0f, 3.1f, 3.2f, 3.3f);

   cout << "Command issued:"
        << "testMat.set(0.0f, 0.1f, 0.2f, 0.3f," << endl
        << "      1.0f, 1.1f, 1.2f, 1.3f," << endl
        << "       2.0f, 2.1f, 2.2f, 2.3f," << endl
        << "       3.0f, 3.1f, 3.2f, 3.3f);" << endl;

   cout << "Matrix:" << endl << testMat << endl;

   // --- Test Inverstion --- //
   cout << "\n---- Test Matrix inverse ---" << endl;
   testMat.set(-0.7f, 0.1f,  0.0f, 2.3f,
               -0.1f, -0.7f, 0.0f, 5.0f,
                0.0f, 0.0f,  1.0f, 0.0f,
                0.0f, 0.0f,  0.0f, 2.0f);
   cout << "Orig Mat:" << endl << testMat << endl;
   testMat.invert(testMat);
   cout << "Inverted Mat:" << endl << testMat << endl;

   // ---- Test Mult --- //
   cout << "\n---- Test Matrix multiply ---" << endl;
   testMat2.set(9.7f, 0.1f,  -1.2f, 2.3f,
               2.1f, -0.7f, 0.75f, 5.0f,
                0.3f, 4.0f,  1.0f, -3.5f,
                2.0f, -3.0f,  7.9f, 2.0f);
   cout << "m1:" << endl << testMat << endl;
   cout << "m2:" << endl << testMat2 << endl;
   testMat.mult(testMat, testMat2);
   cout << "result:" << endl << testMat << endl;

   // ---- Test post mult ---- //
   cout << "\n---- Test Post Matrix multiply ---" << endl;
   testMat2.set(2.34f, 0.7f,  -3.4f, 1.89f,
               4.1f, -0.7f, 0.05f, 5.0f,
                2.7f, 2.3f,  1.0f, -8.0f,
                8.6f, -2.0f,  1.9f, 5.6f);
   cout << "m1:" << endl << testMat << endl;
   cout << "m2:" << endl << testMat2 << endl;
   testMat.postMult(testMat2);
   cout << "testMat.postMult(testMat2)" << endl;
   cout << "result:" << endl << testMat << endl;


   // ----- Test Mat*Vec ----- //

   // ----- Test Make Euler ---- //

   // ----- Test Make XYZ ----- //
   cout << "\n----  Test makeXYZ ---" << endl;
   testMat.makeXYZEuler(-90, -180, 0);
   cout << "Command issued:" << " makeXYZEuler(-90, -180, 0)" << endl;
   cout << "Result:" << endl << testMat << endl;

   
   // --------------------------------------- //
   float x_deg, y_deg, z_deg;
   C2Matrix XYZMat;

   XYZMat.makeXYZEuler(0, 90, 90);
   cout << "makeXYZEuler(0, 90, 90):" << endl
   << XYZMat << endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //


   XYZMat.makeXYZEuler(10, 0, 0);
   cout << "makeXYZEuler(10, 0, 0):" << endl
   << XYZMat << endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //
   

   XYZMat.makeXYZEuler(0, 45, 0);
   cout << "makeXYZEuler(0, 45, 0):" << endl
   << XYZMat << endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //
   

   XYZMat.makeXYZEuler(45, -30, 0);
   cout << "makeXYZEuler(45, -30, 0):" << endl
   << XYZMat << endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //

    XYZMat.makeXYZEuler(-20, 0, -80);
   cout << "makeXYZEuler(-20, 0, -80):" << endl
   << XYZMat << endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //
   testMakeXYZ(175, -20, 35);
   cout << endl;
   
   // --------------------------------------- //
   testMakeXYZ(-135, 20, 15);
   cout << endl;

   // --------------------------------------- //
   testMakeXYZ(0, 90, 90);
   cout << endl;

   // ------------ Test random values ----------------- //
   cout << "\nRandom XYZ Eulers:" << endl;
   
   for(int x=0;x<50;x++)
   {
      int xRot = (rand()%360)-180;
      int yRot = (rand()%360)-180;
      int zRot = (rand()%360)-180;
      
      testMakeXYZ(xRot, yRot, zRot);

      cout << endl;
   }

   cout << "\nRandom ZYX Eulers:" << endl;
   
   for(x=0;x<50;x++)
   {
      int xRot = (rand()%360)-180;
      int yRot = (rand()%360)-180;
      int zRot = (rand()%360)-180;
      
      testMakeZYX(zRot, yRot, xRot);

      cout << endl;
   }

   
   // --------------------------------------- //
   // --- Test serial of transforms --------- //
   // --------------------------------------- //
   XYZMat.makeXYZEuler(-20.0f, 10.0f, 11.0f);
   cout << "makeXYZEuler(-20.0f, 10.0f, 11.0f):" << endl << XYZMat << endl;

   for(x=0;x<50;x++)
   {
      XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
      cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";
      XYZMat.preRot(1.0f, C2Vec3(1.0f, 0.0f, 0.0f), XYZMat);
   }
   
   // --------------------------------------- //
   // ---- Create transform matrix ---------- //
   // --------------------------------------- //
   //  Tracker coord system
   //    x right, y out, z down
   C2Matrix rotMat;                            // Matrix to rotate the tracker coord system to C2 coord system
   //rotMat.makeDirCos(C2Vec3(1,0,0), C2Vec3(0,0,-1), C2Vec3(0,1,0));
   rotMat.makeZYXEuler(0,0,90);

   cout << "--- The rotMat ----\n" << rotMat << endl;

   C2Matrix transRotMat;               // Need the transpose for Eulers
   transRotMat.transpose(rotMat);

   cout << "---- The transpose rotMat ----\n" << transRotMat << endl;

   // ------------------------------------- //
   // ----  Test transform tracker axis --- //
   // ------------------------------------- //
   C2Vec3 tracker_x_axis, tracker_y_axis,tracker_z_axis, temp_axis;
   tracker_x_axis.set(1.0, 0.0, 0.0);
   tracker_y_axis.set(0.0, 1.0, 0.0);
   tracker_z_axis.set(0.0, 0.0, 1.0);
      
   cout << "\n--------- Tracker axis transforms -------\n";
   
   temp_axis.xformVec(transRotMat, tracker_x_axis);
   cout << "X Axis:"
        << "\t   orig:" << tracker_x_axis
        << "\txformed:" << temp_axis << endl;
   
   temp_axis.xformVec(transRotMat, tracker_y_axis);
   cout << "Y Axis:"
        << "\t   orig:" << tracker_y_axis
        << "\txformed:" << temp_axis << endl;
   
   temp_axis.xformVec(transRotMat, tracker_z_axis);
   cout << "Z Axis:"
        << "\t   orig:" << tracker_z_axis
        << "\txformed:" << temp_axis << endl;
  
   cout << "\n\n\n";

   // ----------------------------------------- //
   // ------ Test Or tranformation ------------ //
   // ----------------------------------------- //
   C2Vec3 tracker_base_dir, new_dir;
   tracker_base_dir.set(1.0f, 0.0f, 0.0f);         // Set the same base dir in both coord systems
   
   C2Matrix tTr;     // Transformation of the reciever in tracker coord system
   C2Matrix wTt;     // Transformation of the world coord system to the tracker coord system
   C2Matrix wTr;     // Transformation of the reciever in the world coord system
   
   wTt = rotMat;     // Set the World to tracker coord system rotation matrix

   tTr.makeZYXEuler(0.0, 90.0, 0.0f);            // Ry(90)
   cout << "\n-------- Test tracker Or transform ----------------\n" 
        << "tracker XYZ: 0.0, 90.0, 0.0" << endl;
   
   new_dir.xformVec(tTr, tracker_base_dir);      // Rotate the base
   cout << " tracker rot (tTr):\n" << tTr << endl;
   cout << "   Base Tracker dir: " << tracker_base_dir << endl;
   cout << "Xformed Tracker dir (in T): " << new_dir << endl;

   cout << "\nTest system transform\n";
   
     // --- Test World (c2) coord system rotations --- //
   cout << "wTt:\n" << wTt << endl;
   cout << "tTr:\n" << tTr << endl;

   wTr.mult(wTt, tTr);     // Compute the world to reciever transform

   cout << "wTr rotation\n" << wTr << endl;
   new_dir.xformVec(wTr, tracker_base_dir);           // Rotate the base into the C2 coord system
   cout << "\nBase tracker dir: " << tracker_base_dir << endl; 
   cout << "  Xformed tracker dir (in W): " << new_dir << endl;
  
      // --- Get reciever orientation --- //
   float rot_z, rot_y, rot_x;
   wTr.getZYXEuler(rot_z, rot_y, rot_x);
   
   cout << "\n\nGet ZYX Euler: z:" << rot_z << "\ty:" << rot_y << "\tx:" << rot_x << endl;
   
   C2Matrix test_mat;
   test_mat.makeZYXEuler(rot_z, rot_y, rot_x);

   cout << "Test extraction of same matrix: ";
   compareMats(test_mat, wTr);
   cout << endl;


   C2Matrix known_mat;
   known_mat.makeZYXEuler(90, 0, 90);
   cout << "Test against known matrix: ";
   compareMats(known_mat, wTr);

   cout << endl << endl;

   
   
   // ---------------------------------- //
   // ----- Test Pt transformation  ---- //
   // ---------------------------------- //
   C2Vec3   original_pt, transformed_pt;

   cout << "\n---- Test pt conversion ----\n";
   
   original_pt.set(0.0f, 1.0f, -1.0f);       // This pt is in the tracker coord system
   cout << "orig: " << original_pt << endl;
   
   transformed_pt.xformFull(wTt, original_pt);
   cout << "new pt:" << transformed_pt << endl;

   // ------------------------------------------ //
   
   C2Matrix dir_cos_test;
   dir_cos_test.makeDirCos(C2Vec3(0,1,0), C2Vec3(-1,0,0), C2Vec3(0,0,1));
   cout << "\nmakeDirCos(C2Vec3(0,1,0), C2Vec3(-1,0,0), C2Vec3(0,0,1))" << endl
        << dir_cos_test << endl;

   return 1;    
}


void testMakeXYZ(float x, float y, float z)
{
   C2Matrix XYZMat;
   C2Matrix temp_mat;
   C2Matrix diff_mat;
   float x_deg, y_deg, z_deg;

   cout << "XYZ: " << x << ", " << y << ", " << z;
   
   XYZMat.makeXYZEuler(x, y, z);

   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   cout << "\tGot: " <<  x_deg << ", " << y_deg << ", " << z_deg;

   temp_mat.makeXYZEuler(x_deg, y_deg, z_deg);
   
   compareMats(temp_mat, XYZMat);
}

void testMakeZYX(float z, float y, float x)
{
   C2Matrix ZYXMat;
   C2Matrix temp_mat;
   C2Matrix diff_mat;
   float x_deg, y_deg, z_deg;

   cout << "ZYX: " << z << ", " << y << ", " << x;
   
   ZYXMat.makeZYXEuler(z, y, x);

   ZYXMat.getZYXEuler(z_deg, y_deg, x_deg);
   cout << "\tGot: " <<  z_deg << ", " << y_deg << ", " << x_deg;

   temp_mat.makeZYXEuler(z_deg, y_deg, x_deg);
   
   compareMats(temp_mat, ZYXMat);
}

void compareMats(C2Matrix mat1, C2Matrix mat2)
{
   if(mat1 == mat2)
   {
      cout << "\tMatch.";
   }
   else
   {
      cout << "\tDifferent.";
      C2Matrix diff_mat;
      diff_mat = mat1 - mat2;

      int i=0;
      float* mat;
      mat = diff_mat.getFloatPtr();    // Get the matrix

      while( (fabs(mat[i]) < 1e-5) && (i<16) )
      {
         i++;
      }

      if(i==16)
         cout << "  Within tolerance of 1e-5.";
      else
         cout << "diff:\n" << diff_mat << endl;
   }
}

