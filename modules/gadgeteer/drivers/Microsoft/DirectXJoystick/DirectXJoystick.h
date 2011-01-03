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

#ifndef _GADGET_DIRECTX_JOYSTICK_H_
#define _GADGET_DIRECTX_JOYSTICK_H_

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <utility>

#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <drivers/Microsoft/DirectXJoystick/DirectXJoystickStandalone.h>


namespace gadget
{

/**
 * Driver for DirectX joysticks.
 *
 * @see Digital, Analog
 */
class DirectXJoystick
   : public input_digital_analog_t
{
public:

   /** Constructor. */
   DirectXJoystick();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released
    */
   virtual ~DirectXJoystick();

   /**
    * Configure the driver.
    *
    * @param e A pointer to a config element.
    *
    * @return true if the device was configured succesfully; false if the
    *         configuration element is invalid.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Begin sampling.
    * Connect to the joystick and prepare to read.
    */
   bool startSampling();

   /**
    * Stops sampling.
    * Drop connection to joystick and clear everything.
    */
   bool stopSampling();

   /** Samples a value. */
   bool sample()
   {
      return true;
   }

   /**
    * Updates to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area.
    */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /** Returns digital data. */
   virtual const DigitalData getDigitalData(int devNum = 0)
   {
      // Make sure we have enough space
      vprASSERT(devNum < (int)mCurButtons.size() && "Digital index out of range");
      return (mCurButtons[devNum]);
   }

   /**
    * Return analog data.
    *
    * @pre give the device number you wish to access.
    * @post returns a value that ranges from 0.0f to 1.0f.
    */
   virtual AnalogData* getAnalogData(int devNum = 0)
   {
      // Make sure we have enough space
      vprASSERT(devNum < (int)mCurAxes.size() && "Analog index out of range");
      return &(mCurAxes[devNum]);
   }

private:
   std::vector<unsigned> mAxisButtonIndices; /**< Indices of the axis buttons */

   std::vector<AnalogData>  mCurAxes;        /**< The current (up-to-date) values. */
   std::vector<DigitalData> mCurButtons;     /**< The current button states. */
   std::vector<int>         mAxisToButtonIndexLookup;   /**< Maps axis number to button index (-1 means none) */

   typedef std::pair<float,float> axis_range_t; /**< Current axis range */
   std::vector<axis_range_t>  mCurAxesRanges;   /**<  Current known ranges on the axes */

   std::string mJsLabel;         /**< The VR Juggler name of the joystick device */

   DirectXJoystickStandalone mInputDrv; /** InputDriver for windows */

   bool mActive;
};

} // End of gadget namespace

#endif
