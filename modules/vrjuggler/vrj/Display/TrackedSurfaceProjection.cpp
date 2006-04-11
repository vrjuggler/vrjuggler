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

#include <vrj/vrjConfig.h>

#include <vrj/Display/TrackedSurfaceProjection.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Xforms.h>



namespace vrj
{

void TrackedSurfaceProjection::calcViewMatrix(gmtl::Matrix44f& eyePos, const float scaleFactor)
{
   updateSurfaceParams(scaleFactor);

   calcViewFrustum(eyePos, scaleFactor);

   //Coord eye_coord(eyePos);
   gmtl::Vec3f   eye_pos( gmtl::makeTrans<gmtl::Vec3f>(eyePos) );             // Non-xformed pos

   // Need to post translate to get the view matrix at the position of the eye
   // Don't take into account the surface transform since the eyepos already accounts for that
   //viewMat = m_surface_M_base * gmtl::makeTrans<gmtl::Matrix44f>( -eye_pos );
   // this does more math then needed, but it works for now.
   gmtl::Matrix44f base_M_surf_rotonly(m_base_M_surftrans);
   gmtl::setTrans(base_M_surf_rotonly, gmtl::Vec3f(0.0f, 0.0f, 0.0f));
   gmtl::Matrix44f temp_surf_M_base = base_M_surf_rotonly * m_surftrans_M_surf;
   gmtl::invert(temp_surf_M_base);
   mViewMat = temp_surf_M_base * gmtl::makeInverse(eyePos);
}

// @post: All projection transforms must be updates
void TrackedSurfaceProjection::updateSurfaceParams(const float scaleFactor)
{
   // Get the tracking data for the surface offset
   gmtl::Matrix44f m_base_M_surftrans = mTracker->getData(scaleFactor);     // baseMtracker

   // Compute new transformation
   // Cost: 1 mat mult and one inversion
   m_base_M_surface = m_base_M_surftrans * m_surftrans_M_surf;               // Compute new
   m_surface_M_base = gmtl::invert(m_surface_M_base, m_base_M_surface);    // Set the inverse matrix for later
}

std::ostream& TrackedSurfaceProjection::outStream(std::ostream& out,
                                                  const unsigned int indentLevel)
{
   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << std::setw(pad_width_dot)
       << "Type " << " vrj::TrackedSurfaceProjection\n";
   out << indent_text << std::setw(pad_width_dot)
       << "Pos Proxy " << " " << mTracker.getProxyName() << std::endl;

   return SurfaceProjection::outStream(out, indentLevel);
}

}
