
/****************** <AJ heading BEGIN do not edit this line> *****************
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
 ****************** <AJ heading END do not edit this line> ******************/


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

#include <aj/Matrix44.h> // 4x4 matrix math
#include <aj/Vec3.h>     // vector math
#include <aj/MatVec.h>   // for matrix/vec math

//#include <IO/Socket/InetAddr.h>

namespace ajTest
{

class ajMathTest : public TestCase
{
public:
   ajMathTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~ajMathTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testSimple()
   {
      aj::Matrix44 mat;
      mat.makeRot( 180, 0, 1, 0 );
      
      aj::Vec3 forward( 0,0,-1 );
      
      aj::Vec3 vec = aj::xformVec( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      vec = aj::xformFull( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      
      assertTest( true );
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("ajMathTest");
      test_suite->addTest( new TestCaller<ajMathTest>("testSimple", &ajMathTest::testSimple));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveajMathTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
