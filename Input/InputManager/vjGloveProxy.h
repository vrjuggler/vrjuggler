/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


/////////////////////////////////////////////////////////////////////////
//
// vjGlove proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_GLOVE_PROXY_H_
#define _VJ_GLOVE_PROXY_H_

#include <vjConfig.h>
#include <math.h>
#include <assert.h>

#include <Input/vjGlove/vjGlove.h>
#include <Input/InputManager/vjProxy.h>


//: vjGlove proxy class.
//!PUBLIC_API:
class vjGloveProxy : public vjProxy
{
public:
     //: Construct the proxy to point to the given glove device and sub-unit number.
  vjGloveProxy()
  {
     //Set(glovePtr,unitNum);
     mGlovePtr = NULL;
     mUnitNum = -1;
     mVisible = true;
  }

  virtual ~vjGloveProxy()
  {}

  //: Set the gloveProxy to point to another device and subUnit number.
  void set(vjGlove* glovePtr, int unitNum)
  {
     vjASSERT( glovePtr->fDeviceSupport(DEVICE_GLOVE) );
     mGlovePtr = glovePtr;
     mUnitNum = unitNum;

     vjDEBUG(vjDBG_INPUT_MGR, vjDBG_VERB_LVL) << "glovePtr: " << glovePtr << std::endl
              << "unitNum: " << unitNum << std::endl << vjDEBUG_FLUSH;
  }

  float getAngle(vjGloveData::vjGloveComponent component,
                 vjGloveData::vjGloveJoint joint)
  {
    return mGlovePtr->getGloveAngle(component, joint, mUnitNum);
  }


  vjVec3 getVector(vjGloveData::vjGloveComponent component)
  {
     return mGlovePtr->getGloveVector(component, mUnitNum);
  }

  vjMatrix getPos( vjGloveData::vjGloveComponent component = vjGloveData::WRIST)
  {
     return mGlovePtr->getGlovePos(component, mUnitNum);
  }


  vjGloveData getData()
  {
     return mGlovePtr->getGloveData(mUnitNum);
  }


  //: Returns a pointer to the device held by this proxy.
  vjGlove* getGlovePtr()
  { return mGlovePtr; }


  //: Returns the subUnit number that this proxy points to.
  int getUnit()
  { return mUnitNum; }

   bool isVisible()
   { return mVisible; }

   static std::string getChunkType() { return "GloveProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(mGlovePtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }


private:
   //: Pointer to the glove device we are proxying.
   vjGlove* mGlovePtr;

   //: Should we be drawn on the screen
   bool  mVisible;

   //: The subUnit number to use in the device.
   int mUnitNum;
};

#endif
