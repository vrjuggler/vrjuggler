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

#ifndef _VRJ_PF_UTIL_H_
#define _VRJ_PF_UTIL_H_

/**************************************************
* Performer helper routines                       *
***************************************************/

#include <vrj/vrjConfig.h>

#include <Performer/pr/pfLinMath.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

namespace vrj
{


/**< Converts Performer matrix to Juggler (GMTL) matrix. */
VJ_API(gmtl::Matrix44f) GetVjMatrix(const pfMatrix& perfMat);

/**< Converts Juggler (GMTL) matrix to Pf Matrix. */
VJ_API(pfMatrix) GetPfMatrix(const gmtl::Matrix44f& mat);

/**< Converts Performer 3-element vector to Juggler (GMTL) vector. */
VJ_API(gmtl::Vec3f) GetVjVec(const pfVec3& vec);

/**< Converts Juggler (GMTL) vector to Pf vector. */
VJ_API(pfVec3) GetPfVec(const gmtl::Vec3f& vec);


}

#endif
