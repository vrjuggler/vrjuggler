//----------------------------------------
// Test OpenGL VR Juggler program
//
// main.cpp
//----------------------------------------
#include <wandApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <sched.h>

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();           // Get the kernel
   wandApp* application = new wandApp(kernel);       // Instantiate an instance of the app

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();

   kernel->setApplication(application);

   while(1)
   {
      sched_yield();
   }
}
