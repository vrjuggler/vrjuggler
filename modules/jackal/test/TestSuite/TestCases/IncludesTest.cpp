#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <IncludesTest.h>

/*****************************************************************
 tests out the functionality of definition & config file includes in
 config files.
*******************************************************************/

namespace jcclTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(IncludesTest);

void IncludesTest::BasicInclude()
{
   jccl::ConfigElementPtr element1, element2;
   jccl::Configuration configuration;

   std::string file_path(TESTFILES_PATH);
   configuration.load(file_path + "cfg/include_test.jcfg");

   element1 = configuration.get("Test 1");
   CPPUNIT_ASSERT(element1.get() != NULL && "Failed to load first file");

   element2 = configuration.get("Test 2");
   CPPUNIT_ASSERT(element2.get() != NULL && "Failed to include second file");

   int i = element2->getProperty<int>("int_prop");
   CPPUNIT_ASSERT((i == 100) && "Failed to get contents from second file");
}

void IncludesTest::ValueSuperceding()
{
   jccl::ConfigElementPtr element;
   jccl::Configuration configuration;

   std::string file_path(TESTFILES_PATH);
   configuration.load(file_path + "cfg/include_test.jcfg");

   element = configuration.get("Test 1");
   CPPUNIT_ASSERT(element.get() != NULL);

   element = configuration.get("Superceding Test");
   CPPUNIT_ASSERT(element.get() != NULL);
   int i = element->getProperty<int>("int_prop", 0);
   CPPUNIT_ASSERT((i == 2) && "value from include has been superceded");

   element = configuration.get("Test 2");
   i = element->getProperty<int>("int_prop");
   CPPUNIT_ASSERT((i == 100));
}

}  // namespace jcclTest
