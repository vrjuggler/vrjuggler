#ifndef _CHUNKDESC_TEST_H
#define _CHUNKDESC_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>

/*****************************************************************
 tests out the functionality expected of jccl::ChunkDesc
*******************************************************************/

namespace jcclTest
{

class ChunkDescTest : public CppUnit::TestCase
{
public:
    ChunkDescTest() : CppUnit::TestCase ()
    {
    }
    
    ChunkDescTest(std::string name) : CppUnit::TestCase (name)
    {
    }

    virtual ~ChunkDescTest()
    {
    }


    void BasicFunctions () {
        std::string name = "test desc 1";
        std::string token = "test_desc_1";
        std::string help = "test desc help info";

        jccl::ChunkDesc desc;

        desc.assertValid();

        desc.setName (name);
        desc.setToken (token);
        desc.setHelp (help);
        CPPUNIT_ASSERT (desc.getName() == name);
        CPPUNIT_ASSERT (desc.getToken() == token);
        CPPUNIT_ASSERT (desc.getHelp() == help);

        jccl::ChunkDesc desc2 (desc);

        desc2.assertValid();

        CPPUNIT_ASSERT (desc2.getName() == name);
        CPPUNIT_ASSERT (desc2.getToken() == token);
        CPPUNIT_ASSERT (desc2.getHelp() == help);

        CPPUNIT_ASSERT (desc2 == desc);

    }


    void PropertyDescTests () {
    }
        

    static CppUnit::Test* suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ChunkDescTest");
        test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>("BasicFunctions", &ChunkDescTest::BasicFunctions));
        test_suite->addTest( new CppUnit::TestCaller<ChunkDescTest>("PropertyDescTests", &ChunkDescTest::PropertyDescTests));
        
        return test_suite;
    }
};

}

#endif
