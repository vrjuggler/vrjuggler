//----------------------------------------
// Test OpenGL VR Juggler program
//
//  Test lighting across gl contexts.
//
// torus.cpp
//----------------------------------------
#include <torusApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/vjProjection.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
   vjProjection::setNearFar(0.01, 10000.0f);
   
   vjKernel* kernel = vjKernel::instance();        // Get the kernel
   torusApp* application = new torusApp(kernel);   // Declare an instance of the app

   kernel->setApplication(application);         // Set application

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   while(1)
   {
      sched_yield();
   }
}
