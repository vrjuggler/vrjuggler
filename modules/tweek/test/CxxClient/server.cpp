#include <tweek/CORBA/CorbaManager.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include <StringSubjectImpl.h>


/**
 * This application starts the CORBA server for the C++ side of the test.
 */
int main (int argc, char* argv[])
{
   tweek::CorbaManager mgr;

   // The first thing we have to do is initialize the Tweek CORBA Manager.
   // If this fails, we're out of luck.
   try
   {
      if ( mgr.init("cxx_client_test", argc, argv).success() )
      {
         vpr::ReturnStatus status;

         // Once the CORBA Manager is initialized, we need to create a
         // Subject Manager.  This will hold our StringSubject servant.
         try
         {
            status = mgr.createSubjectManager();

            // If we were able to create the Subject Manager, now we register
            // our objects with it.
            if ( status.success() )
            {
               // Add a test informational item to the Subject Manager.
               mgr.getSubjectManager()->addInfoItem("App Type",
                                                    "String subject server");

               // First, create real instances of the C++ object that will
               // be the CORBA servant.  This must be allocated on the heap.
               CxxClientTest::StringSubjectImpl* slider_subj =
                  new CxxClientTest::StringSubjectImpl();

               // Now we try to register the subjects and give them symbolic,
               // easy-to-remember names.
               try
               {
                  mgr.getSubjectManager()->registerSubject(slider_subj,
                                                           "StringSubject");
               }
               catch (...)
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Failed to register subject\n" << vprDEBUG_FLUSH;
               }
            }
         }
         catch (CORBA::Exception& ex)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Caught an unknown CORBA exception when trying to register!\n"
               << vprDEBUG_FLUSH;
         }

         if ( ! status.success() )
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Failed to register Subject Manager instance\n"
               << vprDEBUG_FLUSH;
         }

         std::cout << "Press 'x' to exit" << std::endl;
         char input;

         // Loop forever so that we can act sort of like a server.
         while ( 1 )
         {
            std::cin >> input;
            if ( input == 'x' )
            {
               break;
            }
            else
            {
               vpr::System::msleep(100);
            }
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "CORBA failed to initialize\n" << vprDEBUG_FLUSH;
      }
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught an unknown exception!\n" << vprDEBUG_FLUSH;
   }

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Exiting\n" << vprDEBUG_FLUSH;

   return 0;
}
