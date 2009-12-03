dnl Doozer++ is (C) Copyright 2000-2009 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

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
