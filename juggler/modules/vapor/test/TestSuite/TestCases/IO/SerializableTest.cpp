#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/SerializableObject.h>

// Serializable objects to test
//#include <plx/Router/PlxAddr.h>
//#include <plx/Router/RouterInfo.h>

#include <SerializableTest.h>


namespace vprTest
{

void SerializableTest::testReadWriteSimple()
{
   Class1 obj1, obj2, obj3;

   obj1.charVal = 0xAB;
   obj1.shortVal = 0xCAFE;
   obj1.longVal = 0xDEADBEEF;
   obj1.longlongVal = 0xFACEBEEFCAFEDEAD;
   obj1.floatVal = 1221.1975f;
   obj1.doubleVal = 25.000034;
   obj2 = obj1;                   // Make a copy

   CPPUNIT_ASSERT(obj1 == obj2);

   vpr::ObjectWriter writer;     // Automagically gets data for storage

   obj1.writeObject(&writer);

   vpr::ObjectReader reader(writer.getData());

   obj3.readObject(&reader);

   CPPUNIT_ASSERT(obj2 == obj3);
}

void SerializableTest::testReadWriteNested()
{
   Class2 obj1, obj2, obj3;

   obj1.mObj1.charVal = 0xAB;
   obj1.mObj1.shortVal = 0xCAFE;
   obj1.mObj1.longVal = 0xDEADBEEF;
   obj1.mObj1.longlongVal = 0xFACEBEEFCAFEDEAD;
   obj1.mObj1.floatVal = 1221.1975f;
   obj1.mObj1.doubleVal = 25.000034;

   obj1.mObj2.charVal = 0xEF;
   obj1.mObj2.shortVal = 0xDEAD;
   obj1.mObj2.longVal = 0xBEEFFACE;
   obj1.mObj2.longlongVal = 0xFACEBEEFCAFEDEAD;
   obj1.mObj2.floatVal = 1321.1975f;
   obj1.mObj2.doubleVal = 25.000034;

   obj1.mFlag = true;

   obj2 = obj1;                   // Make a copy

   CPPUNIT_ASSERT(obj1 == obj2);

   vpr::ObjectWriter writer;     // Automagically gets data for storage
   obj1.writeObject(&writer);

   vpr::ObjectReader reader(writer.getData());
   obj3.readObject(&reader);

   CPPUNIT_ASSERT(obj2 == obj3);
}

/*
void SerializableTest::testReadWritePlxAddr()
{
   vpr::PlxAddr addr1, addr2;

   addr1.setIp(0xFACE);
   addr1.setPort(0xBEEF);
   addr2.setFull(0x0);

   CPPUNIT_ASSERT(addr1 != addr2);

   vpr::ObjectWriter writer;     // Automagically gets data for storage
   addr1.writeObject(&writer);

   vpr::ObjectReader reader(writer.getData());
   addr2.readObject(&reader);

   CPPUNIT_ASSERT(addr1 == addr2);
}

void SerializableTest::testReadWriteRouterInfo()
{
   vpr::RouterInfo ri1, ri2;

   ri1.mNodeAddr.setFull(0xBEEFCAFE);
   ri1.mNumFriends = 0xDEED;
   ri1.mPingTime   = vpr::Interval::now();
   ri1.mMsgRouteRateAvg = 10.10;
   ri1.mMsgRouteRateSTA = 11.11;
   ri1.mMsgRouteRateSTAMax = 12.12;
   ri1.mMsgHandleRateAvg = 13.13;
   ri1.mMsgHandleRateSTA = 14.14;
   ri1.mMsgHandleRateSTAMax = 15.15;
   ri1.mMsgDropRateAvg = 16.16;
   ri1.mMsgDropRateSTA = 17.17;
   ri1.mMsgDropRateSTAMax = 18.18;

   ri1.mReactorInfo.mHandleEventSTA = 19.19;
   ri1.mReactorInfo.mHandleEventAvg = 20.20;
   ri1.mReactorInfo.mHandleEventSTAMax = 21.21;
   ri1.mReactorInfo.mSelectEventsAvg = 22.22;
   ri1.mReactorInfo.mSelectEventsSTA = 23.23;
   ri1.mReactorInfo.mSelectEventsSTAMax = 24.24;
   ri1.mReactorInfo.mSelectTimeAvg = 25.25;
   ri1.mReactorInfo.mSelectTimeSTA = 26.26;
   ri1.mReactorInfo.mSelectTimeSTAMax = 27.27;

   CPPUNIT_ASSERT(!(ri1 == ri2));

   vpr::ObjectWriter writer;     // Automagically gets data for storage
   ri1.writeObject(&writer);

   vpr::ObjectReader reader(writer.getData());
   ri2.readObject(&reader);

   CPPUNIT_ASSERT(ri1 == ri2);
}
*/

CppUnit::Test* SerializableTest::suite()
{
   CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("SerializableTest");
   test_suite->addTest( new CppUnit::TestCaller<SerializableTest>("testReadWriteSimple", &SerializableTest::testReadWriteSimple));
   test_suite->addTest( new CppUnit::TestCaller<SerializableTest>("testReadWriteNested", &SerializableTest::testReadWriteNested));

//   test_suite->addTest( new CppUnit::TestCaller<SerializableTest>("testReadWritePlxAddr", &SerializableTest::testReadWritePlxAddr));
//   test_suite->addTest( new CppUnit::TestCaller<SerializableTest>("testReadWriteRouterInfo", &SerializableTest::testReadWriteRouterInfo));

   return test_suite;
}

}
