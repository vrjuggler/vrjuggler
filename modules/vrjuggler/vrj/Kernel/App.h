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

#ifndef _VRJ_APP_H_
#define _VRJ_APP_H_

#include <vrj/vrjConfig.h>

#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>
#include <vrj/Util/Debug.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <vrj/Sound/SoundManagerFactory.h>

#include <gadget/Type/PositionProxy.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

namespace vrj
{

class DrawManager;
class Kernel;

/** Encapsulates the actually application.
 *
 *     This class defines the class that all API specific
 *  application classes should be derived from.  The interface
 *  given is the interface that the Kernel expects in order to
 *  interface with the application.  Most of the application's
 *  interface will be defined in the derived API specific classes.
 *
 *  Users should sub-class the API specific classes to create
 *  user-defined applications.  A user application is required
 *  to provide function definitions for any of the virual functions
 *  that the application needs to use.  This is the method that
 *  the application programmer uses to interface with VR Juggler.
 *
 *  The control loop will look similar to this: <br> <br>
 *  @note One time through the loop is a Juggler Frame <br>
 *
 *  while (drawing)           <br>
 *  {                         <br>
 *       <b>preFrame()</b>;   <br>
 *       draw();              <br>
 *       <b>intraFrame()</b>; <br>
 *       sync();              <br>
 *       <b>postFrame()</b>;  <br>
 *                            <br>
 *       UpdateTrackers();    <br>
 *  }                         <br>
 */
class VJ_CLASS_API App : public jccl::ConfigElementHandler
{
public:
   /**
    * Constructor.
    * @param kern The Kernel that is active (so application has easy access to
    *             the kernel).
    *    
    */
   App(Kernel* kern);

   /** Just call App(Kernel::instance()). */
   App();

   virtual ~App()
   {
      /* Do nothing. */ ;
   }

public:

   /**
    * Application initialization function.
    * Execute any initialization needed before the API is started.
    * @note Derived classes MUST call base class version of this method.
    */
   virtual void init()
   {;}

   /**
    * Application API initialization function.
    * Execute any initialization needed <b>after</b> API is started
    * but before the drawManager starts the drawing loops.
    */
   virtual void apiInit()
   {;}

   /** Executes any final cleanup needed for the application. */
   virtual void exit()
   {;}

   /**
    * Function called before juggler frame starts.
    * Called after tracker update but before start of a new frame.
    */
   virtual void preFrame()
   {;}

   /** Function called <b>during</b> the application's drawing time. */
   virtual void intraFrame()
   {;}

   /**
    * Function called before updating trackers but after the frame is complete.
    */
   virtual void postFrame()
   {;}

   /**
    * Resets the application.
    * This is used when the system (or applications) would like the application
    * to reset to the initial state that it started in.
    */
   virtual void reset() {;}

   /**
    * Does the application currently have focus?
    * If an application has focus, the user may be attempting to interact with
    * it, so the app should process input.  If not, the user is not interating
    * with it, so ignore all input, but the user may still be viewing it, so
    * render and update any animations, etc.
    *
    * This is akin to the way a user can only interact with a GUI window that
    * has focus (i.e., the mouse is over the window).
    */
   bool haveFocus()
   {
      return mHaveFocus;
   }

   /** Called when the focus state changes. */
   virtual void focusChanged()
   {;}

   /**
    * Sets the focus state.
    * @post If state has changed, then calls focusChanged().
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
    * Returns scale scale factor to get from Juggler units (meters) to
    * application units.  Internally VR Juggler stores and processes all
    * position values in meters.  The scale factor returned by this method is
    * used by VR Juggler to scale the OpenGL drawing state from meters to
    * whatever local units the application wants to use.
    *
    * Example: to use feet as local app unit, return 3.28;
    */
   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }


public:  // --- Default config handlers: (inherited from jccl::ConfigElementHandler) --- //
   /** Default to handling nothing. */
   virtual bool configCanHandle(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      return false;
   }

   /**
    * Are any application dependencies satisfied?
    * If the application requires anything special of the system for successful
    * initialization, check it here.  If the return value is false, then the
    * application will not be started yet.  If the return value is true,
    * application will be allowed to enter the system.
    */
   virtual bool depSatisfied()
   {
      return true;
   }

protected:
   /** @note Inherited from jccl::ConfigElementHandler. */
   virtual bool configAdd(jccl::ConfigElementPtr element)
   {
     boost::ignore_unused_variable_warning(element);
     vprASSERT(false);  return false;
   }

   /** @note Inherited from jccl::ConfigElementHandler. */
   virtual bool configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      vprASSERT(false); return false;
   }

public:
   Kernel* mKernel;     // The library kernel (here for convienence)
   bool    mHaveFocus;

public:  // --- Factory functions --- //
   /**
    * Gets the Draw Manager to use.
    * @note Each derived app MUST implement this function.
    */
   virtual DrawManager* getDrawManager() = 0;

   /**
    * Get the SoundManager to use.
    * @note Each derived app could implement this function if needed.
    */
   virtual SoundManager* getSoundManager()
   {
      return &SoundManagerFactory::get();
   }
};

}

#endif
