/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _VRJ_PF_UTIL_H_
#define _VRJ_PF_UTIL_H_

/**************************************************
* Performer helper routines                       *
***************************************************/

#include <vrj/Draw/Pf/Config.h>

#include <Performer/pr/pfLinMath.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

namespace vrj
{

namespace pf
{

/** Converts a Performer matrix to a Juggler (GMTL) matrix. */
VJ_PF_API(gmtl::Matrix44f) GetVjMatrix(const pfMatrix& perfMat);

/** Converts a Juggler (GMTL) matrix to a Performer Matrix. */
VJ_PF_API(pfMatrix) GetPfMatrix(const gmtl::Matrix44f& mat);

/** Converts a Performer 3-element vector to a Juggler (GMTL) vector. */
VJ_PF_API(gmtl::Vec3f) GetVjVec(const pfVec3& vec);

/** Converts a Juggler (GMTL) vector to a Pf vector. */
VJ_PF_API(pfVec3) GetPfVec(const gmtl::Vec3f& vec);

}

}


#endif
