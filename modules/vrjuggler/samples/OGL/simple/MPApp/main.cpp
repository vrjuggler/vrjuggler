//----------------------------------------
// Simple OpenGL sample application
//
// main.cpp
//----------------------------------------
#include <MPApp.h>

// --- Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <vpr/System.h>

int main(int argc, char* argv[])
{
   // Allocate the kernel object and the application object
   vjKernel* kernel = vjKernel::instance();           // Get the kernel
   MPApp* application = new MPApp();          // Instantiate an instance of the app

   // IF not args passed to the program
   //    Display usage information and exit
   if (argc <= 1)
   {
      cout << "\n\n";
      cout << "Usage: " << argv[0] << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]" << endl;
      exit(1);
   }

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   // Start the kernel running
   kernel->start();

   // Give the kernel an application to execute
   kernel->setApplication(application);

   // Keep thread alive and waiting
   // If the developer makes a way to request that the application exits,
   // then this loop can check for that
   while(1)
   {
      vpr::System::usleep(250000);
   }
}
