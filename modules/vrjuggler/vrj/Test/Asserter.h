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
   /** Throw failure exception with given message and line number info
   */
   void fail( vrj::test::Test* test, vrj::test::Message message, std::string filename, int linenum )
   {
      throw vrj::test::TestFailure(test, message, filename, linenum);                
   }

   /** Throws an exception with given message and line information
   * if shouldFail == true
   */
   void failIf( bool shouldFail, vrj::test::Test* test, vrj::test::Message message, std::string filename, int linenum )
   {
      if( shouldFail )
         fail(test,message,filename,linenum);
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

