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

#ifndef _VRJ_NET_XFORMS_H_
#define _VRJ_NET_XFORMS_H_

#include <gmtl/Xforms.h>


namespace gmtl
{
   template gmtl::VecBase<float, 3>& xform(gmtl::VecBase<float, 3>&,
                                           const gmtl::Quat<float>&,
                                           const gmtl::VecBase<float, 3>&);
   template gmtl::VecBase<double, 3>& xform(gmtl::VecBase<double, 3>&,
                                            const gmtl::Quat<double>&,
                                            const gmtl::VecBase<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to .NET as well.
   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 3, 3>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 3, 3>&,
                                        const gmtl::Vec<double, 3>&);
   template gmtl::Vec<float, 4>& xform(gmtl::Vec<float, 4>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 4>&);
   template gmtl::Vec<double, 4>& xform(gmtl::Vec<double, 4>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 4>&);

   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to .NET as well.
   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 3, 3>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 3, 3>&,
                                          const gmtl::Point<double, 3>&);
   template gmtl::Point<float, 4>& xform(gmtl::Point<float, 4>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 4>&);
   template gmtl::Point<double, 4>& xform(gmtl::Point<double, 4>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 4>&);

   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 3>&);
}

#endif /* _PYGMTL_XFORMS_H_ */
