#include <tweek/CORBA/CorbaManager.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>


int main (int argc, char* argv[])
{
   tweek::CorbaManager mgr;

   try
   {
      if ( mgr.init(argc, argv).success() )
      {
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
