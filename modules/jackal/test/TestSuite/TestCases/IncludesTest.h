#ifndef _INCLUDES_TEST_H
#define _INCLUDES_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

/*****************************************************************
 tests out the functionality expected of jccl::ChunkDesc
*******************************************************************/

namespace jcclTest
{

class IncludesTest : public CppUnit::TestCase
{
public:
    IncludesTest() : CppUnit::TestCase ()
    {
    }
    
    IncludesTest(std::string name) : CppUnit::TestCase (name)
    {
    }

    virtual ~IncludesTest()
    {
    }



    void XMLDescIncludeTest () {
        jccl::ChunkDescPtr desc;
        jccl::ConfigChunkPtr chunk;
        jccl::ConfigChunkDB chunkdb;

        jccl::ChunkFactory::instance()->loadDescs ("${JCCL_BASE_DIR}/share/data/jccl-chunks.desc");


        std::string file_path(TESTFILES_PATH);
        chunkdb.load (file_path + "IncludesTest/xml_desc_include.config");

        //        jccl::ChunkFactory::instance()->loadDescs ("TestFiles/xml_desc_include.desc");


        // skip this one for now cuz we need to figure out how to really
        // get a valid absolute path into the .config file...
//         // get a desc from a file that was included w/ absolute path
//         desc = jccl::ChunkFactory::instance()->getChunkDesc ("xml_included_desc1");
//         CPPUNIT_ASSERT (desc.get() != NULL);
//         CPPUNIT_ASSERT (desc->getToken() == "xml_included_desc1");

        // get a desc from a file that was included w/ relative path
        desc = jccl::ChunkFactory::instance()->getChunkDesc ("xml_included_desc2");
        CPPUNIT_ASSERT (desc.get() != NULL);
        CPPUNIT_ASSERT (desc->getToken() == "xml_included_desc2");

    }


    void StandardDescIncludeTest () {
        jccl::ChunkDescPtr desc;
        jccl::ConfigChunkPtr chunk;
        jccl::ConfigChunkDB chunkdb;


        std::string file_path(TESTFILES_PATH);
        chunkdb.load (file_path + "IncludesTest/standard_desc_include.config");

        // get a desc from a file that was included w/ relative path
        desc = jccl::ChunkFactory::instance()->getChunkDesc ("standard_included_desc1");
        CPPUNIT_ASSERT (desc.get() != NULL);
        CPPUNIT_ASSERT (desc->getToken() == "standard_included_desc1");

    }


    void XMLChunkIncludeTest () {
        jccl::ChunkDescPtr desc;
        jccl::ConfigChunkPtr chunk;
        jccl::ConfigChunkDB chunkdb;


        std::string file_path(TESTFILES_PATH);
        chunkdb.load (file_path + "IncludesTest/xml_chunk_include.config");

        chunk = chunkdb.getChunk ("chunk_using_included_desc");
        CPPUNIT_ASSERT (chunk.get() != NULL);

        chunk = chunkdb.getChunk ("chunk_superceded_included_chunk");
        CPPUNIT_ASSERT (chunk.get() != NULL);
        int i = chunk->getProperty ("int_prop", 0);
        CPPUNIT_ASSERT ((i == 2) && "value from include has been superceded");
    }
        

    static CppUnit::Test* suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("IncludesTest");
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLDescIncludeTest", &IncludesTest::XMLDescIncludeTest));
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("StandardDescIncludeTest", &IncludesTest::StandardDescIncludeTest));
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLChunkIncludeTest", &IncludesTest::XMLChunkIncludeTest));
        
        return test_suite;
    }
};

}

#endif
