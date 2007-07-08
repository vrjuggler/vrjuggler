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

#include <vrj/Draw/OpenGL/Config.h>

#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <gadget/InputManager.h>

// Get info about Gadgeteer keyboard/mouse stuff for registering the simulator
// below.
// Note: This may seem kind of strange (and it is) but we need it
// since all of our derived types are going to be gadget::Eventwindows as well
// as GL Windows.  They need to be so we can get keyboard input for
// the simulators that will run on/in this window
#include <gadget/Type/KeyboardMouse.h>
#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/Type/DeviceInterface.h>

#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
//#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Draw/DrawSimInterface.h>
#include <vrj/Draw/OpenGL/SimInterface.h>

#include <vrj/Draw/OpenGL/Window.h>

// This variable determines which matrix stack we put the viewing transformation
// If it is on the proj matrix, then lighting and env maps work but fog breaks.
#define USE_PROJECTION_MATRIX 1  /* Should we put the camera transforms on the
                                   Projection or modelview matrix */


namespace vrj
{

namespace opengl
{

int Window::mCurMaxWinId = 0;
vpr::Mutex Window::mWinIdMutex;

Window::Window()
   : mSwapCount(0)
   // The context is always dirty when the window is first created
   , mDirtyContext(true)
   , mDirtyViewport(true)
   , mInStereo(false)
   , mHasBorder(false)
   , mIsFullScreen(false)
   , mAlwaysOnTop(false)
   , mHideMouse(false)
   , mWindowIsOpen(false)
   , mWindowWidth(0)
   , mWindowHeight(0)
   , mOriginX(0)
   , mOriginY(0)
   // XXX: Sync problem on window id value assignment
   , mWindowId(getNextWindowId())
   , mIsEventSource(false)
{
   /* Do nothing. */ ;
}

void Window::configWindow(vrj::DisplayPtr displayWindow)
{
   // We can't config to a NULL display
   vprASSERT(displayWindow.get() != NULL);
   mVrjDisplay = displayWindow;
   mVrjDisplay->getOriginAndSize(mOriginX, mOriginY, mWindowWidth,
                                 mWindowHeight);
   mHasBorder = mVrjDisplay->shouldDrawBorder();
   mIsFullScreen = mVrjDisplay->isFullScreen();
   mAlwaysOnTop = mVrjDisplay->isAlwaysOnTop();
   mHideMouse = mVrjDisplay->shouldHideMouse();
}

void Window::swapBuffers()
{
   mSwapCount++;
}

// Complete any setup that is needed after open.
void Window::finishSetup()
{
   vprASSERT(mWindowIsOpen && "Pre-condition of being open failed");

   // --- Setup any attached simulator that is needed --- //
   // XXX: This is where we used to set the keyboard proxy for the simulator
   //   this needs to be customized now since we do not acutally have a device here any more
   //   Ideas: - look up device based on registry
   //          - Do something else???
/*
   ViewportPtr viewport;
   unsigned num_vps = mVrjDisplay->getNumViewports();
   for(unsigned vp_num=0; vp_num < num_vps; vp_num++)
   {
      viewport = mVrjDisplay->getViewport(vp_num);
      if(viewport->isSimulator())
      {
         SimViewportPtr sim_vp =
            boost::dynamic_pointer_cast<SimViewport>(viewport);
         vprASSERT(NULL != sim_vp.get() && "isSimulator lied");

         DrawSimInterface* draw_sim = sim_vp->getDrawSimInterface();
         SimInterface* gl_draw_sim = dynamic_cast<SimInterface*>(draw_sim);
         if(NULL != gl_draw_sim)
         {
            // Setup the simulator
            // - Get the keyboard/mouse device
            // - Register a proxy that we will use
            // - Make device interface for that proxy
            // - Intialize the simulator
            vprASSERT(mIsEventSource && "Tried to use simulator with a non-keyboard enabled GL window. Bad programmer.");
            gadget::EventWindow* kb_dev = dynamic_cast<gadget::EventWindow*>(this);
            gadget::Input* input_dev = dynamic_cast<gadget::Input*>(this);
            vprASSERT((kb_dev != NULL) && (input_dev != NULL) && "Failed to cast glWindow impl to a gadget::EventWindow");
            std::string kb_dev_name = input_dev->getInstanceName();
            vprASSERT( gadget::InputManager::instance()->getDevice(kb_dev_name) != NULL);

            gadget::EventWindowProxy* kb_proxy = new gadget::EventWindowProxy;
            kb_proxy->set(kb_dev_name, kb_dev);

            std::string kb_proxy_name("GlWin-Sim-EventWindow-");
            kb_proxy_name += kb_dev_name;
            kb_proxy_name += std::string("-Proxy");
            kb_proxy->setName(kb_proxy_name);
            bool add_success = gadget::InputManager::instance()->addProxy(kb_proxy);

            if ( ! add_success )
            {
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING")
                  << ": [vrj::opengl::Window::finishSetup()] Failed to add "
                  << "the keyboard/mouse proxy.  Check for a unique name."
                  << std::endl << vprDEBUG_FLUSH;
               vprASSERT(false && "Failed to add sim wind kb proxy: Check for unique name");
            }
            else
            {
               gadget::EventWindowInterface kb_interface;
               kb_interface.setProxy(kb_proxy);
               gl_draw_sim->setEventWindow(kb_interface); // Initialize the simulator
            }
         }
         else
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": [vrj::opengl::Window::finishSetup()] You configured a "
               << "simulator viewport, but I cannot find a DrawSimInterface "
               << "for it.\n" << vprDEBUG_FLUSH;
            vprDEBUG_CONT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << "Check your configuration for missing information.\n"
               << vprDEBUG_FLUSH;
            vprASSERT(false && "You configured a simulator viewport, but I cannot find a DrawSimInterface for it");
         }
      }
   }
*/
}

void Window::updateViewport()
{
   glViewport(0, 0, mWindowWidth, mWindowHeight);
   setDirtyViewport(false);
}

void Window::setViewport(const float xo, const float yo, const float xSize,
                         const float ySize)
{
   vprASSERT(xo + xSize <= 1.0f && "X viewport sizes are out of range");
   vprASSERT(yo + ySize <= 1.0f && "Y viewport sizes are out of range");

   unsigned int ll_x = static_cast<unsigned int>(xo * float(mWindowWidth));
   unsigned int ll_y = static_cast<unsigned int>(yo * float(mWindowHeight));
   unsigned int x_size =
      static_cast<unsigned int>(xSize * float(mWindowWidth));
   unsigned int y_size =
      static_cast<unsigned int>(ySize * float(mWindowHeight));

   glViewport(ll_x, ll_y, x_size, y_size);
}

void Window::setViewBuffer(vrj::Viewport::View view)
{
   if ( ! isStereo() )
   {
      glDrawBuffer(GL_BACK);
   }
   else if ( Viewport::LEFT_EYE == view )
   {
      glDrawBuffer(GL_BACK_LEFT);
   }
   else if ( Viewport::RIGHT_EYE == view )
   {
      glDrawBuffer(GL_BACK_RIGHT);
   }
}

void Window::setProjection(vrj::ProjectionPtr proj)
{
   if ( ! mWindowIsOpen )
   {
      return;
   }

   // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      const std::vector<float>& frust = proj->getFrustum().getValues();

      glLoadIdentity();             // Load identity matrix
      glFrustum(frust[Frustum::VJ_LEFT],frust[Frustum::VJ_RIGHT],
                 frust[Frustum::VJ_BOTTOM],frust[Frustum::VJ_TOP],
                 frust[Frustum::VJ_NEAR],frust[Frustum::VJ_FAR]);
#ifdef USE_PROJECTION_MATRIX
         // Set camera rotation and position
      glMultMatrixf(proj->getViewMatrix().mData);
#endif
   }
   glMatrixMode(GL_MODELVIEW);
#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(proj->getViewMatrix().mData);
#endif
}

int Window::getNextWindowId()
{
vpr::Guard<vpr::Mutex> guard(mWinIdMutex);      // Protect the id
   return mCurMaxWinId++;
}

std::ostream& operator<<(std::ostream& out, vrj::opengl::Window& win)
{
   vprASSERT(win.mVrjDisplay != NULL);

   //out << "-------- vrj::opengl::Window --------" << endl;
   out << "Open: " << (win.mWindowIsOpen ? "Yes" : "No") << std::endl;
   out << "Stereo: " << (win.mInStereo ? "Yes" : "No") << std::endl;
   out << "Swap Count: " << win.mSwapCount << std::endl;
   out << "Display Info:\n" << *(win.mVrjDisplay) << std::endl;
   return out;
}

} // end opengl namespace

} // end vrj namespace
