#ifndef _CONFIG_MANAGER_TEST_H
#define _CONFIG_MANAGER_TEST_H

#include <boost/concept_check.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <jccl/RTRC/ConfigElementHandler.h>


namespace jcclTest
{

/**
 * The purpose of this class is to test the functionality of the JCCL Config
 * Manager (class jccl::ConfigManager).  Because it is a singleton, its state
 * is shared across all tests, so true XP unit testing is difficult to
 * achieve because the results of one test can influence the subsequent tests
 * without some care.
 */
class ConfigManagerTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(ConfigManagerTest);
   CPPUNIT_TEST(AddConfigElementHandler);
   CPPUNIT_TEST(AddConfigurationRemovals);
   CPPUNIT_TEST(GetElementNamed);
//   CPPUNIT_TEST(RemovePending);
   CPPUNIT_TEST(AttemptReconfiguration);
   CPPUNIT_TEST_SUITE_END();

public:
   ConfigManagerTest()
      : CppUnit::TestFixture()
   {
   }

   virtual ~ConfigManagerTest()
   {
   }

   virtual void setUp();
   virtual void tearDown();

   void AddConfigElementHandler();
   void AddConfigurationRemovals();
   void GetElementNamed();
//   void RemovePending();
   void AttemptReconfiguration();

private:
   void clearPendingList();
   void clearActiveList();
};

}

#endif
