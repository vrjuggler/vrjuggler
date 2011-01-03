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

#ifndef _GADGET_P5_GLOVE_WRAPPER_H_
#define _GADGET_P5_GLOVE_WRAPPER_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vector>

#include <gadget/Type/Input.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Type/InputBaseTypes.h>
#include <drivers/EssentialReality/P5Glove/P5GloveStandalone.h>


namespace gadget
{

/**
 * Software interface to EssentialReality P5glove hardware.
 */
class P5GloveWrapper
   : public input_glove_digital_analog_position_t
{
public:
   /** Constructor. */
   P5GloveWrapper();

   /** Destroy the glove */
   virtual ~P5GloveWrapper();

   virtual bool config(jccl::ConfigElementPtr e);
   static std::string getElementType();
   virtual bool startSampling();
   virtual bool stopSampling();
   virtual bool sample();
   virtual void updateData();

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

   /** The main control loop for the object. */
   void controlLoop();
   void copyDataFromGlove();

protected:
   P5GloveStandalone* mGlove;           /**< The actual glove */

   int mGloveNumber;

   // Hand left, right;
   std::vector<AnalogData> mAnalogP5;   /**< analogs for each finger */
   std::vector<DigitalData> mDigitalP5; /**< digital for gesture */
   std::vector<PositionData> mPositionP5;

   bool mExitFlag;
};

} // End of gadget namespace


#endif   /* _GADGET_P5_GLOVE_WRAPPER_H_ */
