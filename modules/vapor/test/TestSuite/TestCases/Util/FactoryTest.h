#ifndef _FACTORY_TEST_H
#define _FACTORY_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/Factory.h>
#include <vpr/Util/Singleton.h>


namespace vprTest
{
   class ftBase
   {
   public:
      virtual int getVal() { return 1; }
      static int id() { return 1; }
   };

   class ftDerivedA : public ftBase
   {
   public:
      virtual int getVal() { return 2; }
      static int id() { return 2; }
   };

   class ftDerivedB : public ftBase
   {
   public:
      virtual int getVal() { return 2; }
      static int id() { return 2; }
   };

   class TestFactory : public vpr::Factory<ftBase, int>, public vpr::Singleton<TestFactory>
   {};

   namespace
   {
      const bool reg_base = TestFactory::instance()->registerCreator( ftBase::id(), vpr::CreateProduct<ftBase, ftBase>);
      const bool reg_da = TestFactory::instance()->registerCreator( ftDerivedA::id(), vpr::CreateProduct<ftBase, ftDerivedA>);
      const bool reg_db = TestFactory::instance()->registerCreator( ftDerivedB::id(), vpr::CreateProduct<ftBase, ftDerivedB>);
   };


class FactoryTest : public CppUnit::TestCase
{
public:
   FactoryTest() : CppUnit::TestCase ()
   {
   }

   FactoryTest (std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~FactoryTest()
   {
   }

   // ---------------------------- //

   void testCreateObject();

   void testIsRegistered();

   void testError();

   void testUnregister();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("FactoryTest");
      test_suite->addTest( new CppUnit::TestCaller<FactoryTest>("testCreateObject", &FactoryTest::testCreateObject));
      test_suite->addTest( new CppUnit::TestCaller<FactoryTest>("testIsRegistered", &FactoryTest::testIsRegistered));
      test_suite->addTest( new CppUnit::TestCaller<FactoryTest>("testError", &FactoryTest::testError));
      test_suite->addTest( new CppUnit::TestCaller<FactoryTest>("testUnregister", &FactoryTest::testUnregister));

      return test_suite;
   }
};

}

#endif
