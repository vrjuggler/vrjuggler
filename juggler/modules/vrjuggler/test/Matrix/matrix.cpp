/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

bool testMakeXYZ(float x, float y, float z);
bool testMakeZYX(float z, float y, float x);
bool compareMats(vjMatrix mat1, vjMatrix mat2, vjMatrix& difMat);

#define testIt(test_desc, test_case) cout << test_desc; if((test_case)) { cout << "\t [ok]\n"; } else { cout << "\t [FAILED]\n";}

int main(void)
{
   vjMatrix testMat;
   vjMatrix testMat2;
   vjMatrix diff_mat;
   const vjVec3 x_axis(1.0f,0.0f,0.0f);
   const vjVec3 y_axis(0.0f,1.0f,0.0f);
   const vjVec3 z_axis(0.0f,0.0f,1.0f);

   bool     tests_pass(false);

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
   vjMatrix XYZMat;

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
   testIt("testMakeXYZ(175, -20, 35): ",(testMakeXYZ(175,-20,35)));
   testIt("testMakeXYZ(-135, 20, 15)",(testMakeXYZ(-135, 20, 15)));
   testIt("testMakeXYZ(0, 90, 90)",(testMakeXYZ(0, 90, 90)));

   // ------------ Test random XYZ values ----------------- //
   cout << "Random XYZ Eulers: " << endl;
   tests_pass = false;

   for(int x=0;x<50;x++)
   {
      int xRot = (rand()%360)-180;
      int yRot = (rand()%360)-180;
      int zRot = (rand()%360)-180;

      if(false == testMakeXYZ(xRot, yRot, zRot))
      { tests_pass = false; }
   }
   if(tests_pass)
   { cout << "\t[ok]\n"; }
   else
   { cout << "\t[FAILED]\n"; }


   // -------------------------------------------
   cout << "\nRandom ZYX Eulers:" << endl;
   tests_pass = false;
   for(x=0;x<50;x++)
   {
      int xRot = (rand()%360)-180;
      int yRot = (rand()%360)-180;
      int zRot = (rand()%360)-180;

      if(false == testMakeZYX(zRot, yRot, xRot))
      { tests_pass = false; }
   }
   if(tests_pass)
   { cout << "\t[ok]\n"; }
   else
   { cout << "\t[FAILED]\n"; }


   // --------------------------------------- //
   // --- Test serial of transforms --------- //
   // --------------------------------------- //
   XYZMat.makeXYZEuler(-20.0f, 10.0f, 11.0f);
   cout << "makeXYZEuler(-20.0f, 10.0f, 11.0f):" << endl << XYZMat << endl;

   for(x=0;x<50;x++)
   {
      XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
      cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";
      XYZMat.preRot(1.0f, x_axis, XYZMat);
   }

   // --------------------------------------- //
   // ---- Create transform matrix ---------- //
   // --------------------------------------- //
   //  Tracker coord system
   //    x right, y out, z down
   vjMatrix rotMat;                            // Matrix to rotate the tracker coord system to C2 coord system
   //rotMat.makeDirCos(vjVec3(1,0,0), vjVec3(0,0,-1), vjVec3(0,1,0));
   rotMat.makeZYXEuler(0,0,90);

   cout << "--- The rotMat ----\n" << rotMat << endl;

   vjMatrix transRotMat;               // Need the transpose for Eulers
   transRotMat.transpose(rotMat);

   cout << "---- The transpose rotMat ----\n" << transRotMat << endl;

   // ------------------------------------- //
   // ----  Test transform tracker axis --- //
   // ------------------------------------- //
   vjVec3 tracker_x_axis, tracker_y_axis,tracker_z_axis, temp_axis;
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
   vjVec3 tracker_base_dir, new_dir;
   tracker_base_dir.set(1.0f, 0.0f, 0.0f);         // Set the same base dir in both coord systems

   vjMatrix tTr;     // Transformation of the reciever in tracker coord system
   vjMatrix wTt;     // Transformation of the world coord system to the tracker coord system
   vjMatrix wTr;     // Transformation of the reciever in the world coord system

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

   vjMatrix test_mat;
   test_mat.makeZYXEuler(rot_z, rot_y, rot_x);

   cout << "Test extraction of same matrix: ";
   if(compareMats(test_mat, wTr, diff_mat) == true)
   {
      cout << " Same.";
   }
   else
   {
      cout << "Different: \n" << diff_mat << endl;
   }
   cout << endl;


   vjMatrix known_mat;
   known_mat.makeZYXEuler(90, 0, 90);
   cout << "Test against known matrix: ";

   if(compareMats(known_mat, wTr, diff_mat))
   {
      cout << " Same.";
   } else {
      cout << "Different: \n" << diff_mat;
   }

   cout << endl << endl;



   // ---------------------------------- //
   // ----- Test Pt transformation  ---- //
   // ---------------------------------- //
   vjVec3   original_pt, transformed_pt;

   cout << "\n---- Test pt conversion ----\n";

   original_pt.set(0.0f, 1.0f, -1.0f);       // This pt is in the tracker coord system
   cout << "orig: " << original_pt << endl;

   transformed_pt.xformFull(wTt, original_pt);
   cout << "new pt:" << transformed_pt << endl;

   // ------------------------------------------ //

   vjMatrix dir_cos_test;
   dir_cos_test.makeDirCos(vjVec3(0,1,0), vjVec3(-1,0,0), vjVec3(0,0,1));
   cout << "\nmakeDirCos(vjVec3(0,1,0), vjVec3(-1,0,0), vjVec3(0,0,1))" << endl
        << dir_cos_test << endl;

   // -----------------------------------
   // Test get?Rot funcs
   //
   testMat2.makeRot(47.0,vjVec3(1,0,0));
   testIt("getXRot..",((testMat2.getXRot() == 47.0f) && (testMat2.getYRot() == 0.0f) && (testMat2.getZRot() == 0.0f)));

   testMat2.makeRot(-56.0,vjVec3(0,1,0));
   testIt("getYRot..",((testMat2.getXRot() == 0.0f) && (testMat2.getYRot() == -56.0f) && (testMat2.getZRot() == 0.0f)));
   //cout << "x: " << testMat2.getXRot() << ", y:" << testMat2.getYRot() << ", z:" << testMat2.getZRot() << endl;

   testMat2.makeRot(78.0, vjVec3(0,0,1));
   testIt("getZRot..",((testMat2.getXRot() == 0.0f) && (testMat2.getYRot() == 0.0f) && (testMat2.getZRot() == 78.0f)));
   /*
   cout << "roll  only: " << "y:" << testMat2.getYaw()
                             << " p:" << testMat2.getPitch()
                             << " r:" << testMat2.getRoll() << endl;
   */
   // ------------------------------------
   // ---- Test axis constraints ---------
   // ------------------------------------
   // Idea: Create a matrix, contrain an axis, then check if the axis has a value
   {
      float pitch(0),yaw(0),roll(0);

      cout << "contrainMat: makeXYZEuler(47.0,67.0,-76.0)\n";
      testMat.makeXYZEuler(47.0,67.0,-76.0);
      cout << "Original Matrix: " << "x:" << testMat.getXRot()
                             << " y:" << testMat.getYRot()
                             << " z:" << testMat.getZRot() << endl;

      testMat.constrainRotAxis(true,false,false,testMat2);
      cout << "x rot only: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;

      testMat.constrainRotAxis(false,true,false,testMat2);
      cout << "y rot only: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;
      testMat.constrainRotAxis(false,false,true,testMat2);
      cout << "z rot only: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;

      testMat.constrainRotAxis(true,true,false,testMat2);
      cout << "x and y rot: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;

      testMat.constrainRotAxis(true,false,true,testMat2);
      cout << "x and z rot: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;
      testMat.constrainRotAxis(false,true,true,testMat2);
      cout << "y and z rot: " << "x:" << testMat2.getXRot()
                             << " y:" << testMat2.getYRot()
                             << " z:" << testMat2.getZRot() << endl;



      // Kevin original
      /*
      testMat._kevn_constrainRotAxis(true,false,false,testMat2);
      cout << "pitch  only: " << "y:" << testMat2.getYaw()
                             << " p:" << testMat2.getPitch()
                             << " r:" << testMat2.getRoll() << endl;

      testMat._kevn_constrainRotAxis(false,true,false,testMat2);
      cout << "yaw only: " << "y:" << testMat2.getYaw()
                             << " p:" << testMat2.getPitch()
                             << " r:" << testMat2.getRoll() << endl;
      testMat._kevn_constrainRotAxis(false,false,true,testMat2);
      cout << "roll  only: " << "y:" << testMat2.getYaw()
                             << " p:" << testMat2.getPitch()
                             << " r:" << testMat2.getRoll() << endl;
      */
   }


   return 1;
}


bool testMakeXYZ(float x, float y, float z)
{
   vjMatrix XYZMat;
   vjMatrix temp_mat;
   vjMatrix diff_mat;
   float x_deg, y_deg, z_deg;

   XYZMat.makeXYZEuler(x, y, z);
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   temp_mat.makeXYZEuler(x_deg, y_deg, z_deg);

   if(compareMats(temp_mat, XYZMat, diff_mat))
   {
      return true;
   }
   else
   {
      cout << "testMakeXYZ failed on: XYZ: " << x << ", " << y << ", " << z
           << "\tGot: " <<  x_deg << ", " << y_deg << ", " << z_deg << endl
           << "diff: \n" << diff_mat << endl;
      return false;
   }
}

bool testMakeZYX(float z, float y, float x)
{
   vjMatrix ZYXMat;
   vjMatrix temp_mat;
   vjMatrix diff_mat;
   float x_deg, y_deg, z_deg;

   ZYXMat.makeZYXEuler(z, y, x);
   ZYXMat.getZYXEuler(z_deg, y_deg, x_deg);
   temp_mat.makeZYXEuler(z_deg, y_deg, x_deg);

   if(compareMats(temp_mat, ZYXMat, diff_mat))
   {
      return true;
   }
   else
   {
      cout << "testMakeZYX failed on: ZYX: " << z << ", " << y << ", " << x
           << "\tGot: " <<  z_deg << ", " << y_deg << ", " << x_deg
           << "diff: \n" << diff_mat << endl;
      return false;
   }
}

// Compares the matrices
// If equal (within tolerance), then returns true
// Else returns false and sets the diff matrix
bool compareMats(vjMatrix mat1, vjMatrix mat2, vjMatrix& difMat)
{
   if(mat1 == mat2)
   {
      return true;
   }
   else
   {
      //cout << "\tDifferent.";
      vjMatrix diff_mat;
      diff_mat = mat1 - mat2;

      int i=0;
      float* mat;
      mat = diff_mat.getFloatPtr();    // Get the matrix

      while( (fabs(mat[i]) < 1e-5) && (i<16) )
      {
         i++;
      }

      if(i==16)
      {
         //cout << "  Within tolerance of 1e-5.";
         return true;
      }
      else
      {
         //cout << "Matrices are diff by:\n" << diff_mat << endl;
         difMat = diff_mat;
         return false;
      }

   }
}

