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
      //TODO:: Add in VPR debugging statements
      std::cout << "Caught an unknown exception!\n" << std::flush ;
      delete mCorbaManager; 
      mCorbaManager = NULL;
      return;
   }

   //Test to see if init succeeded
   if ( !status.success() )
   {
      std::cout << "CorbaManager did not initialize\n" << std::flush ;
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
      //TODO:: Add in VPR debugging statements
      std::cout << "Caught an unknown CORBA exception when trying to create the subject manager!\n" << std::flush ;
   }
      
   if ( !status.success() )
   {
      //TODO:: Add in VPR debugging statements
      std::cout << "CORBA failed to initialize\n" << std::flush ;
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
      //TODO:: Add in VPR debugging statements
      std::cout << "Caught an exception while trying to register subject\n" << std::flush;
   }

   if ( !status.success() )
   {
      //TODO:: Add in VPR debugging statements
      std::cout << "Failed to register Subject Manager instance\n" << std::flush ;
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
      //TODO:: Add in VPR debugging statements
      std::cout << "Caught an exception while trying to unregister subject\n" << std::flush;
   }

   if (!status.success())
   {
      std::cout << "Could not unregister subject\n" << std::flush;
   }

}

};
