#ifndef _BOOST_TEST_H
#define _BOOST_TEST_H

#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/Util/ReturnStatus.h>
#include <boost/smart_ptr.hpp>


/*****************************************************************
 Test any functionality that we require from BOOST in order for 
 our libraries to work right.
 
 This way, if boost is broken we detect it here instead of seeing
 our stuff breaking.
 *******************************************************************/

namespace vprTest
{

class BoostTest : public TestCase
{
public:
   BoostTest( std::string name ) : TestCase (name)
   {
   }


   virtual ~BoostTest()
   {
   }

   void shared_ptr_basic()
   {
      assertTest(true);

      boost::shared_ptr<int>  intPtr1(new int(3));
      boost::shared_ptr<int>  intPtr2;
      intPtr2 = intPtr1;

      assertTest(intPtr1 == intPtr2);
      assertTest(intPtr1.use_count() == 2);
      assertTest(!intPtr2.unique());

      int* temp_int = new int(5);

      intPtr2.reset(temp_int);
      assertTest(intPtr1.unique());
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
      assertTest(true);

      boost::shared_ptr<BaseClass>  base1(new BaseClass(1));
      boost::shared_ptr<DerivedClass>  derived1(new DerivedClass(1, 12.3f));
      
      assertTest(derived1.use_count() == 1);

      // Test an upcast assignment
      {
         boost::shared_ptr<BaseClass>  base_derivedCopy;
         base_derivedCopy = derived1;
         
         assertTest(derived1.use_count() == 2);
         assertTest(base_derivedCopy == derived1);
      }
      assertTest(derived1.use_count() == 1);

      // Test an upcast Copy constructor
      {
         boost::shared_ptr<BaseClass>  base_derivedCopy(derived1);
         
         assertTest(derived1.use_count() == 2);
         assertTest(base_derivedCopy == derived1);
      }
      assertTest(derived1.use_count() == 1);            
   }


   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("BoostTest");

      test_suite->addTest( new TestCaller<BoostTest>("shared_ptr_basic", &BoostTest::shared_ptr_basic));
      test_suite->addTest( new TestCaller<BoostTest>("shared_ptr_upcast", &BoostTest::shared_ptr_upcast));
      
      return test_suite;
   }
};

}

#endif
