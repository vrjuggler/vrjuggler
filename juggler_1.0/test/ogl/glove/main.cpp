//----------------------------------------
// Test OpenGL VR Juggler program
//
// main.cpp
//----------------------------------------
#include <application.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();           // Get the kernel
   gloveApp* application = new gloveApp(kernel);       // Declare an instance of the app

   kernel->setApplication(application);               // Set application

   if(argc > 1)
      kernel->setProgramSpecifiedConfigFile(argv[1]);

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   while(1)
   {;}
}
