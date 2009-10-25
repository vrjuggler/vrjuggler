dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2008 by Iowa State University
dnl
dnl Original Author:
dnl   Patrick Hartling
dnl ---------------------------------------------------------------------------
dnl VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Christopher Just,
dnl   Patrick Hartling, Kevin Meinert,
dnl   Carolina Cruz-Neira, Albert Baker
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Library General Public
dnl License as published by the Free Software Foundation; either
dnl version 2 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public
dnl License along with this library; if not, write to the
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.
dnl
dnl -----------------------------------------------------------------
dnl File:          types.m4,v
dnl Date modified: 2008/01/01 15:29:20
dnl Version:       1.12
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Check for commonly used types.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_TYPE_SOCKLEN_T - Check for the socklen_t type.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Check for the socklen_t type.  The headers sys/socket.h (if available),
dnl sys/types.h, and unistd.h are checked for the definition.  If socklen_t is
dnl not found, define it to int.
dnl
dnl Usage:
dnl     DPP_TYPE_SOCKLEN_T
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_TYPE_SOCKLEN_T],
[
   AC_CHECK_HEADERS([sys/socket.h])

   dnl The following checks for availability of the socklen_t type.  Since it
   dnl is supposed to be defined in sys/socket.h, the usual AC_CHECK_TYPE
   dnl method will not work.  On Linux, it's apparently in unistd.h.  Blah...
   AC_CACHE_CHECK([for socklen_t], [dpp_cv_has_socklen_t],
      [AC_TRY_COMPILE([
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
],
                     [ socklen_t var; ],
                     [dpp_cv_has_socklen_t='yes'],
                     [dpp_cv_has_socklen_t='no'])])

   if test "x$dpp_cv_has_socklen_t" = "xno" ; then
      AC_DEFINE([socklen_t], [int],
                [define to 'int' if your system does not have socklen_t])
   fi
])
