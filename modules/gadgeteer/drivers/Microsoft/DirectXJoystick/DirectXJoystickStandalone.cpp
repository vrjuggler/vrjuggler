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

#include <windows.h>

#ifdef _DEBUG
#  include <iostream>
#endif

#include <dinput.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <drivers/Microsoft/DirectXJoystick/DirectXJoystickExceptions.h>
#include <drivers/Microsoft/DirectXJoystick/DirectXJoystickStandalone.h>


namespace gadget
{

BOOL CALLBACK enumerateJoysticksCallback(const DIDEVICEINSTANCE* dInstance,
                                         void* pContext)
{
   DirectXJoystickStandalone* obj =
      static_cast<DirectXJoystickStandalone*>(pContext);
   return obj->enumerateJoysticks(dInstance);
}

BOOL CALLBACK enumerateAxesCallback(const DIDEVICEOBJECTINSTANCE* doi,
                                    void* pContext)
{
   DirectXJoystickStandalone* obj =
      static_cast<DirectXJoystickStandalone*>(pContext);
   return obj->enumerateAxes(doi);
}

DirectXJoystickStandalone::DirectXJoystickStandalone()
   : mDxObject(NULL)
   , mDxJoystick(NULL)
   , mDataFormatObj(&c_dfDIJoystick)
   , mDataFormatSize(sizeof(mJsData))
{
}

void DirectXJoystickStandalone::init()
{
   HRESULT status;

   // Create a Direct Input manager object.
   status = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                               IID_IDirectInput8, (VOID**) &mDxObject, NULL);

   // An error returned at this point indicates that Direct Input is not
   // available.  That means that we are effectively done with this device.
   if ( FAILED(status) )
   {
      throw DirectXJoystickException("Direct Input is not available");
   }

   // Look for joystick.
   mDxObject->EnumDevices(DI8DEVCLASS_GAMECTRL, enumerateJoysticksCallback,
                          (void*) this, (DWORD) DIEDFL_ATTACHEDONLY);

   status = mDxJoystick->SetDataFormat(mDataFormatObj);
   if ( FAILED(status) )
   {
      if ( status ==  DIERR_ACQUIRED )
      {
         throw DirectXJoystickDataFormatException("Cannot set data format while device is acquired");
      }
      else if ( status == DIERR_INVALIDPARAM )
      {
         throw DirectXJoystickDataFormatException("Invalid data format chosen");
      }
      else if ( status == DIERR_NOTINITIALIZED )
      {
         throw DirectXJoystickDataFormatException("Cannot set data format on uninitialized device");
      }
      else
      {
         throw DirectXJoystickDataFormatException("Unknown error when trying to set the data format");
      }
   }

   // Set the cooperative level to let DInput know how this device should
   // interact with the system and with other DInput applications.
   // NOTE: This always fails because the first parameter is NULL!
   status =
      mDxJoystick->SetCooperativeLevel((HWND) NULL,
                                       DISCL_EXCLUSIVE | DISCL_FOREGROUND);

/*
   if ( FAILED(status) )
   {
      if ( status == DIERR_INVALIDPARAM )
      {
         throw DirectXJoystickCooperationException("Invalid cooperative mode chosen");
      }
      else if ( status == DIERR_NOTINITIALIZED )
      {
         throw DirectXJoystickCooperationException("Cannot set cooperative mode on uninitialized device");
      }
      else if ( status == E_HANDLE )
      {
         throw DirectXJoystickCooperationException("Invalid HWND used when setting cooperative mode");
      }
      else
      {
         throw DirectXJoystickCooperationException("Unknown error when trying to set the cooperative mode");
      }
   }
*/

   mCapabilities.dwSize = sizeof(DIDEVCAPS);
   status = mDxJoystick->GetCapabilities(&mCapabilities);

   if ( FAILED(status) )
   {
      if ( status == DIERR_INVALIDPARAM || status == E_POINTER )
      {
         throw DirectXJoystickQueryException("Invalid data passed to IDirectInputDevice8::GetCapabilities()");
      }
      else if ( status == DIERR_NOTINITIALIZED )
      {
         throw DirectXJoystickQueryException("Cannot query capabilities for an uninitialized device");
      }
      else
      {
         throw DirectXJoystickQueryException("Unknown error when trying to query the joystick capabilities");
      }
   }

   mAxisValueMap.resize(mCapabilities.dwAxes, NULL);

   // Enumerate the axes of the joystick and set the range of each axis found.
   mDxJoystick->EnumObjects(enumerateAxesCallback, (void*) this, DIDFT_AXIS);

   status = mDxJoystick->Acquire();

   // DI_OK indicates that we acquired the device successfully.
   // S_FALSE indicates that we had already acquired the device.
   if ( status != DI_OK && status != S_FALSE )
   {
      if ( status == DIERR_INVALIDPARAM )
      {
         throw DirectXJoystickDataFormatException("Invalid parameter passed during device acquisition");
      }
      else if ( status == DIERR_NOTINITIALIZED )
      {
         throw DirectXJoystickDataFormatException("Cannot acquire an uninitialized device");
      }
      else if ( status == DIERR_OTHERAPPHASPRIO )
      {
         throw DirectXJoystickDataFormatException("A higher priority application has already acquired the device");
      }
      else
      {
         throw DirectXJoystickDataFormatException("Unknown error when trying to acquire the device");
      }
   }

   // Get device-specific information.
   DIDEVICEINSTANCE dx_dev_info;
   dx_dev_info.dwSize = sizeof(DIDEVICEINSTANCE);
   mDxJoystick->GetDeviceInfo(&dx_dev_info);
   mType = dx_dev_info.dwDevType;
   mProductName = std::string(dx_dev_info.tszProductName);
   if ( mProductName.empty() )
   {
      mProductName = "unknown";
   }
}

void DirectXJoystickStandalone::close()
{
   // Unacquire & release any DirectInputDevice objects.
   if ( mDxJoystick != NULL )
   {
      mDxJoystick->Unacquire();
      mDxJoystick->Release();
      mDxJoystick = NULL;
   }

   // Release any DirectInput objects.
   if ( mDxObject != NULL )
   {
      mDxObject->Release();
      mDxObject = NULL;
   }
}

bool DirectXJoystickStandalone::poll()
{
   bool sample_taken(false);

   HRESULT status = mDxJoystick->Poll();

   // If we failed to poll the joystick, try to reacquire it.  When we cannot
   // poll the device, we cannot take a sample.
   if ( FAILED(status) )
   {
      status = mDxJoystick->Acquire();
      // XXX: It might be necessary to put a limit on the number of times we
      // try to reacquire the device as this process could hold up the Input
      // Manager.
      while ( status == DIERR_INPUTLOST )
      {
         status = mDxJoystick->Acquire();
      }
   }
   // We polled the joystick successfully, so we can read a sample.
   else
   {
      status = mDxJoystick->GetDeviceState(mDataFormatSize, &mJsData);
      sample_taken = true;
   }

   return sample_taken;
}

const DIJOYSTATE& DirectXJoystickStandalone::getData() const
{
   return mJsData;
}

DWORD DirectXJoystickStandalone::getType() const
{
   return mType;
}

const std::string& DirectXJoystickStandalone::getProductName() const
{
   return mProductName;
}

LONG DirectXJoystickStandalone::getAxisValue(const unsigned int axisIndex)
{
   if ( axisIndex < mCapabilities.dwAxes )
   {
      return *(mAxisValueMap[axisIndex]);
   }
   else
   {
      // Map the given axis index to the range [0,3] so that we can use
      // the result as the index into mJsData.rgdwPOV.
      const unsigned int pov_index((axisIndex - mCapabilities.dwAxes) / 2);
      const DWORD pov_value(mJsData.rgdwPOV[pov_index]);

      // The hat is centered.
      if ( (LOWORD(pov_value) == 0xffff) )
      {
         return (getAxisMax() + getAxisMin()) / 2;
      }
      else
      {
         const float pi = 3.14159265358979323846f;
         const float pov_rads((pov_value / 100.0) * pi / 180.0f);

         // We treat an even-numbered axis index as an X axis request.
         // An odd-numbered axis index then a Y axis request.
         const bool x_axis((axisIndex % 2) == 0);

         return (x_axis ? getAxisMax() * sinf(pov_rads)
                        : getAxisMax() * cosf(pov_rads));
      }
   }
}

BOOL DirectXJoystickStandalone::enumerateJoysticks(const DIDEVICEINSTANCE* dInstance)
{
   // Obtain an interface to the enumerated joystick.
   HRESULT hr = mDxObject->CreateDevice(dInstance->guidInstance, &mDxJoystick,
                                        NULL);
   return (FAILED(hr) ? DIENUM_CONTINUE : DIENUM_STOP);
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

BOOL DirectXJoystickStandalone::enumerateAxes(const DIDEVICEOBJECTINSTANCE* doi)
{
   // The axes appear to be enumerated in reverse order by Direct Input.
   // XXX: Can we depend on this?  Using DIDFT_GETINSTANCE(doi->dwType) gives
   // back values that I would not expect in certain cases.  In other words,
   // the object instance number does not correlate with the memory ordering
   // in DIJOYSTATE.
   static int axis_index(mCapabilities.dwAxes - 1);
   assert(axis_index >= 0 && "axis_index went negative!");

   // For each axis enumrated, this function will set
   // the minimum and maximum range values for it.
   if ( doi->dwType & DIDFT_AXIS )
   {
      DIPROPRANGE diprg;
      diprg.diph.dwSize       = sizeof(DIPROPRANGE);
      diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      diprg.diph.dwHow        = DIPH_BYID;
      diprg.diph.dwObj        = doi->dwType;
      diprg.lMin              = getAxisMin();
      diprg.lMax              = getAxisMax();

      // Set the range for the current axis.
      if( FAILED(mDxJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)) )
      {
         return DIENUM_STOP;
      }

      LONG* value_addr(NULL);
      if ( doi->guidType == GUID_XAxis )
      {
         value_addr = &mJsData.lX;
#ifdef _DEBUG
         std::cout << "Object for X axis" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_YAxis )
      {
         value_addr = &mJsData.lY;
#ifdef _DEBUG
         std::cout << "Object for Y axis" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_ZAxis )
      {
         value_addr = &mJsData.lZ;
#ifdef _DEBUG
         std::cout << "Object for Z axis" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_RxAxis )
      {
         value_addr = &mJsData.lRx;
#ifdef _DEBUG
         std::cout << "Object for X axis rotation" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_RyAxis )
      {
         value_addr = &mJsData.lRy;
#ifdef _DEBUG
         std::cout << "Object for Y axis rotation" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_RzAxis )
      {
         value_addr = &mJsData.lRz;
#ifdef _DEBUG
         std::cout << "Object for Z axis rotation" << std::endl;
#endif
      }
      else if ( doi->guidType == GUID_Slider )
      {
         // XXX: I do not know the correct way to distinguish between the two
         // possible sliders.  -PH 2/2/2005
         if ( DIDFT_GETINSTANCE(doi->dwType) == 2 )
         {
            value_addr = &mJsData.rglSlider[0];
         }
         else
         {
            value_addr = &mJsData.rglSlider[1];
         }
#ifdef _DEBUG
         std::cout << "Object for slider" << std::endl;
#endif
      }

#ifdef _DEBUG
      std::cout << "Setting mAxisValueMap[" << axis_index << "]" << std::endl;
#endif
      mAxisValueMap[axis_index] = value_addr;
      axis_index--;
   }

   return DIENUM_CONTINUE;
}

} // End of gadget namespace
