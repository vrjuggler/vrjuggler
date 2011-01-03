/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <vector>

#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include <vrj/vrjConfig.h>
#ifdef VJ_API_PERFORMER
#include <TestCases/Pf/PfUtilTest.h> /* needs performer linked in, doh! */
#endif
#include <vrj/Util/Debug.h>


int main (int ac, char **av)
{
   // Do this here to get init text out of the way
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Starting test\n" << vprDEBUG_FLUSH;

   CppUnit::TextUi::TestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------
   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite = new CppUnit::TestSuite("noninteractive");

   // add tests to the suite

#ifdef VJ_API_PERFORMER
   noninteractive_suite->addTest(vrjTest::PfUtilTest::suite());
#endif

   // Add the test suite to the runner
   runner.addTest(noninteractive_suite);

   // run all test suites
   runner.run();

   return 0;
}
