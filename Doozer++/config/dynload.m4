dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2007 by Iowa State University
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
dnl -----------------------------------------------------------------
dnl File:          dynload.m4,v
dnl Date modified: 2006/12/31 16:54:59
dnl Version:       1.6
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's CppUnit installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_CHECK_DYNLOAD_LIB - Determines what library, if any, is required
dnl                             to perform dynamic loading of code at
dnl                             runtime.
dnl
dnl Varitables defined:
dnl     DYN_LOAD_LIB - The linker argument needed to link against the
dnl                    operating system's dynamic code loading library (if one
dnl                    is required at all).
dnl ===========================================================================

dnl dynload.m4,v 1.6 2006/12/31 16:54:59 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determines what library, if any, is required to perform dynamic loading of
dnl code at runtime.  This is primiarly intended to deal with the case when
dnl dlopen(3) is in libc on some platforms and in libdl on others.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CHECK_DYNLOAD_LIB],
[
   dpp_save_LIBS="$LIBS"
   AC_CHECK_FUNC([dlopen],
      [DYN_LOAD_LIB=''
       ifelse([$1], , :, [$1])],
      [AC_CHECK_LIB([dl], [dlopen],
         [DYN_LOAD_LIB='-ldl'
          ifelse([$1], , :, [$1])],
         [$2])])
   LIBS="$dpp_save_LIBS"
])
