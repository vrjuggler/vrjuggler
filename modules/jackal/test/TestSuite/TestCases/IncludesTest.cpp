
#include <IncludesTest.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

/*****************************************************************
 tests out the functionality of desc & config file includes in
 config files.
*******************************************************************/

namespace jcclTest
{


    IncludesTest::IncludesTest() : CppUnit::TestCase ()
    {
    }

    IncludesTest::IncludesTest(std::string name) : CppUnit::TestCase (name)
    {
    }

    /*virtual*/ IncludesTest::~IncludesTest()
    {
    }



    void IncludesTest::XMLDescIncludeTest () {
        jccl::ChunkDescPtr desc;
        jccl::ConfigChunkPtr chunk;
        jccl::ConfigChunkDB chunkdb;

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



    void IncludesTest::XMLChunkIncludeTest ()
    {
        jccl::ChunkDescPtr desc;
        jccl::ConfigChunkPtr chunk;
        jccl::ConfigChunkDB chunkdb;

        std::string file_path(TESTFILES_PATH);
        chunkdb.load(file_path + "IncludesTest/xml_chunk_include.config");

        chunk = chunkdb.get("chunk_using_included_desc");
        CPPUNIT_ASSERT(chunk.get() != NULL);

        chunk = chunkdb.get("chunk_superceded_included_chunk");
        CPPUNIT_ASSERT(chunk.get() != NULL);
        int i = chunk->getProperty<int> ("int_prop", 0);
        CPPUNIT_ASSERT((i == 2) && "value from include has been superceded");
    }



    /*static*/ CppUnit::Test* IncludesTest::suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("IncludesTest");
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLDescIncludeTest", &IncludesTest::XMLDescIncludeTest));
        test_suite->addTest( new CppUnit::TestCaller<IncludesTest>("XMLChunkIncludeTest", &IncludesTest::XMLChunkIncludeTest));

        return test_suite;
    }



}  // namespace jcclTest
