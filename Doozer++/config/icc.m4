dnl Doozer++ is (C) Copyright 2000-2009 by Iowa State University
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
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.

dnl ===========================================================================
dnl Perform tests related to the use of the Intel compiler.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WITH_ICC         - Adds a command-line argument telling whether or
dnl                            not to force the use of the Intel compiler.
dnl     DPP_SETUP_ICC        - Setup software development options that are
dnl                            tied to the Intel compiler.
dnl     DPP_GPLUSPLUS_VER    - Verify that the version of g++ available is at
dnl                            least the given version.
dnl     DPP_GPLUSPLUS_MODERN - Verify that the version of g++ is at least
dnl                            2.91 (EGCS 1.1.2).
dnl
dnl Command-line options added:
dnl     --with-gcc 
dnl
dnl Variables defined:
dnl     USE_ICC - This is set to "yes" if the Intel compiler is to be used.
dnl               If not, it will be "no" or the empty string.  This is set by
dnl               the DPP_WITH_ICC macro.
dnl     ICC     - This is set to "yes" if the Intel C compiler is the C
dnl               compiler being used.
dnl     ICPC    - This is set to "yes" if the Intel C++ compiler is the C++
dnl               compiler being used.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Force the use of the Intel compiler.
dnl
dnl Usage:
dnl     DPP_WITH_ICC(default-ICC-use)
dnl
dnl Arguments:
dnl     default-ICC-use - "yes" to default to using the Intel compiler or
dnl                       "no" to default to not using the Intel compiler.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_WITH_ICC],
[
   dnl Force the use of the Intel compiler.
   AC_ARG_WITH(gcc,
               [  --with-gcc              Force the use of the Intel compiler],
               USE_ICC="$withval", USE_ICC=$1)
   AC_SUBST(USE_ICC)
])

dnl ---------------------------------------------------------------------------
dnl Setup software development options that are tied to the Intel compiler.
dnl
dnl Usage:
dnl     DPP_SETUP_ICC
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_SETUP_ICC],
[
   AR='ar'
   ARFLAGS='-ruv'
   LD='$(CXX) -shared'
   LDOPTS=''
   C_DLL='$(CC) -shared'
   CXX_DLL='$(CXX) -shared'
   C_PLUGIN='$(CC) -shared'
   CXX_PLUGIN='$(CXX) -shared'
   OPT_FLAGS="-O$1"
   DBG_FLAGS='-g'
   LDFLAGS_STATIC='-static'
   LDFLAGS_DYNAMIC=''

   # These are based on the FreeBSD 5.x build system settings.
   C_WARNS_LEVEL_0="-w"
   C_WARNS_LEVEL_1="-Wall -wd180,279,310,383,444,810,981,1418,1469"
   C_WARNS_LEVEL_2="-Wall -wd180,279,310,383,810,981,1418,1469"
   C_WARNS_LEVEL_3="-Wall -wd279,310,981,1418,1469"
   C_WARNS_LEVEL_4="-Wall -wd279,981,1469 -Wcheck"
   C_WARNS_LEVEL_5="-Wall -Wcheck"

   CXX_WARNS_LEVEL_0='-w'
])

dnl ---------------------------------------------------------------------------
dnl Determines if the C compiler (as detected by AC_PROG_CC or comparable
dnl macro) is the Intel C compiler.
dnl
dnl Usage:
dnl    DPP_C_COMPILER_INTEL
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_C_COMPILER_INTEL],
[
   AC_CACHE_CHECK([whether we are using the Intel C compiler],
      [dpp_cv_C_compiler_intel],
      [AC_TRY_COMPILE([],
[
#ifndef __INTEL_COMPILER
   choke me
#endif
],
         [dpp_C_compiler_intel='yes'],
         [dpp_C_compiler_intel='no'])
      dpp_cv_C_compiler_intel="$dpp_C_compiler_intel"])
   ICC="$dpp_cv_C_compiler_intel"
])

dnl ---------------------------------------------------------------------------
dnl Determines if the C++ compiler (as detected by AC_PROG_CXX or comparable
dnl macro) is the Intel C++ compiler.
dnl
dnl Usage:
dnl    DPP_CXX_COMPILER_INTEL
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_COMPILER_INTEL],
[
   AC_CACHE_CHECK([whether we are using the Intel C++ compiler],
      [dpp_cv_CXX_compiler_intel],
      [AC_TRY_COMPILE([],
[
#ifndef __INTEL_COMPILER
   choke me
#endif
],
         [dpp_CXX_compiler_intel='yes'],
         [dpp_CXX_compiler_intel='no'])
      dpp_cv_CXX_compiler_intel="$dpp_CXX_compiler_intel"])
   ICPC="$dpp_cv_CXX_compiler_intel"
])
