#ifndef _INCLUDES_TEST_H
#define _INCLUDES_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

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

    void XMLDefinitionIncludeTest();

    void XMLElementIncludeTest();

    static CppUnit::Test* suite();

};

}

#endif
