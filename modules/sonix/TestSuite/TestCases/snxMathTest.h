
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * Audio Juggler
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/


/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <snx/Matrix44.h> // 4x4 matrix math
#include <snx/Vec3.h>     // vector math
#include <snx/MatVec.h>   // for matrix/vec math

//#include <IO/Socket/InetAddr.h>

namespace snxTest
{

class snxMathTest : public TestCase
{
public:
   snxMathTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~snxMathTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testSimple()
   {
      snx::Matrix44 mat;
      mat.makeRot( 180.0f * (3.145f / 180.0f), 0, 1, 0 );
      
      std::cout<<mat<<"\n"<<std::flush;
      
      
      snx::Vec3 forward( 0,0,-1 );
      
      snx::Vec3 vec = snx::xformVec( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      vec = snx::xformFull( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      
      assertTest( true );
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("snxMathTest");
      test_suite->addTest( new TestCaller<snxMathTest>("testSimple", &snxMathTest::testSimple));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractivesnxMathTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
