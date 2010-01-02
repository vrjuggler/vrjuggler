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

#ifndef _GADGET_LINEAR_SIGMOID_POS_FILTER_H_
#define _GADGET_LINEAR_SIGMOID_POS_FILTER_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/PositionFilter.h>
#include <gmtl/Quat.h>

namespace gadget
{

/** \class LinearSigmoidPositionFilter LinearSigmoidPositionFilter.h gadget/Type/LinearSigmoidPositionFilter.h 
 *
 * A position filter class that uses a linear sigmoid.
 */
class LinearSigmoidPositionFilter : public PositionFilter
{
public:
   LinearSigmoidPositionFilter()
   {
      mMinDist = 0.0f;
      mMaxDist = 0.05f;
      mMaxThreshold = 10000000.0f;        // Just set to some huge number
   }

   /**
    * Gets the position to use>
    * @param newPos The new postion this frame of the physical device.
    */
   const gmtl::Matrix44f getPos(const gmtl::Matrix44f& newPos) const;

   /**
    * Given a distance, return the scale factor based upon the minDist and
    * maxDist values.
    */
   float getScaleFactor(const float distance) const;

   void setMinDist(const float val)
   {
      mMinDist = val;
   }

   float minDist() const
   {
      return mMinDist;
   }

   void setMaxDist(const float val)
   {
      mMaxDist = val;
   }

   float maxDist() const
   {
      return mMaxDist;
   }

   void setMaxThreshold(const float val)
   {
      mMaxThreshold = val;
   }

   float maxTheshold() const
   {
      return mMaxThreshold;
   }

private:
   gmtl::Matrix44f mLastReturnedPos;

   // These parameters influence the sigmoid
   float    mMinDist;            /**< Mininum distance parameter */
   float    mMaxDist;            /**< Maximum distance parameter */

   float    mMaxThreshold;       /**< Maximum distance that the tracker is
                                      allowed to move.  Anything farther is
                                      just ignored and is not interpolated to */

};

} // End of gadget namespace

#endif
