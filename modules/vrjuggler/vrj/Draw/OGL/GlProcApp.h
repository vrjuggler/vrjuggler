/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_GL_PROC_APP_H_
#define _VRJ_GL_PROC_APP_H_

#include <vrj/Draw/OGL/GlApp.h>
#include <boost/function.hpp>     // Could use function pointers, but this is a little easier to read

using namespace vrj;    // Use vrj namespace for simplicity

namespace vrj
{

/** Class to wrap a bunch of callback method for a GL app. */
class GlProcAppWrapper : public vrj::GlApp
{
public:
   typedef boost::function0<void> callback_t;      /** Type for callbacks */

public:
   GlProcAppWrapper()      
   {;}
   virtual ~GlProcAppWrapper()
   {;}

public:  //** Getters and setters for the callback methods */
   void setDrawMethod(callback_t f)
   { mDrawMethod = f; }
   void setContextInitMethod(callback_t f)
   { mContextInit = f; }
   void setBufferPredrawMethod(callback_t f)
   { mBufferPredrawMethod = f; }
   void setPreFrameMethod(callback_t f)
   { mPreframeMethod = f; }
   void setPostFrameMethod(callback_t f)
   { mPostframeMethod = f; }
   void setIntraFrameMethod(callback_t f)
   { mIntraframeMethod = f; }

public:
   virtual void draw()
   {
      if (!mDrawMethod.empty())
      {  mDrawMethod(); }
   }

   virtual void contextInit()
   {
      if (!mContextInit.empty())
      {  mContextInit(); }
   }

   virtual void bufferPreDraw()
   {
      if (!mBufferPredrawMethod.empty())
      {  mBufferPredrawMethod(); }
   }

   virtual void preFrame()
   {
      if (!mPreframeMethod.empty())
      {  mPreframeMethod(); }
   }

   virtual void intraFrame()
   {
      if (!mIntraframeMethod.empty())
      {  mIntraframeMethod(); }
   }

   virtual void postFrame()
   {
      if (!mPostframeMethod.empty())
      {  mPostframeMethod(); }
   }

protected:
   callback_t  mDrawMethod;
   callback_t  mContextInit;
   callback_t  mPreframeMethod;
   callback_t  mPostframeMethod;
   callback_t  mIntraframeMethod;
   callback_t  mBufferPredrawMethod;
};

}

/** Set of procedures for wrapping an application using a procedural interface */
namespace
{
   vrj::GlProcAppWrapper proc_app_singleton;    /** The singleton application to use */
}

namespace vrj
{
   void VRJSetGLDrawMethod(vrj::GlProcAppWrapper::callback_t m)
   {
      proc_app_singleton.setDrawMethod(m);
   }

   void VRJSetGLContextInitMethod(vrj::GlProcAppWrapper::callback_t m)
   { 
      proc_app_singleton.setContextInitMethod(m);
   }
   
   void VRJSetBufferPredrawMethod(vrj::GlProcAppWrapper::callback_t m)
   { 
      proc_app_singleton.setBufferPredrawMethod(m);
   }

   void VRJSetPreFrameMethod(vrj::GlProcAppWrapper::callback_t m)
   {
      proc_app_singleton.setPreFrameMethod(m);
   }

   void VRJSetIntraFrameMethod(vrj::GlProcAppWrapper::callback_t m)
   {
      proc_app_singleton.setIntraFrameMethod(m);
   }
   
   void VRJSetPostFrameMethod(vrj::GlProcAppWrapper::callback_t m)
   {
      proc_app_singleton.setPostFrameMethod(m);
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
         std::cout << "Usage: " << argv[0] << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]" << std::endl;
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
      kernel->setApplication(&proc_app_singleton);

      // Keep thread alive and waiting
      kernel->waitForKernelStop();
   }   
}


#endif

