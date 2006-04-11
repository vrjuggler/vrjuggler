#ifndef _VPRTEST_SIM_SELECTOR_TEST_H_
#define _VPRTEST_SIM_SELECTOR_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>


namespace vprTest
{

class SimSelectorTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SimSelectorTest);
CPPUNIT_TEST( singleThreadTest );
CPPUNIT_TEST_SUITE_END();

public:
   void singleThreadTest(void);   
};

}

#endif
