dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

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
