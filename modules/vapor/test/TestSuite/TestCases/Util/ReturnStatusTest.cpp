#include <vpr/Util/ReturnStatus.h>

#include <TestCases/Util/ReturnStatusTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( ReturnStatusTest );

void ReturnStatusTest::basic()
{
   // default constructor test
   vpr::ReturnStatus s;
   CPPUNIT_ASSERT( s == vpr::ReturnStatus::Succeed );
   CPPUNIT_ASSERT( s.code() == vpr::ReturnStatus::Succeed );

   // ReturnStatus::Code constructor test
   vpr::ReturnStatus a( vpr::ReturnStatus::WouldBlock );
   CPPUNIT_ASSERT( a == vpr::ReturnStatus::WouldBlock );
   CPPUNIT_ASSERT( a.code() == vpr::ReturnStatus::WouldBlock );

   // copy constructor test
   vpr::ReturnStatus b( a );
   CPPUNIT_ASSERT( b == vpr::ReturnStatus::WouldBlock );
   CPPUNIT_ASSERT( b.code() == vpr::ReturnStatus::WouldBlock );

   // setCode test
   a.setCode( vpr::ReturnStatus::Fail );
   CPPUNIT_ASSERT( a == vpr::ReturnStatus::Fail );
   CPPUNIT_ASSERT( a.code() == vpr::ReturnStatus::Fail );

   // status = status test
   s.setCode( vpr::ReturnStatus::Succeed );
   a.setCode( vpr::ReturnStatus::Fail );
   a = s;
   CPPUNIT_ASSERT( a == vpr::ReturnStatus::Succeed );
   CPPUNIT_ASSERT( a.code() == vpr::ReturnStatus::Succeed );

   // status = code test
   a.setCode( vpr::ReturnStatus::Fail );
   a = vpr::ReturnStatus::Succeed;
   CPPUNIT_ASSERT( a == vpr::ReturnStatus::Succeed );
   CPPUNIT_ASSERT( a.code() == vpr::ReturnStatus::Succeed );

   // code() test
   vpr::ReturnStatus c;
   a.setCode( vpr::ReturnStatus::Fail );
   c.setCode( a.code() );
   CPPUNIT_ASSERT( c == vpr::ReturnStatus::Fail );
   CPPUNIT_ASSERT( c.code() == vpr::ReturnStatus::Fail );



   // setup some stuff....
   a.setCode( vpr::ReturnStatus::Succeed );
   b.setCode( vpr::ReturnStatus::Succeed );

   // ReturnStatus == ReturnStatus test
   CPPUNIT_ASSERT( a == b );

   // Code == Code test
   CPPUNIT_ASSERT( a.code() == b.code() );

   // ReturnStatus == Code test
   CPPUNIT_ASSERT( a == b.code() );
   CPPUNIT_ASSERT( b == a.code() );


   // setup some stuff....
   a.setCode( vpr::ReturnStatus::Succeed );
   b.setCode( vpr::ReturnStatus::Fail );

   // ReturnStatus != ReturnStatus test
   CPPUNIT_ASSERT( a != b );

   // Code != Code test
   CPPUNIT_ASSERT( a.code() != b.code() );

   // ReturnStatus != Code test
   CPPUNIT_ASSERT( a != b.code() );
   CPPUNIT_ASSERT( b != a.code() );
}

void ReturnStatusTest::helpers()
{
   // setup some stuff....
   vpr::ReturnStatus a, b, c;
   a.setCode( vpr::ReturnStatus::Succeed );
   b.setCode( vpr::ReturnStatus::Fail );
   c.setCode( vpr::ReturnStatus::WouldBlock );

   CPPUNIT_ASSERT( a.success() );
   CPPUNIT_ASSERT( b.failure() );
   CPPUNIT_ASSERT( c.wouldBlock() );
}

} // End of vprTest namespace
