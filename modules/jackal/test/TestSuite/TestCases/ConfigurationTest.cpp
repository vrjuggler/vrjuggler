#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <ConfigurationTest.h>


namespace jcclTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigurationTest);

void ConfigurationTest::Load()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration cfg;
   CPPUNIT_ASSERT(cfg.vec().empty() && "The configuration should be empty");

   status = cfg.load(file_path + "cfg/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! cfg.vec().empty() && "The configuration should not be empty");
}

/*
void ConfigurationTest::testDependencySort()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Construct the known sorted list of dependencies.
   std::vector<std::string> elt_names;
   elt_names.push_back(std::string("Dependent Element 1"));
   elt_names.push_back(std::string("Dependent Element 2"));
   elt_names.push_back(std::string("Test Element 1"));
   elt_names.push_back(std::string("External Test Element 1"));
   elt_names.push_back(std::string("Dependent Element 3"));
   elt_names.push_back(std::string("Child Element Ptr Test"));

   // Load the test configuration file
   jccl::Configuration cfg_start, cfg_dep;
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   status = cfg_start.load(file_path + "cfg/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   status = cfg_dep.load(file_path + "cfg/ConfigurationTest.jconf");
   CPPUNIT_ASSERT(status && "Failed to load config file with dependencies");

   status = cfg_dep.dependencySort(&cfg_start);
   CPPUNIT_ASSERT(status && "Dependency sort failed");

   // This is pretty gross...
   std::vector<jccl::ConfigElementPtr>::iterator i;
   int j = 0;
   for ( i = cfg_dep.vec().begin(); i != cfg_dep.vec().end(); ++i, ++j )
   {
      CPPUNIT_ASSERT((*i)->getName() == cfg_names[j] && "Incorrect dependency sort");
   }
}

void ConfigurationTest::testDependencySortFailure()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration cfg;
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   status = cfg.load(file_path + "cfg/ConfigurationTest.jconf");
   CPPUNIT_ASSERT(status && "Failed to load config file with dependencies");

   // This dependency sort should fail because cfg_dep depends on elements
   // in cfg_start.  Since it's suppoesd to fail, ignore warnings/errors
   // printed to the screen and focus on the result of the assertion.
   status = cfg.dependencySort(NULL);
   CPPUNIT_ASSERT(! status && "Dependency sort should have failed");
}
*/

void ConfigurationTest::Clear()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration cfg;

   CPPUNIT_ASSERT(cfg.vec().empty() && "The configuration should be empty");

   status = cfg.load(file_path + "cfg/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! cfg.vec().empty() && "The configuration should not be empty");
   cfg.vec().clear();
   CPPUNIT_ASSERT(cfg.vec().empty() && "The configuration now should be empty");
}

}
