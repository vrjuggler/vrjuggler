#ifndef _CONFIG_CHUNK_TEST_H
#define _CONFIG_CHUNK_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

/*****************************************************************
 tests out the functionality expected of enumerations in ConfigChunks
*******************************************************************/

namespace jcclTest
{

class ConfigChunkTest : public CppUnit::TestCase
{
public:
   ConfigChunkTest() : CppUnit::TestCase ()
   {
   }
    
   ConfigChunkTest(std::string name) : CppUnit::TestCase (name)
   {
   }
   
   virtual ~ConfigChunkTest()
   {
   }


   void PropertyValueTests () 
   {
      std::string file_path(TESTFILES_PATH);

      // Load the test configuration file
      jccl::ConfigChunkDB chunkdb;
      jccl::ChunkFactory::instance()->loadDescs (file_path + "ConfigChunkTest/ConfigChunkTest.desc");
      chunkdb.load (file_path + "ConfigChunkTest/ConfigChunkTest.config");

      // Check the chunks
      jccl::ConfigChunkPtr chunk1 = chunkdb.getChunk("Chunk1");
      CPPUNIT_ASSERT (chunk1.get() != NULL);

      CPPUNIT_ASSERT( std::string("Chunk1") == chunk1->getName() );
      CPPUNIT_ASSERT( int(chunk1->getProperty("single_int")) == 21 );
      CPPUNIT_ASSERT( float(chunk1->getProperty("single_float")) == 21.75 );

      CPPUNIT_ASSERT( chunk1->getNum("var_int") == 2 );
      CPPUNIT_ASSERT( chunk1->getNum("var_float") == 2 );

      CPPUNIT_ASSERT( float(chunk1->getProperty("var_float", 0)) == 12.21f );
      CPPUNIT_ASSERT( float(chunk1->getProperty("var_float", 1)) == 19.75f );

      CPPUNIT_ASSERT( int(chunk1->getProperty("var_int", 0)) == 12 );
      CPPUNIT_ASSERT( int(chunk1->getProperty("var_int", 1)) == 21 );

      CPPUNIT_ASSERT( std::string("test string") == std::string( chunk1->getProperty("single_string") ));

      // Test embedded
      CPPUNIT_ASSERT( chunk1->getNum("embedded_chunk") == 1);
      jccl::ConfigChunkPtr embedded_chunk = chunk1->getProperty("embedded_chunk");
      
      CPPUNIT_ASSERT(embedded_chunk.get() != NULL);
      CPPUNIT_ASSERT( int(embedded_chunk->getProperty("single_int")) == 17);

      // Test chunk ptr
      CPPUNIT_ASSERT( chunk1->getNum("single_chunk") == 1);
      std::string chunk2_name = chunk1->getProperty("single_chunk");    // Have to get chunk ptr and string "chunk name"
      jccl::ConfigChunkPtr chunk2 = chunkdb.getChunk(chunk2_name);
            
      CPPUNIT_ASSERT(chunk2.get() != NULL);
      CPPUNIT_ASSERT( int(chunk2->getProperty("single_int")) == 21);
    }
        

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ConfigChunkTest");
      test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>("PropertyValueTests", &ConfigChunkTest::PropertyValueTests));
      
      return test_suite;
   }
};

}

#endif
