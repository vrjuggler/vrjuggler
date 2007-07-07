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

#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <gmtl/Matrix.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/OGL/GlApp.h>


using namespace gmtl;
using namespace jccl;
using namespace gadget;
using namespace vrj;

class ConfigApp : public GlApp
{
public:
   /**
    * Initializes the surface color to a white, gray, and black gradient.
    * This will be the default surface color unless we are told otherwise.
    */
   ConfigApp(vrj::Kernel* kern = NULL);

   /**
    * Define a virtual destructor for good measure.
    */
   virtual ~ConfigApp()
   {
      /* Do nothing. */ ;
   }

   /**
    * Determines if the given configuration element is one this application
    * object can use.  This is deterined based on the type of the element.  We
    * are only interested in those elements of type ConfigApp.
    *
    * @param element The configuration element whose type we will test.
    *
    * @return If the element is of type ConfigApp, true is returned.
    *         Otherwise, false is returned.
    */
   virtual bool configCanHandle(ConfigElementPtr element);

   /**
    * Handles addition of a new configuration element that we can use.
    *
    * @pre The given element is of the type ConfigApp.
    * @post The element will be used to change the state of this application
    *       object..
    *
    * @param element The newly added configuration element.
    *
    * @return true is returned if we used the given element successfully.
    */
   virtual bool configAdd(ConfigElementPtr element);

   /** Initializes the device interfaces used by this application object. */
   virtual void init();

   /** Updates navigation. */
   virtual void preFrame();

   /** Clears the current viewport. */
   virtual void bufferPreDraw();

   /**
    * Renders the scene based on the current application state.  This takes
    * into account configurable parameters such as the colors used for the
    * "ground".
    */
   virtual void draw();

private:
   /**
    * Configures the light(s) used in the scene.  This includes the
    * "flashlight" effect that may be enabled through the use of a wand button.
    * The flashlight button state is tested to determine whether to use the
    * flashlight or the overhead light.
    */
   void initLights();

   /** @name Parameters used for rendering the "ground" in the scene. */
   //@{
   GLfloat mSurfaceWidth;   /**< The width of the ground (x parameter) */
   GLfloat mSurfaceLength;  /**< The length of the ground (z parameter) */
   //@}

   /** @name Color settings for the simple surface drawn for the ground. */
   //@{
   GLfloat mSurfaceColor_ll[3];   /**< Surface corner for lower left corner */
   GLfloat mSurfaceColor_lr[3];   /**< Surface corner for lower right corner */
   GLfloat mSurfaceColor_ur[3];   /**< Surface corner for upper right corner */
   GLfloat mSurfaceColor_ul[3];   /**< Surface corner for upper left corner */
   //@}

   GLfloat mDefaultLightPos[4];

   /** @name Gadgeteer device interface. */
   //@{
   PositionInterface mWand;       /**< Position interface for the wand */
   DigitalInterface  mButtons[3]; /**< Digital interfaces for three wand buttons */
   //@}

   /** @name Indices into mButtons array. */
   //@{
   int mStartButton;      /**< Button used to start navigation */
   int mStopButton;       /**< Button used to stop navigation */
   int mFlashlightButton; /**< Button used to enable wand "flashlight" */
   //@}

   /** @name Navigation bits. */
   //@{
   void updateNavigation();

   bool      mNavigating;
   float     mUserVelocity;
   Matrix44f mNavMatrix;
   //@}
};


#endif /* _CONFIG_APP_H_ */
