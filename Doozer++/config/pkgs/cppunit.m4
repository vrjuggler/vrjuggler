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
dnl File:          cppunit.m4,v
dnl Date modified: 2006/12/31 16:55:00
dnl Version:       1.20
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's CppUnit installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_CPPUNIT - Determine if the target system has CppUnit
dnl                        installed.
dnl
dnl Command-line options added:
dnl     --with-cppunit   - Give the root directory of the CppUnit
dnl                        implementation installation.
dnl
dnl Variables defined:
dnl     CPPUNIT            - Do we have CppUnit on the system?
dnl     CPPUNIT_ROOT       - The CppUnit installation directory.
dnl     LIBCPPUNIT         - The list of libraries to link for CppUnit
dnl                          appliations.
dnl     CPPUNIT_INCLUDES   - Extra include path for the CppUnit header
dnl                          directory.
dnl     CPPUNIT_LDFLAGS    - Extra linker flags for the CppUnit library
dnl                          directory.
dnl     CPPUNIT_EXTRA_LIBS - Extra libraries that are needed for linking
dnl                          against the CppUnit library directory.
dnl ===========================================================================

dnl cppunit.m4,v 1.20 2006/12/31 16:55:00 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has CppUnit installed.  This
dnl adds command-line arguments --with-cppunit.
dnl
dnl Usage:
dnl     DPP_HAVE_CPPUNIT([cppunit-version [, cppunit-root [, action-if-found [, action-if-not-found]]]])
dnl
dnl Arguments:
dnl     cppunit-version     - The minimum required version of CppUnit.
dnl     cppunit-root        - The default directory where the CppUnit
dnl                           installation is rooted.  This directory should
dnl                           contain an include/cppunit directory with the
dnl                           CppUnit headers and a lib directory with the
dnl                           CppUnit library.  The value given is used as the
dnl                           default value of the --with-cppunit command-line
dnl                           argument.
dnl     action-if-found     - The action to take if an CppUnit implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an CppUnit implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_CPPUNIT],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_CHECK_DYNLOAD_LIB])

   dnl Initialize returned data.
   CPPUNIT='n'
   LIBCPPUNIT=''
   CPPUNIT_INCLUDES=''
   CPPUNIT_LDFLAGS=''
   dpp_have_cppunit='no'

   dnl Define the root directory for the CppUnit installation.
   AC_ARG_WITH(cppunit,
               [  --with-cppunit=<PATH>   CppUnit installation directory  [default=$2]],
               CPPUNIT_ROOT="$withval", CPPUNIT_ROOT=$2)

   if test "x$CPPUNIT_ROOT" != "xno" ; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CXXFLAGS="$CXXFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"
      dpp_save_LIBS="$LIBS"

      dnl Add the user-specified CppUnit installation directory to these
      dnl paths.  Ensure that /usr/include and /usr/lib are not included
      dnl multiple times if $CPPUNIT_ROOT is "/usr".
      if test "x$CPPUNIT_ROOT" != "x/usr" ; then
         CPPFLAGS="$CPPFLAGS -I$CPPUNIT_ROOT/include"

         if test "x$OS_TYPE" = "xWin32" ; then
            LDFLAGS="/libpath:$CPPUNIT_ROOT/lib $LDFLAGS"
         else
            LDFLAGS="-L$CPPUNIT_ROOT/lib $LDFLAGS"
         fi
      fi

      dpp_save_PATH="$PATH"

      if test "x$CPPUNIT_ROOT" != "x" ; then
         PATH="$CPPUNIT_ROOT/bin:$PATH"
      fi

      AC_PATH_PROG(CPPUNIT_CONFIG, cppunit-config, no)
      PATH="$dpp_save_PATH"

      dnl Do a sanity check to ensure that $CPPUNIT_CONFIG actually works.
      if ! (eval $CPPUNIT_CONFIG --cflags >/dev/null 2>&1) 2>&1 ; then
         CPPUNIT_CONFIG='no'
      fi

      if test "x$CPPUNIT_CONFIG" != "xno" ; then
         min_cppunit_version=ifelse([$1], ,0.0.1,$1)
         CPPUNIT_VERSION=`$CPPUNIT_CONFIG --version`

         DPP_VERSION_CHECK_MSG_NO_CACHE([CppUnit], [$CPPUNIT_VERSION],
                                        [$min_cppunit_version], , [$4])
      fi

      CXXFLAGS="$CXXFLAGS $ABI_FLAGS"

      if test "x$OS_TYPE" = "xWin32" ; then
         dpp_cppunit_lib='cppunit.lib'
      else
         dpp_cppunit_lib='-lcppunit'
      fi

      LIBS="$LIBS $dpp_cppunit_lib $DYN_LOAD_LIB"

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      AC_CACHE_CHECK([for CppUnit::TestSuite::countTestCases() in $dpp_cppunit_lib],
         [dpp_cv_CppUnit_TestSuite_countTestCases_in_cppunit],
         [AC_TRY_LINK([#include <cppunit/TestSuite.h>],
            [CppUnit::TestSuite t; t.countTestCases();],
            [dpp_cv_CppUnit_TestSuite_countTestCases_in_cppunit='yes'],
            [dpp_cv_CppUnit_TestSuite_countTestCases_in_cppunit='no'])])

      DPP_LANG_RESTORE

      dpp_have_cppunit="$dpp_cv_CppUnit_TestSuite_countTestCases_in_cppunit"

      dnl Restore all the variables now that we are done testing.
      CXXFLAGS="$dpp_save_CXXFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      LDFLAGS="$dpp_save_LDFLAGS"
      LIBS="$dpp_save_LIBS"

      if test "x$dpp_have_cppunit" = "xyes" ; then
         ifelse([$3], , :, [$3])
      else
         AC_MSG_WARN([*** CppUnit will not be available (cppunit library not found) ***])
         ifelse([$4], , :, [$4])
      fi

      dnl If CppUnit API files were found, define this extra stuff that may be
      dnl helpful in some Makefiles.
      if test "x$dpp_have_cppunit" = "xyes" ; then
         LIBCPPUNIT="$dpp_cppunit_lib"
         CPPUNIT_EXTRA_LIBS="$DYN_LOAD_LIB"

         if test "x$CPPUNIT_ROOT" != "x/usr" ; then
            CPPUNIT_INCLUDES="-I$CPPUNIT_ROOT/include"

            if test "x$OS_TYPE" = "xWin32" ; then
               CPPUNIT_LDFLAGS="/libpath:$CPPUNIT_ROOT/lib"
            else
               CPPUNIT_LDFLAGS="-L$CPPUNIT_ROOT/lib"
            fi
         fi

         CPPUNIT='yes'
      fi
   fi

   dnl Export all of the output vars for use by makefiles and configure
   dnl script.
   AC_SUBST(CPPUNIT_ROOT)
   AC_SUBST(LIBCPPUNIT)
   AC_SUBST(CPPUNIT_INCLUDES)
   AC_SUBST(CPPUNIT_LDFLAGS)
   AC_SUBST(CPPUNIT_EXTRA_LIBS)
])
