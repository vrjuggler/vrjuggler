dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
dnl
dnl Original Author:
dnl   Patrick Hartling
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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Check for POSIX types.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_CHECK_TYPE_POSIX - Check for POSIX types such as int32_t and
dnl                            caddr_t.
dnl ===========================================================================

dnl posix.m4,v 1.7 2001/03/03 17:44:41 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Check for POSIX types such as int32_t and caddr_t.
dnl
dnl Usage:
dnl     DPP_CHECK_TYPE_POSIX
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CHECK_TYPE_POSIX,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    AC_CHECK_HEADERS(inttypes.h)

    if test "x$dpp_os_type" = "xWin32" ; then
        AC_CHECK_TYPE(u_int, UINT)
        AC_CHECK_TYPE(int32_t, __int32)
        AC_CHECK_TYPE(uint32_t, unsigned int)
        AC_CHECK_TYPE(int64_t, __int64)
    else
        AC_CHECK_TYPE(u_int, unsigned int)
        AC_CHECK_TYPE(uint32_t, unsigned int)
    fi

    AC_CHECK_TYPE(caddr_t, char*)
])
