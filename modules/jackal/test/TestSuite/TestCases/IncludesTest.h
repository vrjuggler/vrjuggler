#ifndef _INCLUDES_TEST_H
#define _INCLUDES_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/*****************************************************************
 tests out the functionality expected of desc & config file
 includes in config files.
*******************************************************************/

namespace jcclTest
{

class IncludesTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(IncludesTest);
   CPPUNIT_TEST(BasicInclude);
   CPPUNIT_TEST(ValueSuperceding);
   CPPUNIT_TEST_SUITE_END();

public:
   virtual ~IncludesTest()
   {
   }

   void BasicInclude();
   void ValueSuperceding();
};

}

#endif
