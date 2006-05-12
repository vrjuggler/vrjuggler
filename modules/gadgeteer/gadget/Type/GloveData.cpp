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

#include <gadget/gadgetConfig.h>

#include <gmtl/AxisAngle.h>
#include <gmtl/AxisAngleOps.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/GloveData.h>

namespace gadget
{

GloveData::GloveData() : gadget::InputData()
{
   // Zero out the mAngles
   for(int i=0;i<NUM_COMPONENTS;i++)
   {
      for(int j=0;j<NUM_JOINTS;j++)
      {
         mAngles[i][j] = 0;
      }
   }

   // Matrixes are already identities
}

GloveData::GloveData(const GloveData& data) : gadget::InputData(data)
{
   int i;

   for (i=0;i<NUM_COMPONENTS;i++)
   {
      for(int j=0;j<NUM_JOINTS;j++)
      {
         mAngles[i][j] = data.mAngles[i][j];
      }
   }

   for(i=0;i<NUM_COMPONENTS;i++)
   {
      for(int j=0;j<NUM_JOINTS-1;j++)
      {
         mTransforms[i][j] = data.mTransforms[i][j];
      }
   }
}

/**
 * Calulates all the xform matrices
 * This is calculated based upon the mAngles in the data structure
 *
 * I am just rotating around single axis for fingers, not taking abduct into account
 * The thumb is a complete fudge.
 * Wrist is not being done at all
 */
int GloveData::calcXforms()
{
   gmtl::Vec3f xAxis(1.0f, 0.0f, 0.0f);
   gmtl::Vec3f yAxis(0.0f, 1.0f, 0.0f);
   gmtl::Vec3f zAxis(0.0f, 0.0f, 1.0f);
   const float toMeters(1.0f/PositionUnitConversion::ConvertToInches);
   gmtl::Vec3f dims[NUM_COMPONENTS][NUM_JOINTS];
   
   // DIJ+1 = Length distal
   // DIJ   = Length medial
   // PIJ   = Length proximal
   // MPJ   = Length to finger

   // TODO: Do this once at startup, since it doesn't change.
   // And this really should be part of the Glove, so you can specify the dimension of a hand in the config file

   dims[THUMB][DIJ+1] = yAxis * (toMeters * 0.5f);
   dims[THUMB][DIJ] = yAxis * (toMeters * 0.5f);
   dims[THUMB][PIJ] = yAxis * (toMeters * 0.5f);
   dims[THUMB][MPJ] = xAxis * (toMeters * -0.5f);

   dims[INDEX][DIJ+1] = yAxis * (toMeters * 0.5f);
   dims[INDEX][DIJ] = yAxis * (toMeters * 1.0f);
   dims[INDEX][PIJ] = yAxis * (toMeters * 1.3f);
   dims[INDEX][MPJ] = (yAxis * (toMeters * 1.7f)) + (toMeters * xAxis * -0.4f);

   dims[MIDDLE][DIJ+1] = yAxis * (toMeters * 0.5f);
   dims[MIDDLE][DIJ] = yAxis * (toMeters * 1.1f);
   dims[MIDDLE][PIJ] = yAxis * (toMeters * 1.4f);
   dims[MIDDLE][MPJ] = (yAxis * (toMeters * 1.8f)) + (toMeters * xAxis * 0.0f);

   dims[RING][DIJ+1] = yAxis * (toMeters * 0.4f);
   dims[RING][DIJ] = yAxis * (toMeters * 1.0f);
   dims[RING][PIJ] = yAxis * (toMeters * 1.1f);
   dims[RING][MPJ] = (yAxis * (toMeters * 1.7f)) + (toMeters * xAxis * 0.4f);

   dims[PINKY][DIJ+1] = yAxis * (toMeters * 0.3f);
   dims[PINKY][DIJ] = yAxis * (toMeters * 1.0f);
   dims[PINKY][PIJ] = yAxis * (toMeters * 0.85f);
   dims[PINKY][MPJ] = (yAxis * (toMeters * 1.6f)) + (toMeters * xAxis * 0.7f);

   // ----------------------- //
   // ----- XFORMS ---------- //
   // ----------------------- //

   // THUMB
   gmtl::setRot(mTransforms[THUMB][DIJ], gmtl::AxisAnglef( mAngles[THUMB][DIJ], xAxis ) );
   gmtl::preMult(mTransforms[THUMB][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][DIJ]));
   gmtl::setRot(mTransforms[THUMB][PIJ], gmtl::AxisAnglef( mAngles[THUMB][PIJ], xAxis ) );
   gmtl::preMult(mTransforms[THUMB][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][PIJ]));
   gmtl::setRot(mTransforms[THUMB][MPJ], gmtl::AxisAnglef( gmtl::Math::PI_OVER_4, zAxis ) );
   gmtl::preMult(mTransforms[THUMB][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][MPJ]));
   
// Do we need to rotate this by the mAngles too?
// gmtl::setRot(mTransforms[THUMB][MPJ], gmtl::AxisAnglef( mAngles[THUMB][MPJ], xAxis ) );
// gmtl::preMult(mTransforms[THUMB][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][MPJ]));

   // INDEX
   gmtl::setRot(mTransforms[INDEX][DIJ], gmtl::AxisAnglef( mAngles[INDEX][DIJ], xAxis ) );
   gmtl::preMult(mTransforms[INDEX][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][DIJ]));
   gmtl::setRot(mTransforms[INDEX][PIJ], gmtl::AxisAnglef( mAngles[INDEX][PIJ], xAxis ) );
   gmtl::preMult(mTransforms[INDEX][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][PIJ]));
   gmtl::setRot(mTransforms[INDEX][MPJ], gmtl::AxisAnglef( mAngles[INDEX][MPJ], xAxis ) );
   gmtl::preMult(mTransforms[INDEX][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][MPJ]));

   // MIDDLE
   gmtl::setRot(mTransforms[MIDDLE][DIJ], gmtl::AxisAnglef( mAngles[MIDDLE][DIJ], xAxis ) );
   gmtl::preMult(mTransforms[MIDDLE][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][DIJ]));
   gmtl::setRot(mTransforms[MIDDLE][PIJ], gmtl::AxisAnglef( mAngles[MIDDLE][PIJ], xAxis ) );
   gmtl::preMult(mTransforms[MIDDLE][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][PIJ]));
   gmtl::setRot(mTransforms[MIDDLE][MPJ], gmtl::AxisAnglef( mAngles[MIDDLE][MPJ], xAxis ) );
   gmtl::preMult(mTransforms[MIDDLE][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][MPJ]));

   // RING
   gmtl::setRot(mTransforms[RING][DIJ], gmtl::AxisAnglef( mAngles[RING][DIJ], xAxis ) );
   gmtl::preMult(mTransforms[RING][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][DIJ]));
   gmtl::setRot(mTransforms[RING][PIJ], gmtl::AxisAnglef( mAngles[RING][PIJ], xAxis ) );
   gmtl::preMult(mTransforms[RING][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][PIJ]));
   gmtl::setRot(mTransforms[RING][MPJ], gmtl::AxisAnglef( mAngles[RING][MPJ], xAxis ) );
   gmtl::preMult(mTransforms[RING][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][MPJ]));

   // PINKY
   gmtl::setRot(mTransforms[PINKY][DIJ], gmtl::AxisAnglef( mAngles[PINKY][DIJ], xAxis ) );
   gmtl::preMult(mTransforms[PINKY][DIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][DIJ]));
   gmtl::setRot(mTransforms[PINKY][PIJ], gmtl::AxisAnglef( mAngles[PINKY][PIJ], xAxis ) );
   gmtl::preMult(mTransforms[PINKY][PIJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][PIJ]));
   gmtl::setRot(mTransforms[PINKY][MPJ], gmtl::AxisAnglef( mAngles[PINKY][MPJ], xAxis ) );
   gmtl::preMult(mTransforms[PINKY][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][MPJ]));

   return 1;
}

// Output the mAngles to a single line
std::ostream& GloveData::outputAngles(std::ostream& out) const
{
   for(int i=0;i<NUM_COMPONENTS;i++)
   {
      for(int j=0;j<NUM_JOINTS;j++)
      {
         out << mAngles[i][j] << " ";
      }
   }

   return out;
}

// Input the mAngles from one single line
std::istream& GloveData::inputAngles(std::istream& in)
{
   for(int i=0;i<NUM_COMPONENTS;i++)
   {
      for(int j=0;j<NUM_JOINTS;j++)
      {
         in >> mAngles[i][j];
      }
   }

   return in;
}

} // namespace gadget

