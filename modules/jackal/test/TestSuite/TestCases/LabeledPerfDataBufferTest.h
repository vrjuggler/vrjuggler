#ifndef _LABELEDPERFDATABUFFER_TEST_H
#define _LABELEDPERFDATABUFFER_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
// #include <cppunit/TestSuite.h>
// #include <cppunit/TestCaller.h>

// #include <jccl/Plugins/PerformanceMonitor/LabeledPerfDataBuffer.h>
// #include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>
// #include <jccl/Util/Debug.h>
// #include <vpr/Thread/TSObjectProxy.h>
// #include <vpr/Thread/Thread.h>
// #include <vpr/Sync/Barrier.h>
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

    LabeledPerfDataBufferTest();

    LabeledPerfDataBufferTest(std::string name);

    virtual ~LabeledPerfDataBufferTest();


    void testSetOverhead();

//     // test for set overhead with multithread contention
//     struct SetContentionTestLoopData {
//         vpr::Barrier* barrier;
//         int thread_num;
//         vpr::TSObjectProxy<jccl::LabeledPerfDataBuffer>* tsbuffer;
//         SetContentionTestLoopData (vpr::Barrier* _barrier,
//                                    int _thread_num,
//                                    vpr::TSObjectProxy<jccl::LabeledPerfDataBuffer>* _tsbuffer) {
//             barrier = _barrier;
//             thread_num = _thread_num;
//             tsbuffer = _tsbuffer;
//         }
//     };

    void setContentionTestLoop (void* d);


    void testSetContentionOverhead ();



    static CppUnit::Test* suite();

    static CppUnit::Test* metric_suite();


}; // class LabeledPerfDataBufferTest

} // namespace jcclTest


#endif
