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
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.

dnl ===========================================================================
dnl Perform tests related to the use of GCC as the compiler.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WITH_GCC         - Adds a command-line argument telling whether or
dnl                            not to force the use of GCC as the compiler
dnl                            suite.
dnl     DPP_SETUP_GCC        - Setup software development options that are
dnl                            tied to GCC.
dnl     DPP_GPLUSPLUS_VER    - Verify that the version of g++ available is at
dnl                            least the given version.
dnl     DPP_GPLUSPLUS_MODERN - Verify that the version of g++ is at least
dnl                            2.91 (EGCS 1.1.2).
dnl
dnl Command-line options added:
dnl     --with-gcc 
dnl
dnl Variables defined:
dnl     USE_GCC - This is set to "yes" if GCC is to be used.  If not, it will
dnl               be "no" or the empty string.  This is set by the
dnl               DPP_WITH_GCC macro.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Force the use of GCC as the compiler suite.
dnl
dnl Usage:
dnl     DPP_WITH_GCC(default-GCC-use)
dnl
dnl Arguments:
dnl     default-GCC-use - "yes" to default to using GCC or "no" to default to
dnl                       not using GCC.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_WITH_GCC],
[
   dnl Force the use of GCC as the compiler.
   AC_ARG_WITH(gcc,
               [  --with-gcc              Force the use of GCC as the compiler],
               USE_GCC="$withval", USE_GCC=$1)
   AC_SUBST(USE_GCC)
])

dnl ---------------------------------------------------------------------------
dnl Setup software development options that are tied to GCC.
dnl
dnl Usage:
dnl     DPP_SETUP_GCC
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_SETUP_GCC],
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
   C_WARNS_LEVEL_0='-w'
   C_WARNS_LEVEL_1='-Wsystem-headers'
   C_WARNS_LEVEL_2="$C_WARNS_LEVEL_1 -Wall -Wno-format-y2k"
   C_WARNS_LEVEL_3="$C_WARNS_LEVEL_2 -W -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith"
   C_WARNS_LEVEL_4="$C_WARNS_LEVEL_3 -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wshadow -Wcast-align"
   C_WARNS_LEVEL_5="$C_WARNS_LEVEL_4"

   CXX_WARNS_LEVEL_0='-w'
])

dnl ---------------------------------------------------------------------------
dnl Verify that g++ is at least the given version.
dnl
dnl Usage:
dnl     DPP_GPLUSPLUS_VER(minimum-version [, action-if-not-found])
dnl
dnl Arguments:
dnl     minimum-version     - The minimum allowable version number.
dnl     action-if-not-found - The action to take if the version is less than
dnl                           that specified.  Optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_GPLUSPLUS_VER],
[
   AC_REQUIRE([DPP_PROG_CXX])

   dpp_gcc_ver=`$CXX -dumpversion` 
   DPP_VERSION_CHECK_MSG_NO_CACHE([$CXX], [$dpp_gcc_ver], [$1], , , [$2])
])

dnl ---------------------------------------------------------------------------
dnl Verify that g++ is at least 2.91 (aka, EGCS 1.1.2) since it implements the
dnl more modern C++ features such as templates.
dnl
dnl Usage:
dnl     DPP_GPLUSPLUS_MODERN([action-if-not-found])
dnl
dnl     action-if-not-found - The action to take if the version is less than
dnl                           that specified.  Optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_GPLUSPLUS_MODERN],
[
   DPP_GPLUSPLUS_VER([2.91],
      [AC_CHECK_PROG([CC], [egcc], [egcc], [$1])
       AC_CHECK_PROG([CXX], [eg++], [eg++], [$1])])
])
