// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_AABOX_OPS_H_
#define _VRJ_NET_GMTL_AABOX_OPS_H_

#include <gmtl/AABox.h>
#include <gmtl/AABoxOps.h>


namespace gmtl
{
   template bool isEqual(const gmtl::AABoxf&, const gmtl::AABoxf&,
                         const float&);
   template bool isEqual(const gmtl::AABoxd&, const gmtl::AABoxd&,
                         const double&);
}


#endif /* _VRJ_NET_GMTL_AABOX_OPS_H_ */
