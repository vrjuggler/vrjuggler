#ifndef _VPR_TEST_NO_PUSH_WRITER_TEST_H_
#define _VPR_TEST_NO_PUSH_WRITER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <MySuites.h>

#include <vpr/vprDomain.h>


namespace vpr
{
   class NoPushWriter;
}

namespace vprTest
{

class NoPushWriterTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(NoPushWriterTest);
CPPUNIT_TEST(testMinimalAlloc);
CPPUNIT_TEST(testDoublingAlloc);
CPPUNIT_TEST_SUITE_END();

public:
   void testWrite();

   void testMinimalAlloc();

   void testDoublingAlloc();

private:
   void testWriter(const vpr::NoPushAllocStrategy& strategy);
};

}


#endif /* _VPR_TEST_NO_PUSH_WRITER_TEST_H_ */
