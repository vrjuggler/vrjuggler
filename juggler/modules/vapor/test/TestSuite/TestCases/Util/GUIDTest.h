#ifndef _VPRTEST_GUID_TEST_H_
#define _VPRTEST_GUID_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/MetricRegistry.h>


/*****************************************************************
 tests out the functionality expected of vpr::GUID
*******************************************************************/

namespace vprTest
{

class GUIDTest : public CppUnit::TestCase
{
public:
   GUIDTest() : CppUnit::TestCase ()
   {
   }

   GUIDTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~GUIDTest()
   {
   }

   void testConstructor ();

   void testCompare ();

   void testCreationOverhead();

   void testDebugOutput();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("GUIDTest");

      test_suite->addTest(new CppUnit::TestCaller<GUIDTest>("testConstructor",
                                                            &GUIDTest::testConstructor));
      test_suite->addTest(new CppUnit::TestCaller<GUIDTest>("testCompare",
                                                            &GUIDTest::testCompare));
      test_suite->addTest(new CppUnit::TestCaller<GUIDTest>("testCreationOverhead",
                                                            &GUIDTest::testCreationOverhead));

      return test_suite;
   }

   static CppUnit::Test* metric_suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("GUIDTest_metric");

      test_suite->addTest(new CppUnit::TestCaller<GUIDTest>("testDebugOutput", &GUIDTest::testDebugOutput));

      test_suite->addTest(new CppUnit::TestCaller<GUIDTest>("testCreationOverhead",
                                                            &GUIDTest::testCreationOverhead));

      return test_suite;
   }

};

}


#endif
