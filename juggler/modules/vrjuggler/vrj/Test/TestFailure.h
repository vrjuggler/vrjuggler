/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _TEST_FAILURE_H_
#define _TEST_FAILURE_H_

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>

#include <vpr/Util/Assert.h>

#include <vrj/Test/Message.h>
#include <vrj/Test/Test.h>

#include <string>
#include <vector>
#include <sstream>

#include <exception>

namespace vrj {
namespace test
{

/**
* This is the exception that is thrown when a test fails.
*
* It contains enough information to give a description of
* what failed and to track this a little bit.
*
* These are collected to track the failed tests.
* 
*/
class TestFailure : public std::exception
{
public:
   TestFailure(Test* failedTest, vrj::test::Message message, 
               const std::string& fileName, int lineNumber) throw()
      : mFailedTest(failedTest), mMessage(message), 
        mFileName(fileName), mLineNumber(lineNumber)
   {

   }

   virtual ~TestFailure() throw()
   {;}

   Test* getFailedTest()
   { return mFailedTest; }

   std::string getFailedTestName()
   { return mFailedTest->getName(); }

   vrj::test::Message getMessage() const
   { return mMessage; }
   
   std::string getFileName() const
   { return mFileName; }

   int getLineNumber() const
   { return mLineNumber; }

   std::string getFullDescription()
   {
      vprASSERT(NULL != mFailedTest);

      std::stringstream oss;
      oss << getFailedTestName() << ": ";
      oss << getFileName() << ": line:";
      oss << getLineNumber() << ": ";
      oss << getMessage().shortDesc();
      oss << "\n" << getMessage().details();
      return oss.str();
   }


protected:
   Test*                mFailedTest;      /**< The test that failed */
   vrj::test::Message   mMessage;         /**< Message describing the failure */
   std::string          mFileName;        /**< Filename where it occured */
   int                  mLineNumber;      /**< The line number it happened on */
};

} }

#endif

