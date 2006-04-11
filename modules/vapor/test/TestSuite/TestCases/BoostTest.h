#ifndef _BOOST_TEST_H
#define _BOOST_TEST_H

#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <boost/function.hpp>
#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
/*****************************************************************
 Test any functionality that we require from BOOST in order for 
 our libraries to work right.
 
 This way, if boost is broken we detect it here instead of seeing
 our stuff breaking.
 *******************************************************************/
namespace vprTest
{

class BoostTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(BoostTest);
CPPUNIT_TEST( testFunctionBasic );
CPPUNIT_TEST( testStaticAssert );
CPPUNIT_TEST_SUITE_END();

public:

   void shared_ptr_basic();

   // ---- Classes to use in tests --- //
   class BaseClass
   {
   public:
      BaseClass(int val1) : mVal1(val1)
      {;}

      virtual ~BaseClass()
      {;}

      int mVal1;
   };

   class DerivedClass  : public BaseClass
   {
   public:
      DerivedClass(int val1, float val2) : BaseClass(val1), mVal2(val2)
      {;}

      virtual ~DerivedClass()
      {;}

      float mVal2;
   };

   // ---------------------------- //

   void shared_ptr_upcast();

   struct retstat_int_float_functor {
      vpr::ReturnStatus operator()(int intVal, float floatVal) const
      {
         if(intVal > (int)floatVal)
            return vpr::ReturnStatus::Succeed;
         else
            return vpr::ReturnStatus::Fail;
      }
   };
   bool doSomethingWithFunc( boost::function< vpr::ReturnStatus, int, float > f)
   {
      vpr::ReturnStatus ret = f(50,21.1f);
      return ( ret == vpr::ReturnStatus::Succeed);
   }
   void testFunctionBasic();

   void testStaticAssert()
   {
      BOOST_STATIC_ASSERT(true);
      BOOST_STATIC_ASSERT(1==1);
      BOOST_STATIC_ASSERT(2>1);
   }
};

}

#endif
