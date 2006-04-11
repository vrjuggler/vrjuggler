#ifndef _STATUS_TEST_H
#define _STATUS_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class ReturnStatusTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ReturnStatusTest);
CPPUNIT_TEST( basic );
CPPUNIT_TEST( helpers );
CPPUNIT_TEST_SUITE_END();

public:
   void basic();

   void helpers();

};

}

#endif
