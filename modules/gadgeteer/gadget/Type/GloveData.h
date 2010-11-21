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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_GLOVEDATA_H
#define _GADGET_GLOVEDATA_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/InputData.h>
#include <gmtl/Matrix.h>


namespace gadget
{

/** \class GloveData GloveData.h gadget/Type/GloveData.h
 *
 * This is the data stored about a glove
 *
 * The angles are the joint angles for all fingers and the wrist
 *
 * DIJ = Distal Interphalangeal Joint  --- finger tip<br>
 * PIJ = Proximal "              "     --- Middle joint<br>
 * MPJ = Metacarpo "             "     --- closest to hand<br>
 * ABDUCT = spread of fingers
 *
 * YAW and PITCH apply only to WRIST.
 *
 * Transforms transfer you from one coord system to the other.
 * If the transforms are tied together, then they can return
 * complete transformations.
 *
 * @note More docs needed here.
 */
class GADGET_CLASS_API GloveData : public InputData
{
public:
   enum { NUM_JOINTS = 4, NUM_COMPONENTS = 6 };
   enum GloveJoint
      { MPJ = 0, PIJ = 1, DIJ = 2, ABDUCT = 3, YAW = 0, PITCH = 1};
   enum GloveComponent
      { THUMB = 0, INDEX = 1, MIDDLE = 2, RING = 3, PINKY = 4, WRIST = 5};

   GloveData();

   GloveData(const GloveData& data);

   /**
    * Calulates all the transformation matrices.
    * This is calculated based upon the angles in the data structure.
    *
    * @post mTransforms is updated to reflect the current transformations.
    */
   int calcXforms();

   /**
    * Returns the transform matrix of the specified joint
    */
   const gmtl::Matrix44f&
      getLocalTransformMatrix(const GloveComponent component,
                              const GloveJoint joint)
      const
   {
      return mTransforms[component][joint];
   }
   
   /**
    * Outputs the angles.
    * TODO: Convert this to XML?
    */
   std::ostream& outputAngles(std::ostream& out) const;
   std::istream& inputAngles(std::istream& in);

public:
   float mAngles[NUM_COMPONENTS][NUM_JOINTS];

   /**
    * These are the xforms from TO the coord system of the given joint.
    * Ex: xforms[0] ==> <br>
    *     base<b>T</b>mpj mpj<b>T</b>pij pij<b>T</b>dij
    */
   gmtl::Matrix44f mTransforms[NUM_COMPONENTS][(NUM_JOINTS-1)];
}; // class GloveData

} // namespace gadget

#endif
