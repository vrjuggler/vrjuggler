
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{
   class exampleTest : public CppUnit::TestCase
   {
   public:
      exampleTest() {}
      exampleTest( std::string name )
      : CppUnit::TestCase (name)
      {;}

      virtual ~exampleTest()
      {}

      virtual void setUp()
      {;}

      virtual void tearDown()
      {
      }

      void testSimple()
      {
         CPPUNIT_ASSERT(1 != 0);
      }

      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("exampleTest");
         test_suite->addTest( new CppUnit::TestCaller<exampleTest>("testSimple", &exampleTest::testSimple));

         return test_suite;
      }

   protected:

   };
};
