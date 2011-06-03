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
 * version 2.1 of the License, or (at your option) any later version.
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

#include <vpr/System.h>
#include <boost/bind.hpp>
#include <memory.h>

#include "VrpnRumbleEffect.h"
VrpnRumbleEffect::VrpnRumbleEffect(gadget::Vrpn *vrpn, gadget::RumbleEffect::RumbleType type)
   : mVrpn(vrpn)
   , mId(-1)
   , mLengthMilliseconds(1000)
   , mDelayMilliseconds(0)
   , mPeriodMilliseconds(100)
   , mThread(NULL)
   , mPlaying(false)
   , mExitFlag(false)
   , mRumbleType(type)
{
}

VrpnRumbleEffect::~VrpnRumbleEffect()
{
   unload();
}

bool VrpnRumbleEffect::load()
{
   return true;
}

bool VrpnRumbleEffect::update()
{
   return true;
}

bool VrpnRumbleEffect::unload()
{
   return true;
}

bool VrpnRumbleEffect::play(int num_repeats)
{
   if (mPlaying == false)
   {
      //Play effect
      mThread = new vpr::Thread(boost::bind(&VrpnRumbleEffect::playThread, this));
      return true;
   }
   else
   {
      // Already playing
      return false;
   }
}

bool VrpnRumbleEffect::stop()
{
   mExitFlag = true;
   return true;
}

int VrpnRumbleEffect::status()
{
   return 1;
}

bool VrpnRumbleEffect::isLoaded()
{
   return true;
}

gadget::RumbleEffect::RumbleType VrpnRumbleEffect::getType()
{
   return mRumbleType;
}

void VrpnRumbleEffect::setLength(int milliseconds)
{
   mLengthMilliseconds = milliseconds;
}
int VrpnRumbleEffect::getLength()
{
   return mLengthMilliseconds;
}

void VrpnRumbleEffect::setDelay(int milliseconds)
{
    mDelayMilliseconds = milliseconds;
}

int VrpnRumbleEffect::getDelay()
{
   return mDelayMilliseconds;
}

void VrpnRumbleEffect::setPeriod(int milliseconds)
{
   mPeriodMilliseconds = milliseconds;
}

int VrpnRumbleEffect::getPeriod()
{
   return mPeriodMilliseconds;
}

void VrpnRumbleEffect::playThread()
{
    vpr::Interval start, now, interval;
    bool rumbling = false;

    interval.set(mLengthMilliseconds, vpr::Interval::Msec);

    // Delay effect for mDelayMilliseconds
    vpr::System::msleep(mDelayMilliseconds);

    switch(mRumbleType)
    {
    case RumbleEffect::CONSTANT:

       // Start rumble
       mVrpn->startRumble();
       start.setNow();
       now.setNow();
       mPlaying = true;
       mExitFlag = false;

       while ((now.msec() - start.msec()) < interval.msec())
       {
          // sleep
          vpr::System::msleep(20);
          now.setNow();
       }

       // Stop Rumble
       mVrpn->stopRumble();
       mPlaying = false;
       break;
    case RumbleEffect::SQUARE:
       // Square rumble
       // Start rumble
       mVrpn->startRumble();
       start.setNow();
       now.setNow();
       mPlaying = true;
       mExitFlag = false;
       rumbling = true;

       vpr::System::msleep(mPeriodMilliseconds);

       while ((now.msec() - start.msec()) < interval.msec())
       {
          if (rumbling)
          {
             mVrpn->stopRumble();
             rumbling = false;
          }
          else
          {
             mVrpn->startRumble();
             rumbling = true;
          }

          // sleep
          vpr::System::msleep(mPeriodMilliseconds);
          now.setNow();
       }

       // Stop Rumble
       mVrpn->stopRumble();
       mPlaying = false;
       break;
    default:
       break;
    }

}
