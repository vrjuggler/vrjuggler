#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <ConfigurationTest.h>


namespace jcclTest
{

void ConfigurationTest::testLoad()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration chunkdb;
   status = jccl::ElementFactory::instance()->loadDefs(file_path + "ConfigurationTest/ConfigurationTest.jdef");
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   CPPUNIT_ASSERT(chunkdb.vec().empty() && "The DB should be empty");

   status = chunkdb.load(file_path + "ConfigurationTest/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! chunkdb.vec().empty() && "The DB should not be empty");
}

/*
void ConfigurationTest::testDependencySort()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Construct the known sorted list of dependencies.
   std::vector<std::string> chunk_names;
   chunk_names.push_back(std::string("DependentChunk1"));
   chunk_names.push_back(std::string("DependentChunk2"));
   chunk_names.push_back(std::string("Test Chunk 1"));
   chunk_names.push_back(std::string("External Test Chunk 1"));
   chunk_names.push_back(std::string("DependentChunk3"));
   chunk_names.push_back(std::string("Embedded Chunk Ptr Test"));

   // Load the test configuration file
   jccl::Configuration chunkdb_start, chunkdb_dep;
   status = jccl::ElementFactory::instance()->loadDefs(file_path + "ConfigurationTest/ConfigurationTest.jdef");
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   status = chunkdb_start.load(file_path + "ConfigurationTest/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   status = chunkdb_dep.load(file_path + "ConfigurationTest/ConfigurationTest.jconf");
   CPPUNIT_ASSERT(status && "Failed to load config file with dependencies");

   status = chunkdb_dep.dependencySort(&chunkdb_start);
   CPPUNIT_ASSERT(status && "Dependency sort failed");

   // This is pretty gross...
   std::vector<jccl::ConfigElementPtr>::iterator i;
   int j = 0;
   for ( i = chunkdb_dep.vec().begin(); i != chunkdb_dep.vec().end(); ++i, ++j )
   {
      CPPUNIT_ASSERT((*i)->getName() == chunk_names[j] && "Incorrect dependency sort");
   }
}

void ConfigurationTest::testDependencySortFailure()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration chunkdb;
   status = jccl::ElementFactory::instance()->loadDefs(file_path + "ConfigurationTest/ConfigurationTest.jdef");
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   status = chunkdb.load(file_path + "ConfigurationTest/ConfigurationTest.jconf");
   CPPUNIT_ASSERT(status && "Failed to load config file with dependencies");

   // This dependency sort should fail because chunkdb_dep depends on chunks
   // in chunkdb_start.  Since it's suppoesd to fail, ignore warnings/errors
   // printed to the screen and focus on the result of the assertion.
   status = chunkdb.dependencySort(NULL);
   CPPUNIT_ASSERT(! status && "Dependency sort should have failed");
}
*/

void ConfigurationTest::testClear()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::Configuration chunkdb;
   status = jccl::ElementFactory::instance()->loadDefs(file_path + "ConfigurationTest/ConfigurationTest.jdef");
   CPPUNIT_ASSERT(status && "Failed to load definition file");

   CPPUNIT_ASSERT(chunkdb.vec().empty() && "The DB should be empty");

   status = chunkdb.load(file_path + "ConfigurationTest/ConfigurationTest_start.jconf");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! chunkdb.vec().empty() && "The DB should not be empty");
   chunkdb.vec().clear();
   CPPUNIT_ASSERT(chunkdb.vec().empty() && "The DB now should be empty");
}

}
