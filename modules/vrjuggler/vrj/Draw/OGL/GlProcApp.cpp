/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vrj/Draw/OGL/Config.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlProcApp.h>


namespace vrj
{

vprSingletonImp(GlProcAppWrapper);

GlProcAppWrapper::GlProcAppWrapper()      
   : vrj::GlApp()
{
   ;
}

GlProcAppWrapper::~GlProcAppWrapper()
{
   ;
}

void GlProcAppWrapper::contextInit()
{
   if ( ! mContextInit.empty() )
   {
      mContextInit();
   }
}

void GlProcAppWrapper::preFrame()
{
   if ( ! mPreframeMethod.empty() )
   {
      mPreframeMethod();
   }
}

void GlProcAppWrapper::intraFrame()
{
   if ( ! mIntraframeMethod.empty() )
   {
      mIntraframeMethod();
   }
}

void GlProcAppWrapper::postFrame()
{
   if ( ! mPostframeMethod.empty() )
   {
      mPostframeMethod();
   }
}

void GlProcAppWrapper::bufferPreDraw()
{
   if ( ! mBufferPredrawMethod.empty() )
   {
      mBufferPredrawMethod();
   }
}

void GlProcAppWrapper::draw()
{
   if ( ! mDrawMethod.empty() )
   {
      mDrawMethod();
   }
}

void VRJSetGLDrawMethod(vrj::GlProcAppWrapper::callback_t m)
{
   GlProcAppWrapper::instance()->setDrawMethod(m);
}

void VRJSetGLContextInitMethod(vrj::GlProcAppWrapper::callback_t m)
{ 
   GlProcAppWrapper::instance()->setContextInitMethod(m);
}
   
void VRJSetBufferPredrawMethod(vrj::GlProcAppWrapper::callback_t m)
{ 
   GlProcAppWrapper::instance()->setBufferPredrawMethod(m);
}

void VRJSetPreFrameMethod(vrj::GlProcAppWrapper::callback_t m)
{
   GlProcAppWrapper::instance()->setPreFrameMethod(m);
}

void VRJSetIntraFrameMethod(vrj::GlProcAppWrapper::callback_t m)
{
   GlProcAppWrapper::instance()->setIntraFrameMethod(m);
}
   
void VRJSetPostFrameMethod(vrj::GlProcAppWrapper::callback_t m)
{
   GlProcAppWrapper::instance()->setPostFrameMethod(m);
}

void VRJConfigure(int argc, char* argv[])
{
    // Allocate the kernel object and the application object
   Kernel* kernel = Kernel::instance();           // Get the kernel
   
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
   Kernel* kernel = Kernel::instance();           // Get the kernel
   
   // Start the kernel running
   kernel->start();

   // Give the kernel an application to execute
   kernel->setApplication(GlProcAppWrapper::instance());

   // Keep thread alive and waiting
   kernel->waitForKernelStop();
}   

}
