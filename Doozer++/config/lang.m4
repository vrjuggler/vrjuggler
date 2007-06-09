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
dnl File:          lang.m4,v
dnl Date modified: 2006/12/31 16:54:59
dnl Version:       1.9
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl This file provides simple macros for language-specific state changes in a
dnl configure script.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_LANG_SAVE      - Save the current language state.
dnl     DPP_LANG_RESTORE   - Restore the previously saved language state.
dnl     DPP_LANG_C         - Set the current language to C.
dnl     DPP_LANG_CPLUSPLUS - Set the current language to C++.
dnl     DPP_LANG_JAVA      - Set the current language to Java.
dnl ===========================================================================

dnl lang.m4,v 1.9 2006/12/31 16:54:59 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Save the current language state.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_LANG_SAVE], [ AC_LANG_SAVE ])

dnl ---------------------------------------------------------------------------
dnl Restore the previous language state.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_LANG_RESTORE], [ AC_LANG_RESTORE ])

dnl ---------------------------------------------------------------------------
dnl Set the current language to C.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_LANG_C], [ AC_LANG_C ])

dnl ---------------------------------------------------------------------------
dnl Set the current language to C++.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_LANG_CPLUSPLUS],
[
   AC_LANG_CPLUSPLUS
   ac_ext='cpp'
])

dnl ---------------------------------------------------------------------------
dnl Set the current language to Java.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_LANG_JAVA],
[
])
