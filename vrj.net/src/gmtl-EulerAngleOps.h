// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_EULER_ANGLE_OPS_H_
#define _VRJ_NET_GMTL_EULER_ANGLE_OPS_H_

#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>


namespace gmtl
{
   template bool isEqual(const EulerAngleXYZf&, const EulerAngleXYZf&,
                         const float);
   template bool isEqual(const EulerAngleZXYf&, const EulerAngleZXYf&,
                         const float);
   template bool isEqual(const EulerAngleZYXf&, const EulerAngleZYXf&,
                         const float);
}


#endif /*_VRJ_NET_GMTL_EULER_ANGLE_OPS_H_ */
