#include <iostream>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/PropertyDefinition.h>

#include <ConfigDefinitionTest.h>


namespace jcclTest
{

/*
void ConfigDefinitionTest::basicFunctions() 
{
   std::string name = "test desc 1";
   std::string token = "test_desc_1";
   std::string help = "test desc help info";

   jccl::ConfigDefinition def;

   def.assertValid();

   def.setName( name );
   def.setToken( token );
   def.setHelp( help );
   CPPUNIT_ASSERT( def.getName() == name );
   CPPUNIT_ASSERT( def.getToken() == token );
   CPPUNIT_ASSERT( def.getHelp() == help );

   jccl::ConfigDefinition def2( def );

   def2.assertValid();

   CPPUNIT_ASSERT( def2.getName() == name );
   CPPUNIT_ASSERT( def2.getToken() == token );
   CPPUNIT_ASSERT( def2.getHelp() == help );

   CPPUNIT_ASSERT( def2 == def );
}
*/

void ConfigDefinitionTest::propertyDefinitionTests()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);

   CPPUNIT_ASSERT( def->getName() == "chuck" );
   CPPUNIT_ASSERT( def->getToken() == "test_element" );
   CPPUNIT_ASSERT( def->getHelp() == "wood chuckin'" );
   
   jccl::PropertyDefinition p = def->getPropertyDefinition( "test_prop_multi" );
   CPPUNIT_ASSERT( p.getName() == "test_prop_multi" );
   CPPUNIT_ASSERT( p.getToken() == "test_prop_multi" );
   CPPUNIT_ASSERT( p.getHelp() == "multi property" );
   CPPUNIT_ASSERT( p.getVarType() == jccl::T_STRING );
   CPPUNIT_ASSERT( p.getNumAllowed() == 1 );
}

void ConfigDefinitionTest::testEqual()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);
   
   jccl::ConfigDefinition receiving;
   
   CPPUNIT_ASSERT( def->getName() != receiving.getName() );
   CPPUNIT_ASSERT( def->getToken() != receiving.getToken() );
   CPPUNIT_ASSERT( def->getHelp() != receiving.getHelp() );
   
   receiving = (*def);

   CPPUNIT_ASSERT( def->getName() == receiving.getName() );
   CPPUNIT_ASSERT( def->getToken() == receiving.getToken() );
   CPPUNIT_ASSERT( def->getHelp() == receiving.getHelp() );
}

void ConfigDefinitionTest::testCopyConstr()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);

   jccl::ConfigDefinition receiving(*def);

   CPPUNIT_ASSERT( def->getName() == receiving.getName() );
   CPPUNIT_ASSERT( def->getToken() == receiving.getToken() );
   CPPUNIT_ASSERT( def->getHelp() == receiving.getHelp() );
}

void ConfigDefinitionTest::testIsEqual()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);
   
   jccl::ConfigDefinition receiving;
   receiving = (*def);

   CPPUNIT_ASSERT( receiving == (*def) );
}

void ConfigDefinitionTest::testIsNotEqual()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);
   
   jccl::ConfigDefinition receiving;

   CPPUNIT_ASSERT( receiving != (*def) );
}

void ConfigDefinitionTest::addPropDefinition()
{
   jccl::ConfigDefinitionPtr def =
      jccl::ElementFactory::instance()->getConfigDefinition("test_element", 1);
   
   // shouldn't exist (yet!)
   jccl::PropertyDefinition pd = def->getPropertyDefinition( "chuck e cheeze" );
   CPPUNIT_ASSERT( pd.getNode().get() == NULL );
   
/*
   jccl::PropertyDefinition pdef;
   pdef.setName( "chuckli brocolli" );
   pdef.setToken( "chuck_e_cheeze" );
   pdef.setHelp( "lend a chucking hand" );
   def->add( pdef ); // suspicious that it doesn't take a const ptr (or shared_ptr!)
   
   CPPUNIT_ASSERT( def->getPropertyDefinition( "chuck_e_cheeze" ).getNode().get() != NULL );
*/
   
   // @todo fails here, fixme. def->add() should copy the propdef -or- should use a smartptr
   //CPPUNIT_ASSERT( def->getPropertyDefinition( "chuck e cheeze" ) != pdef );
}
      
void ConfigDefinitionTest::remPropDefinition()
{
   /*
   jccl::ConfigDefinitionPtr def = jccl::ElementFactory::instance()->getConfigDefinition( "config-chuck-the-beaver" );

   // shouldn't exist (yet!)
   CPPUNIT_ASSERT( def->getPropertyDefinition( "cheer up chuck" ) == NULL );

   jccl::PropertyDefinition* pdef = new jccl::PropertyDefinition;
   pdef->setName( "chuckli brocolli" );
   pdef->setToken( "cheer up chuck" );
   pdef->setHelp( "hey chuck isn't that your friend ralph?" );
   def->add( pdef );
   CPPUNIT_ASSERT( def->getPropertyDefinition( "cheer up chuck" ) != NULL );

   // @todo remove deletes memory that i created, 
   // this would not work well in a dll (memory system conflicts...)
   bool result = def->remove( "cheer up chuck" );
   CPPUNIT_ASSERT( result == true );
   CPPUNIT_ASSERT( def->getPropertyDefinition( "cheer up chuck" ) == NULL );
   */
}

CppUnit::Test* ConfigDefinitionTest::suite()
{
    CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "ConfigDefinitionTest" );
//    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "basicFunctions", &ConfigDefinitionTest::basicFunctions ) );
    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "propertyDefinitionTests", &ConfigDefinitionTest::propertyDefinitionTests ) );

    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "testEqual", &ConfigDefinitionTest::testEqual ) );
    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "testCopyConstr", &ConfigDefinitionTest::testCopyConstr ) );

    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "testIsEqual", &ConfigDefinitionTest::testIsEqual ) );
    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "testIsNotEqual", &ConfigDefinitionTest::testIsNotEqual ) );

    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "addPropDefinition", &ConfigDefinitionTest::addPropDefinition ) );
    test_suite->addTest( new CppUnit::TestCaller<ConfigDefinitionTest>( "remPropDefinition", &ConfigDefinitionTest::remPropDefinition ) );

    return test_suite;
}

}
