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
      ConfigChunkTest() : CppUnit::TestCase()
      {
      }

      ConfigChunkTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~ConfigChunkTest()
      {
      }

      void setPropTests()
      {
         bool retval = false;

         // make cure they fail without a desc
         {
            // set property integer
            jccl::ConfigChunkPtr chunk_int = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
            retval = chunk_int->setProperty( "test prop int", 2, 0 );
            CPPUNIT_ASSERT( retval == false );

            // set property float
            jccl::ConfigChunkPtr chunk_float = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
            retval =  chunk_float->setProperty( "test prop float", 78.2f, 0 );
            CPPUNIT_ASSERT( retval == false );

            // set property string
            jccl::ConfigChunkPtr chunk_string = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
            retval =  chunk_string->setProperty( "test prop string", "green hairy meatball", 0 );
            CPPUNIT_ASSERT( retval == false );

            // set multiple types of props on one chunk...
            jccl::ConfigChunkPtr chunk_multi = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
            chunk_multi->setProperty( "test prop multi", 2, 0 );
            chunk_multi->setProperty( "test prop multi", 678.098f, 0 );
            chunk_multi->setProperty( "test prop multi", "chunky munky", 0 );
         }

         // make sure they succeed with a desc
         {
            // start fresh and new (and shiny!!!)
            jccl::ChunkFactory::instance()->getChunkDescDB()->removeAll();
         
            std::string file_path(TESTFILES_PATH);
            jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
            jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

            // set property integer
            jccl::ConfigChunkPtr chunk_int = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
            retval = chunk_int->setProperty( "test prop int", 2, 0 );
            CPPUNIT_ASSERT( retval == true );

            // set property float
            jccl::ConfigChunkPtr chunk_float = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
            retval =  chunk_float->setProperty( "test prop float", 78.2f, 0 );
            CPPUNIT_ASSERT( retval == true );

            // set property string
            jccl::ConfigChunkPtr chunk_string = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
            retval =  chunk_string->setProperty( "test prop string", "green hairy meatball", 0 );
            CPPUNIT_ASSERT( retval == true );

            // set multiple types of props on one chunk...
            jccl::ConfigChunkPtr chunk_multi = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );

            retval = chunk_multi->setProperty( "test prop multi", "chunky munky", 0 );
            CPPUNIT_ASSERT( retval == true );
            CPPUNIT_ASSERT( std::string( "chunky munky" ) == (std::string)chunk_multi->getProperty( "test prop multi", 0 ) );

            retval = chunk_multi->setProperty( "test prop multi", 2, 0 );
            CPPUNIT_ASSERT( retval == true );
            // @todo this test currently fails, but it shouldn't
            CPPUNIT_ASSERT( std::string( "chunky munky" ) != (std::string)chunk_multi->getProperty( "test prop multi", 0 ) );
            CPPUNIT_ASSERT( 2 == (int)chunk_multi->getProperty( "test prop multi", 0 ) );

            retval = chunk_multi->setProperty( "test prop multi", 678.098f, 0 );
            CPPUNIT_ASSERT( retval == true );
            // @todo this test currently fails, but it shouldn't
            CPPUNIT_ASSERT( 2 != (int)chunk_multi->getProperty( "test prop multi", 0 ) );
            CPPUNIT_ASSERT( 678.098f == (float)chunk_multi->getProperty( "test prop multi", 0 ) );

            retval = chunk_multi->setProperty( "test prop multi", "chunky munky", 0 );
            CPPUNIT_ASSERT( retval == true );
            // @todo this test currently fails, but it shouldn't
            CPPUNIT_ASSERT( 678.098f != (float)chunk_multi->getProperty( "test prop multi", 0 ) );
            CPPUNIT_ASSERT( std::string( "chunky munky" ) == (std::string)chunk_multi->getProperty( "test prop multi", 0 ) );
         }
      }

      void PropertyValueTests ()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->removeAll();
         
         std::string file_path(TESTFILES_PATH);

         // Load the test configuration file
         jccl::ConfigChunkDB chunkdb;
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
         chunkdb.load( file_path + "ConfigChunkTest/ConfigChunkTest.config" );

         // Check the chunks
         jccl::ConfigChunkPtr chunk1 = chunkdb.getChunk( "Chunk1" );
         CPPUNIT_ASSERT( chunk1.get() != NULL );

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


      void testEqual()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->removeAll();
         
         std::string file_path(TESTFILES_PATH);
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         // set property integer
         jccl::ConfigChunk chunk_int( desc );
         bool retval = chunk_int.setProperty( "test prop int", 2, 0 );
         CPPUNIT_ASSERT( retval == true );
         
         jccl::ConfigChunk receiving( desc );
         CPPUNIT_ASSERT( chunk_int.getProperty( "test prop int", 0 ) != receiving.getProperty( "test prop int", 0 ) );
         receiving = chunk_int;
         CPPUNIT_ASSERT( chunk_int.getProperty( "test prop int", 0 ) == receiving.getProperty( "test prop int", 0 ) );
      }

      void testCopyConstr()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->removeAll();
         
         std::string file_path(TESTFILES_PATH);
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         // set property integer
         jccl::ConfigChunk chunk_int( desc );
         bool retval = chunk_int.setProperty( "test prop int", 2, 0 );
         CPPUNIT_ASSERT( retval == true );
         
         jccl::ConfigChunk receiving( chunk_int );
         CPPUNIT_ASSERT( chunk_int.getProperty( "test prop int", 0 ) == receiving.getProperty( "test prop int", 0 ) );
      }

      void testIsEqual()
      {
      }

      void testIsNotEqual()
      {
      }


      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "ConfigChunkTest" );
         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "PropertyValueTests", &ConfigChunkTest::PropertyValueTests ) );
         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "setPropTests", &ConfigChunkTest::setPropTests ) );

         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testEqual", &ConfigChunkTest::testEqual ) );
         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testCopyConstr", &ConfigChunkTest::testCopyConstr ) );

         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testIsEqual", &ConfigChunkTest::testIsEqual ) );
         test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testIsNotEqual", &ConfigChunkTest::testIsNotEqual ) );

         return test_suite;
      }
   };

} // end jcclTest space

#endif
