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
      assertTest( s == vpr::ReturnStatus::Success );
      assertTest( s.code() == vpr::ReturnStatus::Success );

      // ReturnStatus::Code constructor test
      vpr::ReturnStatus a( vpr::ReturnStatus::WouldBlock );
      assertTest( a == vpr::ReturnStatus::WouldBlock );
      assertTest( a.code() == vpr::ReturnStatus::WouldBlock );

      // copy constructor test
      vpr::ReturnStatus b( a );
      assertTest( b == vpr::ReturnStatus::WouldBlock );
      assertTest( b.code() == vpr::ReturnStatus::WouldBlock );

      // setCode test
      a.setCode( vpr::ReturnStatus::Failure );
      assertTest( a == vpr::ReturnStatus::Failure );
      assertTest( a.code() == vpr::ReturnStatus::Failure );

      // status = status test
      s.setCode( vpr::ReturnStatus::Success );
      a.setCode( vpr::ReturnStatus::Failure );
      a = s;
      assertTest( a == vpr::ReturnStatus::Success );
      assertTest( a.code() == vpr::ReturnStatus::Success );

      // status = code test
      a.setCode( vpr::ReturnStatus::Failure );
      a = vpr::ReturnStatus::Success;
      assertTest( a == vpr::ReturnStatus::Success );
      assertTest( a.code() == vpr::ReturnStatus::Success );

      // code() test
      vpr::ReturnStatus c;
      a.setCode( vpr::ReturnStatus::Failure );
      c.setCode( a.code() );
      assertTest( c == vpr::ReturnStatus::Failure );
      assertTest( c.code() == vpr::ReturnStatus::Failure );



      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Success );
      b.setCode( vpr::ReturnStatus::Success );

      // ReturnStatus == ReturnStatus test
      assertTest( a == b );

      // Code == Code test
      assertTest( a.code() == b.code() );

      // ReturnStatus == Code test
      assertTest( a == b.code() );
      assertTest( b == a.code() );


      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Success );
      b.setCode( vpr::ReturnStatus::Failure );

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
      a.setCode( vpr::ReturnStatus::Success );
      b.setCode( vpr::ReturnStatus::Failure );
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
