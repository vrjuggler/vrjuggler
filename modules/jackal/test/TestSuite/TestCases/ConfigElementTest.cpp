#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <ConfigElementTest.h>


namespace jcclTest
{

void ConfigElementTest::setPropTests()
{
   bool retval = false;

   // make cure they fail without a desc
   {
      /*
      // set property integer
      jccl::ConfigElementPtr chunk_int = jccl::ConfigElementPtr( new jccl::ConfigElement );
      retval = chunk_int->setProperty( "test_prop_int", 2, 0 );
      CPPUNIT_ASSERT( retval == false );

      // set property float
      jccl::ConfigElementPtr chunk_float = jccl::ConfigElementPtr( new jccl::ConfigElement );
      retval =  chunk_float->setProperty( "test_prop_float", 78.2f, 0 );
      CPPUNIT_ASSERT( retval == false );

      // set property string
      jccl::ConfigElementPtr chunk_string = jccl::ConfigElementPtr( new jccl::ConfigElement );
      retval =  chunk_string->setProperty( "test_prop_string", "green hairy meatball", 0 );
      CPPUNIT_ASSERT( retval == false );

      // set multiple types of props on one chunk...
      jccl::ConfigElementPtr chunk_multi = jccl::ConfigElementPtr( new jccl::ConfigElement );
      chunk_multi->setProperty( "test_prop_multi", 2, 0 );
      chunk_multi->setProperty( "test_prop_multi", 678.098f, 0 );
      chunk_multi->setProperty( "test_prop_multi", "chunky munky", 0 );
      */
   }

   // make sure they succeed with a desc
   {
      // start fresh and new (and shiny!!!)
      jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

      std::string file_path(TESTFILES_PATH);
      jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
      jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

      // set property integer
      jccl::ConfigElementPtr chunk_int = jccl::ConfigElementPtr( new jccl::ConfigElement( desc ) );
      retval = chunk_int->setProperty( "test_prop_int", 0, 2 );
      CPPUNIT_ASSERT( retval == true );

      // set property float
      jccl::ConfigElementPtr chunk_float = jccl::ConfigElementPtr( new jccl::ConfigElement( desc ) );
      retval =  chunk_float->setProperty( "test_prop_float", 0, 78.2f );
      CPPUNIT_ASSERT( retval == true );

      // set property bool
      jccl::ConfigElementPtr chunk_bool = jccl::ConfigElementPtr( new jccl::ConfigElement( desc ) );
      bool bool_prop_val(true);
      retval =  chunk_bool->setProperty( "test_prop_bool", 0, bool_prop_val );
      CPPUNIT_ASSERT( retval == true );
      bool bool_val = chunk_bool->getProperty<bool>( "test_prop_bool", 0 );
      CPPUNIT_ASSERT( bool_prop_val == bool_val );

      // set property string
      jccl::ConfigElementPtr chunk_string = jccl::ConfigElementPtr( new jccl::ConfigElement( desc ) );
      retval =  chunk_string->setProperty( "test_prop_string", 0, "green hairy meatball");
      CPPUNIT_ASSERT( retval == true );

      // set multiple types of props on one chunk...
      jccl::ConfigElementPtr chunk_multi = jccl::ConfigElementPtr( new jccl::ConfigElement( desc ) );

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

void ConfigElementTest::PropertyValueTests()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path( TESTFILES_PATH );

   // Load the test configuration file
   jccl::Configuration chunkdb;
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   chunkdb.load( file_path + "ConfigElementTest/ConfigElementTest.jconf" );

   // Check the chunks
   jccl::ConfigElementPtr chunk1 = chunkdb.get( "Chunk1" );
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
   jccl::ConfigElementPtr embedded_chunk = chunk1->getProperty<jccl::ConfigElementPtr>("embedded_chunk");

   CPPUNIT_ASSERT(embedded_chunk.get() != NULL);
   CPPUNIT_ASSERT(std::string("embedded_chunk 0") == embedded_chunk->getName());
   CPPUNIT_ASSERT(std::string("Chunk1/embedded_chunk/embedded_chunk 0") == embedded_chunk->getFullName());
   CPPUNIT_ASSERT( embedded_chunk->getProperty<int>("single_int") == 17);

   // Test chunk ptr
   CPPUNIT_ASSERT( chunk1->getNum("single_chunk") == 1);
   std::string chunk2_name = chunk1->getProperty<std::string>("single_chunk");    // Have to get chunk ptr and string "chunk name"
   jccl::ConfigElementPtr chunk2 = chunkdb.get(chunk2_name);

   CPPUNIT_ASSERT(chunk2.get() != NULL);
   CPPUNIT_ASSERT( chunk2->getProperty<int>("single_int") == 21);
}

void ConfigElementTest::testEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigElement chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 2 );
   CPPUNIT_ASSERT( retval == true );

   // Verify that setProperty() actually did something.
   int value = chunk_int.getProperty<int>( "test_prop_int", 0 );
   CPPUNIT_ASSERT( 2 == value );

   jccl::ConfigElement receiving( desc );
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) != receiving.getProperty<int>( "test_prop_int", 0 ) );
   receiving = chunk_int;
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) == receiving.getProperty<int>( "test_prop_int", 0 ) );
}

void ConfigElementTest::testCopyConstr()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigElement chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 69 );
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving( chunk_int );
   CPPUNIT_ASSERT( chunk_int.getProperty<int>( "test_prop_int", 0 ) == receiving.getProperty<int>( "test_prop_int", 0 ) );
}

void ConfigElementTest::testIsEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigElement chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, -928);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving( chunk_int );
   CPPUNIT_ASSERT( chunk_int == receiving );
}

void ConfigElementTest::testIsNotEqual()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   // set property integer
   jccl::ConfigElement chunk_int( desc );
   bool retval = chunk_int.setProperty( "test_prop_int", 0, 798324);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving( desc );
   CPPUNIT_ASSERT( chunk_int != receiving );
}

void ConfigElementTest::testAreDefaultsProperlyReturned()
{
   // start fresh and new (and shiny!!!)
   jccl::ElementFactory::instance()->getConfigDefinitionRepository()->map().clear();

   std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs( file_path + "ConfigElementTest/ConfigElementTest.jdef" );
   jccl::ConfigDefinitionPtr desc = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   jccl::ConfigElement defaults( desc );
   CPPUNIT_ASSERT( 1029384756 == defaults.getProperty<int>( "test_prop_int", 0 ) );
   CPPUNIT_ASSERT( 3.1459f == defaults.getProperty<float>( "test_prop_float", 0 ) );
   CPPUNIT_ASSERT( std::string( "upchuck" ) == defaults.getProperty<std::string>( "test_prop_string", 0 ) );
   CPPUNIT_ASSERT( std::string( "woodchuck" ) == defaults.getProperty<std::string>( "test_prop_multi", 0 ) );
   CPPUNIT_ASSERT( true == defaults.getProperty<bool>( "test_prop_bool", 0 ) );
}

CppUnit::Test* ConfigElementTest::suite()
{
   CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "ConfigElementTest" );
   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "PropertyValueTests", &ConfigElementTest::PropertyValueTests ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "setPropTests", &ConfigElementTest::setPropTests ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "testEqual", &ConfigElementTest::testEqual ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "testCopyConstr", &ConfigElementTest::testCopyConstr ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "testIsEqual", &ConfigElementTest::testIsEqual ) );
   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "testIsNotEqual", &ConfigElementTest::testIsNotEqual ) );

   test_suite->addTest( new CppUnit::TestCaller<ConfigElementTest>( "testAreDefaultsProperlyReturned", &ConfigElementTest::testAreDefaultsProperlyReturned ) );

   return test_suite;
}

}  // End of jcclTest namespace
