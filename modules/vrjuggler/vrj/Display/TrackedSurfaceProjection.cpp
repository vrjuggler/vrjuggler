/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <iomanip>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Xforms.h>

#include <gadget/Type/Position/PositionUnitConversion.h>
#include <vrj/Display/TrackedSurfaceProjection.h>


namespace vrj
{

TrackedSurfaceProjection::
TrackedSurfaceProjection(const gmtl::Point3f& llCorner,
                         const gmtl::Point3f& lrCorner,
                         const gmtl::Point3f& urCorner,
                         const gmtl::Point3f& ulCorner,
                         const std::string& trackerName)
   : SurfaceProjection(llCorner, lrCorner, urCorner, ulCorner)
   , mOriginalLLCorner(llCorner)
   , mOriginalLRCorner(lrCorner)
   , mOriginalURCorner(urCorner)
   , mOriginalULCorner(ulCorner)
{
   //XXX: Watch for timing problems here if trakcer is not inited first.
   //     It shoulbe be though from dependency checking
   mTracker.init(trackerName);              // Intialize the tracker
}

void TrackedSurfaceProjection::calcViewMatrix(const gmtl::Matrix44f& eyePos,
                                              const float scaleFactor)
{
   updateSurfaceParams(scaleFactor);

   SurfaceProjection::calcViewMatrix(eyePos,scaleFactor);
}

void TrackedSurfaceProjection::updateSurfaceParams(const float scaleFactor)
{
   /* Here we rotate the original screen corners, and then have SurfaceProjection::calculateOffsets
   *  handle the calculation of mOriginToScreen, etc.
   */

   // Get the tracking data for the surface offset
   gmtl::Matrix44f trackerMatrix = mTracker->getData(scaleFactor);

   gmtl::Vec3f trans=gmtl::makeTrans<gmtl::Vec3f>(trackerMatrix);
   trans=trans/scaleFactor;
   gmtl::setTrans(trackerMatrix,trans);

   mLLCorner=trackerMatrix * mOriginalLLCorner;
   mLRCorner=trackerMatrix * mOriginalLRCorner;
   mURCorner=trackerMatrix * mOriginalURCorner;
   mULCorner=trackerMatrix * mOriginalULCorner;

   calculateOffsets();
}

std::ostream& TrackedSurfaceProjection::outStream(std::ostream& out,
                                                  const unsigned int indentLevel)
{
   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << "vrj::TrackedSurfaceProjection:\n";
   out << indent_text << std::setw(pad_width_dot)
       << "  Pos Proxy " << " " << mTracker.getProxyName() << std::endl;

   return SurfaceProjection::outStream(out, indentLevel);
}

}
