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

#include <Input/vjGlove/vjGlove.h>
#include <Math/vjVec4.h>


/////////////////////////////////////////////////////////////////////////
//                vjGloveData                                          //
/////////////////////////////////////////////////////////////////////////

/*
 * Implementation of the vjGlove
 * abstract device class
 *
 * This class is the base class for all gloves.
 * We will implment all the necessary data operations
 * on the glove data. (Get finger pos, get vectors, get matrices, etc.)
 */

vjGloveData::vjGloveData()
{
   // Zero out the angles
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         angles[i][j] = 0.0f;

   // Matrices are already identities
}

//: Calulate all the xform matrices
// This is calculated based upon the angles in the data structure
// This code will be very complex, so I will HACK it for now.
// XXX: This hack should be fixed soon though
//
// I am just rotating around single axis for fingers, not taking abduct into account
// The thumb is a complete fudge.
// Wrist is not being done at all
int vjGloveData::calcXforms()
{
   vjVec3 xAxis(1.0f, 0.0f, 0.0f);
   vjVec3 yAxis(0.0f, 1.0f, 0.0f);
   const float oneIn(1/12.0f);


   dims[INDEX][DIJ+1] = yAxis * (oneIn * 0.5f);                      // Length distal
   dims[INDEX][DIJ] = yAxis * (oneIn * 1.0f);                        // Length medial
   dims[INDEX][PIJ] = yAxis * (oneIn * 1.3f);                        // Length Proximal
   dims[INDEX][MPJ] = (yAxis * (oneIn * 1.7f)) + (oneIn* xAxis * -0.4f);   // To finger

   dims[MIDDLE][DIJ+1] = yAxis * (oneIn * 0.5f);
   dims[MIDDLE][DIJ] = yAxis * (oneIn * 1.1f);
   dims[MIDDLE][PIJ] = yAxis * (oneIn * 1.4f);
   dims[MIDDLE][MPJ] = (yAxis * (oneIn * 1.8f)) + (oneIn* xAxis * 0.0f);

   dims[RING][DIJ+1] = yAxis * (oneIn * 0.4f);
   dims[RING][DIJ] = yAxis * (oneIn * 1.0f);
   dims[RING][PIJ] = yAxis * (oneIn * 1.1f);
   dims[RING][MPJ] = (yAxis * (oneIn * 1.7f)) + (oneIn* xAxis * 0.4f);

   dims[PINKY][DIJ+1] = yAxis * (oneIn * 0.3f);
   dims[PINKY][DIJ] = yAxis * (oneIn * 1.0f);
   dims[PINKY][PIJ] = yAxis * (oneIn * 0.85f);
   dims[PINKY][MPJ] = (yAxis * (oneIn * 1.6f)) + (oneIn* xAxis * 0.7f);


   // ----------------------- //
   // ----- XFORMS ---------- //
   // ----------------------- //

   // THUMB
   // INDEX
   xforms[INDEX][DIJ].makeRot(angles[INDEX][DIJ],xAxis);
   xforms[INDEX][DIJ].preTrans(dims[INDEX][DIJ],xforms[INDEX][DIJ]);
   xforms[INDEX][PIJ].makeRot(angles[INDEX][PIJ],xAxis);
   xforms[INDEX][PIJ].preTrans(dims[INDEX][PIJ],xforms[INDEX][PIJ]);
   xforms[INDEX][MPJ].makeRot(angles[INDEX][MPJ],xAxis);
   xforms[INDEX][MPJ].preTrans(dims[INDEX][MPJ],xforms[INDEX][MPJ]);

   // MIDDLE
   xforms[MIDDLE][DIJ].makeRot(angles[MIDDLE][DIJ],xAxis);
   xforms[MIDDLE][DIJ].preTrans(dims[MIDDLE][DIJ],xforms[MIDDLE][DIJ]);
   xforms[MIDDLE][PIJ].makeRot(angles[MIDDLE][PIJ],xAxis);
   xforms[MIDDLE][PIJ].preTrans(dims[MIDDLE][PIJ],xforms[MIDDLE][PIJ]);
   xforms[MIDDLE][MPJ].makeRot(angles[MIDDLE][MPJ],xAxis);
   xforms[MIDDLE][MPJ].preTrans(dims[MIDDLE][MPJ],xforms[MIDDLE][MPJ]);

   // RING
   xforms[RING][DIJ].makeRot(angles[RING][DIJ],xAxis);
   xforms[RING][DIJ].preTrans(dims[RING][DIJ],xforms[RING][DIJ]);
   xforms[RING][PIJ].makeRot(angles[RING][PIJ],xAxis);
   xforms[RING][PIJ].preTrans(dims[RING][PIJ],xforms[RING][PIJ]);
   xforms[RING][MPJ].makeRot(angles[RING][MPJ],xAxis);
   xforms[RING][MPJ].preTrans(dims[RING][MPJ],xforms[RING][MPJ]);

   // PINKY
   xforms[PINKY][DIJ].makeRot(angles[PINKY][DIJ],xAxis);
   xforms[PINKY][DIJ].preTrans(dims[PINKY][DIJ],xforms[PINKY][DIJ]);
   xforms[PINKY][PIJ].makeRot(angles[PINKY][PIJ],xAxis);
   xforms[PINKY][PIJ].preTrans(dims[PINKY][PIJ],xforms[PINKY][PIJ]);
   xforms[PINKY][MPJ].makeRot(angles[PINKY][MPJ],xAxis);
   xforms[PINKY][MPJ].preTrans(dims[PINKY][MPJ],xforms[PINKY][MPJ]);

   // WRIST

   return 1;
}

// Output the angles in one single line
std::ostream& vjGloveData::outputAngles(std::ostream& out) const
{
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         out << angles[i][j] << " ";

   return out;
}

// Input the angles from one single line
std::istream& vjGloveData::inputAngles(std::istream& in)
{
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         in >> angles[i][j];

   return in;
}

////////////////////////////////////////////////////////////////////////
//                          vjGlove                                   //
////////////////////////////////////////////////////////////////////////
vjGlove::vjGlove()
{
   //vjDEBUG(vjDBG_INPUT_MGR,3)<<"*** vjGlove::vjGlove()\n"<< vjDEBUG_FLUSH;
   deviceAbilities = deviceAbilities | DEVICE_GLOVE;
   for(int i=0;i<VJ_MAX_GLOVE_DEVS;i++)
      mGlovePos[i] = NULL;
}


// Just get the data from the current vector entry
float vjGlove::getGloveAngle(vjGloveData::vjGloveComponent component,
                             vjGloveData::vjGloveJoint joint, int devNum)
{
   return mTheData[devNum][current].angles[component][joint];
}

//: This returns a vector ponting "out" of the component
// Can be used for selection, etc.
// Use getGlovePos to get the transformation matrix
vjVec3 vjGlove::getGloveVector(vjGloveData::vjGloveComponent component, int devNum)
{
   // Take a normalized ray up default (yAxis), and transform by finger tip rot matrix
   // ret_val = wTt yAxis
   vjVec3 y_axis(0.0f, 1.0f, 0.0f);
   vjVec3 ret_val(0.0f, 0.0f, 0.0f);

   ret_val.xformVec(getGlovePos(component, devNum), y_axis);      // Compute the vector direction
   return ret_val;
}

// Calculated from the matrices in xforms
// <sub>world</sub><b>T</b><sub>tip</sub> = <sub>world</sub><b>T</b><sub>base</sub> <sub>base</sub><b>T</b><sub>dij</sub> <sub>dij</sub><b>T</b><sub>tip</sub>
//  i.e. wTt = wTb bTd dTt
vjMatrix vjGlove::getGlovePos(vjGloveData::vjGloveComponent component, int devNum)
{
   if(mGlovePos[devNum] != NULL)
   {
      vjMatrix ret_val;       // The returned matrix.
      vjMatrix baseTdij;      // Transform from base to dig coord system
      vjMatrix dijTtip;       // Transform to the tip of the finger

      switch(component)
      {
      case vjGloveData::WRIST:
         baseTdij.makeIdent();      // No transform
         dijTtip.makeIdent();       // No transform
         break;
      case vjGloveData::INDEX:
      case vjGloveData::MIDDLE:
      case vjGloveData::RING:
      case vjGloveData::PINKY:
      case vjGloveData::THUMB:
         dijTtip.makeTrans(mTheData[devNum][current].dims[component][vjGloveData::DIJ+1][VJ_X],
                           mTheData[devNum][current].dims[component][vjGloveData::DIJ+1][VJ_Y],
                           mTheData[devNum][current].dims[component][vjGloveData::DIJ+1][VJ_Z]);
         baseTdij = mTheData[devNum][current].xforms[component][vjGloveData::MPJ];            // baseTmpj
         baseTdij.postMult(mTheData[devNum][current].xforms[component][vjGloveData::PIJ]);    // mpjTpij
         baseTdij.postMult(mTheData[devNum][current].xforms[component][vjGloveData::DIJ]);    // pijTdij
         break;

      default:
         vjASSERT(false);
         break;
      }

      // Compute return value: retVal = TIPw = wTb bTd dTt
      ret_val = *(mGlovePos[devNum]->getData());      // wTb
      ret_val.postMult(baseTdij);                     // bTd
      ret_val.postMult(dijTtip);                      // dTt

      return ret_val;
   }
   else
   {
      vjDEBUG( vjDBG_INPUT_MGR,0) << clrOutNORM(clrRED, "ERROR:") << " vjGlove: Trying to get a glove without a position proxy set for device number: "<<devNum<<".\n" << vjDEBUG_FLUSH;
      vjASSERT( mGlovePos[devNum] != NULL );      // should be false in here
      return vjMatrix();
   }
}

// Grab a copy of the most current glove data
vjGloveData vjGlove::getGloveData(int devNum)
{
   return mTheData[devNum][current];
}


