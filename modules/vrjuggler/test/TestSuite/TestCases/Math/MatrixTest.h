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
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Vec3.h>

#include "TestCase.h"
#include "TestSuite.h"
#include "TestCaller.h"

#define testIt(test_desc, test_case) std::cout << test_desc;if((test_case)) { std::cout << "\t [ok]\n"; } else { std::cout << "\t [FAILED]\n";}

class MatrixTest : public TestCase
{
public:
   MatrixTest( std::string name ) : TestCase (name), 
      x_axis( 1.0f, 0.0f, 0.0f ),
      y_axis( 0.0f, 1.0f, 0.0f ),
      z_axis( 0.0f, 0.0f, 1.0f ),
      tests_pass( false )
   {
   }
   virtual ~MatrixTest() {}
   virtual void setUp()
   {
   }

   void setmatrix()
   {
      // ---- Test Matrix set ---- //
      std::cout << "\n---- Testing Set Matrix ---" << std::endl;

      testMat.set(0.0f, 0.1f, 0.2f, 0.3f,
                  1.0f, 1.1f, 1.2f, 1.3f,
                  2.0f, 2.1f, 2.2f, 2.3f,
                  3.0f, 3.1f, 3.2f, 3.3f);

      std::cout << "Command issued:"
                << "testMat.set(0.0f, 0.1f, 0.2f, 0.3f," << std::endl
                << "      1.0f, 1.1f, 1.2f, 1.3f," << std::endl
                << "       2.0f, 2.1f, 2.2f, 2.3f," << std::endl
                << "       3.0f, 3.1f, 3.2f, 3.3f);" << std::endl;
      std::cout << "Matrix:" << std::endl << testMat << std::endl;
   }

   static Test* suite()
   {
      TestSuite *testSuite = new TestSuite ("vrj::MatrixTest");

      testSuite->addTest( new TestCaller<MatrixTest>( "setmatrix", setmatrix ));
      return testSuite;
   }

/*
      void matrixinverse()
      {
         // --- Test Inverstion --- //
   std::cout << "\n---- Test Matrix inverse ---" << std::endl;
   testMat.set(-0.7f, 0.1f,  0.0f, 2.3f,
               -0.1f, -0.7f, 0.0f, 5.0f,
                0.0f, 0.0f,  1.0f, 0.0f,
                0.0f, 0.0f,  0.0f, 2.0f);
   std::cout << "Orig Mat:" << std::endl << testMat << std::endl;
   testMat.invert(testMat);
   std::cout << "Inverted Mat:" << std::endl << testMat << std::endl;
      }      
      
      void mult()
      {
         // ---- Test Mult --- //
   std::cout << "\n---- Test Matrix multiply ---" << std::endl;
   testMat2.set(9.7f, 0.1f,  -1.2f, 2.3f,
               2.1f, -0.7f, 0.75f, 5.0f,
                0.3f, 4.0f,  1.0f, -3.5f,
                2.0f, -3.0f,  7.9f, 2.0f);
   std::cout << "m1:" << std::endl << testMat << std::endl;
   std::cout << "m2:" << std::endl << testMat2 << std::endl;
   testMat.mult(testMat, testMat2);
   std::cout << "result:" << std::endl << testMat << std::endl;
      }
            
      void matmultvec()
      {
      }
      
      void makeeuler()
{
      }
      
      void makexyz()
      {
         // ----- Test Make XYZ ----- //
   std::cout << "\n----  Test makeXYZ ---" << std::endl;
   testMat.makeXYZEuler(-90, -180, 0);
   std::cout << "Command issued:" << " makeXYZEuler(-90, -180, 0)" << std::endl;
   std::cout << "Result:" << std::endl << testMat << std::endl;


   // --------------------------------------- //
   float x_deg, y_deg, z_deg;
   vrj::Matrix XYZMat;

   XYZMat.makeXYZEuler(0, 90, 90);
   std::cout << "makeXYZEuler(0, 90, 90):" << std::endl << XYZMat << std::endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   std::cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //


   XYZMat.makeXYZEuler(10, 0, 0);
   std::cout << "makeXYZEuler(10, 0, 0):" << std::endl << XYZMat << std::endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   std::cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //


   XYZMat.makeXYZEuler(0, 45, 0);
   std::cout << "makeXYZEuler(0, 45, 0):" << std::endl << XYZMat << std::endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   std::cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //


   XYZMat.makeXYZEuler(45, -30, 0);
   std::cout << "makeXYZEuler(45, -30, 0):" << std::endl << XYZMat << std::endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   std::cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //

    XYZMat.makeXYZEuler(-20, 0, -80);
   std::cout << "makeXYZEuler(-20, 0, -80):" << std::endl << XYZMat
             << std::endl;
   XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
   std::cout << "getXYZ:" << x_deg << ", " << y_deg << ", " << z_deg << "\n\n\n";

   // --------------------------------------- //
   testIt("testMakeXYZ(175, -20, 35): ",(testMakeXYZ(175,-20,35)));
   testIt("testMakeXYZ(-135, 20, 15)",(testMakeXYZ(-135, 20, 15)));
   testIt("testMakeXYZ(0, 90, 90)",(testMakeXYZ(0, 90, 90)));
      }            
            
      void randomxyz()
      {
         // ------------ Test random XYZ values ----------------- //
   std::cout << "Random XYZ Eulers: " << std::endl;
   tests_pass = false;

   for(x=0;x<50;x++)
   {
      int xRot = (rand()%360)-180;
      int yRot = (rand()%360)-180;
      int zRot = (rand()%360)-180;

      if(false == testMakeXYZ(xRot, yRot, zRot))
      { tests_pass = false; }
   }
   if(tests_pass)
   { std::cout << "\t[ok]\n"; }
   else
   { std::cout << "\t[FAILED]\n"; }


   // -------------------------------------------
   std::cout << "\nRandom ZYX Eulers:" << std::endl;
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
   { std::cout << "\t[ok]\n"; }
   else
   { std::cout << "\t[FAILED]\n"; }
      }      
      
      
      void serialtransforms()
      {
         // --------------------------------------- //
   // --- Test serial of transforms --------- //
   // --------------------------------------- //
   XYZMat.makeIdent();
   std::cout << "Testing serial transforms around x_axis from identity:"
             << std::endl << XYZMat << std::endl;

   for(x=0;x<(60*60*60*2);x++)
   {
      XYZMat.getXYZEuler(x_deg, y_deg, z_deg);
      std::cout << "getXYZ:iter:" << x << " : xEuler:" << x_deg << ", yEuler:"
                << y_deg << ", zEuler:" << z_deg << "\t\tXRot:"
                << XYZMat.getXRot() << "  YRot:" << XYZMat.getYRot()
                << "  ZRot:" << XYZMat.getZRot() << "\r" << std::flush;
      XYZMat.preRot(0.005f, x_axis, XYZMat);
   }
   std::cout << std::endl << std::endl;
      }      
      
      void transformcreate()
      {
         // --------------------------------------- //
   // ---- Create transform matrix ---------- //
   // --------------------------------------- //
   //  Tracker coord system
   //    x right, y out, z down
   vrj::Matrix rotMat;                            // Matrix to rotate the tracker coord system to C2 coord system
   //rotMat.makeDirCos(vrj::Vec3(1,0,0), vrj::Vec3(0,0,-1), vrj::Vec3(0,1,0));
   rotMat.makeZYXEuler(0,0,90);

   std::cout << "--- The rotMat ----\n" << rotMat << std::endl;

   vrj::Matrix transRotMat;               // Need the transpose for Eulers
   transRotMat.transpose(rotMat);

   std::cout << "---- The transpose rotMat ----\n" << transRotMat << std::endl;

      }      

      void xformtrackeraxis()
      {
         // ------------------------------------- //
   // ----  Test transform tracker axis --- //
   // ------------------------------------- //
   vrj::Vec3 tracker_x_axis, tracker_y_axis,tracker_z_axis, temp_axis;
   tracker_x_axis.set(1.0, 0.0, 0.0);
   tracker_y_axis.set(0.0, 1.0, 0.0);
   tracker_z_axis.set(0.0, 0.0, 1.0);

   std::cout << "\n--------- Tracker axis transforms -------\n";

   temp_axis.xformVec(transRotMat, tracker_x_axis);
   std::cout << "X Axis:"
             << "\t   orig:" << tracker_x_axis
             << "\txformed:" << temp_axis << std::endl;

   temp_axis.xformVec(transRotMat, tracker_y_axis);
   std::cout << "Y Axis:"
             << "\t   orig:" << tracker_y_axis
             << "\txformed:" << temp_axis << std::endl;

   temp_axis.xformVec(transRotMat, tracker_z_axis);
   std::cout << "Z Axis:"
             << "\t   orig:" << tracker_z_axis
             << "\txformed:" << temp_axis << std::endl;

   std::cout << "\n\n\n";
      }      
            
      
      void testortrans()
      {
         // ----------------------------------------- //
   // ------ Test Or tranformation ------------ //
   // ----------------------------------------- //
   vrj::Vec3 tracker_base_dir, new_dir;
   tracker_base_dir.set(1.0f, 0.0f, 0.0f);         // Set the same base dir in both coord systems

   vrj::Matrix tTr;     // Transformation of the reciever in tracker coord system
   vrj::Matrix wTt;     // Transformation of the world coord system to the tracker coord system
   vrj::Matrix wTr;     // Transformation of the reciever in the world coord system

   wTt = rotMat;     // Set the World to tracker coord system rotation matrix

   tTr.makeZYXEuler(0.0, 90.0, 0.0f);            // Ry(90)
   std::cout << "\n-------- Test tracker Or transform ----------------\n"
             << "tracker XYZ: 0.0, 90.0, 0.0" << std::endl;

   new_dir.xformVec(tTr, tracker_base_dir);      // Rotate the base
   std::cout << " tracker rot (tTr):\n" << tTr << std::endl;
   std::cout << "   Base Tracker dir: " << tracker_base_dir << std::endl;
   std::cout << "Xformed Tracker dir (in T): " << new_dir << std::endl;

   std::cout << "\nTest system transform\n";

     // --- Test World (c2) coord system rotations --- //
   std::cout << "wTt:\n" << wTt << std::endl;
   std::cout << "tTr:\n" << tTr << std::endl;

   wTr.mult(wTt, tTr);     // Compute the world to reciever transform

   std::cout << "wTr rotation\n" << wTr << std::endl;
   new_dir.xformVec(wTr, tracker_base_dir);           // Rotate the base into the C2 coord system
   std::cout << "\nBase tracker dir: " << tracker_base_dir << std::endl;
   std::cout << "  Xformed tracker dir (in W): " << new_dir << std::endl;

      // --- Get reciever orientation --- //
   float rot_z, rot_y, rot_x;
   wTr.getZYXEuler(rot_z, rot_y, rot_x);

   std::cout << "\n\nGet ZYX Euler: z:" << rot_z << "\ty:" << rot_y << "\tx:"
             << rot_x << std::endl;

   vrj::Matrix test_mat;
   test_mat.makeZYXEuler(rot_z, rot_y, rot_x);

   std::cout << "Test extraction of same matrix: ";
   if(compareMats(test_mat, wTr, diff_mat) == true)
   {
      std::cout << " Same.";
   }
   else
   {
      std::cout << "Different: \n" << diff_mat << std::endl;
   }
   std::cout << std::endl;


   vrj::Matrix known_mat;
   known_mat.makeZYXEuler(90, 0, 90);
   std::cout << "Test against known matrix: ";

   if(compareMats(known_mat, wTr, diff_mat))
   {
      std::cout << " Same.";
   } else {
      std::cout << "Different: \n" << diff_mat;
   }

   std::cout << std::endl << std::endl;
      }      
      
      void testptxform()
      {
         // ---------------------------------- //
   // ----- Test Pt transformation  ---- //
   // ---------------------------------- //
   vrj::Vec3   original_pt, transformed_pt;

   std::cout << "\n---- Test pt conversion ----\n";

   original_pt.set(0.0f, 1.0f, -1.0f);       // This pt is in the tracker coord system
   std::cout << "orig: " << original_pt << std::endl;

   transformed_pt.xformFull(wTt, original_pt);
   std::cout << "new pt:" << transformed_pt << std::endl;

   // ------------------------------------------ //

   vrj::Matrix dir_cos_test;
   dir_cos_test.makeDirCos(vrj::Vec3(0,1,0), vrj::Vec3(-1,0,0), vrj::Vec3(0,0,1));
   std::cout << "\nmakeDirCos(vrj::Vec3(0,1,0), vrj::Vec3(-1,0,0), vrj::Vec3(0,0,1))"
             << std::endl << dir_cos_test << std::endl;

   // -----------------------------------
   // Test get?Rot funcs
   //
   testMat2.makeRot(47.0,vrj::Vec3(1,0,0));
   testIt("getXRot..",((testMat2.getXRot() == 47.0f) && (testMat2.getYRot() == 0.0f) && (testMat2.getZRot() == 0.0f)));

   testMat2.makeRot(-56.0,vrj::Vec3(0,1,0));
   testIt("getYRot..",((testMat2.getXRot() == 0.0f) && (testMat2.getYRot() == -56.0f) && (testMat2.getZRot() == 0.0f)));
   std::cout << "x: " << testMat2.getXRot() << ", y:" << testMat2.getYRot()
             << ", z:" << testMat2.getZRot() << std::endl;

   testMat2.makeRot(78.0, vrj::Vec3(0,0,1));
   testIt("getZRot..",((testMat2.getXRot() == 0.0f) && (testMat2.getYRot() == 0.0f) && (testMat2.getZRot() == 78.0f)));
      }
     
      void testaxisconstraints()
      {
          // ------------------------------------
   // ---- Test axis constraints ---------
   // ------------------------------------
   // Idea: Create a matrix, contrain an axis, then check if the axis has a value
   {
      float pitch(0),yaw(0),roll(0);

      std::cout << "contrainMat: makeXYZEuler(2.0,67.0,-76.0)\n";
      testMat.makeXYZEuler(2.0,67.0,-76.0);
      std::cout << "Original Matrix: " << "x:" << testMat.getXRot()
                                       << " y:" << testMat.getYRot()
                                       << " z:" << testMat.getZRot()
                                       << std::endl;

      testMat.constrainRotAxis(true,false,false,testMat2);
      std::cout << "x rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;

      testMat.constrainRotAxis(false,true,false,testMat2);
      std::cout << "y rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;
      testMat.constrainRotAxis(false,false,true,testMat2);
      std::cout << "z rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;

      testMat.constrainRotAxis(true,true,false,testMat2);
      std::cout << "x and y rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;

      testMat.constrainRotAxis(true,false,true,testMat2);
      std::cout << "x and z rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;
      testMat.constrainRotAxis(false,true,true,testMat2);
      std::cout << "y and z rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;

      // Kevin's original

      std::cout << "Kevin's original version." << std::endl;
      testMat._kevn_constrainRotAxis(true,false,false,testMat2);
      std::cout << "x rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;

      testMat._kevn_constrainRotAxis(false,true,false,testMat2);
      std::cout << "y rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;
      testMat._kevn_constrainRotAxis(false,false,true,testMat2);
      std::cout << "z rot only: " << "x:" << testMat2.getXRot()
                                  << " y:" << testMat2.getYRot()
                                  << " z:" << testMat2.getZRot() << std::endl;

      testMat._kevn_constrainRotAxis(true,true,false,testMat2);
      std::cout << "x and y rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;

      testMat._kevn_constrainRotAxis(true,false,true,testMat2);
      std::cout << "x and z rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;
      testMat._kevn_constrainRotAxis(false,true,true,testMat2);
      std::cout << "y and z rot: " << "x:" << testMat2.getXRot()
                                   << " y:" << testMat2.getYRot()
                                   << " z:" << testMat2.getZRot() << std::endl;

   }
      }             
      
      
      protected:
            bool testMakeXYZ(float x, float y, float z)
{
   vrj::Matrix XYZMat;
   vrj::Matrix temp_mat;
   vrj::Matrix diff_mat;
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
      std::cout << "testMakeXYZ failed on: XYZ: " << x << ", " << y << ", "
                << z << "\tGot: " <<  x_deg << ", " << y_deg << ", " << z_deg
                << std::endl << "diff: \n" << diff_mat << std::endl;
      return false;
   }
}

bool testMakeZYX(float z, float y, float x)
{
   vrj::Matrix ZYXMat;
   vrj::Matrix temp_mat;
   vrj::Matrix diff_mat;
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
      std::cout << "testMakeZYX failed on: ZYX: " << z << ", " << y << ", "
                << x << "\tGot: " <<  z_deg << ", " << y_deg << ", " << x_deg
                << "diff: \n" << diff_mat << std::endl;
      return false;
   }
}

// Compares the matrices
// If equal (within tolerance), then returns true
// Else returns false and sets the diff matrix
bool compareMats(vrj::Matrix mat1, vrj::Matrix mat2, vrj::Matrix& difMat)
{
   if(mat1 == mat2)
   {
      return true;
   }
   else
   {
      //std::cout << "\tDifferent.";
      vrj::Matrix diff_mat;
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
         //std::cout << "  Within tolerance of 1e-5.";
         return true;
      }
      else
      {
         //std::cout << "Matrices are diff by:\n" << diff_mat << std::endl;
         difMat = diff_mat;
         return false;
      }

   }
}

*/

protected:
   vrj::Matrix testMat;
   vrj::Matrix testMat2;
   vrj::Matrix diff_mat;
   const vrj::Vec3 x_axis;
   const vrj::Vec3 y_axis;
   const vrj::Vec3 z_axis;

   bool     tests_pass;
   long x;
};
