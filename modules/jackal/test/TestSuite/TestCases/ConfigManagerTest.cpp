#include <jccl/RTRC/ConfigManager.h>

#include <ConfigManagerTest.h>


namespace jcclTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigManagerTest);

static const std::string sFilePath(TESTFILES_PATH);

class ElementType1Handler : public jccl::ConfigElementHandler
{
public:
   virtual ~ElementType1Handler()
   {
   }

   virtual bool configCanHandle(jccl::ConfigElementPtr e)
   {
      static const std::string my_type("element_type_1");
      return e->getID() == my_type;
   }

   virtual bool configAdd(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }

   virtual bool configRemove(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }
};

class ElementType2Handler : public jccl::ConfigElementHandler
{
public:
   virtual ~ElementType2Handler()
   {
   }

   virtual bool configCanHandle(jccl::ConfigElementPtr e)
   {
      static const std::string my_type("element_type_2");
      return e->getID() == my_type;
   }

   virtual bool configAdd(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }

   virtual bool configRemove(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }
};

void ConfigManagerTest::setUp()
{
   vpr::Debug::instance()->disableOutput();
}

void ConfigManagerTest::tearDown()
{
   clearPendingList();
   clearActiveList();
   vpr::Debug::instance()->enableOutput();
}

void ConfigManagerTest::AddPendingAdds()
{
   bool status;

   jccl::Configuration cfg;
   status = cfg.load(sFilePath + "cfg/ConfigManagerTest1.jconf");
   CPPUNIT_ASSERT(status);

   jccl::ConfigManager::instance()->addPendingAdds(&cfg);
   CPPUNIT_ASSERT(! jccl::ConfigManager::instance()->isPendingStale());

   std::list<jccl::ConfigManager::PendingElement>::iterator i;
   std::list<jccl::ConfigManager::PendingElement>::iterator pending_end;

   jccl::ConfigManager::instance()->lockPending();
   {
      i           = jccl::ConfigManager::instance()->getPendingBegin();
      pending_end = jccl::ConfigManager::instance()->getPendingEnd();
      CPPUNIT_ASSERT(i != pending_end);
   }
   jccl::ConfigManager::instance()->unlockPending();

   status = jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_1");
   CPPUNIT_ASSERT(status);

   status = jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_bogus");
   CPPUNIT_ASSERT(! status);

   status = jccl::ConfigManager::instance()->isElementTypeInActiveList("element_type_1");
   CPPUNIT_ASSERT(! status);
}

void ConfigManagerTest::AddPendingRemoves()
{
   bool status;

   jccl::Configuration cfg;
   status = cfg.load(sFilePath + "cfg/ConfigManagerTest1.jconf");
   CPPUNIT_ASSERT(status);

   jccl::ConfigManager::instance()->addPendingRemoves(&cfg);
   int num = jccl::ConfigManager::instance()->getNumPending();
   CPPUNIT_ASSERT(num == 3);
   CPPUNIT_ASSERT(! jccl::ConfigManager::instance()->isPendingStale());

   jccl::ConfigManager::instance()->lockPending();
   {
      std::list<jccl::ConfigManager::PendingElement>::iterator i =
         jccl::ConfigManager::instance()->getPendingBegin();
      std::list<jccl::ConfigManager::PendingElement>::iterator pending_end =
         jccl::ConfigManager::instance()->getPendingEnd();
      CPPUNIT_ASSERT(i != pending_end);
   }
   jccl::ConfigManager::instance()->unlockPending();

   status = jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_1");
   CPPUNIT_ASSERT(status);

   status = jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_bogus");
   CPPUNIT_ASSERT(! status);

   status = jccl::ConfigManager::instance()->isElementTypeInActiveList("element_type_1");
   CPPUNIT_ASSERT(! status);
}

void ConfigManagerTest::AddConfigElementHandler()
{
   jccl::Configuration cfg;
   bool status = cfg.load(sFilePath + "cfg/ConfigManagerTest2.jconf");
   CPPUNIT_ASSERT(status);

   ElementType2Handler e2_handler;
   jccl::ConfigManager::instance()->addConfigElementHandler(&e2_handler);
   jccl::ConfigManager::instance()->addPendingAdds(&cfg);
   int num = jccl::ConfigManager::instance()->getNumPending();
   CPPUNIT_ASSERT(num == 1);

   status =
      jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_2");
   CPPUNIT_ASSERT(status);

   status = jccl::ConfigManager::instance()->isActiveEmpty();
   CPPUNIT_ASSERT(status);

   jccl::ConfigManager::instance()->lockPending();
   {
      int count = e2_handler.configProcessPending();
      CPPUNIT_ASSERT(count == 1);
   }
   jccl::ConfigManager::instance()->unlockPending();

   status = jccl::ConfigManager::instance()->isActiveEmpty();
   CPPUNIT_ASSERT(! status);

   status =
      jccl::ConfigManager::instance()->isElementInActiveList("External Element");
   CPPUNIT_ASSERT(status);

   status =
      jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_2");
   CPPUNIT_ASSERT(! status);

   jccl::ConfigManager::instance()->removeConfigElementHandler(&e2_handler);
}

void ConfigManagerTest::RemovePending()
{
   jccl::Configuration cfg;
   bool status = cfg.load(sFilePath + "cfg/ConfigManagerTest2.jconf");
   CPPUNIT_ASSERT(status);

   jccl::ConfigManager::instance()->addPendingAdds(&cfg);
   int num = jccl::ConfigManager::instance()->getNumPending();
   CPPUNIT_ASSERT(num == 1);

   jccl::ConfigManager::instance()->lockPending();
   {
      std::list<jccl::ConfigManager::PendingElement>::iterator i =
         jccl::ConfigManager::instance()->getPendingBegin();
      std::list<jccl::ConfigManager::PendingElement>::iterator pending_end =
         jccl::ConfigManager::instance()->getPendingEnd();

      for ( ; i != pending_end; )
      {
         std::list<jccl::ConfigManager::PendingElement>::iterator j = i;
         i++;
         jccl::ConfigManager::instance()->removePending(j);
      }
   }
   jccl::ConfigManager::instance()->unlockPending();

   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 0);

   jccl::ConfigManager::instance()->addPendingRemoves(&cfg);
   num = jccl::ConfigManager::instance()->getNumPending();
   CPPUNIT_ASSERT(num == 1);

   jccl::ConfigManager::instance()->lockPending();
   {
      std::list<jccl::ConfigManager::PendingElement>::iterator i =
         jccl::ConfigManager::instance()->getPendingBegin();
      std::list<jccl::ConfigManager::PendingElement>::iterator pending_end =
         jccl::ConfigManager::instance()->getPendingEnd();

      for ( ; i != pending_end; )
      {
         std::list<jccl::ConfigManager::PendingElement>::iterator j = i;
         i++;
         jccl::ConfigManager::instance()->removePending(j);
      }
   }
   jccl::ConfigManager::instance()->unlockPending();

   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 0);
}

void ConfigManagerTest::AttemptReconfiguration()
{
   bool status;
   int processed;

   ElementType1Handler e1_handler;
   ElementType2Handler e2_handler;

   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isActiveEmpty());
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 0);

   jccl::ConfigManager::instance()->addConfigElementHandler(&e1_handler);

   jccl::Configuration cfg1, cfg2;
   status = cfg1.load(sFilePath + "cfg/ConfigManagerTest1.jconf");
   CPPUNIT_ASSERT(status);

   jccl::ConfigManager::instance()->addPendingAdds(&cfg1);
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 3);

   // Nothing will be processed this first time because there are no handlers
   // for element_type_2, and the only element_type_1 instance depends on an
   // unloaded element_type_2 instance.
   processed = jccl::ConfigManager::instance()->attemptReconfiguration();
   CPPUNIT_ASSERT(processed == 0);

   // Now, load the second configuration file that will allow dependency
   // resolutoin of the pending element_type_1 instance.
   status = cfg2.load(sFilePath + "cfg/ConfigManagerTest2.jconf");
   CPPUNIT_ASSERT(status);

   // Add the handler for the element_type_2 type.
   jccl::ConfigManager::instance()->addConfigElementHandler(&e2_handler);

   // Hand the second configuration off to the Config Manager.
   jccl::ConfigManager::instance()->addPendingAdds(&cfg2);
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 4);

   // Make the next reconfiguration pass.  This will add the elements named
   // "Element2" and "External Element" to the active list.
   processed = jccl::ConfigManager::instance()->attemptReconfiguration();
   CPPUNIT_ASSERT(processed == 2);

   // At this point, all of the following must be true:
   //    - There are still two elements in the pending list.
   //    - Both elements are of type element_type_1.
   //    - The active list is no longer empty.
   //    - The active list contains an element of type element_type_2 named
   //      "External Element" and another named "Element2".
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 2);
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_1"));
   CPPUNIT_ASSERT(! jccl::ConfigManager::instance()->isElementTypeInPendingList("element_type_2"));
   CPPUNIT_ASSERT(! jccl::ConfigManager::instance()->isActiveEmpty());
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementTypeInActiveList("element_type_2"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("External Element"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("Element2"));

   // Make yet another reconfiguration pass.  This will add the elements named
   // "Element1" and "External3" to the active list.
   processed = jccl::ConfigManager::instance()->attemptReconfiguration();
   CPPUNIT_ASSERT(processed == 2);

   // At this point, all of the following must be true:
   //    - There are no more elements in the pending list.
   //    - The active list contains four elements:
   //        1. "External Element", type element_type_2
   //        3. "Element1", type element_type_1
   //        2. "Element2", type element_type_2
   //        4. "Element3", type element_type_1
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 0);
   CPPUNIT_ASSERT(! jccl::ConfigManager::instance()->isActiveEmpty());
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementTypeInActiveList("element_type_1"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementTypeInActiveList("element_type_2"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("External Element"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("Element1"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("Element2"));
   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isElementInActiveList("Element3"));

   // Make the final reconfiguration pass.  There is nothing left to process
   // at this point.
   processed = jccl::ConfigManager::instance()->attemptReconfiguration();
   CPPUNIT_ASSERT(processed == 0);

   jccl::ConfigManager::instance()->removeConfigElementHandler(&e1_handler);
   jccl::ConfigManager::instance()->removeConfigElementHandler(&e2_handler);
}

void ConfigManagerTest::clearPendingList()
{
   // Clear out the pending list.
   jccl::ConfigManager::instance()->lockPending();
   {
      while ( jccl::ConfigManager::instance()->getNumPending() > 0 )
      {
         std::list<jccl::ConfigManager::PendingElement>::iterator i =
            jccl::ConfigManager::instance()->getPendingBegin();
         jccl::ConfigManager::instance()->removePending(i);
      }
   }
   jccl::ConfigManager::instance()->unlockPending();

   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->getNumPending() == 0);
}

void ConfigManagerTest::clearActiveList()
{
   std::vector<jccl::ConfigElementPtr>::iterator i, active_end;

   // Clear out the active list.
   while ( ! jccl::ConfigManager::instance()->isActiveEmpty() )
   {
      jccl::ConfigManager::instance()->lockActive();
      {
         i = jccl::ConfigManager::instance()->getActiveBegin();
      }
      jccl::ConfigManager::instance()->unlockActive();

      jccl::ConfigManager::instance()->removeActive((*i)->getName());
   }

   CPPUNIT_ASSERT(jccl::ConfigManager::instance()->isActiveEmpty());
}

}
