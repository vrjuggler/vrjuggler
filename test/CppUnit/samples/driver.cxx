#include <TestRunner.h>
#include "ExampleTestCase.h"
#include "MulticasterTest.h"

int main (int ac, char **av)
{
    TestRunner runner;

    runner.addTest( "example", ExampleTestCase::suite () );
    runner.addTest( "multicaster", MulticasterTest::suite () );
    runner.run( ac, av );

    return 0;
}
