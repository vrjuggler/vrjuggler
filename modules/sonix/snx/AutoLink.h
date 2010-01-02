/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <SNX heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _SNX_AUTO_LINK_H_
#define _SNX_AUTO_LINK_H_

#if defined(WIN32) || defined(WIN64)
#  include <snx/snxParam.h>

#  if defined(SNX_DEBUG) && defined(_DEBUG)
#     define SNX_LIB_RT_OPT "_d"
#  else
#     define SNX_LIB_RT_OPT ""
#  endif

#  define SNX_STRINGIZE(X) SNX_DO_STRINGIZE(X)
#  define SNX_DO_STRINGIZE(X) #X
#  define SNX_VERSION_STR SNX_STRINGIZE(__SNX_MAJOR__) "_" \
                          SNX_STRINGIZE(__SNX_MINOR__) "_" \
                          SNX_STRINGIZE(__SNX_PATCH__)

#  pragma comment(lib, "sonix" SNX_LIB_RT_OPT "-" SNX_VERSION_STR ".lib")

#  undef SNX_VERSION_STR
#  undef SNX_DO_STRINGIZE
#  undef SNX_STRINGIZE
#  undef SNX_LIB_RT_OPT

#endif

#endif /* _SNX_AUTO_LINK_H_ */
