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

#include <gadget/gadgetConfig.h>

#include <gadget/Type/Glove.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>
#include <gmtl/EulerAngle.h>


namespace gadget
{


/////////////////////////////////////////////////////////////////////////
//                GloveData                                          //
/////////////////////////////////////////////////////////////////////////

/*
 * Implementation of the Glove
 * abstract device class
 *
 * This class is the base class for all gloves.
 * We will implment all the necessary data operations
 * on the glove data. (Get finger pos, get vectors, get matrices, etc.)
 */

GloveData::GloveData(): InputData()
{
   // Zero out the angles
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         angles[i][j] = 0.0f;

   // Matrices are already identities
}

GloveData::GloveData(const GloveData& data)
{
   int i;

   for (i=0;i<NUM_COMPONENTS;i++) {
      for(int j=0;j<NUM_JOINTS;j++) {
          angles[i][j] = data.angles[i][j];
      }
   }

   for (i=0;i<NUM_COMPONENTS;i++) {
      for(int j=0;j<NUM_JOINTS-1;j++) {
          xforms[i][j] = data.xforms[i][j];
      }
   }

   for (i=0;i<NUM_COMPONENTS;i++) {
      for(int j=0;j<NUM_JOINTS;j++) {
          dims[i][j] = data.dims[i][j];
      }
   }
}

/**
 * Calulates all the xform matrices
 * This is calculated based upon the angles in the data structure
 * This code will be very complex, so I will HACK it for now.
 * XXX: This hack should be fixed soon though
 *
 * I am just rotating around single axis for fingers, not taking abduct into account
 * The thumb is a complete fudge.
 * Wrist is not being done at all
 */
int GloveData::calcXforms()
{
   gmtl::Vec3f xAxis(1.0f, 0.0f, 0.0f);
   gmtl::Vec3f yAxis(0.0f, 1.0f, 0.0f);
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
   gmtl::setRot(xforms[INDEX][DIJ], gmtl::AxisAnglef( angles[INDEX][DIJ], xAxis ) );
   gmtl::preMult(xforms[INDEX][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][DIJ]));
   gmtl::setRot(xforms[INDEX][PIJ], gmtl::AxisAnglef( angles[INDEX][PIJ],xAxis ) );
   gmtl::preMult(xforms[INDEX][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][PIJ]));
   gmtl::setRot(xforms[INDEX][MPJ], gmtl::AxisAnglef( angles[INDEX][MPJ],xAxis ) );
   gmtl::preMult(xforms[INDEX][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][MPJ]));

   // MIDDLE
   gmtl::setRot(xforms[MIDDLE][DIJ], gmtl::AxisAnglef( angles[MIDDLE][DIJ], xAxis ) );
   gmtl::preMult(xforms[MIDDLE][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][DIJ]));
   gmtl::setRot(xforms[MIDDLE][PIJ], gmtl::AxisAnglef( angles[MIDDLE][PIJ], xAxis ) );
   gmtl::preMult(xforms[MIDDLE][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][PIJ]));
   gmtl::setRot(xforms[MIDDLE][MPJ], gmtl::AxisAnglef( angles[MIDDLE][MPJ], xAxis ) );
   gmtl::preMult(xforms[MIDDLE][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][MPJ]));

   // RING
   gmtl::setRot(xforms[RING][DIJ], gmtl::AxisAnglef( angles[RING][DIJ], xAxis ) );
   gmtl::preMult(xforms[RING][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][DIJ]));
   gmtl::setRot(xforms[RING][PIJ], gmtl::AxisAnglef( angles[RING][PIJ], xAxis ) );
   gmtl::preMult(xforms[RING][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][PIJ]));
   gmtl::setRot(xforms[RING][MPJ], gmtl::AxisAnglef( angles[RING][MPJ], xAxis ) );
   gmtl::preMult(xforms[RING][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][MPJ]));

   // PINKY
   gmtl::setRot(xforms[PINKY][DIJ], gmtl::AxisAnglef( angles[PINKY][DIJ], xAxis ) );
   gmtl::preMult(xforms[PINKY][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][DIJ]));
   gmtl::setRot(xforms[PINKY][PIJ], gmtl::AxisAnglef( angles[PINKY][PIJ], xAxis ) );
   gmtl::preMult(xforms[PINKY][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][PIJ]));
   gmtl::setRot(xforms[PINKY][MPJ], gmtl::AxisAnglef( angles[PINKY][MPJ], xAxis ) );
   gmtl::preMult(xforms[PINKY][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][MPJ]));

   // WRIST

   return 1;
}

// Output the angles in one single line
std::ostream& GloveData::outputAngles(std::ostream& out) const
{
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         out << angles[i][j] << " ";

   return out;
}

// Input the angles from one single line
std::istream& GloveData::inputAngles(std::istream& in)
{
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         in >> angles[i][j];

   return in;
}

////////////////////////////////////////////////////////////////////////
//                          Glove                                   //
////////////////////////////////////////////////////////////////////////
Glove::Glove()
{
   //vprDEBUG(gadgetDBG_INPUT_MGR,3)<<"*** Glove::Glove()\n"<< vprDEBUG_FLUSH;

   for(int i=0;i<GADGET_MAX_GLOVE_DEVS;i++)
      mGlovePos[i] = NULL;
}


// Just get the data from the current vector entry
float Glove::getGloveAngle(GloveData::GloveComponent component,
                             GloveData::GloveJoint joint, int devNum)
{
   // Temporarily removed
   // return mTheData[devNum][current].angles[component][joint];
   return 0;
}

/**
 * Returns a vector ponting "out" of the component.
 * Can be used for selection, etc.
 * Use getGlovePos to get the transformation matrix.
 */
gmtl::Vec3f Glove::getGloveVector(GloveData::GloveComponent component, int devNum)
{
   // Take a normalized ray up default (yAxis), and transform by finger tip rot matrix
   // ret_val = wTt yAxis
   gmtl::Vec3f y_axis(0.0f, 1.0f, 0.0f);
   gmtl::Vec3f ret_val(0.0f, 0.0f, 0.0f);

   ret_val = getGlovePos(component, devNum) * y_axis;      // Compute the vector direction
   return ret_val;
}

// Calculated from the matrices in xforms
// <sub>world</sub><b>T</b><sub>tip</sub> = <sub>world</sub><b>T</b><sub>base</sub> <sub>base</sub><b>T</b><sub>dij</sub> <sub>dij</sub><b>T</b><sub>tip</sub>
//  i.e. wTt = wTb bTd dTt
gmtl::Matrix44f Glove::getGlovePos(GloveData::GloveComponent component, int devNum)
{
   // Temporarily removed
   /*
   if(mGlovePos[devNum] != NULL)
   {
      gmtl::Matrix44f ret_val;       // The returned matrix.
      gmtl::Matrix44f baseTdij;      // Transform from base to dig coord system
      gmtl::Matrix44f dijTtip;       // Transform to the tip of the finger

      switch(component)
      {
      case GloveData::WRIST:
         baseTdij.makeIdent();      // No transform
         dijTtip.makeIdent();       // No transform
         break;
      case GloveData::INDEX:
      case GloveData::MIDDLE:
      case GloveData::RING:
      case GloveData::PINKY:
      case GloveData::THUMB:
         dijTtip.makeTrans(mTheData[devNum][current].dims[component][GloveData::DIJ+1][VJ_X],
                           mTheData[devNum][current].dims[component][GloveData::DIJ+1][VJ_Y],
                           mTheData[devNum][current].dims[component][GloveData::DIJ+1][VJ_Z]);
         baseTdij = mTheData[devNum][current].xforms[component][GloveData::MPJ];            // baseTmpj
         baseTdij.postMult(mTheData[devNum][current].xforms[component][GloveData::PIJ]);    // mpjTpij
         baseTdij.postMult(mTheData[devNum][current].xforms[component][GloveData::DIJ]);    // pijTdij
         break;

      default:
         vprASSERT(false);
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
      vprDEBUG( gadgetDBG_INPUT_MGR,0)
         << clrOutNORM(clrRED, "ERROR:")
         << " Glove: Trying to get a glove without a position proxy set for device number: "
         << devNum << ".\n" << vprDEBUG_FLUSH;
      vprASSERT( mGlovePos[devNum] != NULL );      // should be false in here
      return gmtl::Matrix44f();
   }*/
   gmtl::Matrix44f ret_val;       // The returned matrix.
   return ret_val;
}

// Grab a copy of the most current glove data
GloveData Glove::getGloveData(int devNum)
{
   // Temporarily removed
   //return mTheData[devNum][current];
   return mTheData[0][0];
}


} // End of gadget namespace
