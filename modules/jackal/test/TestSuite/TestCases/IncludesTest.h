#ifndef _INCLUDES_TEST_H
#define _INCLUDES_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

// #include <jccl/Config/VarValue.h>
// #include <jccl/Config/ConfigChunk.h>
// #include <jccl/Config/ChunkDesc.h>
// #include <jccl/Config/ChunkDescDB.h>
// #include <jccl/Config/ConfigChunkDB.h>
// #include <jccl/Config/ChunkFactory.h>

/*****************************************************************
 tests out the functionality expected of desc & config file
 includes in config files.
*******************************************************************/

namespace jcclTest
{

class IncludesTest : public CppUnit::TestCase
{
public:

    IncludesTest();
    
    IncludesTest(std::string name);

    virtual ~IncludesTest();

    void XMLDescIncludeTest ();

    void XMLChunkIncludeTest ();

    static CppUnit::Test* suite();

};

}

#endif
