#ifndef _CHUNKDESC_TEST_H
#define _CHUNKDESC_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/PropertyDesc.h>

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
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         CPPUNIT_ASSERT( desc->getName() == "chuck" );
         CPPUNIT_ASSERT( desc->getToken() == "config-chuck-the-beaver" );
         CPPUNIT_ASSERT( desc->getHelp() == "wood chuckin'" );
         
         jccl::PropertyDesc p = desc->getPropertyDesc( "test_prop_multi" );
         CPPUNIT_ASSERT( p.getName() == "big bad beaver" );
         CPPUNIT_ASSERT( p.getToken() == "test_prop_multi" );
         CPPUNIT_ASSERT( p.getHelp() == "multi beaver" );
         CPPUNIT_ASSERT( p.getVarType() == jccl::T_STRING );
         CPPUNIT_ASSERT( p.getNumAllowed() == 1 );
      }

      void testEqual()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );
         
         jccl::ChunkDesc receiving;
         
         CPPUNIT_ASSERT( desc->getName() != receiving.getName() );
         CPPUNIT_ASSERT( desc->getToken() != receiving.getToken() );
         CPPUNIT_ASSERT( desc->getHelp() != receiving.getHelp() );
         
         receiving = (*desc);

         CPPUNIT_ASSERT( desc->getName() == receiving.getName() );
         CPPUNIT_ASSERT( desc->getToken() == receiving.getToken() );
         CPPUNIT_ASSERT( desc->getHelp() == receiving.getHelp() );
      }

      void testCopyConstr()
      {
         // start fresh and new (and shiny!!!) who cares about the rest of the system - blahhhh!!!
         // @todo is there a way to not do this globally?
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         jccl::ChunkDesc receiving(*desc);

         CPPUNIT_ASSERT( desc->getName() == receiving.getName() );
         CPPUNIT_ASSERT( desc->getToken() == receiving.getToken() );
         CPPUNIT_ASSERT( desc->getHelp() == receiving.getHelp() );
      }

      void testIsEqual()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );
         
         jccl::ChunkDesc receiving;
         receiving = (*desc);

         CPPUNIT_ASSERT( receiving == (*desc) );
      }

      void testIsNotEqual()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );
         
         jccl::ChunkDesc receiving;

         CPPUNIT_ASSERT( receiving != (*desc) );
      }

      void addPropDesc()
      {
         // start fresh and new (and shiny!!!)
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();
         
         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );
         
         // shouldn't exist (yet!)
         jccl::PropertyDesc pd = desc->getPropertyDesc( "chuck e cheeze" );
         CPPUNIT_ASSERT( pd.getNode().get() == NULL );
         
         jccl::PropertyDesc pdesc;
         pdesc.setName( "chuckli brocolli" );
         pdesc.setToken( "chuck_e_cheeze" );
         pdesc.setHelp( "lend a chucking hand" );
         desc->add( pdesc ); // suspicious that it doesn't take a const ptr (or shared_ptr!)
         
         CPPUNIT_ASSERT( desc->getPropertyDesc( "chuck_e_cheeze" ).getNode().get() != NULL );
         
         // @todo fails here, fixme. desc->add() should copy the propdesc -or- should use a smartptr
         //CPPUNIT_ASSERT( desc->getPropertyDesc( "chuck e cheeze" ) != pdesc );
      }
            
      void remPropDesc()
      {
         // start fresh and new (and shiny!!!)
         /*
         jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

         std::string file_path( TESTFILES_PATH );
         jccl::ChunkFactory::instance()->loadDescs( file_path + "ChunkDescTest/ChunkDescTest.desc" );
         jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

         // shouldn't exist (yet!)
         CPPUNIT_ASSERT( desc->getPropertyDesc( "cheer up chuck" ) == NULL );

         jccl::PropertyDesc* pdesc = new jccl::PropertyDesc;
         pdesc->setName( "chuckli brocolli" );
         pdesc->setToken( "cheer up chuck" );
         pdesc->setHelp( "hey chuck isn't that your friend ralph?" );
         desc->add( pdesc );
         CPPUNIT_ASSERT( desc->getPropertyDesc( "cheer up chuck" ) != NULL );

         // @todo remove deletes memory that i created, 
         // this would not work well in a dll (memory system conflicts...)
         bool result = desc->remove( "cheer up chuck" );
         CPPUNIT_ASSERT( result == true );
         CPPUNIT_ASSERT( desc->getPropertyDesc( "cheer up chuck" ) == NULL );
         */
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

          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "addPropDesc", &ChunkDescTest::addPropDesc ) );
          test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>( "remPropDesc", &ChunkDescTest::remPropDesc ) );

          return test_suite;
      }
   };

}

#endif
