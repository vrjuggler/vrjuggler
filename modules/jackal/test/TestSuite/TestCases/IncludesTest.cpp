
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



    void IncludesTest::XMLDefinitionIncludeTest()
    {
        jccl::ConfigDefinitionPtr def;
        jccl::ConfigElementPtr element;
        jccl::Configuration configuration;

        std::string file_path(TESTFILES_PATH);
        configuration.load(file_path + "IncludesTest/xml_desc_include.config");

        //        jccl::ElementFactory::instance()->loadDefs("TestFiles/xml_desc_include.desc");


        // skip this one for now cuz we need to figure out how to really
        // get a valid absolute path into the .config file...
//         // get a desc from a file that was included w/ absolute path
//         def = jccl::ElementFactory::instance()->getConfigDefinition("xml_included_desc1");
//         CPPUNIT_ASSERT(def.get() != NULL);
//         CPPUNIT_ASSERT(def->getToken() == "xml_included_desc1");

        // get a definition from a file that was included w/ relative path
        def = jccl::ElementFactory::instance()->getConfigDefinition("xml_included_desc2");
        CPPUNIT_ASSERT(def.get() != NULL);
        CPPUNIT_ASSERT(def->getToken() == "xml_included_desc2");

    }



    void IncludesTest::XMLElementIncludeTest()
    {
        jccl::ConfigDefinitionPtr def;
        jccl::ConfigElementPtr element;
        jccl::Configuration configuration;

        std::string file_path(TESTFILES_PATH);
        configuration.load(file_path + "IncludesTest/xml_chunk_include.config");

        element = configuration.get("chunk_using_included_desc");
        CPPUNIT_ASSERT(element.get() != NULL);

        element = configuration.get("chunk_superceded_included_chunk");
        CPPUNIT_ASSERT(element.get() != NULL);
        int i = element->getProperty<int>("int_prop", 0);
        CPPUNIT_ASSERT((i == 2) && "value from include has been superceded");
    }



    /*static*/ CppUnit::Test* IncludesTest::suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("IncludesTest");
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLDefinitionsIncludeTest", &IncludesTest::XMLDefinitionIncludeTest));
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLElementIncludeTest", &IncludesTest::XMLElementIncludeTest));

        return test_suite;
    }



}  // namespace jcclTest
