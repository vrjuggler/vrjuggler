#ifndef _CONFIG_CHUNK_TEST_H
#define _CONFIG_CHUNK_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


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
