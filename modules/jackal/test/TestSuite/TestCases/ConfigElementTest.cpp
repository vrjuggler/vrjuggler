#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

#include <ConfigElementTest.h>


namespace jcclTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigElementTest);

void ConfigElementTest::SetProperty()
{
   bool retval = false;

   // make cure they fail without a desc
   {
      /*
      // set property integer
      jccl::ConfigElementPtr elt_int =
         jccl::ConfigElementPtr(new jccl::ConfigElement);
      retval = elt_int->setProperty("test_prop_int", 2, 0);
      CPPUNIT_ASSERT( retval == false );

      // set property float
      jccl::ConfigElementPtr elt_float =
         jccl::ConfigElementPtr(new jccl::ConfigElement);
      retval =  elt_float->setProperty("test_prop_float", 78.2f, 0);
      CPPUNIT_ASSERT( retval == false );

      // set property string
      jccl::ConfigElementPtr elt_string =
         jccl::ConfigElementPtr(new jccl::ConfigElement);
      retval =  elt_string->setProperty("test_prop_string",
                                        "green hairy meatball", 0);
      CPPUNIT_ASSERT( retval == false );

      // set multiple types of props on one element...
      jccl::ConfigElementPtr elt_multi =
         jccl::ConfigElementPtr(new jccl::ConfigElement);
      elt_multi->setProperty("test_prop_multi", 2, 0);
      elt_multi->setProperty("test_prop_multi", 678.098f, 0);
      elt_multi->setProperty("test_prop_multi", "chunky munky", 0);
      */
   }

   // make sure they succeed with a desc
   {
      jccl::ConfigDefinitionPtr desc =
         jccl::ElementFactory::instance()->getConfigDefinition("test_element");

      // set property integer
      jccl::ConfigElementPtr elt_int =
         jccl::ConfigElementPtr(new jccl::ConfigElement(desc));
      retval = elt_int->setProperty("test_prop_int", 0, 2);
      CPPUNIT_ASSERT( retval == true );

      // set property float
      jccl::ConfigElementPtr elt_float =
         jccl::ConfigElementPtr(new jccl::ConfigElement(desc));
      retval =  elt_float->setProperty("test_prop_float", 0, 78.2f);
      CPPUNIT_ASSERT( retval == true );

      // set property bool
      jccl::ConfigElementPtr elt_bool =
         jccl::ConfigElementPtr(new jccl::ConfigElement(desc));
      bool bool_prop_val(true);
      retval =  elt_bool->setProperty("test_prop_bool", 0, bool_prop_val);
      CPPUNIT_ASSERT( retval == true );
      bool bool_val = elt_bool->getProperty<bool>("test_prop_bool", 0);
      CPPUNIT_ASSERT( bool_prop_val == bool_val );

      // set property string
      jccl::ConfigElementPtr elt_string =
         jccl::ConfigElementPtr(new jccl::ConfigElement(desc));
      retval = elt_string->setProperty("test_prop_string", 0,
                                       "green hairy meatball");
      CPPUNIT_ASSERT( retval == true );

      // set multiple types of props on one element...
      jccl::ConfigElementPtr elt_multi =
         jccl::ConfigElementPtr(new jccl::ConfigElement(desc));

      retval = elt_multi->setProperty("test_prop_multi", 0, "chunky munky");
      CPPUNIT_ASSERT( retval == true );
      CPPUNIT_ASSERT(std::string("chunky munky") == elt_multi->getProperty<std::string>("test_prop_multi", 0));

      retval = elt_multi->setProperty("test_prop_multi", 0, 2);
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT(std::string("chunky munky") != elt_multi->getProperty<std::string>("test_prop_multi", 0));
      CPPUNIT_ASSERT(2 == elt_multi->getProperty<int>("test_prop_multi", 0));

      retval = elt_multi->setProperty("test_prop_multi", 0, 678.098f);
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT(2 != elt_multi->getProperty<int>("test_prop_multi", 0));
      CPPUNIT_ASSERT(678.098f == elt_multi->getProperty<float>("test_prop_multi", 0));

      retval = elt_multi->setProperty("test_prop_multi", 0, "chunky munky");
      CPPUNIT_ASSERT( retval == true );
      // @todo this test currently fails, but it shouldn't
      CPPUNIT_ASSERT(678.098f != elt_multi->getProperty<float>("test_prop_multi", 0));
      CPPUNIT_ASSERT(std::string("chunky munky") == elt_multi->getProperty<std::string>("test_prop_multi", 0));
   }
}

void ConfigElementTest::PropertyValues()
{
   const std::string file_path(TESTFILES_PATH);

   // Load the test configuration file
   jccl::Configuration cfg;
   cfg.load(file_path + "cfg/ConfigElementTest.jconf");

   // Check the elements
   jccl::ConfigElementPtr elt1 = cfg.get( "Element1" );
   CPPUNIT_ASSERT( elt1.get() != NULL );

   CPPUNIT_ASSERT(std::string("Element1") == elt1->getName());
   CPPUNIT_ASSERT(std::string("Element1") == elt1->getFullName());
   CPPUNIT_ASSERT(elt1->getProperty<int>("single_int") == 21);
   CPPUNIT_ASSERT(elt1->getProperty<float>("single_float") == 21.75);
   CPPUNIT_ASSERT(elt1->getProperty<bool>("single_bool") == false);

   CPPUNIT_ASSERT(elt1->getNum("var_int") == 2);
   CPPUNIT_ASSERT(elt1->getNum("var_float") == 2);

   CPPUNIT_ASSERT(elt1->getProperty<float>("var_float", 0) == 12.21f);
   CPPUNIT_ASSERT(elt1->getProperty<float>("var_float", 1) == 19.75f);

   CPPUNIT_ASSERT(elt1->getProperty<int>("var_int", 0) == 12);
   CPPUNIT_ASSERT(elt1->getProperty<int>("var_int", 1) == 21);

   CPPUNIT_ASSERT(std::string("test string") == elt1->getProperty<std::string>("single_string"));

   // Test embedded
   CPPUNIT_ASSERT(elt1->getNum("embedded_elt") == 1);
   jccl::ConfigElementPtr embedded_elt =
      elt1->getProperty<jccl::ConfigElementPtr>("embedded_elt");

   CPPUNIT_ASSERT(embedded_elt.get() != NULL);
   CPPUNIT_ASSERT(std::string("embedded_elt 0") == embedded_elt->getName());
   CPPUNIT_ASSERT(std::string("Element1/embedded_elt/embedded_elt 0") == embedded_elt->getFullName());
   CPPUNIT_ASSERT(embedded_elt->getProperty<int>("single_int") == 17);

   // Test element ptr
   CPPUNIT_ASSERT( elt1->getNum("single_ptr") == 1);
   std::string elt2_name = elt1->getProperty<std::string>("single_ptr");    // Have to get element ptr and string "element name"
   jccl::ConfigElementPtr elt2 = cfg.get(elt2_name);

   CPPUNIT_ASSERT(elt2.get() != NULL);
   CPPUNIT_ASSERT(elt2->getProperty<int>("single_int") == 21);
}

void ConfigElementTest::BasicEquality()
{
   jccl::ConfigDefinitionPtr desc =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element");

   // set property integer
   jccl::ConfigElement elt_int(desc);
   bool retval = elt_int.setProperty("test_prop_int", 0, -928);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving(elt_int);
   CPPUNIT_ASSERT(elt_int == receiving);
}

void ConfigElementTest::CopyConstructor()
{
   jccl::ConfigDefinitionPtr desc =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element");

   // set property integer
   jccl::ConfigElement elt_int(desc);
   bool retval = elt_int.setProperty( "test_prop_int", 0, 69 );
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving(elt_int);
   CPPUNIT_ASSERT(elt_int.getProperty<int>("test_prop_int", 0) == receiving.getProperty<int>("test_prop_int", 0));
}

void ConfigElementTest::ContentEquality()
{
   jccl::ConfigDefinitionPtr desc =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element");

   // set property integer
   jccl::ConfigElement elt_int(desc);
   bool retval = elt_int.setProperty("test_prop_int", 0, 2);
   CPPUNIT_ASSERT( retval == true );

   // Verify that setProperty() actually did something.
   int value = elt_int.getProperty<int>("test_prop_int", 0);
   CPPUNIT_ASSERT( 2 == value );

   jccl::ConfigElement receiving( desc );
   CPPUNIT_ASSERT(elt_int.getProperty<int>("test_prop_int", 0) != receiving.getProperty<int>("test_prop_int", 0));
   receiving = elt_int;
   CPPUNIT_ASSERT(elt_int.getProperty<int>("test_prop_int", 0) == receiving.getProperty<int>("test_prop_int", 0));
}

void ConfigElementTest::BasicInequality()
{
   jccl::ConfigDefinitionPtr desc =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element");

   // set property integer
   jccl::ConfigElement elt_int(desc);
   bool retval = elt_int.setProperty("test_prop_int", 0, 798324);
   CPPUNIT_ASSERT( retval == true );

   jccl::ConfigElement receiving( desc );
   CPPUNIT_ASSERT(elt_int != receiving);
}

void ConfigElementTest::ReadPropertyValues()
{
   jccl::ConfigDefinitionPtr desc =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element");

   jccl::ConfigElement defaults( desc );
   CPPUNIT_ASSERT( 1029384756 == defaults.getProperty<int>( "test_prop_int", 0 ) );
   CPPUNIT_ASSERT( 3.1459f == defaults.getProperty<float>( "test_prop_float", 0 ) );
   CPPUNIT_ASSERT( std::string( "upchuck" ) == defaults.getProperty<std::string>( "test_prop_string", 0 ) );
   CPPUNIT_ASSERT( std::string( "woodchuck" ) == defaults.getProperty<std::string>( "test_prop_multi", 0 ) );
   CPPUNIT_ASSERT( true == defaults.getProperty<bool>( "test_prop_bool", 0 ) );
}

}  // End of jcclTest namespace
