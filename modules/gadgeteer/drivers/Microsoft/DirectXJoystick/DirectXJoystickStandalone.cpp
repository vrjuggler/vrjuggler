/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <windows.h>
#include <dinput.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

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

BOOL DirectXJoystickStandalone::enumerateJoysticks(const DIDEVICEINSTANCE* dInstance)
{
   // Obtain an interface to the enumerated joystick.
   HRESULT hr = mDxObject->CreateDevice(dInstance->guidInstance, &mDxJoystick, NULL);
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
   // For each axis enumrated, this function will set
   // the minimum and maximum range values for it.
   DIPROPRANGE diprg;
   diprg.diph.dwSize       = sizeof(DIPROPRANGE);
   diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
   diprg.diph.dwHow        = DIPH_BYID;
   diprg.diph.dwObj        = doi->dwType;
   diprg.lMin              = getAxisMin();
   diprg.lMax              = getAxisMax();

   // Set the range for the axis
   if( FAILED(mDxJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)) )
   {
      return DIENUM_STOP;
   }

   return DIENUM_CONTINUE;
}

} // End of gadget namespace
