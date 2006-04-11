#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <EnumerationsTest.h>


namespace jcclTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(EnumerationsTest);

void EnumerationsTest::Read()
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

}
