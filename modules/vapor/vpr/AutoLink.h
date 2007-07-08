/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef _VPR_AUTO_LINK_H_
#define _VPR_AUTO_LINK_H_

#if defined(WIN32) || defined(WIN64)
#  include <vpr/vprParam.h>

#  if defined(VPR_DEBUG) && defined(_DEBUG)
#     define VPR_LIB_RT_OPT "_d"
#  else
#     define VPR_LIB_RT_OPT ""
#  endif

#  define VPR_STRINGIZE(X) VPR_DO_STRINGIZE(X)
#  define VPR_DO_STRINGIZE(X) #X
#  define VPR_VERSION_STR VPR_STRINGIZE(__VPR_MAJOR__) "_" \
                          VPR_STRINGIZE(__VPR_MINOR__) "_" \
                          VPR_STRINGIZE(__VPR_PATCH__)

#  pragma comment(lib, "vpr" VPR_LIB_RT_OPT "-" VPR_VERSION_STR ".lib")

#  undef VPR_VERSION_STR
#  undef VPR_DO_STRINGIZE
#  undef VPR_STRINGIZE
#  undef VPR_LIB_RT_OPT

#endif

#endif /* _VPR_AUTO_LINK_H_ */
