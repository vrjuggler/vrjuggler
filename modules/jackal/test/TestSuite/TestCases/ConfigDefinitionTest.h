#ifndef _JCCL_CONFIG_DEFINITION_TEST_H
#define _JCCL_CONFIG_DEFINITION_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


namespace jcclTest
{
   class ConfigDefinitionTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(ConfigDefinitionTest);
//      CPPUNIT_TEST(BasicFunctions);
      CPPUNIT_TEST(PropertyDefinition);
      CPPUNIT_TEST(CopyConstructor);
      CPPUNIT_TEST(BasicEquality);
      CPPUNIT_TEST(ContentEquality);
      CPPUNIT_TEST(BasicInequality);
      CPPUNIT_TEST(AddPropertyDefinition);
      CPPUNIT_TEST(RemovePropertyDefinition);
      CPPUNIT_TEST_SUITE_END();

   public:
      virtual ~ConfigDefinitionTest()
      {
      }

//      void BasicFunctions();

      void PropertyDefinition();

      void CopyConstructor();

      void BasicEquality();

      void ContentEquality();

      void BasicInequality();

      void AddPropertyDefinition();

      void RemovePropertyDefinition();
   };

}

#endif
