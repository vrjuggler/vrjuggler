#ifndef _JCCL_CONFIG_ELEMENT_TEST_H
#define _JCCL_CONFIG_ELEMENT_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace jcclTest
{
   class ConfigElementTest : public CppUnit::TestCase
   {
   public:
      ConfigElementTest() : CppUnit::TestCase()
      {
      }

      ConfigElementTest(std::string name) : CppUnit::TestCase(name)
      {
      }

      virtual ~ConfigElementTest()
      {
      }

      void setPropTests();
      void PropertyValueTests();
      void testEqual();
      void testCopyConstr();
      void testIsEqual();
      void testIsNotEqual();
      void testAreDefaultsProperlyReturned();

      static CppUnit::Test* suite();
   };

} // end jcclTest space

#endif
