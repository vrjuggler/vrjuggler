#ifndef _BOOST_TEST_H
#define _BOOST_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <boost/function.hpp>

#include <vpr/Util/ReturnStatus.h>


/*****************************************************************
 Test any functionality that we require from BOOST in order for 
 our libraries to work right.
 
 This way, if boost is broken we detect it here instead of seeing
 our stuff breaking.
 *******************************************************************/

namespace vprTest
{

class BoostTest : public CppUnit::TestCase
{
public:
   BoostTest() : CppUnit::TestCase ()
   {
   }

   BoostTest (std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~BoostTest()
   {
   }

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

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("BoostTest");
      test_suite->addTest( new CppUnit::TestCaller<BoostTest>("shared_ptr_basic", &BoostTest::shared_ptr_basic));
      test_suite->addTest( new CppUnit::TestCaller<BoostTest>("shared_ptr_upcast", &BoostTest::shared_ptr_upcast));
      test_suite->addTest( new CppUnit::TestCaller<BoostTest>("testFunctionBasic", &BoostTest::testFunctionBasic));

      return test_suite;
   }
};

}

#endif
