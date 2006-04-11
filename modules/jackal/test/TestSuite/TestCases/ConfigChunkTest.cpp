#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

#include <ConfigChunkTest.h>


namespace jcclTest
{

void ConfigChunkTest::setPropTests()
{
   bool retval = false;

   // make cure they fail without a desc
   {
      /*
      // set property integer
      jccl::ConfigChunkPtr chunk_int = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
      retval = chunk_int->setProperty( "test_prop_int", 2, 0 );
      CPPUNIT_ASSERT( retval == false );

      // set property float
      jccl::ConfigChunkPtr chunk_float = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
      retval =  chunk_float->setProperty( "test_prop_float", 78.2f, 0 );
      CPPUNIT_ASSERT( retval == false );

      // set property string
      jccl::ConfigChunkPtr chunk_string = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
      retval =  chunk_string->setProperty( "test_prop_string", "green hairy meatball", 0 );
      CPPUNIT_ASSERT( retval == false );

      // set multiple types of props on one chunk...
      jccl::ConfigChunkPtr chunk_multi = jccl::ConfigChunkPtr( new jccl::ConfigChunk );
      chunk_multi->setProperty( "test_prop_multi", 2, 0 );
      chunk_multi->setProperty( "test_prop_multi", 678.098f, 0 );
      chunk_multi->setProperty( "test_prop_multi", "chunky munky", 0 );
      */
   }

   // make sure they succeed with a desc
   {
      // start fresh and new (and shiny!!!)
      jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

      std::string file_path(TESTFILES_PATH);
      jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
      jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

      // set property integer
      jccl::ConfigChunkPtr chunk_int = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
      retval = chunk_int->setProperty( "test_prop_int", 0, 2 );
      CPPUNIT_ASSERT( retval == true );

      // set property float
      jccl::ConfigChunkPtr chunk_float = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
      retval =  chunk_float->setProperty( "test_prop_float", 0, 78.2f );
      CPPUNIT_ASSERT( retval == true );

      // set property bool
      jccl::ConfigChunkPtr chunk_bool = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
      bool bool_prop_val(true);
      retval =  chunk_bool->setProperty( "test_prop_bool", 0, bool_prop_val );
      CPPUNIT_ASSERT( retval == true );
      bool bool_val = chunk_bool->getProperty<bool>( "test_prop_bool", 0 );
      CPPUNIT_ASSERT( bool_prop_val == bool_val );

      // set property string
      jccl::ConfigChunkPtr chunk_string = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );
      retval =  chunk_string->setProperty( "test_prop_string", 0, "green hairy meatball");
      CPPUNIT_ASSERT( retval == true );

      // set multiple types of props on one chunk...
      jccl::ConfigChunkPtr chunk_multi = jccl::ConfigChunkPtr( new jccl::ConfigChunk( desc ) );

      retval = chunk_multi->setProperty( "test_prop_multi", 0, "chunky munky");
      CPPUNIT_ASSERT( retval == true );
      CPPUNIT_ASSERT( std::string( "chunky munky" ) == chunk_multi->getProperty<std::string>( "test_prop_multi", 0 ) );

      retval = chunk_multi->setProperty( "test_prop_multi", 0, 2 );
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT( std::string( "chunky munky" ) != chunk_multi->getProperty<std::string>( "test_prop_multi", 0 ) );
      CPPUNIT_ASSERT( 2 == chunk_multi->getProperty<int>( "test_prop_multi", 0 ) );

      retval = chunk_multi->setProperty( "test_prop_multi", 0, 678.098f );
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT( 2 != chunk_multi->getProperty<int>( "test_prop_multi", 0 ) );
      CPPUNIT_ASSERT( 678.098f == chunk_multi->getProperty<float>( "test_prop_multi", 0 ) );

      retval = chunk_multi->setProperty( "test_prop_multi", 0, "chunky munky");
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT( 678.098f != chunk_multi->getProperty<float>( "test_prop_multi", 0 ) );
      CPPUNIT_ASSERT( std::string( "chunky munky" ) == chunk_multi->getProperty<std::string>( "test_prop_multi", 0 ) );
   }
}

void ConfigChunkTest::PropertyValueTests()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path( TESTFILES_PATH );

   // Load the test configuration file
   jccl::ConfigChunkDB chunkdb;
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   chunkdb.load( file_path + "ConfigChunkTest/ConfigChunkTest.config" );

   // Check the chunks
   jccl::ConfigChunkPtr chunk1 = chunkdb.get( "Chunk1" );
   CPPUNIT_ASSERT( chunk1.get() != NULL );

   CPPUNIT_ASSERT( std::string("Chunk1") == chunk1->getName() );
   CPPUNIT_ASSERT( std::string("Chunk1") == chunk1->getFullName() );
   CPPUNIT_ASSERT( chunk1->getProperty<int>("single_int") == 21 );
   CPPUNIT_ASSERT( chunk1->getProperty<float>("single_float") == 21.75 );
   CPPUNIT_ASSERT( chunk1->getProperty<bool>("single_bool") == false );

   CPPUNIT_ASSERT( chunk1->getNum("var_int") == 2 );
   CPPUNIT_ASSERT( chunk1->getNum("var_float") == 2 );

   CPPUNIT_ASSERT( chunk1->getProperty<float>("var_float", 0) == 12.21f );
   CPPUNIT_ASSERT( chunk1->getProperty<float>("var_float", 1) == 19.75f );

   CPPUNIT_ASSERT( chunk1->getProperty<int>("var_int", 0) == 12 );
   CPPUNIT_ASSERT( chunk1->getProperty<int>("var_int", 1) == 21 );

   CPPUNIT_ASSERT( std::string("test string") == chunk1->getProperty<std::string>("single_string") );

   // Test embedded
   CPPUNIT_ASSERT( chunk1->getNum("embedded_chunk") == 1);
   jccl::ConfigChunkPtr embedded_chunk = chunk1->getProperty<jccl::ConfigChunkPtr>("embedded_chunk");

   CPPUNIT_ASSERT(embedded_chunk.get() != NULL);
   CPPUNIT_ASSERT(std::string("embedded_chunk 0") == embedded_chunk->getName());
   CPPUNIT_ASSERT(std::string("Chunk1/embedded_chunk/embedded_chunk 0") == embedded_chunk->getFullName());
   CPPUNIT_ASSERT( embedded_chunk->getProperty<int>("single_int") == 17);

   // Test chunk ptr
   CPPUNIT_ASSERT( chunk1->getNum("single_chunk") == 1);
   std::string chunk2_name = chunk1->getProperty<std::string>("single_chunk");    // Have to get chunk ptr and string "chunk name"
   jccl::ConfigChunkPtr chunk2 = chunkdb.get(chunk2_name);

   CPPUNIT_ASSERT(chunk2.get() != NULL);
   CPPUNIT_ASSERT( chunk2->getProperty<int>("single_int") == 21);
}

void ConfigChunkTest::testEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigChunk chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 2 );
   CPPUNIT_ASSERT( retval == true );

   // Verify that setProperty() actually did something.
   int value = chunk_int.getProperty<int>( "test_prop_int", 0 );
   CPPUNIT_ASSERT( 2 == value );

   jccl::ConfigChunk receiving( desc );
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) != receiving.getProperty<int>( "test_prop_int", 0 ) );
   receiving = chunk_int;
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) == receiving.getProperty<int>( "test_prop_int", 0 ) );
}

void ConfigChunkTest::testCopyConstr()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigChunk chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 69 );
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigChunk receiving( chunk_int );
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) == receiving.getProperty<int>( "test_prop_int", 0 ) );
}

void ConfigChunkTest::testIsEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigChunk chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, -928);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigChunk receiving( chunk_int );
   CPPUNIT_ASSERT( chunk_int == receiving );
}

void ConfigChunkTest::testIsNotEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigChunk chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 798324);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigChunk receiving( desc );
   CPPUNIT_ASSERT( chunk_int != receiving );
}

void ConfigChunkTest::testAreDefaultsProperlyReturned()
{
   // start fresh and new (and shiny!!!)
   jccl::ChunkFactory::instance()->getChunkDescDB()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ChunkFactory::instance()->loadDescs( file_path + "ConfigChunkTest/ConfigChunkTest.desc" );
   jccl::ChunkDescPtr desc = jccl::ChunkFactory::instance()->getChunkDesc( "config-chuck-the-beaver" );

   jccl::ConfigChunk defaults( desc );
   CPPUNIT_ASSERT( 1029384756 == defaults.getProperty<int>( "test_prop_int", 0 ) );
   CPPUNIT_ASSERT( 3.1459f == defaults.getProperty<float>( "test_prop_float", 0 ) );
   CPPUNIT_ASSERT( std::string( "upchuck" ) == defaults.getProperty<std::string>( "test_prop_string", 0 ) );
   CPPUNIT_ASSERT( std::string( "woodchuck" ) == defaults.getProperty<std::string>( "test_prop_multi", 0 ) );
   CPPUNIT_ASSERT( true == defaults.getProperty<bool>( "test_prop_bool", 0 ) );
}

CppUnit::Test* ConfigChunkTest::suite()
{
   CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "ConfigChunkTest" );
   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "PropertyValueTests", &ConfigChunkTest::PropertyValueTests ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "setPropTests", &ConfigChunkTest::setPropTests ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testEqual", &ConfigChunkTest::testEqual ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testCopyConstr", &ConfigChunkTest::testCopyConstr ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testIsEqual", &ConfigChunkTest::testIsEqual ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testIsNotEqual", &ConfigChunkTest::testIsNotEqual ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigChunkTest>( "testAreDefaultsProperlyReturned", &ConfigChunkTest::testAreDefaultsProperlyReturned ) );

   return test_suite;
}

}  // End of jcclTest namespace
