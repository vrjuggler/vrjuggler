#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

#include <ConfigChunkDBTest.h>


namespace jcclTest
{

void ConfigChunkDBTest::testLoad ()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::ConfigChunkDB chunkdb;
   status = jccl::ChunkFactory::instance()->loadDescs(file_path + "ConfigChunkDBTest/ConfigChunkDBTest.desc");
   CPPUNIT_ASSERT(status && "Failed to load description file");

   CPPUNIT_ASSERT(chunkdb.isEmpty() && "The DB should be empty");

   status = chunkdb.load(file_path + "ConfigChunkDBTest/ConfigChunkDBTest_start.config");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! chunkdb.isEmpty() && "The DB should not be empty");
}

void ConfigChunkDBTest::testDependencySort ()
{
   std::string file_path(TESTFILES_PATH);
   bool status;
   int result;

   // Load the test configuration file
   jccl::ConfigChunkDB chunkdb_start, chunkdb_dep;
   status = jccl::ChunkFactory::instance()->loadDescs(file_path + "ConfigChunkDBTest/ConfigChunkDBTest.desc");
   CPPUNIT_ASSERT(status && "Failed to load description file");

   status = chunkdb_start.load(file_path + "ConfigChunkDBTest/ConfigChunkDBTest_start.config");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   status = chunkdb_dep.load(file_path + "ConfigChunkDBTest/ConfigChunkDBTest.config");
   CPPUNIT_ASSERT(status && "Failed to load config file with dependencies");

   result = chunkdb_dep.dependencySort(&chunkdb_start);
   CPPUNIT_ASSERT(result == 0 && "Dependency sort failed");

   // This dependency sort should fail because chunkdb_dep depends on chunks
   // in chunkdb_start.  Since it's suppoesd to fail, ignore warnings/errors
   // printed to the screen and focus on the result of the assertion.
   result = chunkdb_dep.dependencySort(NULL);
   CPPUNIT_ASSERT(result == -1 && "Dependency sort should have failed");

}

void ConfigChunkDBTest::testClear ()
{
   std::string file_path(TESTFILES_PATH);
   bool status;

   // Load the test configuration file
   jccl::ConfigChunkDB chunkdb;
   status = jccl::ChunkFactory::instance()->loadDescs(file_path + "ConfigChunkDBTest/ConfigChunkDBTest.desc");
   CPPUNIT_ASSERT(status && "Failed to load description file");

   CPPUNIT_ASSERT(chunkdb.isEmpty() && "The DB should be empty");

   status = chunkdb.load(file_path + "ConfigChunkDBTest/ConfigChunkDBTest_start.config");
   CPPUNIT_ASSERT(status && "Failed to load initial config file");

   CPPUNIT_ASSERT(! chunkdb.isEmpty() && "The DB should not be empty");
   chunkdb.clear();
   CPPUNIT_ASSERT(chunkdb.isEmpty() && "The DB now should be empty");
}

}
