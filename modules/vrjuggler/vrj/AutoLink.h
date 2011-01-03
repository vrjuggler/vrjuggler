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

// NOTE: This header file lacks the usual "include guard" stuff because it
// contains code that can vary based on how the including file is set up.
// The symbol VJ_LIB_NAME must be defined by including code in order for
// the #pragma directive below to work.

#if defined(WIN32) || defined(WIN64)

#  if ! defined(_VJ_PARAM_H_)
#     include <vrj/vrjParam.h>
#  endif

#  if defined(VJ_DEBUG) && defined(_DEBUG)
#     define VJ_LIB_RT_OPT "_d"
#  else
#     define VJ_LIB_RT_OPT ""
#  endif

#  define VJ_STRINGIZE(X) VJ_DO_STRINGIZE(X)
#  define VJ_DO_STRINGIZE(X) #X
#  define VJ_VERSION_STR VJ_STRINGIZE(__VJ_MAJOR__) "_" \
                         VJ_STRINGIZE(__VJ_MINOR__) "_" \
                         VJ_STRINGIZE(__VJ_PATCH__)

#  if ! defined(VJ_LIB_NAME)
#     error "Macro VJ_LIB_NAME not set (internal error)"
#  endif

#  pragma comment(lib, VJ_LIB_NAME VJ_LIB_RT_OPT "-" VJ_VERSION_STR ".lib")

#  undef VJ_VERSION_STR
#  undef VJ_DO_STRINGIZE
#  undef VJ_STRINGIZE
#  undef VJ_LIB_RT_OPT

#endif  /* defined(WIN32) || defined(WIN64) */
