#ifndef _VPR_TEST_SIGNAL_TEST_H_
#define _VPR_TEST_SIGNAL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

namespace vprTest
{

class SignalTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SignalTest);
CPPUNIT_TEST( testSegFault );
CPPUNIT_TEST_SUITE_END();

public:
   void testSegFault(void);
};

}

#endif
