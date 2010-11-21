dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

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
dnl Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl Boston, MA 02110-1301, USA.

dnl ===========================================================================
dnl Check for POSIX types.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_CHECK_TYPE_POSIX - Check for POSIX types such as uint32_t and
dnl                            int64_t.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Check for POSIX types such as uint32_t and int64_t.
dnl
dnl Usage:
dnl     DPP_CHECK_TYPE_POSIX
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CHECK_TYPE_POSIX],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   AC_CHECK_HEADERS(inttypes.h)

   if test "x$dpp_os_type" = "xWin32" ; then
      AC_CHECK_TYPE([u_int], [UINT])
      AC_CHECK_TYPE([int32_t], [__int32])
      AC_CHECK_TYPE([uint32_t], [unsigned int])
      AC_CHECK_TYPE([int64_t], [__int64])
   else
      AC_CHECK_TYPE([u_int], [unsigned int])
      AC_CACHE_CHECK([for uint32_t], [ac_cv_has_uint32_t],
         [AC_TRY_COMPILE([
#include <sys/types.h>
#ifdef HAVE_INTTYPES_H
#include<inttypes.h>
#endif
],
            [ uint32_t var; ],
            [ac_cv_has_uint32_t='yes'], [ac_cv_has_uint32_t='no'])])

      if test "x$ac_cv_has_uint32_t" = "xno" ; then
         AC_DEFINE([uint32_t], [unsigned int])
      fi
   fi
])
