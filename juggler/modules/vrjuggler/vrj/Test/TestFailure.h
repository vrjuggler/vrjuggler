#ifndef _TEST_FAILURE_H_
#define _TEST_FAILURE_H_

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>

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
      : mMessage(message), 
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
      std::stringstream oss;
      oss << getFailedTestName() << ": " << getFileName() << ": line:" << getLineNumber() 
          << ": " << getMessage().shortDesc() << " -- " << getMessage().details();
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

