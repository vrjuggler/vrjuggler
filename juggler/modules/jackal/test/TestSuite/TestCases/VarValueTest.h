#ifndef _SYSTEM_TEST_H
#define _SYSTEM_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>


/*****************************************************************
 tests out the functionality expected of jccl::VarValue
*******************************************************************/

namespace jcclTest
{

class VarValueTest : public CppUnit::TestCase
{
public:
    VarValueTest() : CppUnit::TestCase ()
    {
    }
    
    VarValueTest(std::string name) : CppUnit::TestCase (name)
    {
    }

    virtual ~VarValueTest()
    {
    }


    void BasicFunctions () {
        jccl::VarValue v(jccl::T_INT);
        CPPUNIT_ASSERT (jccl::T_INT == v.getType());
        CPPUNIT_ASSERT ((int)v == 0);
        CPPUNIT_ASSERT ((float)v == 0.0f);
    }


    static CppUnit::Test* suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("VarValueTest");
        test_suite->addTest( new CppUnit::TestCaller<VarValueTest>("BasicFunctions", &VarValueTest::BasicFunctions));
        
        return test_suite;
    }
};

}

#endif
