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

#ifndef _JCCL_AUTO_LINK_H_
#define _JCCL_AUTO_LINK_H_

#if defined(WIN32) || defined(WIN64)
#  include <jccl/jcclParam.h>

#  if defined(JCCL_DEBUG)
#     define JCCL_LIB_RT_OPT "_d"
#  else
#     define JCCL_LIB_RT_OPT ""
#  endif

#  define JCCL_STRINGIZE(X) JCCL_DO_STRINGIZE(X)
#  define JCCL_DO_STRINGIZE(X) #X
#  define JCCL_VERSION_STR JCCL_STRINGIZE(__JCCL_MAJOR__) "_" \
                           JCCL_STRINGIZE(__JCCL_MINOR__) "_" \
                           JCCL_STRINGIZE(__JCCL_PATCH__)

#  pragma comment(lib, "jccl" JCCL_LIB_RT_OPT "-" JCCL_VERSION_STR ".lib")

#  undef JCCL_VERSION_STR
#  undef JCCL_DO_STRINGIZE
#  undef JCCL_STRINGIZE
#  undef JCCL_LIB_RT_OPT

#endif

#endif /* _JCCL_AUTO_LINK_H_ */
