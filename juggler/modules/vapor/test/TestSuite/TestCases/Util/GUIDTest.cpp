#include <vpr/vpr.h>
#include <vpr/Util/GUID.h>
//#include <vpr/Util/GUIDFactory.h>
#include <vpr/Util/Interval.h>

#include <vpr/Util/Debug.h>

#include <TestCases/Util/GUIDTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( GUIDTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GUIDMetricTest, MySuites::metric() );

void GUIDTest::testConstructor ()
{
   std::string guid_str("4A781D61-3D28-11D2-8DB8-00609703C14E");
   vpr::GUID::StdGUID guid_struct =
   { 0x4A781D61, 0x3D28, 0x11D2, 0x8D, 0xB8,
     { 0x00, 0x60, 0x97, 0x03, 0xC1, 0x4E }
   };

   vpr::GUID* guid1 = new vpr::GUID(guid_str);
   CPPUNIT_ASSERT(guid_str.compare(guid1->toString()) == 0);

   vpr::GUID* guid2 = new vpr::GUID(guid_struct);
   CPPUNIT_ASSERT(*guid1 == *guid2);

   vpr::GUID guid1_again("4A781D61-3D28-11D2-8DB8-00609703C14E");
   CPPUNIT_ASSERT(guid1_again == (*guid2));

   // Test assignment
   //vpr::GUID::generate_tag_type gen_tag;
   //vpr::GUID guid3( gen_tag );
   //typedef vpr::GUID::generate_tag_type gen_type;
   vpr::GUID guid3( vpr::GUID::generateTag );
   //vpr::GUID guidtemp( gen_type() );
   vpr::GUID guid4;
   guid4 = guid3;
   CPPUNIT_ASSERT(guid3 == guid4);
   CPPUNIT_ASSERT(guid3 != vpr::GUID::NullGUID);   // Make sure it generated something

   // Test copy constructor
   vpr::GUID guid5(guid3);
   CPPUNIT_ASSERT(guid5 == guid3);

   delete guid1;
   delete guid2;
}

void GUIDTest::testCompare ()
{
   vpr::GUID* guid1 = new vpr::GUID; guid1->generate();
   vpr::GUID* guid2 = new vpr::GUID; guid2->generate();

   CPPUNIT_ASSERT(*guid1 != *guid2);

   delete guid1;
   delete guid2;

   vpr::GUID guid3("90c8e44b-8a50-4b78-8167-bb944f439a3a");
   vpr::GUID guid4("a7be7b42-f876-4268-878a-46c01736efe3");

   //std::cout << "guid3: " << guid3 << " --- should be: 90c8e44b-8a50-4b78-8167-bb944f439a3a" << std::endl;

   //std::cout << "vprDBG_ALL: " << vprDBG_ALL << std::endl;

   vpr::GUID guid5(vprDBG_ALL.mGuid);     // Copy over known guid
   vpr::GUID guid6;

   CPPUNIT_ASSERT(guid3 != guid4);
   CPPUNIT_ASSERT(guid3 == guid3);
   CPPUNIT_ASSERT(guid4 == guid4);

   CPPUNIT_ASSERT(vprDBG_ALL.mGuid == guid5);
   CPPUNIT_ASSERT(guid5 == vprDBG_ALL.mGuid);

   guid6 = guid3;
   CPPUNIT_ASSERT(guid6 == guid3);
}

void GUIDMetricTest::testCreationOverhead()
{
   const vpr::Uint32 iters(1000);
   vpr::Uint32 loops = iters;
   vpr::GUID guid1;

   CPPUNIT_METRIC_START_TIMING();

   while(loops--)
   {
     guid1.generate();    // = *(vpr::GUIDFactory::createRandomGUID());
   }

   CPPUNIT_METRIC_STOP_TIMING();
   CPPUNIT_ASSERT_METRIC_TIMING_LE("GUIDTest/CreationOverhead", iters, 0.05f, 0.1f);
}

void GUIDMetricTest::testDebugOutput()
{
    /*
    if (0>100)
       ;
    else
    {
       bool enabled = (vpr::Debug::instance()->isDebugEnabled());
       bool low_level = (0 <= vpr::Debug::instance()->getLevel());
       bool cat_allowed = (vpr::Debug::instance()->isCategoryAllowed(vprDBG_ALL));

       if(enabled && low_level && cat_allowed)
         vpr::Debug::instance()->getStream(vprDBG_ALL, 0, true) << "out: -1" << vpr::StreamUnLock(vpr::Debug::instance()->debugLock()) << std::flush;
    }
    */

   std::cout << "\n------ Should see debug output ----" << std::endl;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "out: 0" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "out: 1" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "out: 2" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "out: 3" << vprDEBUG_FLUSH;
   std::cout << std::endl;

}

} // End of vprTest namespace
