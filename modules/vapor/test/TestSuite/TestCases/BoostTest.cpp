#include <boost/smart_ptr.hpp>

#include <BoostTest.h>

namespace vprTest
{

void BoostTest::shared_ptr_basic ()
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

void BoostTest::shared_ptr_upcast ()
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

void BoostTest::testFunctionBasic()
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

} // End of vprTest namespace
