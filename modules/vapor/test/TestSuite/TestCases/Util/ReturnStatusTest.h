#ifndef _STATUS_TEST_H
#define _STATUS_TEST_H

#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/Util/ReturnStatus.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class ReturnStatusTest : public TestCase
{
public:
   ReturnStatusTest( std::string name ) : TestCase (name)
   {
   }


   virtual ~ReturnStatusTest()
   {
   }

   void basic()
   {
      // default constructor test
      vpr::ReturnStatus s;
      assertTest( s == vpr::ReturnStatus::Succeed );
      assertTest( s.code() == vpr::ReturnStatus::Succeed );

      // ReturnStatus::Code constructor test
      vpr::ReturnStatus a( vpr::ReturnStatus::WouldBlock );
      assertTest( a == vpr::ReturnStatus::WouldBlock );
      assertTest( a.code() == vpr::ReturnStatus::WouldBlock );

      // copy constructor test
      vpr::ReturnStatus b( a );
      assertTest( b == vpr::ReturnStatus::WouldBlock );
      assertTest( b.code() == vpr::ReturnStatus::WouldBlock );

      // setCode test
      a.setCode( vpr::ReturnStatus::Fail );
      assertTest( a == vpr::ReturnStatus::Fail );
      assertTest( a.code() == vpr::ReturnStatus::Fail );

      // status = status test
      s.setCode( vpr::ReturnStatus::Succeed );
      a.setCode( vpr::ReturnStatus::Fail );
      a = s;
      assertTest( a == vpr::ReturnStatus::Succeed );
      assertTest( a.code() == vpr::ReturnStatus::Succeed );

      // status = code test
      a.setCode( vpr::ReturnStatus::Fail );
      a = vpr::ReturnStatus::Succeed;
      assertTest( a == vpr::ReturnStatus::Succeed );
      assertTest( a.code() == vpr::ReturnStatus::Succeed );

      // code() test
      vpr::ReturnStatus c;
      a.setCode( vpr::ReturnStatus::Fail );
      c.setCode( a.code() );
      assertTest( c == vpr::ReturnStatus::Fail );
      assertTest( c.code() == vpr::ReturnStatus::Fail );



      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Succeed );

      // ReturnStatus == ReturnStatus test
      assertTest( a == b );

      // Code == Code test
      assertTest( a.code() == b.code() );

      // ReturnStatus == Code test
      assertTest( a == b.code() );
      assertTest( b == a.code() );


      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Fail );

      // ReturnStatus != ReturnStatus test
      assertTest( a != b );

      // Code != Code test
      assertTest( a.code() != b.code() );

      // ReturnStatus != Code test
      assertTest( a != b.code() );
      assertTest( b != a.code() );
   }

   void helpers()
   {
      // setup some stuff....
      vpr::ReturnStatus a, b, c;
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Fail );
      c.setCode( vpr::ReturnStatus::WouldBlock );

      assertTest( a.success() );
      assertTest( b.failure() );
      assertTest( c.wouldBlock() );
   }

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("ReturnStatusTest");

      test_suite->addTest( new TestCaller<ReturnStatusTest>("basic vpr::ReturnStatus Test", &ReturnStatusTest::basic));
      test_suite->addTest( new TestCaller<ReturnStatusTest>("Helpers for vpr::ReturnStatus test", &ReturnStatusTest::helpers));
      return test_suite;
   }
};

}

#endif
