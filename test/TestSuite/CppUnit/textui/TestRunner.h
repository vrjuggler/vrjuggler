#ifndef TESTRUNNER_H
#define TESTRUNNER_H

/*
 * A command line based tool to run tests.
 * TestRunner expects as its only argument the name of a TestCase class.
 * TestRunner prints out a trace as the tests are executed followed by a
 * summary at the end.
 *
 * You can add to the tests that the TestRunner knows about by 
 * making additional calls to "addTest (...)" in main.
 *
 * Here is the synopsis:
 *
 * TestRunner [-wait] ExampleTestCase
 *
 */

#include <iostream>
#include <vector>

#include "TextTestResult.h"


using namespace std;

typedef pair<string, Test *>           mapping;
typedef vector<pair<string, Test *> >   mappings;

class TestRunner
{
protected:
    bool                                m_wait;
    vector<pair<string,Test *> >        m_mappings;

public:
	             TestRunner() : m_wait (false) {}
                ~TestRunner();

    void        run(int ac, char **av);
    void        runAllTests();
    void        runTest( const string& testSuite );
    void        addTest( const string& name, Test *test )
    { 
       m_mappings.push_back( mapping( name, test ) ); 
    }

protected:
    void        run( Test *test);
    void        printBanner();

};

#endif
