#ifndef _ENUMERATIONS_TEST_H
#define _ENUMERATIONS_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

/*****************************************************************
 tests out the functionality expected of enumerations in ConfigElements
*******************************************************************/

namespace jcclTest
{

class EnumerationsTest : public CppUnit::TestCase
{
public:
   EnumerationsTest() : CppUnit::TestCase ()
   {
   }
    
   EnumerationsTest(std::string name) : CppUnit::TestCase (name)
   {
   }
   
   virtual ~EnumerationsTest()
   {
   }


   void ReadTests()
   {
      const std::string file_path(TESTFILES_PATH);

      jccl::Configuration configuration;
      configuration.load(file_path + "cfg/enumerations_test.jconf");

      jccl::ConfigElementPtr ch = configuration.get("Motion Star master");
      CPPUNIT_ASSERT(ch.get() != NULL);

      int port = ch->getProperty<int>("server_port");
      CPPUNIT_ASSERT(port == 5000);

      int hemisphere = ch->getProperty<int>("hemisphere");
      CPPUNIT_ASSERT(hemisphere == 0);
      
      int format = ch->getProperty<int>("data_format");
      CPPUNIT_ASSERT(format == 4);
   }

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("EnumerationsTest");
      test_suite->addTest( new CppUnit::TestCaller<EnumerationsTest>("ReadTests", &EnumerationsTest::ReadTests));
      
      return test_suite;
   }
};

}

#endif
