#ifndef _JCCL_CONFIG_ELEMENT_TEST_H
#define _JCCL_CONFIG_ELEMENT_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


namespace jcclTest
{
   class ConfigElementTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(ConfigElementTest);
      CPPUNIT_TEST(SetProperty);
      CPPUNIT_TEST(PropertyValues);
      CPPUNIT_TEST(BasicEquality);
      CPPUNIT_TEST(CopyConstructor);
      CPPUNIT_TEST(ContentEquality);
      CPPUNIT_TEST(BasicInequality);
      CPPUNIT_TEST(ReadPropertyValues);
      CPPUNIT_TEST_SUITE_END();

   public:
      virtual ~ConfigElementTest()
      {
      }

      void SetProperty();
      void PropertyValues();
      void BasicEquality();
      void CopyConstructor();
      void ContentEquality();
      void BasicInequality();
      void ReadPropertyValues();
   };

} // end jcclTest namespace

#endif
