#ifndef _BOOST_TEST_H
#define _BOOST_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/ReturnStatus.h>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>


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

   void shared_ptr_basic()
   {
      CPPUNIT_ASSERT(true);

      boost::shared_ptr<int>  intPtr1(new int(3));
      boost::shared_ptr<int>  intPtr2;
      intPtr2 = intPtr1;

      CPPUNIT_ASSERT(intPtr1 == intPtr2);
      CPPUNIT_ASSERT(intPtr1.use_count() == 2);
      CPPUNIT_ASSERT(!intPtr2.unique());

      int* temp_int = new int(5);

      intPtr2.reset(temp_int);
      CPPUNIT_ASSERT(intPtr1.unique());
   }

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

   void shared_ptr_upcast()
   {
      CPPUNIT_ASSERT(true);

      boost::shared_ptr<BaseClass>  base1(new BaseClass(1));
      boost::shared_ptr<DerivedClass>  derived1(new DerivedClass(1, 12.3f));
      
      CPPUNIT_ASSERT(derived1.use_count() == 1);

      // Test an upcast assignment
      {
         boost::shared_ptr<BaseClass>  base_derivedCopy;
         base_derivedCopy = derived1;
         
         CPPUNIT_ASSERT(derived1.use_count() == 2);
         CPPUNIT_ASSERT(base_derivedCopy == derived1);
      }
      CPPUNIT_ASSERT(derived1.use_count() == 1);

      // Test an upcast Copy constructor
      {
         boost::shared_ptr<BaseClass>  base_derivedCopy(derived1);
         
         CPPUNIT_ASSERT(derived1.use_count() == 2);
         CPPUNIT_ASSERT(base_derivedCopy == derived1);
      }
      CPPUNIT_ASSERT(derived1.use_count() == 1);            
   }


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
   void testFunctionBasic()
   {
      boost::function< vpr::ReturnStatus, int, float > func;

      func = retstat_int_float_functor();    // Make a new one

      CPPUNIT_ASSERT( func(10, 50.0f) == vpr::ReturnStatus::Fail);

      retstat_int_float_functor functor;
      func = functor;

      CPPUNIT_ASSERT( func(10, 50.0f) == vpr::ReturnStatus::Fail );

      bool ret_val = doSomethingWithFunc( func );
      CPPUNIT_ASSERT( ret_val == true );
   }

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
