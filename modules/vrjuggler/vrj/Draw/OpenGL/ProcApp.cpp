/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Draw/OpenGL/Config.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OpenGL/ProcApp.h>


namespace vrj
{

namespace opengl
{

vprSingletonImp(ProcAppWrapper);

ProcAppWrapper::ProcAppWrapper()      
   : vrj::opengl::App()
{
   ;
}

ProcAppWrapper::~ProcAppWrapper()
{
   ;
}

void ProcAppWrapper::contextInit()
{
   if ( ! mContextInit.empty() )
   {
      mContextInit();
   }
}

void ProcAppWrapper::preFrame()
{
   if ( ! mPreFrameMethod.empty() )
   {
      mPreFrameMethod();
   }
}

void ProcAppWrapper::intraFrame()
{
   if ( ! mIntraframeMethod.empty() )
   {
      mIntraframeMethod();
   }
}

void ProcAppWrapper::postFrame()
{
   if ( ! mPostFrameMethod.empty() )
   {
      mPostFrameMethod();
   }
}

void ProcAppWrapper::bufferPreDraw()
{
   if ( ! mBufferPreDrawMethod.empty() )
   {
      mBufferPreDrawMethod();
   }
}

void ProcAppWrapper::draw()
{
   if ( ! mDrawMethod.empty() )
   {
      mDrawMethod();
   }
}

void VRJSetGLDrawMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{
   vrj::opengl::ProcAppWrapper::instance()->setDrawMethod(m);
}

void VRJSetGLContextInitMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{ 
   vrj::opengl::ProcAppWrapper::instance()->setContextInitMethod(m);
}
   
void VRJSetBufferPreDrawMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{ 
   vrj::opengl::ProcAppWrapper::instance()->setBufferPreDrawMethod(m);
}

void VRJSetPreFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{
   vrj::opengl::ProcAppWrapper::instance()->setPreFrameMethod(m);
}

void VRJSetIntraFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{
   vrj::opengl::ProcAppWrapper::instance()->setIntraFrameMethod(m);
}
   
void VRJSetPostFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m)
{
   vrj::opengl::ProcAppWrapper::instance()->setPostFrameMethod(m);
}

void VRJConfigure(int& argc, char* argv[])
{
    // Allocate the kernel object and the application object
   vrj::Kernel* kernel = vrj::Kernel::instance();           // Get the kernel

   // IF no args passed to the program
   //    Display usage information and exit
   if (argc <= 1)
   {
      std::cout << "\n\n";
      std::cout << "Usage: " << argv[0]
                << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]"
                << std::endl;
      exit(1);
   }

   kernel->init(argc, argv);
   
   // Load any config files specified on the command line
   for( int i = 1; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }
}

/** Start everything running */
void VRJProcRunSystem()
{
   // Allocate the kernel object and the application object
   vrj::Kernel* kernel = vrj::Kernel::instance();           // Get the kernel
   
   // Start the kernel running
   kernel->start();

   // Give the kernel an application to execute
   kernel->setApplication(vrj::opengl::ProcAppWrapper::instance());

   // Keep thread alive and waiting
   kernel->waitForKernelStop();
}   

}

}
