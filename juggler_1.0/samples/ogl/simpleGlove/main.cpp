//----------------------------------------
// Test OpenGL VR Juggler program
//
// main.cpp
//----------------------------------------
#include <simpleGloveApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();           // Get the kernel
   simpleGloveApp* application = new simpleGloveApp(kernel);       // Declare an instance of the app


   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();

   kernel->setApplication(application);               // Set application

      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   while(1)
   {;}
}
