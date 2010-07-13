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

GloveData::GloveData()
{
   typedef GloveValues::angles_type angles_type;

   // Zero out the mAngles
   angles_type& angles(editValue().mAngles);
   for (angles_type::iterator i = angles.begin(); i != angles.end(); ++i)
   {
      typedef GloveValues::angular_joints_type::iterator iter_type;
      for (iter_type j = (*i).begin(); j != (*i).end(); ++j)
      {
         *j = 0.0f;
      }
   }

   // Matrixes are already identities
}

/**
 * Calulates all the xform matrices
 * This is calculated based upon the mAngles in the data structure
 *
 * I am just rotating around single axis for fingers, not taking abduct into account
 * The thumb is a complete fudge.
 * Wrist is not being done at all
 */
bool GloveData::calcXforms()
{
   gmtl::Vec3f x_axis(1.0f, 0.0f, 0.0f);
   gmtl::Vec3f y_axis(0.0f, 1.0f, 0.0f);
   gmtl::Vec3f z_axis(0.0f, 0.0f, 1.0f);
   const float to_meters(1.0f / PositionUnitConversion::ConvertToInches);
   gmtl::Vec3f dims[NUM_COMPONENTS][NUM_JOINTS];
   
   // DIJ+1 = Length distal
   // DIJ   = Length medial
   // PIJ   = Length proximal
   // MPJ   = Length to finger

   // TODO: Do this once at startup, since it doesn't change.
   // And this really should be part of the Glove, so you can specify the dimension of a hand in the config file

   dims[THUMB][DIJ + 1] = y_axis * (to_meters * 0.5f);
   dims[THUMB][DIJ] = y_axis * (to_meters * 0.5f);
   dims[THUMB][PIJ] = y_axis * (to_meters * 0.5f);
   dims[THUMB][MPJ] = x_axis * (to_meters * -0.5f);

   dims[INDEX][DIJ + 1] = y_axis * (to_meters * 0.5f);
   dims[INDEX][DIJ] = y_axis * (to_meters * 1.0f);
   dims[INDEX][PIJ] = y_axis * (to_meters * 1.3f);
   dims[INDEX][MPJ] = (y_axis * (to_meters * 1.7f)) +
                        (to_meters * x_axis * -0.4f);

   dims[MIDDLE][DIJ + 1] = y_axis * (to_meters * 0.5f);
   dims[MIDDLE][DIJ] = y_axis * (to_meters * 1.1f);
   dims[MIDDLE][PIJ] = y_axis * (to_meters * 1.4f);
   dims[MIDDLE][MPJ] = (y_axis * (to_meters * 1.8f)) +
                        (to_meters * x_axis * 0.0f);

   dims[RING][DIJ + 1] = y_axis * (to_meters * 0.4f);
   dims[RING][DIJ] = y_axis * (to_meters * 1.0f);
   dims[RING][PIJ] = y_axis * (to_meters * 1.1f);
   dims[RING][MPJ] = (y_axis * (to_meters * 1.7f)) +
                        (to_meters * x_axis * 0.4f);

   dims[PINKY][DIJ + 1] = y_axis * (to_meters * 0.3f);
   dims[PINKY][DIJ] = y_axis * (to_meters * 1.0f);
   dims[PINKY][PIJ] = y_axis * (to_meters * 0.85f);
   dims[PINKY][MPJ] = (y_axis * (to_meters * 1.6f)) +
                        (to_meters * x_axis * 0.7f);

   // ----------------------- //
   // ----- XFORMS ---------- //
   // ----------------------- //

   GloveValues::xform_type& xforms(editValue().mTransforms);
   GloveValues::angles_type& angles(editValue().mAngles);

   // THUMB
   gmtl::setRot(xforms[THUMB][DIJ],
                gmtl::AxisAnglef(angles[THUMB][DIJ], x_axis));
   gmtl::preMult(xforms[THUMB][DIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][DIJ]));
   gmtl::setRot(xforms[THUMB][PIJ],
                gmtl::AxisAnglef(angles[THUMB][PIJ], x_axis));
   gmtl::preMult(xforms[THUMB][PIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][PIJ]));
   gmtl::setRot(xforms[THUMB][MPJ],
                gmtl::AxisAnglef(gmtl::Math::PI_OVER_4, z_axis));
   gmtl::preMult(xforms[THUMB][MPJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][MPJ]));
   
// Do we need to rotate this by the angles too?
// gmtl::setRot(xforms[THUMB][MPJ], gmtl::AxisAnglef(angles[THUMB][MPJ], x_axis));
// gmtl::preMult(xforms[THUMB][MPJ], gmtl::makeTrans<gmtl::Matrix44f>(dims[THUMB][MPJ]));

   // INDEX
   gmtl::setRot(xforms[INDEX][DIJ],
                gmtl::AxisAnglef(angles[INDEX][DIJ], x_axis));
   gmtl::preMult(xforms[INDEX][DIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][DIJ]));
   gmtl::setRot(xforms[INDEX][PIJ],
                gmtl::AxisAnglef(angles[INDEX][PIJ], x_axis));
   gmtl::preMult(xforms[INDEX][PIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][PIJ]));
   gmtl::setRot(xforms[INDEX][MPJ],
                gmtl::AxisAnglef(angles[INDEX][MPJ], x_axis));
   gmtl::preMult(xforms[INDEX][MPJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[INDEX][MPJ]));

   // MIDDLE
   gmtl::setRot(xforms[MIDDLE][DIJ],
                gmtl::AxisAnglef(angles[MIDDLE][DIJ], x_axis));
   gmtl::preMult(xforms[MIDDLE][DIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][DIJ]));
   gmtl::setRot(xforms[MIDDLE][PIJ],
                gmtl::AxisAnglef(angles[MIDDLE][PIJ], x_axis));
   gmtl::preMult(xforms[MIDDLE][PIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][PIJ]));
   gmtl::setRot(xforms[MIDDLE][MPJ],
                gmtl::AxisAnglef(angles[MIDDLE][MPJ], x_axis));
   gmtl::preMult(xforms[MIDDLE][MPJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[MIDDLE][MPJ]));

   // RING
   gmtl::setRot(xforms[RING][DIJ],
                gmtl::AxisAnglef(angles[RING][DIJ], x_axis));
   gmtl::preMult(xforms[RING][DIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][DIJ]));
   gmtl::setRot(xforms[RING][PIJ],
                gmtl::AxisAnglef(angles[RING][PIJ], x_axis));
   gmtl::preMult(xforms[RING][PIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][PIJ]));
   gmtl::setRot(xforms[RING][MPJ],
                gmtl::AxisAnglef(angles[RING][MPJ], x_axis));
   gmtl::preMult(xforms[RING][MPJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[RING][MPJ]));

   // PINKY
   gmtl::setRot(xforms[PINKY][DIJ],
                gmtl::AxisAnglef(angles[PINKY][DIJ], x_axis));
   gmtl::preMult(xforms[PINKY][DIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][DIJ]));
   gmtl::setRot(xforms[PINKY][PIJ],
                gmtl::AxisAnglef(angles[PINKY][PIJ], x_axis));
   gmtl::preMult(xforms[PINKY][PIJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][PIJ]));
   gmtl::setRot(xforms[PINKY][MPJ],
                gmtl::AxisAnglef(angles[PINKY][MPJ], x_axis));
   gmtl::preMult(xforms[PINKY][MPJ],
                 gmtl::makeTrans<gmtl::Matrix44f>(dims[PINKY][MPJ]));

   return true;
}

// Output the mAngles to a single line
std::ostream& GloveData::outputAngles(std::ostream& out) const
{
   typedef GloveValues::angles_type angles_type;

   const angles_type& angles(getValue().mAngles);
   for (angles_type::const_iterator i = angles.begin(); i != angles.end(); ++i)
   {
      typedef GloveValues::angular_joints_type::const_iterator iter_type;
      for (iter_type j = (*i).begin(); j != (*i).end(); ++j)
      {
         out << *j << " ";
      }
   }

   return out;
}

// Input the mAngles from one single line
std::istream& GloveData::inputAngles(std::istream& in)
{
   typedef GloveValues::angles_type angles_type;

   angles_type& angles(editValue().mAngles);
   for (angles_type::iterator i = angles.begin(); i != angles.end(); ++i)
   {
      typedef GloveValues::angular_joints_type::iterator iter_type;
      for (iter_type j = (*i).begin(); j != (*i).end(); ++j)
      {
         in >> *j;
      }
   }

   return in;
}

} // namespace gadget

