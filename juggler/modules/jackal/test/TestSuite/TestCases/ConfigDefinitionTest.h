#ifndef _JCCL_CONFIG_DEFINITION_TEST_H
#define _JCCL_CONFIG_DEFINITION_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace jcclTest
{
   class ConfigDefinitionTest : public CppUnit::TestCase
   {
   public:
      ConfigDefinitionTest() : CppUnit::TestCase()
      {
      }

      ConfigDefinitionTest(std::string name) : CppUnit::TestCase(name)
      {
      }

      virtual ~ConfigDefinitionTest()
      {
      }

//      void basicFunctions();

      void propertyDefinitionTests();

      void testEqual();

      void testCopyConstr();

      void testIsEqual();

      void testIsNotEqual();

      void addPropDefinition();

      void remPropDefinition();

      static CppUnit::Test* suite();
   };

}

#endif
