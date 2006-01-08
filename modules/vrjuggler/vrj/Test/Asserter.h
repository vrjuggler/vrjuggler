/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_TEST_ASSERTER_H
#define _VRJ_TEST_ASSERTER_H

#include <vrj/Test/TestFailure.h>
#include <vrj/Test/Test.h>
#include <vrj/Test/Message.h>

namespace vrj {
namespace test
{

namespace  Asserter
{
   /** Throw failure exception with given message and line number info. */
   void fail(vrj::test::Test* test, vrj::test::Message message,
             const std::string& filename, const int linenum)
   {
      throw vrj::test::TestFailure(test, message, filename, linenum);
   }

   /**
    * Throws an exception with given message and line information.
    * if shouldFail == true.
    */
   void failIf(const bool shouldFail, vrj::test::Test* test,
               vrj::test::Message message, const std::string& filename,
               const int linenum)
   {
      if( shouldFail )
      {
         fail(test, message, filename, linenum);
      }
   }
}

} }


/** Assertions that a condition is \c true.
 * \ingroup Assertions
 */
#define VRJTEST_ASSERT(condition)                                                 \
  ( vrj::test::Asserter::failIf( !(condition), this,                              \
                                 vrj::test::Message( "assertion failed",          \
                                                      "Expression: " #condition), \
                                 __FILE__, __LINE__ ) )


#endif

