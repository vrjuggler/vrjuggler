// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_VEC_OPS_H_
#define _VRJ_NET_GMTL_VEC_OPS_H_

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>


namespace gmtl
{
   typedef gmtl::VecBase<float, 3> VecBase3f;
   typedef gmtl::VecBase<double, 3> VecBase3d;
   typedef gmtl::VecBase<float, 4> VecBase4f;
   typedef gmtl::VecBase<double, 4> VecBase4d;

   template float dot(const gmtl::Vec3f&, const gmtl::Vec3f&);
   template float dot(const gmtl::Vec4f&, const gmtl::Vec4f&);

   template float length(const gmtl::Vec3f&);
   template float length(const gmtl::Vec4f&);

   template float lengthSquared(const gmtl::Vec3f&);
   template float lengthSquared(const gmtl::Vec4f&);

   template float normalize(gmtl::Vec3f&);
   template float normalize(gmtl::Vec4f&);

   template bool isNormalized(const gmtl::Vec3f&, const float);
   template bool isNormalized(const gmtl::Vec4f&, const float);

   template gmtl::Vec3i& cross(gmtl::Vec3i&, const gmtl::Vec3i&,
                               const gmtl::Vec3i&);
   template gmtl::Vec3d& cross(gmtl::Vec3d&, const gmtl::Vec3d&,
                               const gmtl::Vec3d&);
   template gmtl::Vec3f& cross(gmtl::Vec3f&, const gmtl::Vec3f&,
                               const gmtl::Vec3f&);

   template VecBase3f& reflect(VecBase3f&, const VecBase3f&, const Vec3f&);
   template VecBase3d& reflect(VecBase3d&, const VecBase3d&, const Vec3d&);
   template VecBase4f& reflect(VecBase4f&, const VecBase4f&, const Vec4f&);
   template VecBase4d& reflect(VecBase4d&, const VecBase4d&, const Vec4d&);

   template VecBase3f& lerp(VecBase3f&, const float&, const VecBase3f&,
                            const VecBase3f&);
   template VecBase3d& lerp(VecBase3d&, const double&, const VecBase3d&,
                            const VecBase3d&);
   template VecBase4f& lerp(VecBase4f&, const float&, const VecBase4f&,
                            const VecBase4f&);
   template VecBase4d& lerp(VecBase4d&, const double&, const VecBase4d&,
                            const VecBase4d&);

   template bool isEqual(const gmtl::VecBase3f&, const gmtl::VecBase3f&,
                         const float);
   template bool isEqual(const gmtl::VecBase3d&, const gmtl::VecBase3d&,
                         const double);
   template bool isEqual(const gmtl::VecBase4f&, const gmtl::VecBase4f&,
                         const float);
   template bool isEqual(const gmtl::VecBase4d&, const gmtl::VecBase4d&,
                         const double);
}


#endif /* _VRJ_NET_GMTL_VEC_OPS_H_ */
