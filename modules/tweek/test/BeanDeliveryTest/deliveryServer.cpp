#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include <tweek/CORBA/CorbaManager.h>
#include <tweek/CORBA/BeanDeliverySubjectImpl.h>


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
      if ( mgr.init("delivery_test", argc, argv).success() )
      {
         vpr::ReturnStatus status;

         // Once the CORBA Manager is initialized, we need to create the
         // Subject Manager.
         try
         {
            status = mgr.createSubjectManager();

            // If we were able to create the Subject Manager, now we register
            // our objects with it.
            if ( status.success() )
            {
               tweek::BeanDeliverySubjectImpl* delivery_subject =
                  mgr.getBeanDeliverySubject();

               if ( NULL != delivery_subject )
               {
                  tweek::BeanDeliverySubjectImpl::BeanData data;

                  data.filename = std::string(argv[1]);
                  data.introspectorClassName =
                     std::string("beandeliverytest.TestIntrospector");

                  const std::string bean_name("Delivery Test Bean");
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Adding a Bean named '" << bean_name << "'\n"
                     << vprDEBUG_FLUSH;
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Bean data:\n" << vprDEBUG_FLUSH;
                  vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "   filename = " << data.filename << std::endl
                     << vprDEBUG_FLUSH;
                  vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "   introspectorClassName = "
                     << data.introspectorClassName << std::endl
                     << vprDEBUG_FLUSH;

                  delivery_subject->addBean(bean_name, data);
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
