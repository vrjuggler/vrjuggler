#ifndef _CHUNKDESC_TEST_H
#define _CHUNKDESC_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ChunkFactory.h>

namespace jcclTest
{
   class ChunkDescTest : public CppUnit::TestCase
   {
   public:
      ChunkDescTest() : CppUnit::TestCase()
      {
      }

      ChunkDescTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~ChunkDescTest()
      {
      }

      void basicFunctions() 
      {
         std::string name = "test desc 1";
         std::string token = "test_desc_1";
         std::string help = "test desc help info";

         jccl::ChunkDesc desc;

         desc.assertValid();

         desc.setName( name );
         desc.setToken( token );
         desc.setHelp( help );
         CPPUNIT_ASSERT( desc.getName() == name );
         CPPUNIT_ASSERT( desc.getToken() == token );
         CPPUNIT_ASSERT( desc.getHelp() == help );

         jccl::ChunkDesc desc2( desc );

         desc2.assertValid();

         CPPUNIT_ASSERT( desc2.getName() == name );
         CPPUNIT_ASSERT( desc2.getToken() == token );
         CPPUNIT_ASSERT( desc2.getHelp() == help );

         CPPUNIT_ASSERT( desc2 == desc );
      }

      void propertyDescTests()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->removeAll();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         CPPUNIT_ASSERT( desc->getName() == "chuck" );
         CPPUNIT_ASSERT( desc->getToken() == "config-chuck-the-beaver" );
         CPPUNIT_ASSERT( desc->getHelp() == "wood chuckin'" );
         
         jccl::PropertyDesc* p = desc->getPropertyDesc( "test prop multi" );
         std::cout<<"BIG FIST!!!! " <<  p->getName()<<std::endl;
         CPPUNIT_ASSERT( p->getName() == "big bad beaver" );
         CPPUNIT_ASSERT( p->getToken() == "test prop multi" );
         CPPUNIT_ASSERT( p->getHelp() == "multi beaver" );
         CPPUNIT_ASSERT( p->getType() == jccl::T_STRING );
         CPPUNIT_ASSERT( p->getNumAllowed() == 1 );
      }

      void testEqual()
      {
      }
      
      void testCopyConstr()
      {
      }
      
      void testIsEqual()
      {
      }

      void testIsNotEqual()
      {
      }

      static CppUnit::Test* suite()
      {
          CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "ChunkDescTest" );
          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "basicFunctions", &ChunkDescTest::basicFunctions ) );
          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "propertyDescTests", &ChunkDescTest::propertyDescTests ) );

          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "testEqual", &ChunkDescTest::testEqual ) );
          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "testCopyConstr", &ChunkDescTest::testCopyConstr ) );

          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "testIsEqual", &ChunkDescTest::testIsEqual ) );
          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "testIsNotEqual", &ChunkDescTest::testIsNotEqual ) );

          return test_suite;
      }
   };

}

#endif
