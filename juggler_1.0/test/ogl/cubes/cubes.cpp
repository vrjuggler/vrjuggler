//----------------------------------------
// Test OpenGL VR Juggler program
//
// cubes.cpp
//----------------------------------------
#include <cubesApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/vjProjection.h>
//#include <unistd.h>

int main(int argc, char* argv[])
{
   vjProjection::setNearFar(0.01, 10000.0f);
   
   vjKernel* kernel = vjKernel::instance();        // Get the kernel
   cubesApp* application = new cubesApp(kernel);   // Declare an instance of the app

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(application);         // Set application

   while(1)
   {
       usleep (250000);
       //sched_yield();
   }
}





