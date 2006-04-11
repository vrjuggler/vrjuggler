#ifndef _XML_READER_WRITER_TEST_H_
#define _XML_READER_WRITER_TEST_H_

#include <string>

#include <vpr/Util/ReturnStatus.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <vpr/IO/XMLObjectWriter.h>
#include <vpr/IO/XMLObjectReader.h>


namespace vprTest
{

class XMLReaderWriterTest : public CppUnit::TestFixture
{
public:
CPPUNIT_TEST_SUITE(XMLReaderWriterTest);
CPPUNIT_TEST( testBasicWriteRead );
CPPUNIT_TEST_SUITE_END();

public:
   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   // Test reading and writing of data
   void testBasicWriteRead();

};

};

#endif

