// reconfig.cpp
//----------------------------------------

#include <reconfigApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>

#ifndef WIN32
#include <sched.h>
#endif

using namespace vrj;

int main(int argc, char* argv[])
{
   Projection::setNearFar(0.01f, 10000.0f);
   
   Kernel* kernel = Kernel::instance();        // Get the kernel
   reconfigApp* application = new reconfigApp(kernel);   // Declare an instance of the app

   if (argc <= 1)
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate 
      //  and can load application plugins.
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"Usage: "<<argv[0]
               <<" vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
               <<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      exit(1);
   }
   
   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(application);         // Set application

   kernel->waitForKernelStop();

   return 1;
}
