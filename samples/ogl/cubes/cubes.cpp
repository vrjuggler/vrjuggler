//----------------------------------------
// Test OpenGL VR Juggler program
//
// cubes.cpp
//----------------------------------------
#include <cubesApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();        // Get the kernel
   cubesApp* application = new cubesApp(kernel);   // Declare an instance of the app

   kernel->setApplication(application);         // Set application

   if(argc > 1)
      kernel->setProgramSpecifiedConfigFile(argv[1]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   while(1)
   {
      sched_yield();
   }
}
