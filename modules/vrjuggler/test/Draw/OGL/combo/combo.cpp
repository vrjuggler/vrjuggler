//----------------------------------------
// Test OpenGL VR Juggler program
//
//  Test lighting across gl contexts.
//
// torus.cpp
//----------------------------------------
#include <torusApp.h>
#include <wandApp.h>
#include <cubesApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/vjProjection.h>
//#include <unistd.h>

int main(int argc, char* argv[])
{
   vjProjection::setNearFar(0.01, 10000.0f);

   vjKernel* kernel = vjKernel::instance();        // Get the kernel
   torusApp* torus_app = new torusApp(kernel);   // Declare an instance of the app
   wandApp*  wand_app = new wandApp(kernel);
   cubesApp* cubes_app = new cubesApp(kernel);

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(torus_app);         // Set application

   while(1)
   {

      //sginap(1);
      sleep(5);
      kernel->setApplication(wand_app);
      //sginap(1);
      sleep(5);
      kernel->setApplication(cubes_app);
      //sginap(1);
      sleep(5);
      kernel->setApplication(torus_app);

      sched_yield();
   }
}
