#ifndef _GUID_TEST_H_
#define _GUID_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/GUID.h>
#include <vpr/Util/GUIDFactory.h>
#include <vpr/Util/Interval.h>


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


   virtual ~GUIDTest()
   {
   }

   void testConstructor ()
   {
      std::string guid_str("4A781D61-3D28-11D2-8DB8-00609703C14E");
      vpr::GUID::StdGUID guid_struct =
      { 0x4A781D61, 0x3D28, 0x11D2, 0x8D, 0xB8,
        { 0x00, 0x60, 0x97, 0x03, 0xC1, 0x4E }
      };

      vpr::GUID* guid1 = vpr::GUIDFactory::createGUID(guid_str);
      CPPUNIT_ASSERT(guid_str.compare(guid1->toString()) == 0);

      vpr::GUID* guid2 = vpr::GUIDFactory::createGUID(guid_struct);
      CPPUNIT_ASSERT(*guid1 == *guid2);

      // Test assignment
      vpr::GUID guid3(true), guid4(true);
      guid3 = guid4;
      CPPUNIT_ASSERT(guid3 == guid4);

      // Test copy constructor
      vpr::GUID guid5(guid3);
      CPPUNIT_ASSERT(guid5 == guid3);

      delete guid1;
      delete guid2;
   }

   void testCompare ()
   {
      vpr::GUID* guid1 = vpr::GUIDFactory::createRandomGUID();
      vpr::GUID* guid2 = vpr::GUIDFactory::createRandomGUID();

      CPPUNIT_ASSERT(*guid1 != *guid2);

      delete guid1;
      delete guid2;
   }

   void testCreationOverhead()
   {
      const vpr::Uint32 iters(100000);
      vpr::Uint32 loops = iters;
      vpr::GUID guid1;

      vpr::Interval time_in, time_out;
      time_in.setNow();

      while(loops--)
      {
        guid1 = vpr::GUIDFactory::createRandomGUID();
      }

      vpr::Interval diff = time_out - time_in;

      double per_call;      // Num ns per call
      per_call = (diff.usecf()*1000.0f) / double(iters);

      std::cout << "vpr::GUID(): overhead = " << per_call << "ns per call\n"
                << std::flush;
   }

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest(new CppUnit::TestCaller<GUIDTest>("testConstructor",
                                                       &GUIDTest::testConstructor));
      suite->addTest(new CppUnit::TestCaller<GUIDTest>("testCompare",
                                                       &GUIDTest::testCompare));
   }

   void registerMetricsTests (CppUnit::TestSuite* suite)
   {
      suite->addTest(new CppUnit::TestCaller<GUIDTest>("testCreationOverhead",
                                                       &GUIDTest::testCreationOverhead));
   }

};

}


#endif
