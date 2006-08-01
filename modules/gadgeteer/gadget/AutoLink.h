/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _GADGET_AUTO_LINK_H_
#define _GADGET_AUTO_LINK_H_

#if defined(WIN32) || defined(WIN64)
#  include <gadget/gadgetParam.h>

#  if defined(GADGET_DEBUG) && !defined(_USE_RELEASE_RUNTIME)
#     define GADGET_LIB_RT_OPT "_d"
#  else
#     define GADGET_LIB_RT_OPT ""
#  endif

#  define GADGET_STRINGIZE(X) GADGET_DO_STRINGIZE(X)
#  define GADGET_DO_STRINGIZE(X) #X
#  define GADGET_VERSION_STR GADGET_STRINGIZE(__GADGET_MAJOR__) "_" \
                             GADGET_STRINGIZE(__GADGET_MINOR__) "_" \
                             GADGET_STRINGIZE(__GADGET_PATCH__)

#  pragma comment(lib, "gadget" GADGET_LIB_RT_OPT "-" GADGET_VERSION_STR ".lib")

#  undef GADGET_VERSION_STR
#  undef GADGET_DO_STRINGIZE
#  undef GADGET_STRINGIZE
#  undef GADGET_LIB_RT_OPT

#endif

#endif /* _GADGET_AUTO_LINK_H_ */
