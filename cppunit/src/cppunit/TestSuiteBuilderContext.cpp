#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFixtureFactory.h>
#include <cppunit/extensions/TestNamer.h>
#include <cppunit/extensions/TestSuiteBuilderContext.h>


CPPUNIT_NS_BEGIN

TestSuiteBuilderContextBase::TestSuiteBuilderContextBase( 
                                 TestSuite &suite,
                                 const TestNamer &namer,
                                 TestFixtureFactory &factory )
  : m_suite( suite )
  , m_namer( namer )
  , m_factory( factory )
{
}


void 
TestSuiteBuilderContextBase::addTest( Test *test )
{
  m_suite.addTest( test );
}


std::string 
TestSuiteBuilderContextBase::getFixtureName() const
{
  return m_namer.getFixtureName();
}


std::string 
TestSuiteBuilderContextBase::getTestNameFor( 
                                 const std::string &testMethodName ) const
{
  return m_namer.getTestNameFor( testMethodName );
}


TestFixture *
TestSuiteBuilderContextBase::makeTestFixture() const
{
  return m_factory.makeFixture();
}


CPPUNIT_NS_END
