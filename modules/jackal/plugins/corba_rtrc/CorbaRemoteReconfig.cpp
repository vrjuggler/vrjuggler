#include <jccl/RTRC/RTRCInterface.h>

namespace jccl
{

void RTRCInterface::init()
{
   //Create new corba manager and initialize it
   //Create new RTRCInterfaceSubjectImpl object

   mCorbaManager = new tweek::CorbaManager;

   //TODO: How do we get a unique instance name?
   mInterfaceName = "RTRCInterface";

   try
   {
      int dummy_int = 0 ;

      //Attempt to initialize the corba manager
      vpr::ReturnStatus status = mCorbaManager.init("corba_test", dummy_int, NULL);

   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Caught an unknown exception while initializing CorbaManager\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   //Test to see if init succeeded
   if ( !status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Could not initialize CorbaManager\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   try
   {
      //Attempt to create the subject manager
      status = mCorbaManager.createSubjectManager();
   }
   catch (CORBA::Exception& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: Caught an unknown CORBA exception while creating the subject manager\n" 
         << vprDEBUG_FLUSH ;
   }
      
   if ( !status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::init: CORBA subject manager failed to initialize\n" 
         << vprDEBUG_FLUSH ;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   //Create an intstance of our interface subject
   mInterface = new RTRCInterfaceSubjectImpl();

}

void RTRCInterface::enable()
{
   //Register the subject with the corba manager
   try
   {
      //Attempt to register the subject
      mCorbaManager.getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::enable: Caught an exception while trying to register subject\n" 
         << vprDEBUG_FLUSH ;
   }

   if ( !status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::enable: Failed to register subject\n" 
         << vprDEBUG_FLUSH ;

      delete mInterface;
      mInterface = NULL;
      return;
   }

}

void RTRCInterface::disable()
{
   //Unregister the subject (note that observers must handle this disconnection)
   vpr::ReturnStatus status;

   try
   {
      //Attempt to register the subject
      status = mCorbaManager.getSubjectManager()->unregisterSubject(mInterfaceName.c_str());
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::disable: Caught an exception while trying to unregister subject\n" 
         << vprDEBUG_FLUSH ;
   }

   if (!status.success())
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "RTRCInterface::disable: Could not unregister subject\n" 
         << vprDEBUG_FLUSH ;
   }

}

};
