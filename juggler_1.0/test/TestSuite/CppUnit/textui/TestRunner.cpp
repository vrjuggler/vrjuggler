

/*
 * A command line based tool to run tests.
 * TestRunner expects as its only argument the name of a TestCase class.
 * TestRunner prints out a trace as the tests are executed followed by a
 * summary at the end.
 *
 * You can add to the tests that the TestRunner knows about by
 * making additional calls to "addTest( ...)" in main.
 *
 * Here is the synopsis:
 *
 * TestRunner [-wait] ExampleTestCase
 *
 */

#include "Test.h"
#include "TestResult.h"
#include "TestRunner.h"

void TestRunner::printBanner()
{
   cout << "Usage: driver [ -wait ] testName, where name is the name of a test case class" << endl;
   std::cout << "   Available Tests\n"
             << "   ---------------\n";
   for(mappings::iterator it = m_mappings.begin(); it != m_mappings.end(); it++)
   {
      std::cout << "   " << (*it).first << std::endl;
   }

   std::cout << std::endl;
}

void TestRunner::runAllTests()
{
   for( mappings::iterator it = m_mappings.begin();
        it != m_mappings.end();
        ++it)
   {
      Test* testToRun = (*it).second;
      run( testToRun );
   }
}

void TestRunner::runTest( const string& testSuite )
{
   Test* testToRun( NULL );

   for( mappings::iterator it = m_mappings.begin();
        it != m_mappings.end();
        ++it)
   {
     if ( (*it).first == testSuite)
     {
        testToRun = (*it).second;
        run( testToRun );
     }
   }

   if ( !testToRun)
   {
     cout << "Test " << testSuite << " not found." << endl;
     return;
   }
}

void TestRunner::run( int ac, char **av)
{
   string  testCase;
   int    numberOfTests = 0;

   for( int i = 1; i < ac; i++)
   {

      if ( string( av [i]) == "-wait") {
         m_wait = true;
         continue;
      }

      testCase = av [i];

      if ( testCase == "")
      {
         printBanner();
         return;
      }

      else if ( testCase == "all")
      {
         runAllTests();
         return;
      }

      else
      {
         this->runTest( testCase );
      }

      numberOfTests++;
   }

   if ( numberOfTests == 0)
   {
      std::cout << "There are no tests to run!!!\n\n" << std::flush;
      printBanner();
      return;
   }

   if ( m_wait)
   {
      cout << "<RETURN> to continue" << endl;
      cin.get();
   }


}


TestRunner::~TestRunner()
{
   for( mappings::iterator it = m_mappings.begin();
       it != m_mappings.end();
       ++it)
   {
     delete (*it).second;
   }
}


void TestRunner::run( Test *test)
{
   TextTestResult result;
   test->run( &result);
   cout << result << endl;
}
