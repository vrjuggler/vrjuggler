/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _FAILURE_TEST_CASE_H
#define _FAILURE_TEST_CASE_H

#include <vrj/Test/TestCase.h>
#include <vrj/Test/Asserter.h>

#include <iostream>
#include <string>

                     
/** A very small class that demonstrates how to 
* fail a test.
*/
class FailureTestCase : public vrj::test::TestCase
{
public:
   FailureTestCase()
      : vrj::test::TestCase("FailureTestCase")
   {;}

   virtual ~FailureTestCase()
   {;}

   /** Do any setup needed before the test starts */
   virtual void setUp()
   {
      std::cout << "FailureTestCase::setup: Nothing to setup\n";
   }

   /** Do any cleanup needed after running */
   virtual void tearDown()
   {
      std::cout << "FailureTestCase::tearDown: Nothing to tear down\n";
   }

   /** Gives the test time to process (ie. run & test)
   * This method is called by the runner at then end
   * of preFrame until isDone is true.
   * @pre isDone() == false
   * @throws vrj::test::TestFailure - Thrown if the test fails
   */
   virtual void processTest()
   {
      VRJTEST_ASSERT(false && "This is supposed to fail");

      /*
      vrj::test::Asserter::failIf( !(false), this,                         \
                                 vrj::test::Message( "assertion failed",  \
                                                      "Expression: "),    \
                                 __FILE__, __LINE__ );
                                 */

   }

   /** Return true when the test is done processing
   * testing 
   */
   virtual bool isDone()
   {
      return false;     /* should just error out instead */
   }

};


#endif

