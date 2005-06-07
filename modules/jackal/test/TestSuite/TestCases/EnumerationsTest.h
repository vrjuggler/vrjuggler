#ifndef _ENUMERATIONS_TEST_H
#define _ENUMERATIONS_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/*****************************************************************
 tests out the functionality expected of enumerations in ConfigElements
*******************************************************************/

namespace jcclTest
{

class EnumerationsTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(EnumerationsTest);
   CPPUNIT_TEST(Read);
   CPPUNIT_TEST_SUITE_END();

public:
   void Read();
};

}

#endif
