/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OPENGL_DRAW_MANAGER_H_
#define _VRJ_OPENGL_DRAW_MANAGER_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <vpr/vpr.h>
//#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObjectProxy.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <jccl/Config/ConfigElementPtr.h>

#include <vrj/Draw/DrawManager.h>
#include <vrj/Draw/OpenGL/UserData.h>
#include <vrj/Draw/OpenGL/WindowPtr.h>


namespace vrj
{

namespace opengl
{

class App;
class Pipe;

/** \class vrj::opengl::DrawManager DrawManager.h vrj/Draw/OpenGL/DrawManager.h
 *
 * Concrete Singleton Class for OpenGL drawing.  Responsible for all
 * OpenGL-based rendering.
 *
 * vrj::opengl::DrawManager is an active object.  It manages OpenGL pipes and
 * windows.  In addition, it triggers rendering, swapping, and syncing of the
 * windows under its control.
 *
 * All access to the OpenGL rendering structures has to happen from the control
 * thread or in the case of context sensitive functions, from the control
 * thread of the managed pipes.  Because of this, the object uses queues to
 * hold new windows.
 *
 * @date January 7, 1998
 *
 * @note This class was renamed from vrj::GlDrawManager in VR Juggler 2.3.11.
 */
class VJ_OGL_API DrawManager
   : public vrj::DrawManager
   , private boost::noncopyable
{
public:
   friend class Pipe;
   friend class ContextDataBase;

   typedef boost::function<int (const unsigned int)> cpu_affinity_strategy_t;

   /**
    * Changes the callable object used for determining the draw thread CPU
    * affinity to use the given value. In order for this to have the
    * desired effect, it must be called before any render threads have been
    * started.
    *
    * @post \c getDrawThreadAffinity is assigned the value of \p strategy.
    *
    * @param strategy A callable (either a C function pointer, a value
    *                 returned by boost::bind(), or an object whose class
    *                 overloads operator()) that serves to map zero-based
    *                 thread identifiers to zero-based CPU values in order
    *                 to assign affinity.
    *
    * @see vpr::Thead::setRunOn()
    * @see addDisplay()
    *
    * @since 2.3.14
    */
   void setCpuAffinityStrategy(const cpu_affinity_strategy_t& strategy);

   /** Starts the control loop. */
   virtual void start();

   /** Enables a frame to be drawn. */
   virtual void draw();

   /**
    * Blocks until the end of the frame.
    *
    * @post The frame has been drawn.
    */
   virtual void sync();

   /** Control loop for the manager. */
   void main();

   /** Initializes the drawing API (if not already running). */
   virtual void initAPI();

   /**
    * Callback when display is added to the Display Manager.
    *
    * @note This function can only be called by the display manager
    *       functioning in the kernel thread to signal a new display added
    *       This guarantees that we are not rendering currently.
    *       We will most likely be waiting for a render trigger.
    */
   virtual void addDisplay(DisplayPtr disp);

   /** Callback when display is removed from the Display Manager. */
   virtual void removeDisplay(DisplayPtr disp);

   /** Shutdown the drawing API. */
   virtual void closeAPI();

   /** Outputs some debug info. */
   virtual void outStream(std::ostream& out);

   /** Draws all the OpenGL pipes/windows. */
   void drawAllPipes();

   /** Set the app the draw should interact with. */
   virtual void setApp(vrj::App* app);

   /** Returns the app we are rendering. */
   vrj::opengl::App* getApp();

   //void setDisplayManager(DisplayManager* _dispMgr);

public:
   /** @name Config element handler implementation */
   //@{
   /**
    * Adds the element to the configuration.
    *
    * @pre configCanHandle(element) == true
    * @return success
    */
   virtual bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    *
    * @pre configCanHandle(element) == true
    * @return success
    */
   virtual bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Can the handler handle the given config element?
    *
    * @return true if we can handle it; false if we can't.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element);
   //@}

public:
   /**
    * Gets pointer to the current user data.  Should be used in the draw
    * function.
    *
    * @note This user data is valid ONLY in draw().  It is not valid anywhere
    *       else.
    */
   vrj::opengl::UserData* currentUserData();

   /**
    * Returns a unique identifier for the current context.
    *
    * @note This id is ONLY valid in contextInit() and draw().
    */
   int getCurrentContext();

protected:
   /**
    * Factory function to get system specific OpenGL window.
    *
    * @post Returns an OpenGL window for the current system.
    */
   vrj::opengl::WindowPtr getGLWindow();

   void setCurrentContext(const int val);

   /** Sets the dirty bits off all the GL windows. */
   void dirtyAllWindows();

   /** Is the window a valid window for the draw manager? */
   bool isValidWindow(vrj::opengl::WindowPtr win);


protected:
   /** @name Config Data */
   //@{
   int      numPipes;     /**<  The number of pipes in the system */
   //@}

   /** @name API data */
   //@{
   vrj::opengl::App*                   mApp;   /**< The OpenGL application */
   std::vector<vrj::opengl::WindowPtr> mWins;  /**< A list of the windows */
   std::vector<vrj::opengl::Pipe*>     pipes;  /**< A list of the pipes */
   //@}

   /** @name Helper field data */
   //@{
   vpr::TSObjectProxy<int>                   mContextId; /**< TS Data for context id */
   vpr::TSObjectProxy<vrj::opengl::UserData> mUserData;  /**< User data for draw func */
   //@}

   /** @name MP Stuff */
   //@{
   vpr::Semaphore    drawTriggerSema;  /**< Semaphore for draw trigger */
   vpr::Semaphore    drawDoneSema;     /**< Semaphore for drawing done */

   /**
    * Protects run-time config.  Only when this semaphore is acquired can
    * run-time config occur.
    */
   bool              mRunning;         /**< Used to stop the drawing thread. */

   vpr::Thread* mControlThread;
   vpr::Mutex   mCreateWindowMutex;    /**< Mutex for opening windows across
                                            pipes. */
   //@}

protected:
   virtual ~DrawManager();

private:
   DrawManager();

   cpu_affinity_strategy_t getDrawThreadAffinity;

   vprSingletonHeader(DrawManager);
};

} // end of opengl namespace

} // end of vrj namespace


#endif
