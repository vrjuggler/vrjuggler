#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/Factory.h>
#include <vpr/Util/Singleton.h>

#include <TestCases/Util/FactoryTest.h>

namespace vprTest
{
  


   void FactoryTest::testCreateObject()
   {
      ftBase* cur_obj(NULL);
      cur_obj = TestFactory::instance()->createObject(ftBase::id());
      CPPUNIT_ASSERT(ftBase::id() == cur_obj->getVal());

      cur_obj = TestFactory::instance()->createObject(ftDerivedA::id());
      CPPUNIT_ASSERT(ftDerivedA::id() == cur_obj->getVal());

      cur_obj = TestFactory::instance()->createObject(ftDerivedB::id());
      CPPUNIT_ASSERT(ftDerivedB::id() == cur_obj->getVal());
   }

   void  FactoryTest::testIsRegistered()
   {
      CPPUNIT_ASSERT(TestFactory::instance()->isRegistered( ftBase::id()));
      CPPUNIT_ASSERT(TestFactory::instance()->isRegistered( ftDerivedA::id()));
      CPPUNIT_ASSERT(TestFactory::instance()->isRegistered( ftDerivedB::id()));
   }


   void  FactoryTest::testError()
   {
      ftBase* cur_obj(NULL);
      cur_obj = TestFactory::instance()->createObject(1221);      // Should fail
      CPPUNIT_ASSERT(cur_obj == NULL);
   }

   void  FactoryTest::testUnregister()
   {
      ftBase* cur_obj(NULL);
      cur_obj = TestFactory::instance()->createObject(ftBase::id());
      CPPUNIT_ASSERT(cur_obj != NULL);
      CPPUNIT_ASSERT(ftBase::id() == cur_obj->getVal());

      bool ret_val = TestFactory::instance()->unregisterCreator(ftBase::id());
      CPPUNIT_ASSERT(ret_val);

      cur_obj = TestFactory::instance()->createObject(ftBase::id());;
      CPPUNIT_ASSERT(cur_obj == NULL);       // Should ahve failed
   }

}

