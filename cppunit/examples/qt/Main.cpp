#include <qapplication.h>
#define QTTESTRUNNER_API __declspec(dllimport)
#include <cppunit/ui/qt/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	CppUnit::QtUi::TestRunner runner;
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
	runner.run( true );

	return 0;
}

