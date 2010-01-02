/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _SIMPLE_INPUT
#define _SIMPLE_INPUT

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>

#include <gmtl/Matrix.h>
#include <gmtl/Output.h>
#include <vrj/Draw/OpenGL/App.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

using namespace gmtl;
using namespace vrj;

/**
 * Simple Input Demonstration application.
 *
 * This application demonstrates getting and printing input
 *
 * @note It is derived from opengl::App only because App is an abstract base
 *       class.
 */
class simpleInput : public opengl::App
{
public:
   simpleInput()
   {;}

   virtual ~simpleInput (void) {;}

public: // ---- INITIALIZATION FUNCITONS ---- //
   /**
    * Execute any initialization needed before the API is started.
    *
    * @post Device interfaces are initialized with the device names
    *       we want to use.
    * @note This is called once before OpenGL is initialized.
    */
   virtual void init()
   {
      std::cout << "---------- App:init() ---------------" << std::endl;
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
      mButton0.init("VJButton0");
      mButton1.init("VJButton1");
   }

public:

   /**
    * Called before start of frame.
    *
    * @note Function called after device updates but before start of drawing.
    */
   virtual void preFrame()
   {
      if(mButton0->getData())
         { std::cout << "Button 0 pressed" << std::endl; }
      if(mButton1->getData())
         { std::cout << "Button 1 pressed" << std::endl; }

      std::cout  << "Wand Buttons:"
                 << " 0:" << mButton0->getData()
                 << " 1:" << mButton1->getData()
                 << std::endl;

      // -- Get Wand matrix --- //
      Matrix44f wand_matrix;
      wand_matrix = mWand->getData();
      std::cout << "Wand pos: \n" << wand_matrix << std::endl;
   }

public:
   virtual void draw() {;}

public:
   gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
   gadget::DigitalInterface   mButton0; /**< Digital interface for button 0 */
   gadget::DigitalInterface   mButton1; /**< Digital interface for button 1 */
};


#endif
