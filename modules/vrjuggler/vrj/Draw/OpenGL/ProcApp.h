/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VRJ_OPENGL_PROC_APP_H_
#define _VRJ_OPENGL_PROC_APP_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <boost/function.hpp>     // Could use function pointers, but this is a little easier to read

#include <vpr/Util/Singleton.h>
#include <vrj/Draw/OpenGL/App.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::ProcAppWrapper ProcApp.h vrj/Draw/OpenGL/ProcApp.h
 *
 * Wrapper class used to allow procedural-style application programming.
 * Callbacks are registered with this class using helper functions.  Those
 * callbacks are invoked indirectly by the VR Juggler kernel through the
 * \c vrj::App and \c vrj::opengl::App interface methods implemented in this
 * class.
 *
 * @note This class was renamed from vrj::GlProcAppWrapper in VR Juggler
 *       2.3.11.
 */
class VJ_OGL_CLASS_API ProcAppWrapper : public vrj::opengl::App
{
public:
   typedef boost::function0<void> callback_t;      /** Type for callbacks */

private:
   ProcAppWrapper();

   virtual ~ProcAppWrapper();

   vprSingletonHeader(ProcAppWrapper);

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

   void setBufferPreDrawMethod(callback_t f)
   {
      mBufferPreDrawMethod = f;
   }

   void setPreFrameMethod(callback_t f)
   {
      mPreFrameMethod = f;
   }

   void setPostFrameMethod(callback_t f)
   {
      mPostFrameMethod = f;
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

private:
   callback_t  mDrawMethod;
   callback_t  mContextInit;
   callback_t  mPreFrameMethod;
   callback_t  mPostFrameMethod;
   callback_t  mIntraframeMethod;
   callback_t  mBufferPreDrawMethod;
};

/**
 * Set of procedures for wrapping an application using a procedural interface.
 * These are in the vrj::opengl namespace. It is recommended that user code
 * utilize the C++ using statement to simplify the usage of these functions.
 *
 * \code
 * using namespace vrj::opengl;
 *
 * void preFrame();
 * void contextInit();
 * void draw();
 *
 * int main(int argc, char* argv[])
 * {
 *    VRJSetGLPreFrameMethod(preFrame);
 *    VRJSetGLContextInitMethod(contextInit);
 *    VRJSetGLDrawMethod(draw);
 *    VRJProcRunSystem();
 *
 *    return 0;
 * }
 * \endcode
 */
//@{
VJ_OGL_API(void) VRJSetGLDrawMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJSetGLContextInitMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJSetBufferPreDrawMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJSetPreFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJSetIntraFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJSetPostFrameMethod(vrj::opengl::ProcAppWrapper::callback_t m);

VJ_OGL_API(void) VRJConfigure(int argc, char* argv[]);

/** Start everything running */
VJ_OGL_API(void) VRJProcRunSystem();
//@}

}

}


#endif
