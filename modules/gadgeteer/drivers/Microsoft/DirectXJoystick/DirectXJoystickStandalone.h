/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_
#define _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_

#include <windows.h>
#include <dinput.h>
#include <string>
#include <vector>


namespace gadget
{

class DirectXJoystickStandalone
{
public:
   DirectXJoystickStandalone();

   void init();

   void close();

   bool poll();

   const DIJOYSTATE& getData() const;

   DWORD getType() const;

   const std::string& getProductName() const;

   /**
    * Returns the number of axes on this joystick.
    *
    * @pre init() has completed successfully.
    */
   DWORD getNumAxes() const
   {
      return mCapabilities.dwAxes + mCapabilities.dwPOVs * 2;
   }

   /**
    * Returns the number of buttons on this joystick.
    *
    * @pre init() has completed successfully.
    */
   DWORD getNumButtons() const
   {
      return mCapabilities.dwButtons;
   }

   /**
    * Returns the firmware revision number for this joystick.
    *
    * @pre init() has completed successfully.
    */
   DWORD getFirmwareRev() const
   {
      return mCapabilities.dwFirmwareRevision;
   }

   /**
    * Returns the hardware revision number for this joystick.
    *
    * @pre init() has completed successfully.
    */
   DWORD getHardwareRev() const
   {
      return mCapabilities.dwHardwareRevision;
   }

   LONG getAxisValue(const unsigned int axisIndex);

   BYTE getButtonValue(const unsigned int buttonIndex) const
   {
      return mJsData.rgbButtons[buttonIndex];
   }

   /** @name Direct Input callbacks */
   //@{
   BOOL enumerateJoysticks(const DIDEVICEINSTANCE* dInstance);
   BOOL enumerateAxes(const DIDEVICEOBJECTINSTANCE* doi);
   //@}

   static int getAxisMin()
   {
      return -1000;
   }

   static int getAxisMax()
   {
      return 1000;
   }

private:
   /** @name Direct Input data */
   //@{
   LPDIRECTINPUT8       mDxObject;     /**< DirectInput object */
   LPDIRECTINPUTDEVICE8 mDxJoystick;   /**< DirectInput device */
   DIDEVCAPS            mCapabilities; /**< Device capabilities */
   //@}

   /** @name Data format information */
   //@{
   // Use DIJOYSTATE for the data format.  We do not need the extended capabilities
   // of DIJOYSTATE2.
   DIJOYSTATE           mJsData;      /**< Joystick state data structure */

   const DIDATAFORMAT* mDataFormatObj;
   size_t              mDataFormatSize;
   //@}

   /**
    * Maps a Gadgeteer zero-based axis index to the memory containing the
    * value for that index.  This vector is filled in enumerateAxies() based
    * on the way that Direct Input iterates over the available axes.
    *
    * @see enumerateAxes
    */
   std::vector<LONG*> mAxisValueMap;

   DWORD mType;
   std::string mProductName;
};

}

#endif
