#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

//#include <IO/Socket/InetAddr.h>

namespace ajTest
{

class ajMathTest : public TestCase
{
public:
   ajMathTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~ajMathTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testSimple()
   {
      ajMatrix mat;
      mat.makeRot( 180, 0, 1, 0 );
      
      ajVec3 forward( 0,0,-1 );
      
      ajVec3 vec = xformVec( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      vec = xformFull( mat, forward );
      std::cout<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"\n"<<std::flush;
      
      assertTest( true );
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("ajMathTest");
      test_suite->addTest( new TestCaller<ajMathTest>("testSimple", &ajMathTest::testSimple));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveajMathTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
