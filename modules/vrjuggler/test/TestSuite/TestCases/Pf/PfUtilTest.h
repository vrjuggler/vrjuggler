#ifndef VRJ_PFUTILS_TESTS
#define VRJ_PFUTILS_TESTS

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace vrjTest
{

class PfUtilTest : public CppUnit::TestCase
{
public:
   PfUtilTest (std::string name) : CppUnit::TestCase (name)
   {;}
   
   PfUtilTest() : CppUnit::TestCase ()
   {;}
   
   virtual ~PfUtilTest()
   {;}

   void testMatrixConversion();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* suite = new CppUnit::TestSuite ("PfUtilTest");
      suite->addTest( new CppUnit::TestCaller<PfUtilTest>( "test pf --> vj matrix conversion", &PfUtilTest::testMatrixConversion));

      return suite;
   }
};

}

#endif
