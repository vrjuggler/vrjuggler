#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include <TestCases/exampleTest.h> // dummy test...


void addNon( CppUnit::TextUi::TestRunner& runner )
{
   CppUnit::TestSuite* suite = new CppUnit::TestSuite( "NonInteractive" );

   // add tests to the suite
   suite->addTest( synTest::exampleTest::suite() );
  
   // Add the test suite to the runner
   runner.addTest( suite );
}
void addInt( CppUnit::TextUi::TestRunner& runner )
{
   CppUnit::TestSuite* suite = new CppUnit::TestSuite( "Interactive" );

   // add tests to the suite
   suite->addTest( synTest::exampleTest::suite() );

   // Add the test suite to the runner
   runner.addTest( suite );
}

void addAll( CppUnit::TextUi::TestRunner& runner )
{
   addNon( runner );
   addInt( runner );
}




int main( int ac, char **av )
{
   // display usage if needed
   if (ac == 1 || ac > 2)
   {
      std::cout<<"Usage: "<<av[0]<<" [non|int|all]\n"<<std::flush;
      std::cout<<" - run with 'non' for non-interactive tests\n"<<std::flush;
      std::cout<<" - run with 'int' for interactive tests\n"<<std::flush;
      std::cout<<" - default is 'non'... (broken)\n\n"<<std::flush;
   }   
   
   // add the tests
   CppUnit::TextUi::TestRunner runner;
   for (int x = 1; x < ac; ++x)
   {
      std::string arg = av[x];
      
      if (arg == "non" || arg == "noninteractive")
         addNon( runner );
      else if (arg == "int" || arg == "interactive")
         addInt( runner );
      else if (arg == "all")
         addAll( runner );
   }

   // run the tests
   runner.run();
   return 0;
}
