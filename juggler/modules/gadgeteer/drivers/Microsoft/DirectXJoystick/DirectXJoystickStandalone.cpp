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

#include <drivers/Microsoft/DirectXJoystick/DirectXJoystickStandalone.h>


namespace gadget
{

LPDIRECTINPUT8       mDxObject;      // DirectInput object
LPDIRECTINPUTDEVICE8 mDxJoystick;    // DirectInput device
DIDEVICEINSTANCE mDxDeviceInfo;
DIJOYSTATE  mJsData; 		// joystick state data-structure

// The Callback function must be static
BOOL CALLBACK enumerateJoysticksCallback(const DIDEVICEINSTANCE* dInstance,
                                         void* pContext);
BOOL CALLBACK enumerateAxesCallback(const DIDEVICEOBJECTINSTANCE* doi,
                                    void* pContext);

BOOL CALLBACK enumerateJoysticksCallback(const DIDEVICEINSTANCE* dInstance,
                                         void* pContext)
{
   // Obtain an interface to the enumerated joystick.
   mDxObject->CreateDevice(dInstance->guidInstance, &mDxJoystick, NULL);
   return DIENUM_STOP;
/* // FIXME: allow configurable joystick station
   static int count = 0;
   ++count;
   if( count == 2 )
   {
      mDxObject->CreateDevice(dInstance->guidInstance, &mDxJoystick, NULL);
      return DIENUM_STOP;
   }
   return DIENUM_CONTINUE;
*/
}

BOOL CALLBACK enumerateAxesCallback(const DIDEVICEOBJECTINSTANCE* doi,
                                    void* pContext)
{
   // For each axis enumrated, this function will set
   // the minimum and maximum range values for it.
   DIPROPRANGE diprg;
   diprg.diph.dwSize       = sizeof(DIPROPRANGE);
   diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
   diprg.diph.dwHow        = DIPH_BYOFFSET;
   diprg.diph.dwObj        = doi->dwOfs; // Specify the enumerated axis
   diprg.lMin              = -100;
   diprg.lMax              = +100;

   // Set the range for the axis
   if( FAILED(mDxJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)) )
   {
      return DIENUM_STOP;
   }

   return DIENUM_CONTINUE;
}

DirectXJoystickStandalone::DirectXJoystickStandalone()
{
}

HRESULT DirectXJoystickStandalone::init()
{
   // Create a DInput object.
   HRESULT err = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                    IID_IDirectInput8, (VOID**) &mDxObject,
                                    NULL);

   // some error.
   if(err != DI_OK)
   {
      return err;
   }

   // Look for joystick .
   mDxObject->EnumDevices(DI8DEVCLASS_GAMECTRL, enumerateJoysticksCallback,
                          (LPVOID) NULL, (DWORD) DIEDFL_ATTACHEDONLY);

   mDxJoystick->SetDataFormat(&c_dfDIJoystick);

   // Set the cooperative level to let DInput know how this device should
   // interact with the system and with other DInput applications.
   mDxJoystick->SetCooperativeLevel((HWND) NULL,
                                    DISCL_EXCLUSIVE | DISCL_FOREGROUND);

   // Enumerate the axes of the joystick and set the range of each axis found.
   mDxJoystick->EnumObjects(enumerateAxesCallback, (VOID*) NULL, DIDFT_AXIS);

   mDxJoystick->Acquire();

   // Get device specific information
   mDxJoystick->GetDeviceInfo(&mDxDeviceInfo);
   mType = mDxDeviceInfo.dwDevType;
   mProductName = std::string(mDxDeviceInfo.tszProductName);
   if ( mProductName.size() == 0 )
   {
      mProductName = "unknown";
   }

   return S_OK;
}

DWORD DirectXJoystickStandalone::getType()
{
   return mType;
}

std::string DirectXJoystickStandalone::getProductName()
{
   return mProductName;
}

/*
 DIJOYSTATE Datastructure
   X Axis =  js.lX, Y Axis =  js.lY,  Z Axis =  js.lZ
   X Axis Rotation js.lRx, Y Axis Rotation js.lRy, Z Axis Rotation js.lRz
   // hat-switches or Point-of-View buttons
   POV 1 = js.rgdwPOV[0], POV 2 = js.rgdwPOV[1]
   POV 3 = js.rgdwPOV[2], POV 4 = js.rgdwPOV[3]
   Slider 1 = js.rglSlider[0], Slider 2 = js.rglSlider[1]
   Button 1 = js.rgbButtons[0], Button 2 = js.rgbButtons[1]
   Button 3 = js.rgbButtons[2], Button 4 = js.rgbButtons[3]
   Button 5 = js.rgbButtons[4], Button 6 = js.rgbButtons[5]
   Button 7 = js.rgbButtons[6], Button 8 = js.rgbButtons[7]
*/
HRESULT DirectXJoystickStandalone::poll()
{
   //DIJOYSTATE  js; // DInput joystick state data-structure
   mDxJoystick->Poll();
   // FIXME: error tracking
   mDxJoystick->GetDeviceState(sizeof(DIJOYSTATE), &mJsData);
   return S_OK;
}

/*
DIJOYSTATE:  regular joystick
DIJOYSTATE2:  joystick with extended capabilities
lX: X-axis, usually the left-right movement of a stick.
lY: Y-axis, usually the forward-backward movement of a stick.
lZ: Z-axis, often the throttle control. If the joystick does not have this axis, the value is 0.
lRx: X-axis rotation. If the joystick does not have this axis, the value is 0.
lRy: Y-axis rotation. If the joystick does not have this axis, the value is 0.
lRz: Z-axis rotation (often called the rudder). If the joystick does not have this axis, the value is 0.
rglSlider: Two additional axes, formerly called the u-axis and v-axis, whose semantics depend on the joystick. Use the IDirectInputDevice8::GetObjectInfo method to obtain semantic information about these values.
rgdwPOV: Direction controllers, such as point-of-view hats. The position is indicated in hundredths of a degree clockwise from north (away from the user). The center position is normally reported as 1; but see Remarks.

For indicators that have only five positions,
the value for a controller is 1, 0, 9,000, 18,000, or 27,000.
rgbButtons: Array of buttons. The high-order bit of the byte is set if the
corresponding button is down, and clear if the
button is up or does not exist.
*/

DIJOYSTATE DirectXJoystickStandalone::getData()
{
   return mJsData;
}

void DirectXJoystickStandalone::close()
{
   // Unacquire & release any DirectInputDevice objects.
   if( mDxJoystick != NULL )
   {
      mDxJoystick->Unacquire();
      if ( mDxJoystick )
      {
         mDxJoystick->Release();
         mDxJoystick = NULL;
      }
   }

   // Release any DirectInput objects.
   if(mDxObject)
   {
      mDxObject->Release();
      mDxObject = NULL;
   }
}

} // End of gadget namespace
