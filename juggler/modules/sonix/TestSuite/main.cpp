
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * Juggler Juggler
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


#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>

//#include <TestCases/snxMathTest.h>
#include <TestCases/exampleTest.h>
//#include <Utils/Debug.h>


int main( int ac, char **av )
{
//    vprDEBUG(0,0) << "Starting test\n" << vprDEBUG_FLUSH;       

   CppUnit::TextTestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite = new CppUnit::TestSuite("NonInteractive");

   // add tests to the suite
   noninteractive_suite->addTest( snxTest::exampleTest::suite());
  // noninteractive_suite->addTest( snxTest::snxMathTest::suite());

   // Add the test suite to the runner
   runner.addTest( noninteractive_suite );

   // create test suite #2
   //CppUnit::TestSuite* interactive_suite = new CppUnit::TestSuite("Interactive");

   // add tests to the suite
   //interactive_suite->addTest(ThreadTest::suite());

   // Add the test suite to the runner
   //runner.addTest("interactive", interactive_suite);

   // run all test suites
   runner.run();


   return 0;
}
