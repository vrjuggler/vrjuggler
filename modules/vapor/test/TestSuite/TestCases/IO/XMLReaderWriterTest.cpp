#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/SerializableObject.h>

#include <TestCases/IO/XMLReaderWriterTest.h>
#include <fstream>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( XMLReaderWriterTest );

void XMLReaderWriterTest::testBasicWriteRead()
{
   const vpr::Uint8  data_uint8(21);
   const vpr::Uint16 data_uint16(1221);
   const vpr::Uint32 data_uint32(12211975);
#ifdef VPR_OS_Windows
   const vpr::Uint64 data_uint64(122119757911221);
#else
   const vpr::Uint64 data_uint64(122119757911221ll);
#endif
   const float       data_float(1221.75f);
   const double      data_double(1.2211975);
   const std::string data_string1("test string1");
   const std::string data_string2("test string2");
   const std::string empty_string("");                      // Empty on purpose
   const bool        data_bool(true);
   std::string long_string("X");
   for(unsigned s=0;s<1500;s++)
      long_string += "X";

   vpr::Uint8 read_uint8;
   vpr::Uint16 read_uint16;
   vpr::Uint16 read_uint16_2;
   vpr::Uint32 read_uint32;
   vpr::Uint64 read_uint64;
   float       read_float;
   double      read_double;
   std::string read_string;
   std::string read_empty_string("XXX");
   std::string read_empty_string_attrib("XXX");
   std::string read_string_attrib;
   std::string read_long_string;
   bool        read_bool;


   try
   {
      std::vector<vpr::Uint8> data_buffer;
      vpr::XMLObjectWriter xml_writer;

      // Things to test
      // - strings in cdata
      // - string in attribute
      // - multiple non-string in attribute
      // - non string in cdata
      // - Multiple cdata areas for a node
      // - Very long string (> 1024chars)
      // - Empty tags
      // - Two attributes in one tag
      xml_writer.beginTag("FirstLevel");
         xml_writer.beginAttribute("uint8_16");
            xml_writer.writeUint8(data_uint8);
            xml_writer.writeUint16(data_uint16);
         xml_writer.endAttribute();
         xml_writer.beginAttribute("uint16");
            xml_writer.writeUint16(data_uint16);
         xml_writer.endAttribute();
         xml_writer.writeUint32(data_uint32);
         xml_writer.writeUint64(data_uint64);
         xml_writer.beginTag("LargeNumberLevel");
            xml_writer.writeFloat(data_float);
            xml_writer.beginAttribute("StringAttrib");
               xml_writer.writeString(data_string1);
            xml_writer.endAttribute();
            xml_writer.writeDouble(data_double);
         xml_writer.endTag();
         xml_writer.writeString(data_string2);
         xml_writer.writeBool(data_bool);
         xml_writer.beginTag("EmptyString");
            xml_writer.beginAttribute("Empty");
               xml_writer.writeString(empty_string);
            xml_writer.endAttribute();
            xml_writer.writeString(empty_string);
         xml_writer.endTag();
         xml_writer.beginTag("DataLevel");
            xml_writer.writeString(long_string);
         xml_writer.endTag();
         xml_writer.beginTag("EmptyLevel");
         xml_writer.endTag();
      xml_writer.endTag();

      std::ofstream xml_out("XMLReaderWriterTest-testBasicWriteRead.out");
      xml_writer.getRootNode()->save(xml_out);
      xml_out.close();

      /*
      std::cout << "------ Current data -----\n";
      xml_writer.getRootNode()->save(std::cout);
      std::cout << "------ End data -----\n";
      */

      data_buffer = xml_writer.getData();

      vpr::XMLObjectReader xml_reader(data_buffer);

      // Test for re-initialization
      for(unsigned iter=0;iter<5;iter++)
      {

         xml_reader.beginTag("FirstLevel");
            xml_reader.beginAttribute("uint8_16");
               xml_reader.readUint8(read_uint8);
               xml_reader.readUint16(read_uint16);
            xml_reader.endAttribute();
            xml_reader.beginAttribute("uint16");
               xml_reader.readUint16(read_uint16_2);
            xml_reader.endAttribute();
            xml_reader.readUint32(read_uint32);
            xml_reader.readUint64(read_uint64);
            xml_reader.pushState();                   // *** Push state for later ***
            xml_reader.beginTag("LargeNumberLevel");
               xml_reader.readFloat(read_float);
               xml_reader.beginAttribute("StringAttrib");
                  xml_reader.readString(read_string_attrib);
               xml_reader.endAttribute();
               xml_reader.readDouble(read_double);
            xml_reader.endTag();
            xml_reader.readString(read_string);
            xml_reader.readBool(read_bool);
            xml_reader.beginTag("EmptyString");
               xml_reader.beginAttribute("EmptyStringAttrib");
                  xml_reader.readString(read_empty_string_attrib);
               xml_reader.endAttribute();
               xml_reader.readString(read_empty_string);
            xml_reader.endTag();
            xml_reader.beginTag("DataLevel");
               xml_reader.readString(read_long_string);
            xml_reader.endTag();
            xml_reader.beginTag("EmptyLevel");
            xml_reader.endTag();
         xml_reader.endTag();

         CPPUNIT_ASSERT_EQUAL(data_uint8, read_uint8);
         CPPUNIT_ASSERT_EQUAL(data_uint16, read_uint16);
         CPPUNIT_ASSERT_EQUAL(data_uint16, read_uint16_2);
         CPPUNIT_ASSERT_EQUAL(data_uint32, read_uint32);
         CPPUNIT_ASSERT_EQUAL(data_uint64, read_uint64);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(data_float, read_float, 0.001f);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(data_double, read_double, 0.001f);
         CPPUNIT_ASSERT_EQUAL(data_string1, read_string_attrib);
         CPPUNIT_ASSERT_EQUAL(data_string2, read_string);
         CPPUNIT_ASSERT_EQUAL(read_empty_string, empty_string);
         CPPUNIT_ASSERT_EQUAL(read_empty_string_attrib, empty_string);
         CPPUNIT_ASSERT_EQUAL(data_bool, read_bool);
         CPPUNIT_ASSERT_EQUAL(long_string, read_long_string);

         // Test pop state
         xml_reader.popState();           // *** POP state *** //

            xml_reader.beginTag("LargeNumberLevel");
               xml_reader.readFloat(read_float);
               xml_reader.beginAttribute("StringAttrib");
                  xml_reader.readString(read_string_attrib);
               xml_reader.endAttribute();
               xml_reader.readDouble(read_double);
            xml_reader.endTag();

         CPPUNIT_ASSERT_DOUBLES_EQUAL(data_float, read_float, 0.001f);
         CPPUNIT_ASSERT_EQUAL(data_string1, read_string_attrib);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(data_double, read_double, 0.001f);


         xml_reader.resetReading();       // Reset the reading for another pass         
      }
   }
   catch (cppdom::Error& ce)
   {
      std::cerr << "Cppdom Error [building]: " << ce.getString() << std::endl;
      if (ce.getInfo().size())
      {
         std::cerr << "Info: " << ce.getInfo() << std::endl;
      }
   }
   catch (std::exception& std_error)
   {
      std::cerr << "Std::Error: type:[" << typeid(std_error).name() << "]"
                << "  what:" << std_error.what() << std::endl;
      throw;   // Rethrow it since we don't know what to do
   }
   catch(...)
   {
      std::cerr << "Unrecognized error: rethrowing..." << std::endl;
      throw;
   }

}


}
