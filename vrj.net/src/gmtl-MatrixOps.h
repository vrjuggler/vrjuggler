/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

#ifndef _VRJ_NET_GMTL_MATRIX_OPS_H_
#define _VRJ_NET_GMTL_MATRIX_OPS_H_

#include <gmtl/MatrixOps.h>

namespace gmtl
{
   template gmtl::Matrix44d& identity(gmtl::Matrix44d&);
   template gmtl::Matrix44f& identity(gmtl::Matrix44f&);
   template gmtl::Matrix33d& identity(gmtl::Matrix33d&);
   template gmtl::Matrix33f& identity(gmtl::Matrix33f&);

   template gmtl::Matrix44d& zero(gmtl::Matrix44d&);
   template gmtl::Matrix44f& zero(gmtl::Matrix44f&);
   template gmtl::Matrix33d& zero(gmtl::Matrix33d&);
   template gmtl::Matrix33f& zero(gmtl::Matrix33f&);

   template gmtl::Matrix44d& mult(gmtl::Matrix44d&, const gmtl::Matrix44d&,
                                  const gmtl::Matrix44d&);
   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  const gmtl::Matrix44f&);
   template gmtl::Matrix33d& mult(gmtl::Matrix33d&, const gmtl::Matrix33d&,
                                  const gmtl::Matrix33d&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const gmtl::Matrix33f&);

   template gmtl::Matrix44d& sub(gmtl::Matrix44d&, const gmtl::Matrix44d&,
                                 const gmtl::Matrix44d&);
   template gmtl::Matrix44f& sub(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33d& sub(gmtl::Matrix33d&, const gmtl::Matrix33d&,
                                 const gmtl::Matrix33d&);
   template gmtl::Matrix33f& sub(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44d& add(gmtl::Matrix44d&, const gmtl::Matrix44d&,
                                 const gmtl::Matrix44d&);
   template gmtl::Matrix44f& add(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33d& add(gmtl::Matrix33d&, const gmtl::Matrix33d&,
                                 const gmtl::Matrix33d&);
   template gmtl::Matrix33f& add(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44d& postMult(gmtl::Matrix44d&, const gmtl::Matrix44d&);
   template gmtl::Matrix44f& postMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33d& postMult(gmtl::Matrix33d&, const gmtl::Matrix33d&);
   template gmtl::Matrix33f& postMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44d& preMult(gmtl::Matrix44d&, const gmtl::Matrix44d&);
   template gmtl::Matrix44f& preMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33d& preMult(gmtl::Matrix33d&, const gmtl::Matrix33d&);
   template gmtl::Matrix33f& preMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44d& mult(gmtl::Matrix44d&, const gmtl::Matrix44d&,
                                  const double&);
   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  const float&);
   template gmtl::Matrix33d& mult(gmtl::Matrix33d&, const gmtl::Matrix33d&,
                                  const double&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const float&);

   template gmtl::Matrix44d& mult(gmtl::Matrix44d&, double);
   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, float);
   template gmtl::Matrix33d& mult(gmtl::Matrix33d&, double);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, float);

   template gmtl::Matrix44d& transpose(gmtl::Matrix44d&);
   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&);
   template gmtl::Matrix33d& transpose(gmtl::Matrix33d&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&);

   template gmtl::Matrix44d& transpose(gmtl::Matrix44d&,
                                       const gmtl::Matrix44d&);
   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&,
                                       const gmtl::Matrix44f&);
   template gmtl::Matrix33d& transpose(gmtl::Matrix33d&,
                                       const gmtl::Matrix33d&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&,
                                       const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invertTrans(gmtl::Matrix44d&,
                                         const gmtl::Matrix44d&);
   template gmtl::Matrix44f& invertTrans(gmtl::Matrix44f&,
                                         const gmtl::Matrix44f&);
   template gmtl::Matrix33d& invertTrans(gmtl::Matrix33d&,
                                         const gmtl::Matrix33d&);
   template gmtl::Matrix33f& invertTrans(gmtl::Matrix33f&,
                                         const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invertOrthogonal(gmtl::Matrix44d&,
                                              const gmtl::Matrix44d&);
   template gmtl::Matrix44f& invertOrthogonal(gmtl::Matrix44f&,
                                              const gmtl::Matrix44f&);
   template gmtl::Matrix33d& invertOrthogonal(gmtl::Matrix33d&,
                                              const gmtl::Matrix33d&);
   template gmtl::Matrix33f& invertOrthogonal(gmtl::Matrix33f&,
                                              const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invertAffine(gmtl::Matrix44d&,
                                          const gmtl::Matrix44d&);
   template gmtl::Matrix44f& invertAffine(gmtl::Matrix44f&,
                                          const gmtl::Matrix44f&);
   template gmtl::Matrix33d& invertAffine(gmtl::Matrix33d&,
                                          const gmtl::Matrix33d&);
   template gmtl::Matrix33f& invertAffine(gmtl::Matrix33f&,
                                          const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invertFull(gmtl::Matrix44d&,
                                        const gmtl::Matrix44d&);
   template gmtl::Matrix44f& invertFull(gmtl::Matrix44f&,
                                        const gmtl::Matrix44f&);
   template gmtl::Matrix33d& invertFull(gmtl::Matrix33d&,
                                        const gmtl::Matrix33d&);
   template gmtl::Matrix33f& invertFull(gmtl::Matrix33f&,
                                        const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invert(gmtl::Matrix44d&, const gmtl::Matrix44d&);
   template gmtl::Matrix44f& invert(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33d& invert(gmtl::Matrix33d&, const gmtl::Matrix33d&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44d& invert(gmtl::Matrix44d&);
   template gmtl::Matrix44f& invert(gmtl::Matrix44f&);
   template gmtl::Matrix33d& invert(gmtl::Matrix33d&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&);

   template bool isEqual(const gmtl::Matrix44d&, const gmtl::Matrix44d&,
                         const double&);
   template bool isEqual(const gmtl::Matrix44f&, const gmtl::Matrix44f&,
                         const float&);
   template bool isEqual(const gmtl::Matrix33d&, const gmtl::Matrix33d&,
                         const double&);
   template bool isEqual(const gmtl::Matrix33f&, const gmtl::Matrix33f&,
                         const float&);
}


#endif /* _VRJ_NET_GMTL_MATRIX_OPS_H_ */
