
#include <IncludesTest.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ElementFactory.h>

/*****************************************************************
 tests out the functionality of definition & config file includes in
 config files.
*******************************************************************/

namespace jcclTest
{

    IncludesTest::IncludesTest() : CppUnit::TestCase()
    {
    }

    IncludesTest::IncludesTest(std::string name) : CppUnit::TestCase(name)
    {
    }

    IncludesTest::~IncludesTest()
    {
    }

    void IncludesTest::XMLElementIncludeTest()
    {
        jccl::ConfigElementPtr element;
        jccl::Configuration configuration;

        std::string file_path(TESTFILES_PATH);
        configuration.load(file_path + "cfg/include_test.jcfg");

        element = configuration.get("Test 1");
        CPPUNIT_ASSERT(element.get() != NULL);

        element = configuration.get("Superceding Test");
        CPPUNIT_ASSERT(element.get() != NULL);
        int i = element->getProperty<int>("int_prop", 0);
        CPPUNIT_ASSERT((i == 2) && "value from include has been superceded");
    }

    CppUnit::Test* IncludesTest::suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("IncludesTest");
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLElementIncludeTest", &IncludesTest::XMLElementIncludeTest));

        return test_suite;
    }

}  // namespace jcclTest
