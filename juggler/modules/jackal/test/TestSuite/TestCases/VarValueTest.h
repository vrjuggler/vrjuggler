#ifndef _SYSTEM_TEST_H
#define _SYSTEM_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/VarValue.h>
#include <jccl/Config/ConfigChunk.h>

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
        CPPUNIT_ASSERT ((bool)v == false);
    }

    void Conversions () {
        char* cs;
        char buf[256];

        // conversions for int varvalue
        {
            jccl::VarValue v(jccl::T_INT);
            int value = 7;
            v = value;
            CPPUNIT_ASSERT ((int)v == value);
            CPPUNIT_ASSERT ((float)v == (float)value);
            CPPUNIT_ASSERT ((bool)v == (bool)value);
            CPPUNIT_ASSERT (((jccl::ConfigChunkPtr)v).get() == 0);
        }

        // conversions for float varvalue
        {
            jccl::VarValue v(jccl::T_FLOAT);
            float value = 7.443f;
            v = value;
            CPPUNIT_ASSERT ((int)v == (int)value);
            CPPUNIT_ASSERT ((float)v == value);
            CPPUNIT_ASSERT ((bool)v == (bool)value);
            CPPUNIT_ASSERT (((jccl::ConfigChunkPtr)v).get() == 0);
        }

        // conversions for bool varvalue
        {
            jccl::VarValue v(jccl::T_BOOL);
            bool value = true;
            v = value;
            // casting from bool
            CPPUNIT_ASSERT ((int)v == (value?1:0));
            CPPUNIT_ASSERT ((float)v == (value?1.0f:0.0f));
            CPPUNIT_ASSERT ((bool)v == value);
            CPPUNIT_ASSERT (((jccl::ConfigChunkPtr)v).get() == 0);

            // assigning to bool
            v = 0.0f;
            CPPUNIT_ASSERT ((bool)v == false);
            v = 5.5f;
            CPPUNIT_ASSERT ((bool)v == true);
            v = 0;
            CPPUNIT_ASSERT ((bool)v == false);
            v = 3;
            CPPUNIT_ASSERT ((bool)v == true);
        }

    }
        

    static CppUnit::Test* suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("VarValueTest");
        test_suite->addTest( new CppUnit::TestCaller<VarValueTest>("BasicFunctions", &VarValueTest::BasicFunctions));
        test_suite->addTest( new CppUnit::TestCaller<VarValueTest>("Conversions", &VarValueTest::Conversions));
        
        return test_suite;
    }
};

}

#endif
