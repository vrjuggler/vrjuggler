#ifndef _LABELEDPERFDATABUFFER_TEST_H
#define _LABELEDPERFDATABUFFER_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Plugins/PerformanceMonitor/LabeledPerfDataBuffer.h>
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>
#include <jccl/Util/Debug.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/Thread.h>
// #include <jccl/Config/VarValue.h>
// #include <jccl/Config/ConfigChunk.h>
// #include <jccl/Config/ChunkDesc.h>

/*****************************************************************
 tests out the functionality expected of jccl::LabeledPerfDataBuffer
*******************************************************************/

namespace jcclTest
{

class LabeledPerfDataBufferTest : public CppUnit::TestCase
{
public:
    LabeledPerfDataBufferTest() : CppUnit::TestCase ()
    {
    }
    
    LabeledPerfDataBufferTest(std::string name) : CppUnit::TestCase (name)
    {
    }

    virtual ~LabeledPerfDataBufferTest()
    {
    }


    void testSetOverhead() {

        int i, n;
        i = n = 100000;

        jccl::LabeledPerfDataBuffer buffer1("testSetOverhead buffer 1", n+1);
        buffer1.activate();

        jccl::TimeStamp ts1, ts2;

        ts1.set();

        while(i--) {
            buffer1.set(jcclDBG_PERFORMANCE, "foobar");
        }

        ts2.set();

        double average = (ts2.usecs() - ts1.usecs())/n;

        std::cout << "jccl::LabeledPerfDataBuffer::set()\n";
        std::cout << "\tOverhead = " << average << " us per call" << std::endl;

        vpr::TSObjectProxy<jccl::LabeledPerfDataBuffer> tsbuffer;
        tsbuffer->activate();

        i = n;

        ts1.set();

        while(i--) {
            tsbuffer->set(jcclDBG_PERFORMANCE, "foobar");
        }

        ts2.set();

        average = (ts2.usecs() - ts1.usecs())/n;

        std::cout << "jccl::LabeledPerfDataBuffer::set() (Thread-specific)\n";
        std::cout << "\tOverhead = " << average << " us per call" << std::endl;

    }

        

    static CppUnit::Test* suite()
    {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("LabeledPerfDataBufferTest");
        return test_suite;
    }

    static CppUnit::Test* metric_suite() {
        CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("LabeledPerfDataBufferTest_metric");

        test_suite->addTest( new CppUnit::TestCaller<LabeledPerfDataBufferTest>("testSetOverhead", &LabeledPerfDataBufferTest::testSetOverhead));
        
        return test_suite;
   }

};

}

#endif
