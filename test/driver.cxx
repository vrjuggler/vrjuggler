#include <TestRunner.h>
#include <Math/vjMatrixTest.h>


int main (int ac, char **av)
{
    TestRunner runner;

    runner.addTest( "vjMatrixTest", vjMatrixTest::suite() );
    runner.run( ac, av );

    return 0;
}
