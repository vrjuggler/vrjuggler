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

#ifndef _GADGET_LINUX_JOYDEV_H_
#define _GADGET_LINUX_JOYDEV_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/InputMixer.h>
#include <utility>


namespace gadget
{
   class InputManager;
}

extern "C" GADGET_DRIVER_API(void) initDevice(gadget::InputManager* inputMgr);

namespace gadget
{

/**
 * Driver to Linux joystick input.
 *
 * @see Digital, Analog
 */
class LinuxJoydev : public InputMixer<InputMixer<Input,Digital>,Analog>
{
public:

   /** Constructor. */
   LinuxJoydev();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released
    */
   ~LinuxJoydev();

   /**
    * Configure the driver.
    *
    * @param c A pointer to a config element.
    *
    * @return true if the device was configured succesfully; false if the
    *         configuration element is invalid.
    */
   virtual bool config(jccl::ConfigChunkPtr e);

   /** Begin sampling.
   * Connect to the joystick and prepare to read.
   */
   int startSampling();

   /** Stops sampling.
   * Drop connection to joystick and clear everything.
   */
   int stopSampling();

   /** Samples a value. */
   int sample()
   { return 1;}

   /**
    * Updates to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area.
    */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getChunkType();

   /** Returns digital data. */
   virtual const DigitalData getDigitalData(int devNum=0)
   {
      vprASSERT(devNum < (int)mCurButtons.size() && "Digital index out of range");    // Make sure we have enough space
      return (mCurButtons[devNum]);
   }

   /**
    * Return "analog data"..
    *  Gee, that's ambiguous especially on a discrete system such as a digital computer....
    *
    * @pre give the device number you wish to access.
    * @post returns a value that ranges from 0.0f to 1.0f
    *
    */
   virtual AnalogData* getAnalogData(int devNum=0)
   {
      vprASSERT(devNum < (int)mCurAxes.size() && "Analog index out of range");    // Make sure we have enough space
      return &(mCurAxes[devNum]);
   }

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

private:
   std::string                   mPortName;        /**< Name of the port to connect to */
   std::vector<unsigned>         mAxisButtonIndices;  /**< Indices of the axis buttons */

   std::vector<AnalogData>    mCurAxes;               /**< The current (up-to-date) values. */
   std::vector<DigitalData>   mCurButtons;            /**< The current button states. */
   std::vector<int>           mAxisToButtonIndexLookup;   /**< Maps axis number to button index (-1 means none) */

   typedef std::pair<float,float> axis_range_t;       /**< Current axis range */
   std::vector<axis_range_t>  mCurAxesRanges;         /**<  Current known ranges on the axes */

   std::string       mJsLabel;         /**< The VR Juggler name of the joystick device */
   std::string       mPhysicalJsName;  /**< Name of the joystick */
   unsigned          mNumAxes;         /**< Number of axes we have on the joystick */
   unsigned          mNumButtons;      /**< Number of buttons that we have on the joystick */

   int               mJsFD;            /**< File descriptor for the joystick */
};

} // End of gadget namespace


#endif
