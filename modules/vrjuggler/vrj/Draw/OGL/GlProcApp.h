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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_GL_PROC_APP_H_
#define _VRJ_GL_PROC_APP_H_

#include <vrj/vrjConfig.h>

#include <boost/function.hpp>     // Could use function pointers, but this is a little easier to read

#include <vpr/Util/Singleton.h>
#include <vrj/Draw/OGL/GlApp.h>


namespace vrj
{

/** \class GlProcAppWrapper GlProcApp.h vrj/Draw/OGL/GlProcApp.h
 *
 * Wrapper class used to allow procedural-style application programming.
 * Callbacks are registered with this class using helper functions.  Those
 * callbacks are invoked indirectly by the VR Juggler kernel through the
 * \c vrj::App and \c vrj::GlApp interface methods implemented in this class.
 */
class VJ_CLASS_API GlProcAppWrapper : public vrj::GlApp
{
public:
   typedef boost::function0<void> callback_t;      /** Type for callbacks */

protected:
   GlProcAppWrapper();

   virtual ~GlProcAppWrapper();

   vprSingletonHeader(GlProcAppWrapper);

public:
   /** @name Getters and setters for the callback methods */
   //@{
   void setDrawMethod(callback_t f)
   {
      mDrawMethod = f;
   }

   void setContextInitMethod(callback_t f)
   {
      mContextInit = f;
   }

   void setBufferPredrawMethod(callback_t f)
   {
      mBufferPredrawMethod = f;
   }

   void setPreFrameMethod(callback_t f)
   {
      mPreframeMethod = f;
   }

   void setPostFrameMethod(callback_t f)
   {
      mPostframeMethod = f;
   }

   void setIntraFrameMethod(callback_t f)
   {
      mIntraframeMethod = f;
   }
   //@}

   virtual void contextInit();

   virtual void preFrame();

   virtual void intraFrame();

   virtual void postFrame();

   virtual void bufferPreDraw();

   virtual void draw();

protected:
   callback_t  mDrawMethod;
   callback_t  mContextInit;
   callback_t  mPreframeMethod;
   callback_t  mPostframeMethod;
   callback_t  mIntraframeMethod;
   callback_t  mBufferPredrawMethod;
};

/**
 * Set of procedures for wrapping an application using a procedural interface.
 */
//@{
   VJ_API(void) VRJSetGLDrawMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJSetGLContextInitMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJSetBufferPredrawMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJSetPreFrameMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJSetIntraFrameMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJSetPostFrameMethod(vrj::GlProcAppWrapper::callback_t m);

   VJ_API(void) VRJConfigure(int argc, char* argv[]);

   /** Start everything running */
   VJ_API(void) VRJProcRunSystem();
//@}
}


#endif
