#ifndef CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_H
#define CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_H

#include <cppunit/Portability.h>
#include <string>

CPPUNIT_NS_BEGIN

class TestSuite;
class TestFixture;
class TestFixtureFactory;
class TestNamer;

/*! \brief Context used when creating test suite in HelperMacros.
 *
 * Base class for all context used when creating test suite. The
 * actual context type during test suite creation is TestSuiteBuilderContext.
 *
 * \sa CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_ADD_TEST, 
 *     CPPUNIT_TEST_SUITE_ADD_CUSTOM_TESTS.
 */
class CPPUNIT_API TestSuiteBuilderContextBase
{
public:
  /*! \brief Constructs a new context.
   *
   * You should not use this. The context is created in 
   * CPPUNIT_TEST_SUITE().
   */
  TestSuiteBuilderContextBase( TestSuite &suite,
                               const TestNamer &namer,
                               TestFixtureFactory &factory );

  /*! \brief Adds a test to the fixture suite.
   *
   * \param test Test to add to the fixture suite. Must not be \c NULL.
   */
  void addTest( Test *test );

  /*! \brief Returns the fixture name.
   * \return Fixture name. It is the name used to name the fixture
   *         suite.
   */
  std::string getFixtureName() const;

  /*! \brief Returns the name of the test for the specified method.
   *
   * \param testMethodName Name of the method that implements a test.
   * \return A string that is the concatenation of the test fixture name 
   *         (returned by getFixtureName()) and\a testMethodName, 
   *         separated using '::'. This provides a fairly unique name for a given
   *         test.
   */
  std::string getTestNameFor( const std::string &testMethodName ) const;

protected:
  TestFixture *makeTestFixture() const;

  TestSuite &m_suite;
  const TestNamer &m_namer;
  TestFixtureFactory &m_factory;
};


/*! \brief Type-sage context used when creating test suite in HelperMacros.
 * 
 * \sa TestSuiteBuilderContextBase.
 */
template<class Fixture>
class TestSuiteBuilderContext : public TestSuiteBuilderContextBase
{
public:
  typedef Fixture FixtureType;

  TestSuiteBuilderContext( TestSuiteBuilderContextBase &contextBase )
      : TestSuiteBuilderContextBase( contextBase )
  {
  }

  /*! \brief Returns a new TestFixture instance.
   * \return A new fixture instance. The fixture instance is returned by
   *         the TestFixtureFactory passed on construction. The actual type 
   *         is that of the fixture on which the static method suite() 
   *         was called.
   */
  FixtureType *makeFixture() const
  {
    return CPPUNIT_STATIC_CAST( FixtureType *, 
                                TestSuiteBuilderContextBase::makeTestFixture() );
  }
};


CPPUNIT_NS_END

#endif // CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_H