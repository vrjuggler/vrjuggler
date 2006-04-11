#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/SerializableObject.h>

#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>

// Serializable objects to test
//#include <plx/Router/PlxAddr.h>
//#include <plx/Router/RouterInfo.h>
#include <TestCases/IO/SerializableTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( SerializableTest );

void SerializableTest::testReaderWriter()
{
   const vpr::Uint8 data_uint8(0xAB);
   const vpr::Uint16 data_uint16(0xBEEF);
   const vpr::Uint32 data_uint32(0xDEADBEEF);
#ifdef VPR_OS_Windows
   const vpr::Uint64 data_uint64(0xCAFEABCDBEEF1221);
#else
   const vpr::Uint64 data_uint64(0xCAFEABCDBEEF1221ll);
#endif

   const vpr::Int8 data_int8(-7);
   const vpr::Int16 data_int16(-298);
   const vpr::Int32 data_int32(-14567);
   const vpr::Int64 data_int64(-12345678);

   const float       data_float(1221.75f);
   const double      data_double(1.2211975);
   const std::string data_string("test string");
   const bool        data_bool(true);

   std::vector<vpr::Uint8> data_buffer;
   vpr::ObjectWriter* writer;

   writer = new vpr::BufferObjectWriter(&data_buffer);

   writer->writeUint8(data_uint8);
   writer->writeUint16(data_uint16);
   writer->writeUint32(data_uint32);
   writer->writeUint64(data_uint64);

   writer->writeUint8(data_int8);
   writer->writeUint16(data_int16);
   writer->writeUint32(data_int32);
   writer->writeUint64(data_int64);

   writer->writeFloat(data_float);
   writer->writeDouble(data_double);
   writer->writeString(data_string);
   writer->writeBool(data_bool);

   vpr::Uint8 read_uint8;
   vpr::Uint16 read_uint16;
   vpr::Uint32 read_uint32;
   vpr::Uint64 read_uint64;
   vpr::Int8 read_int8;
   vpr::Int16 read_int16;
   vpr::Int32 read_int32;
   vpr::Int64 read_int64;

   float       read_float;
   double      read_double;
   std::string read_string;
   bool        read_bool;

   vpr::ObjectReader* reader;
   reader = new vpr::BufferObjectReader(&data_buffer);

   reader->readUint8(read_uint8);
   reader->readUint16(read_uint16);
   reader->readUint32(read_uint32);
   reader->readUint64(read_uint64);

   vpr::Uint8* temp_uint8   = (vpr::Uint8*) &read_int8;
   vpr::Uint16* temp_uint16 = (vpr::Uint16*) &read_int16;
   vpr::Uint32* temp_uint32 = (vpr::Uint32*) &read_int32;
   vpr::Uint64* temp_uint64 = (vpr::Uint64*) &read_int64;

   reader->readUint8(*temp_uint8);
   reader->readUint16(*temp_uint16);
   reader->readUint32(*temp_uint32);
   reader->readUint64(*temp_uint64);
   reader->readFloat(read_float);
   reader->readDouble(read_double);
   reader->readString(read_string);
   reader->readBool(read_bool);

   CPPUNIT_ASSERT(data_uint8 == read_uint8);
   CPPUNIT_ASSERT(data_uint16 == read_uint16);
   CPPUNIT_ASSERT(data_uint32 == read_uint32);
   CPPUNIT_ASSERT(data_uint64 == read_uint64);
   CPPUNIT_ASSERT(data_int8 == read_int8);
   CPPUNIT_ASSERT(data_int16 == read_int16);
   CPPUNIT_ASSERT(data_int32 == read_int32);
   CPPUNIT_ASSERT(data_int64 == read_int64);
   CPPUNIT_ASSERT(data_float == read_float);
   CPPUNIT_ASSERT(data_double == read_double);
   CPPUNIT_ASSERT(data_string == read_string);
   CPPUNIT_ASSERT(data_bool == read_bool);
}

// Test reading and writing data from many memory offsets
// - Loop over several different offsets and at each read and write all types
// - Tests for memory alignment errors (ie. bus errors and seg faults)
void SerializableTest::testDataOffsets()
{

   const vpr::Uint8 data_uint8(0xAB);
   const vpr::Uint16 data_uint16(0xBEEF);
   const vpr::Uint32 data_uint32(0xDEADBEEF);
#ifdef VPR_OS_Windows
   const vpr::Uint64 data_uint64(0xCAFEABCDBEEF1221);
#else
   const vpr::Uint64 data_uint64(0xCAFEABCDBEEF1221ll);
#endif
   const float       data_float(1221.75f);
   const double      data_double(1.2211975);
   const std::string data_string("test string");
   const bool        data_bool(true);

   vpr::Uint8 read_uint8;
   vpr::Uint16 read_uint16;
   vpr::Uint32 read_uint32;
   vpr::Uint64 read_uint64;
   float       read_float;
   double      read_double;
   std::string read_string;
   bool        read_bool;


   // For each offset size
   // - write out raw data of size offset bytes
   // - write all data types and read them
   for(unsigned offset=0; offset<16; offset++)
   {
      std::vector<vpr::Uint8> data_buffer;
      vpr::ObjectWriter* writer;
      writer = new vpr::BufferObjectWriter(&data_buffer);

      for(unsigned i=0;i<offset;++i)
      {
         writer->writeUint8(0xFF);
      }

      writer->writeUint8(data_uint8);
      writer->writeUint16(data_uint16);
      writer->writeUint32(data_uint32);
      writer->writeUint64(data_uint64);
      writer->writeFloat(data_float);
      writer->writeDouble(data_double);
      writer->writeString(data_string);
      writer->writeBool(data_bool);

      vpr::ObjectReader* reader;
      reader = new vpr::BufferObjectReader(&data_buffer);

      for(unsigned i=0;i<offset;++i)
      {
         vpr::Uint8 ff_data(0);
         reader->readUint8(ff_data);
         CPPUNIT_ASSERT(0xFF == ff_data);
      }

      reader->readUint8(read_uint8);
      reader->readUint16(read_uint16);
      reader->readUint32(read_uint32);
      reader->readUint64(read_uint64);
      reader->readFloat(read_float);
      reader->readDouble(read_double);
      reader->readString(read_string);
      reader->readBool(read_bool);

      CPPUNIT_ASSERT(data_uint8 == read_uint8);
      CPPUNIT_ASSERT(data_uint16 == read_uint16);
      CPPUNIT_ASSERT(data_uint32 == read_uint32);
      CPPUNIT_ASSERT(data_uint64 == read_uint64);
      CPPUNIT_ASSERT(data_float == read_float);
      CPPUNIT_ASSERT(data_double == read_double);
      CPPUNIT_ASSERT(data_string == read_string);
      CPPUNIT_ASSERT(data_bool == read_bool);
   }
}


void SerializableTest::testReadWriteSimple()
{
   Class1 obj1, obj2, obj3;

   obj1.charVal = 0xAB;
   obj1.shortVal = 0xCAFE;
   obj1.longVal = 0xDEADBEEF;
#ifdef VPR_OS_Windows
   obj1.longlongVal = 0xFACEBEEFCAFEDEAD;
#else
   obj1.longlongVal = 0xFACEBEEFCAFEDEADll;
#endif

   obj1.scharVal = -10;
   obj1.sshortVal = -233;
   obj1.slongVal = -10000;
   obj1.slonglongVal = -12345678;

   obj1.floatVal = 1221.1975f;
   obj1.doubleVal = 25.000034;
   obj2 = obj1;                   // Make a copy

   CPPUNIT_ASSERT(obj1 == obj2);

   vpr::BufferObjectWriter* writer;     // Automagically gets data for storage
   writer = new vpr::BufferObjectWriter;

   obj1.writeObject(writer);

   vpr::ObjectReader* reader;
   reader = new vpr::BufferObjectReader(writer->getData());

   obj3.readObject(reader);

   CPPUNIT_ASSERT(obj2 == obj3);
}

void SerializableTest::testReadWriteNested()
{
   Class2 obj1, obj2;

   obj1.mObj1.charVal = 0xAB;
   obj1.mObj1.shortVal = 0xCAFE;
   obj1.mObj1.longVal = 0xDEADBEEF;
#ifdef VPR_OS_Windows
   obj1.mObj1.longlongVal = 0xFACEBEEFCAFEDEAD;
#else
   obj1.mObj1.longlongVal = 0xFACEBEEFCAFEDEADll;
#endif
   obj1.mObj1.floatVal = 1221.1975f;
   obj1.mObj1.doubleVal = 25.000034;

   obj1.mObj2.charVal = 0xEF;
   obj1.mObj2.shortVal = 0xDEAD;
   obj1.mObj2.longVal = 0xBEEFFACE;
#ifdef VPR_OS_Windows
   obj1.mObj2.longlongVal = 0xFACEBEEFCAFEDEAD;
#else
   obj1.mObj2.longlongVal = 0xFACEBEEFCAFEDEADll;
#endif
   obj1.mObj2.floatVal = 1321.1975f;
   obj1.mObj2.doubleVal = 25.000034;

   obj1.mFlag = true;

   obj2 = obj1;                   // Make a copy

   CPPUNIT_ASSERT(obj1 == obj2);

   vpr::BufferObjectWriter* writer;     // Automagically gets data for storage
   writer = new vpr::BufferObjectWriter;

   obj1.writeObject(writer);

   vpr::ObjectReader* reader;
   reader = new vpr::BufferObjectReader(writer->getData());

   // Test for re-initialization
   for(unsigned iter=0;iter<10;iter++)
   {
      Class2 obj3;
      obj3.readObject(reader);

      CPPUNIT_ASSERT(obj2 == obj3);
      reader->resetReading();             // Reset the reading source ro iter test
   }

   // Test pushing and poping state
   {
      vpr::BufferObjectWriter* writer;     // Automagically gets data for storage
      writer = new vpr::BufferObjectWriter;
      writer->writeFloat(1221.75f);
   
      obj1.writeObject(writer);
   
      vpr::ObjectReader* reader;
      reader = new vpr::BufferObjectReader(writer->getData());
      float r_float = reader->readFloat();
      reader->pushState();
   
      Class2 obj3;
      obj3.readObject(reader);
      CPPUNIT_ASSERT(obj2 == obj3);
      
      reader->popState();
      obj3.readObject(reader);
      CPPUNIT_ASSERT(obj2 == obj3);
   }
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


}
