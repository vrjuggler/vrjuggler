#include <tweek/CORBA/CorbaManager.h>
#include <tweek/CORBA/SubjectManagerImpl.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include <SliderSubjectImpl.h>
#include <WhiteboardSubjectImpl.h>


int main (int argc, char* argv[])
{
   tweek::CorbaManager mgr;

   try
   {
      if ( mgr.init("corba_test", argc, argv).success() )
      {
         vpr::ReturnStatus status;

         try
         {
            status = mgr.registerSubjectManager(tweek::SubjectManagerImpl::instance());

            if ( status.success() )
            {
               networktest::SliderSubjectImpl* slider_subj =
                  new networktest::SliderSubjectImpl();
               networktest::WhiteboardSubjectImpl* whiteboard_subj =
                  new networktest::WhiteboardSubjectImpl();

               try
               {
                  mgr.getChildPOA()->activate_object(slider_subj);
                  mgr.getChildPOA()->activate_object(whiteboard_subj);
                  tweek::SubjectManagerImpl::instance()->registerSubject(slider_subj->_this(),
                                                                         "SliderSubject");
                  tweek::SubjectManagerImpl::instance()->registerSubject(whiteboard_subj->_this(),
                                                                         "WhiteboardSubject");
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

         // Loop forever so that we can act sort of like a server.
         while ( 1 )
         {
            vpr::Thread::msleep(100);
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
