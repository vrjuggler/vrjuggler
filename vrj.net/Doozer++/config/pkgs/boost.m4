dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          boost.m4,v
dnl Date modified: 2004/01/09 23:46:09
dnl Version:       1.18
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's Boost installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_BOOST - Determine if the target system has Boost installed.
dnl
dnl Command-line options added:
dnl     --with-boost   - Give the root directory of the Boost implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     BOOST          - do we have boost on the system?
dnl     BOOST_ROOT     - The Boost installation directory.
dnl     BOOST_INCLUDES - Extra include path for the Boost header directory.
dnl     BOOST_LDFLAGS  - Extra library path for the Boost libraries.
dnl ===========================================================================

dnl boost.m4,v 1.18 2004/01/09 23:46:09 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has Boost installed.  This adds the
dnl command-line argument --with-boost.
dnl
dnl Usage:
dnl     DPP_HAVE_BOOST(version, boost-root [, boost-inc, [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     version             - Minimum required version.
dnl     boost-root          - The default directory where the Boost
dnl                           installation is rooted.  This directory should
dnl                           contain an include/boost directory with the Boost
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the Boost libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-boost command-line argument.
dnl     boost-inc           - The directory where the Boost headers can be
dnl                           found.  This is used to override the use of
dnl                           <boost-root>/include as the default path to
dnl                           those headers.
dnl     action-if-found     - The action to take if an Boost implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an Boost implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_BOOST,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   
   dnl initialize returned data...
   BOOST='n'
   BOOST_INCLUDES=''
   dpp_have_boost='no'

   dnl Define the root directory for the Boost installation.
   AC_ARG_WITH([boost],
               [  --with-boost=<PATH>     Boost installation directory    [default=$2]],
               [BOOST_ROOT="$withval"], [BOOST_ROOT=$2])

   AC_ARG_WITH(boost-includes,
               [  --with-boost-includes=<DIR>
                          Boost header file directory     [default=$2/include]],
               [_with_boost_inc="$withval"],
               ifelse([$3], , [_with_boost_inc=no], [_with_boost_inc=$3]))

   dnl Save these values in case they need to be restored later.
   dpp_save_CPPFLAGS="$CPPFLAGS"

   dnl Add the user-specified Boost installation directory to the preprocessor
   dnl arguments.  Ensure that /usr/include is not included multiple times if
   dnl $BOOST_ROOT is "/usr".  The goal here is to let the user specify either
   dnl the Boost root directory, the Boost include directory, or both.
   if test "x$BOOST_ROOT" != "x/usr" -o "x${_with_boost_inc}" != "xno" ; then
      dnl No Boost include directory, so append "/include" to $BOOST_ROOT.
      if test "x${_with_boost_inc}" = "xno" ; then
         dpp_boost_incdir="$BOOST_ROOT/include"
      dnl We have a Boost include directory, so we'll use it and hope for
      dnl the best.
      else
         dpp_boost_incdir="${_with_boost_inc}"
      fi

      CPPFLAGS="$CPPFLAGS -I$dpp_boost_incdir"
   fi

   dnl This is not quite right because it restricts the user to testing for
   dnl the exact version required.
   if test ! -d "$dpp_boost_incdir/boost" ; then
      dpp_boost_ver=`echo $1 | sed -e 's/\./ /g' | awk '{ print $[1] "_" $[2] }' -`
      dpp_boost_incdir="$dpp_boost_incdir/boost-$dpp_boost_ver"

      CPPFLAGS="$dpp_save_CPPFLAGS -I$dpp_boost_incdir"
   fi

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CHECK_HEADER([boost/version.hpp], [dpp_have_boost='yes'], [$5])

   DPP_LANG_RESTORE

   if test "x$dpp_have_boost" = "xyes" ; then
      dnl This expression passed to grep(1) is not great.  It could stand to
      dnl test for one or more whitespace characters instead of just one for
      dnl book-ending BOOST_VERSION.
      BOOST_VERSION=`grep 'define BOOST_VERSION ' $dpp_boost_incdir/boost/version.hpp | awk '{ print $[3] }' -`
      dpp_boost_patch=`expr $BOOST_VERSION % 100`
      dpp_boost_minor=`expr $BOOST_VERSION / 100 % 1000`
      dpp_boost_major=`expr $BOOST_VERSION / 100000`
      dpp_boost_version="$dpp_boost_major.$dpp_boost_minor.$dpp_boost_patch"

      AC_MSG_CHECKING([whether Boost version is >= $1])
      AC_MSG_RESULT([$dpp_boost_version])
      DPP_VERSION_CHECK([$dpp_boost_version], [$1], , [dpp_have_boost=no])
   fi

   dnl If Boost API files were found, define this extra stuff that may be
   dnl helpful in some Makefiles.
   if test "x$dpp_have_boost" = "xyes" ; then
      if test "x$dpp_boost_incdir" != "x" ; then
         BOOST_INCLUDES="-I$dpp_boost_incdir"
      fi

      if test "x$BOOST_ROOT" != "x/usr" ; then
         BOOST_LDFLAGS="-L$BOOST_ROOT/lib"
      fi

      BOOST_LDFLAGS_LINK_EXE="/libpath:$BOOST_ROOT/lib"
      BOOST='yes'

      ifelse([$4], , :, [$4])
   else
      ifelse([$5], , :, [$5])
   fi

   dnl Extend the include path for the Boost C++ C compatibility headers if
   dnl we are on IRIX and not using g++.
   if test "x$PLATFORM" = "xIRIX" -a "x$GXX" != "xyes" ; then
      BOOST_INCLUDES="$BOOST_INCLUDES -I$dpp_boost_incdir/boost/compatibility/cpp_c_headers"
   fi

   dnl Restore all the variables now that we are done testing.
   CPPFLAGS="$dpp_save_CPPFLAGS"

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(BOOST_ROOT)
   AC_SUBST(BOOST_INCLUDES)
   AC_SUBST(BOOST_LDFLAGS)
   AC_SUBST(BOOST_LDFLAGS_LINK_EXE)
])
