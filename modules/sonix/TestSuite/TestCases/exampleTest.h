#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

//#include <IO/Socket/InetAddr.h>

namespace ajTest
{

class exampleTest : public TestCase
{
public:
   exampleTest( std::string name )
   : TestCase (name)
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
      assertTest(1 != 0);
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("exampleTest");
      test_suite->addTest( new TestCaller<exampleTest>("testSimple", &exampleTest::testSimple));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveexampleTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
