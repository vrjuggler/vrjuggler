#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/SerializableObject.h>

#include <TestCases/IO/XMLReaderWriterTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( XMLReaderWriterTest );

void XMLReaderWriterTest::testBasicWriteRead()
{
   const vpr::Uint8  data_uint8(21);
   const vpr::Uint16 data_uint16(1221);
   const vpr::Uint32 data_uint32(12211975);
   const vpr::Uint64 data_uint64(122119757911221);
   const float       data_float(1221.75f);
   const double      data_double(1.2211975);
   const std::string data_string("test string");
   const bool        data_bool(true);

   std::vector<vpr::Uint8> data_buffer;
   vpr::XMLObjectWriter xml_writer;
   
   xml_writer.beginTag("FirstLevel");
      xml_writer.beginAttribute("uint8");
         xml_writer.writeUint8(data_uint8);
      xml_writer.endAttribute();
      xml_writer.writeUint16(data_uint16);
      xml_writer.writeUint32(data_uint32);
      xml_writer.writeUint64(data_uint64);
      xml_writer.beginTag("LargeNumberLevel");
         xml_writer.writeFloat(data_float);
         xml_writer.writeDouble(data_double);
         xml_writer.writeUint8(data_string.length());
      xml_writer.endTag();
      xml_writer.writeString(data_string);
      xml_writer.writeBool(data_bool);
   xml_writer.endTag();

   vpr::Uint8 read_uint8;
   vpr::Uint16 read_uint16;
   vpr::Uint32 read_uint32;
   vpr::Uint64 read_uint64;
   float       read_float;
   double      read_double;
   vpr::Uint8  read_str_len;
   std::string read_string;
   bool        read_bool;

   std::cout << "------ Current data -----\n";
   xml_writer.getRootNode()->save(std::cout);
   std::cout << "------ End data -----\n";

   /*
   vpr::ObjectReader* reader;
   reader = new vpr::BufferObjectReader(&data_buffer);

   reader->readUint8(read_uint8);
   reader->readUint16(read_uint16);
   reader->readUint32(read_uint32);
   reader->readUint64(read_uint64);
   reader->readFloat(read_float);
   reader->readDouble(read_double);
   reader->readUint8(read_str_len);
   reader->readString(read_string, read_str_len);
   reader->readBool(read_bool);

   CPPUNIT_ASSERT(data_uint8 == read_uint8);
   CPPUNIT_ASSERT(data_uint16 == read_uint16);
   CPPUNIT_ASSERT(data_uint32 == read_uint32);
   CPPUNIT_ASSERT(data_uint64 == read_uint64);
   CPPUNIT_ASSERT(data_float == read_float);
   CPPUNIT_ASSERT(data_double == read_double);
   CPPUNIT_ASSERT(data_string == read_string);
   CPPUNIT_ASSERT(data_bool == read_bool);
   */
}


}
