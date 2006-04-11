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
#include <vpr/Util/Assert.h>
#include <vrj/Util/Debug.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
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
 *  NOTE: One time through the loop is a Juggler Frame <br>
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
class VJ_CLASS_API App : public jccl::ConfigChunkHandler
{
public:
   /** Constructor
    * @param kern - The Kernel that is active.
    *    So application has easy access to kernel
    */
   App(Kernel* kern);

   /** Just call App(Kernel::instance()) */
   App();

   virtual ~App()
   {
      /* Do nothing. */ ;
   }

public:
   /** Application init function
    * Execute any initialization needed before the API is started
    * @note: Derived classes MUST call base class version of this method
    */
   virtual void init()
   {;}

   /** Application API init function
    * Execute any initialization needed <b>after</b> API is started
    *  but before the drawManager starts the drawing loops.
    */
   virtual void apiInit()
   {;}

   /** Execute any final cleanup needed for the application
    */
   virtual void exit()
   {;}

   /** Function called before juggler frame starts.
    * Called after tracker update but before start of a new frame
    */
   virtual void preFrame()
   {;}

   /** Function called <b>during</b> the application's drawing time
    */
   virtual void intraFrame()
   {;}

   /** Function called before updating trackers but after the frame is complete
    */
   virtual void postFrame()
   {;}


   /** Reset the application
    * This is used when the system (or applications) would like the application
    * to reset to the initial state that it started in.
    */
   virtual void reset() {;}

   /** Does the application currently have focus
    * If an application has focus:
    * - The user may be attempting to interact with it, so the app should process input
    * If not,
    * - The user is not interating with it, so ignore all input
    * - BUT, the user may still be viewing it, so render and update any animations, etc.
    *
    * This is akin to the way a user can only interact with a GUI window that has focus
    * (ie.The mouse is over the window)
    */
   bool haveFocus() { return mHaveFocus;}

   /** Called when the focus state changes
    */
   virtual void focusChanged()
   {;}

   /** Sets the focus state
    * POST: If state has changed, then calls focusChanged
    */
   void setFocus(bool newState)
   {
      if(newState != mHaveFocus)
      {
         mHaveFocus = newState;
         this->focusChanged();
      }
   }

   /** Return scale scale factor to get from Juggler units (meters) to application units.
   * Internally VR Juggler stores and processes all position values
   * in meters.  The scale factor returned by this method is
   * used by VR Juggler to scale the OpenGL drawing state from meters to
   * whatever local units the application wants to use
   *
   * Example: to use feet as local app unit, return 3.28;
   */
   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }


public:  // --- DEfault config handlers: (inherited from jccl::ConfigChunkHandler) --- //
   // Default to not handling anything
   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk)
   { return false; }

   /** Are any application dependencies satisfied
    * If the application requires anything special of the system for successful
    * initialization, check it here.
    * If retval == false, then the application will not be started yet
    *    retval == true, application will be allowed to enter the system
    */
   virtual bool depSatisfied()
   { return true; }

protected:
   //! NOTE: Inherited from jccl::ConfigChunkHandler
   virtual bool configAdd(jccl::ConfigChunkPtr chunk)
   { vprASSERT(false);  return false; }
   //! NOTE: INherited from jccl::ConfigChunkHandler
   virtual bool configRemove(jccl::ConfigChunkPtr chunk)
   { vprASSERT(false); return false; }

public:
   //vjAPI       api;        // Used to signal which API this application works with
   Kernel*     mKernel;     // The library kernel (here for convienence)
   bool        mHaveFocus;

public:  // --- Factory functions --- //
   /** Get the DrawManager to use
    * @NOTE Each derived app MUST implement this function
    */
   virtual DrawManager*    getDrawManager() = 0;

   /** Get the SoundManager to use
    * @NOTE Each derived app could implement this function if needed
    */
   virtual SoundManager*    getSoundManager()
   {
      return &SoundManagerFactory::get();
   }
};

};
#endif
