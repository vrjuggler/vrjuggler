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

class ReturnStatusTest : public CppUnit::TestCase
{
public:
   ReturnStatusTest() : CppUnit::TestCase ()
   {
   }


   virtual ~ReturnStatusTest()
   {
   }

   void basic()
   {
      // default constructor test
      vpr::ReturnStatus s;
      assert( s == vpr::ReturnStatus::Succeed );
      assert( s.code() == vpr::ReturnStatus::Succeed );

      // ReturnStatus::Code constructor test
      vpr::ReturnStatus a( vpr::ReturnStatus::WouldBlock );
      assert( a == vpr::ReturnStatus::WouldBlock );
      assert( a.code() == vpr::ReturnStatus::WouldBlock );

      // copy constructor test
      vpr::ReturnStatus b( a );
      assert( b == vpr::ReturnStatus::WouldBlock );
      assert( b.code() == vpr::ReturnStatus::WouldBlock );

      // setCode test
      a.setCode( vpr::ReturnStatus::Fail );
      assert( a == vpr::ReturnStatus::Fail );
      assert( a.code() == vpr::ReturnStatus::Fail );

      // status = status test
      s.setCode( vpr::ReturnStatus::Succeed );
      a.setCode( vpr::ReturnStatus::Fail );
      a = s;
      assert( a == vpr::ReturnStatus::Succeed );
      assert( a.code() == vpr::ReturnStatus::Succeed );

      // status = code test
      a.setCode( vpr::ReturnStatus::Fail );
      a = vpr::ReturnStatus::Succeed;
      assert( a == vpr::ReturnStatus::Succeed );
      assert( a.code() == vpr::ReturnStatus::Succeed );

      // code() test
      vpr::ReturnStatus c;
      a.setCode( vpr::ReturnStatus::Fail );
      c.setCode( a.code() );
      assert( c == vpr::ReturnStatus::Fail );
      assert( c.code() == vpr::ReturnStatus::Fail );



      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Succeed );

      // ReturnStatus == ReturnStatus test
      assert( a == b );

      // Code == Code test
      assert( a.code() == b.code() );

      // ReturnStatus == Code test
      assert( a == b.code() );
      assert( b == a.code() );


      // setup some stuff....
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Fail );

      // ReturnStatus != ReturnStatus test
      assert( a != b );

      // Code != Code test
      assert( a.code() != b.code() );

      // ReturnStatus != Code test
      assert( a != b.code() );
      assert( b != a.code() );
   }

   void helpers()
   {
      // setup some stuff....
      vpr::ReturnStatus a, b, c;
      a.setCode( vpr::ReturnStatus::Succeed );
      b.setCode( vpr::ReturnStatus::Fail );
      c.setCode( vpr::ReturnStatus::WouldBlock );

      assert( a.success() );
      assert( b.failure() );
      assert( c.wouldBlock() );
   }

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<ReturnStatusTest>("basic vpr::ReturnStatus Test", &ReturnStatusTest::basic));
      suite->addTest( new CppUnit::TestCaller<ReturnStatusTest>("Helpers for vpr::ReturnStatus test", &ReturnStatusTest::helpers));
   }
};

}

#endif
