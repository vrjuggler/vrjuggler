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
#include <vector>

#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>

#include <TestCases/Math/QuatSelfTest.h>
//#include <TestCases/Pf/PfUtilTest.h> // needs performer linked in, doh!
#include <vrj/Util/Debug.h>


int main (int ac, char **av)
{
   vprDEBUG(0,0) << "Starting test\n" << vprDEBUG_FLUSH;       // Do this here to get init text out of the way

   CppUnit::TextTestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------
   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite = new CppUnit::TestSuite("noninteractive");

   // add tests to the suite
   noninteractive_suite->addTest(vrjTest::QuatSelfTest::suite());
//   noninteractive_suite->addTest(vrjTest::PfUtilTest::suite());

   // Add the test suite to the runner
   runner.addTest(noninteractive_suite);

   // run all test suites
   runner.run();

   return 0;
}
