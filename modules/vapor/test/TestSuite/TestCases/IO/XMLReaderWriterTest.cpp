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
   const std::string long_string("X", 1500);
      
   std::vector<vpr::Uint8> data_buffer;
   vpr::XMLObjectWriter xml_writer;
   
   // Things to test
   // - strings in cdata
   // - string in attribute
   // - multiple non-string in attribute
   // - non string in cdata
   // - Multiple cdata areas for a node
   // - Very long string (> 1024chars)
   try
   {
      xml_writer.beginTag("FirstLevel");
         xml_writer.beginAttribute("uint8");
            xml_writer.writeUint8(data_uint8);
            xml_writer.writeUint16(data_uint16);
         xml_writer.endAttribute();
         xml_writer.writeUint32(data_uint32);
         xml_writer.writeUint64(data_uint64);
         xml_writer.beginTag("LargeNumberLevel");
            xml_writer.writeFloat(data_float);
            xml_writer.beginAttribute("String Attrib");
               xml_writer.writeString(data_string);
            xml_writer.endAttribute();
            xml_writer.writeDouble(data_double);         
         xml_writer.endTag();
         xml_writer.writeString(data_string);
         xml_writer.writeBool(data_bool);
         xml_writer.beginTag("DataLevel");
            //xml_writer.writeString(long_string);
         xml_writer.endTag();
      xml_writer.endTag();
   }
   catch (cppdom::Error e)
   {
      std::cerr << "Error [building]: " << e.getString() << std::endl;
      if (e.getInfo().size())
      {
         std::cerr << "Info: " << e.getInfo() << std::endl;
      }           
   }


   vpr::Uint8 read_uint8;
   vpr::Uint16 read_uint16;
   vpr::Uint32 read_uint32;
   vpr::Uint64 read_uint64;
   float       read_float;
   double      read_double;
   vpr::Uint8  read_str_len;
   std::string read_string;
   std::string read_string_attrib;
   std::string read_long_string;
   bool        read_bool;

   try
   {
      std::cout << "------ Current data -----\n";
      xml_writer.getRootNode()->save(std::cout);
      std::cout << "------ End data -----\n";

      data_buffer = xml_writer.getData();
   }
   catch (cppdom::Error e)
   {
      std::cerr << "Error [output]: " << e.getString() << std::endl;
      if (e.getInfo().size())
      {
         std::cerr << "Info: " << e.getInfo() << std::endl;
      }           
   }
      
   vpr::XMLObjectReader xml_reader(data_buffer);
   
   try
   {
      xml_reader.beginTag("FirstLevel");
         xml_writer.beginAttribute("uint8");
            xml_reader.readUint8(read_uint8);
            xml_reader.readUint16(read_uint16);
         xml_reader.endAttribute();
         xml_reader.readUint32(read_uint32);
         xml_reader.readUint64(read_uint64);
         xml_writer.beginTag("LargeNumberLevel");
            xml_reader.readFloat(read_float);
            xml_reader.beginAttribute("String Attrib");
               xml_reader.readString(read_string_attrib, read_str_len);
            xml_reader.endAttribute();
            xml_reader.readDouble(read_double);
         xml_reader.endTag();
         xml_reader.readString(read_string, read_str_len);
         xml_reader.readBool(read_bool);
         xml_reader.beginTag("DataLevel");
            //xml_reader.readString(read_long_string, 0);
         xml_reader.endTag();
      xml_reader.endTag();
   }
   catch (cppdom::Error e)
   {
     std::cerr << "Error [reading]: " << e.getString() << std::endl;
     if (e.getInfo().size())
     {
        std::cerr << "Info: " << e.getInfo() << std::endl;
     }           
   }


   CPPUNIT_ASSERT(data_uint8 == read_uint8);
   CPPUNIT_ASSERT(data_uint16 == read_uint16);
   CPPUNIT_ASSERT(data_uint32 == read_uint32);
   CPPUNIT_ASSERT(data_uint64 == read_uint64);
   CPPUNIT_ASSERT(data_float == read_float);
   CPPUNIT_ASSERT(data_double == read_double);
   CPPUNIT_ASSERT(data_string == read_string);
   CPPUNIT_ASSERT(data_string == read_string_attrib);
   CPPUNIT_ASSERT(data_bool == read_bool);
   CPPUNIT_ASSERT(long_string == read_long_string);
}


}
