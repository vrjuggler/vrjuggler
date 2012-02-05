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

#ifndef _VRJ_APP_H_
#define _VRJ_APP_H_

#include <vrj/vrjConfig.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <gadget/Type/Position/PositionUnitConversion.h>


namespace vrj
{

class DrawManager;
class Kernel;
class SoundManager;

/** \class App App.h vrj/Kernel/App.h
 *
 * Encapsulates the actual application.
 *
 * This defines the base class for all graphics API-specific application
 * object types.  The interface given is what the VR Juggler kernel expects in
 * order to communicate with the application.  Most of the application's
 * interface will be defined in the derived graphics API-specific classes.
 *
 * Users should write their application objectcs as subclasses of the graphics
 * API-specific classes.  Overriding the virtual functions in this class and
 * in the graphics API-specific subclasses is the method by which the
 * application programmer interfaces with VR Juggler.
 *
 * The VR Juggler kernel control loop will look similar to this:
 *
 * \code
 * while (drawing)
 * {
 *    <b>app_obj->preFrame()</b>;
 *    <b>app_obj->latePreFrame()</b>;
 *    draw();
 *    <b>app_obj->intraFrame()</b>;
 *    sync();
 *    <b>app_obj->postFrame()</b>;
 *
 *    updateAllDevices();
 * }
 * \endcode
 *
 * @note One time through the loop is a "Juggler Frame."
 *
 * @see vrj::Kernel
 */
class VJ_API App
   : public jccl::ConfigElementHandler
{
public:
   /**
    * Constructor.
    *
    * @param kern The vrj::Kernel instance that is active (so that the
    *             application has easy access to the kernel).
    *             If NULL, defaults to value from vrj::Kernel::instance()
    */
   App(Kernel* kern=NULL);


   virtual ~App()
   {
      /* Do nothing. */ ;
   }

public:

   /**
    * Application initialization function.
    * Execute any initialization needed before the grahpics API is started.
    *
    * @note Derived classes <b>must</b> call the base class version of this
    *       method.
    */
   virtual void init()
   {;}

   /**
    * Application graphics API initialization function.
    * Execute any initialization needed <b>after</b> the graphics API is
    * started but before the Draw Manager starts the rendering loop(s).
    */
   virtual void apiInit()
   {;}

   /**
    * Executes any final clean-up needed for this application before exiting.
    * This is invoked by the kernel prior to this application object being
    * removed from the kernel.
    */
   virtual void exit()
   {;}

   /**
    * Executes any code needed to set up the application object state prior
    * to rendering the scene.  This is invoked by the kernel once per pass
    * through the Juggler frame loop.  This is called after input device
    * updates but before the start of rendering a new frame of the scene.
    */
   virtual void preFrame()
   {;}

   /**
    * Function called after preFrame() and application-specific data
    * synchronization (in a cluster configuration) but before the start of a
    * new frame.
    *
    * @note This is required because we cannot update data during the
    *       rendering process since it might be using multiple threads.
    */
   virtual void latePreFrame()
   {;}

   /**
    * Function called <b>during</b> this application's drawing time.  This
    * can be used for "free" parallel processing, but it introduces a critical
    * section to the code.  If the rendering process is reading state data,
    * this method should not be modifying any of that data.  Instead, a
    * double- or triple-buffering scheme must be used to allow parallel
    * reading and writing of all application state used for rendering.
    */
   virtual void intraFrame()
   {;}

   /**
    * Function called before updating input devices but after the frame
    * rendering is complete.
    */
   virtual void postFrame()
   {;}

   /**
    * Resets this application.
    * This is used when the kernel (or applications) would like this
    * application object to reset to its initial state.  This is one of the
    * only methods that can be safe for cross-calls from other application
    * object interface methodss.  That is, of course, as long as the override
    * of this method is implemented in a safe manner.  The override should be
    * implemented such that it can be invoked at any time during the Juggler
    * frame loop by the kernel or from a method such as preFrame() or
    * postFrame() by the application object itself.
    */
   virtual void reset()
   {;}

   /**
    * Does this application currently have focus?
    * If an application has focus, the user may be attempting to interact with
    * it, so this application should process input.  If not, the user is not
    * interating with it, so ignore all input.  However, the user may still be
    * viewing it, so render, update any animations, etc.  This is akin to the
    * way a user can only interact with a GUI window that has focus.
    */
   bool haveFocus()
   {
      return mHaveFocus;
   }

   /** Called by the kernel when the focus state changes. */
   virtual void focusChanged()
   {;}

   /**
    * Sets the focus state.
    *
    * @post If the focus state has changed, then focusChanged() is called.
    *
    * @param newState A Boolean value indicating whether this application now
    *                 has focus.
    *
    * @see focusChanged()
    */
   void setFocus(bool newState)
   {
      if(newState != mHaveFocus)
      {
         mHaveFocus = newState;
         this->focusChanged();
      }
   }

   /**
    * Returns the scale factor to convert from Juggler units (meters) to
    * application units.  Internally, VR Juggler stores and processes all
    * position values in meters.  The scale factor returned by this method is
    * used by VR Juggler to scale the rendering state from meters to whatever
    * units this application wants to use.
    *
    * For example, to use feet as this application object's unit, return 3.28.
    * Conversion constants can be found in the namespace
    * \c gadget::PositionUnitConversion which is defined in the header file
    * \c gadget/Type/Position/PositionUnitConversion.h from the Gadgeteer
    * library.
    */
   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }

   /**
    * @name Default config handlers.
    */
   //@{
   /**
    * Defaults to handling nothing.
    *
    * @note Inherited from jccl::ConfigElementHandler.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element);

   /**
    * Are any application dependencies satisfied?
    * If this application requires anything special of the system for
    * successful initialization, check it here.  If the return value is false,
    * then this application will not be started yet.  If the return value is
    * true, then this application will be allowed to enter the system.
    *
    * @note Inherited from jccl::ConfigElementHandler.
    */
   virtual bool depSatisfied()
   {
      return true;
   }
   //@}

protected:
   /** @note Inherited from jccl::ConfigElementHandler. */
   virtual bool configAdd(jccl::ConfigElementPtr element);

   /** @note Inherited from jccl::ConfigElementHandler. */
   virtual bool configRemove(jccl::ConfigElementPtr element);

public:
   Kernel* mKernel;     /**< The Juggler kernel (here for convienence) */
   bool    mHaveFocus;  /**< The current focus state of this app object */

public:
   /** @name Factory functions */
   //@{
   /**
    * Gets the Draw Manager to use.
    * @note Each derived app MUST implement this function.
    */
   virtual DrawManager* getDrawManager() = 0;

   /**
    * Get the SoundManager to use.
    * @note Each derived app could implement this function if needed.
    */
   virtual SoundManager* getSoundManager();
   //@}
};

}

#endif
