dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++
dnl
dnl Original Author:
dnl   Patrick Hartling
dnl ---------------------------------------------------------------------------
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
dnl
dnl -----------------------------------------------------------------
dnl File:          cppunit.m4,v
dnl Date modified: 2002/04/06 20:23:20
dnl Version:       1.5.2.1
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's CppUnit installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_CPPUNIT - Determine if the target system has CppUnit installed.
dnl
dnl Command-line options added:
dnl     --with-cppunitroot - Give the root directory of the CppUnit implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     CPPUNIT      - do we have cppunit on the system?
dnl     CPPUNITROOT     - The CppUnit installation directory.
dnl     LIBCPPUNIT   - The list of libraries to link for CppUnit appliations.
dnl     CPPUNIT_INCLUDES - Extra include path for the CppUnit header directory.
dnl     CPPUNIT_LDFLAGS  - Extra linker flags for the CppUnit library directory.
dnl ===========================================================================

dnl cppunit.m4,v 1.5.2.1 2002/04/06 20:23:20 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has CppUnit installed.  This
dnl adds command-line arguments --with-cppunitroot.
dnl
dnl Usage:
dnl     DPP_HAVE_CPPUNIT(cppunitroot [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     cppunitroot         - The default directory where the CppUnit
dnl                           installation is rooted.  This directory should
dnl                           contain an include/CPPUNIT directory with the CPPUNIT
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the CPPUNIT libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-cppunitroot command-line argument.
dnl     action-if-found     - The action to take if an CppUnit implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an CppUnit implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_CPPUNIT,
[
   dnl initialize returned data...
   CPPUNIT='n'
   LIBCPPUNIT=''
   CPPUNIT_INCLUDES=''
   CPPUNIT_LDFLAGS=''
   dpp_have_cppunit='no'
    
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl Define the root directory for the CppUnit installation.
   AC_ARG_WITH(cppunitroot,
               [  --with-cppunitroot=<PATH>   CppUnit installation directory   [default=$1]],
               CPPUNITROOT="$withval", CPPUNITROOT=$1)

   dnl Save these values in case they need to be restored later.
   dpp_save_CFLAGS="$CFLAGS"
   dpp_save_CPPFLAGS="$CPPFLAGS"
   dpp_save_INCLUDES="$INCLUDES"
   dpp_save_LDFLAGS="$LDFLAGS"

   dnl Add the user-specified CppUnit installation directory to these
   dnl paths.  Ensure that /usr/include and /usr/lib are not included
   dnl multiple times if $CPPUNITROOT is "/usr".
   if test "x$CPPUNITROOT" != "x/usr" ; then
      CPPFLAGS="$CPPFLAGS -I$CPPUNITROOT/include"
      INCLUDES="$INCLUDES -I$CPPUNITROOT/include"
      if test -d "$CPPUNITROOT/lib$LIBBITSUF" ; then
          LDFLAGS="-L$CPPUNITROOT/lib$LIBBITSUF $LDFLAGS"
      fi
   fi
   
   dnl debugging output...
   dnl echo "+ setting CPPFLAGS == $CPPFLAGS"
   dnl echo "+ setting INCLUDES == $INCLUDES"
   dnl echo "+ setting LDFLAGS == $LDFLAGS"

   CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

   echo "+ looking for CppUnit in $CPPUNITROOT/lib$LIBBITSUF and $CPPUNITROOT/include"

   dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   if test "x$dpp_os_type" = "xWin32" ; then
      AC_LANG_SAVE
      AC_LANG_C

      LIBS="$LIBS CppUnit32.lib"

      AC_CACHE_CHECK([for TestSuite in CppUnit32.lib],
                     [dpp_cv_TestSuite_cppunit_lib],
                     [AC_TRY_LINK([#include <cppunit/TestSuite.h>],
                        [TestSuite t;t.countTestCases();],
                        [dpp_cv_TestSuite_cppunit_lib='yes'],
                        [dpp_cv_TestSuite_cppunit_lib='no'])])

      AC_LANG_RESTORE
        
      if test "x$dpp_cv_TestSuite_cppunit_lib" = "xno" ; then
         ifelse([$3], , :, [$3])
      fi

      LIBS="$dpp_save_LIBS"

      if test "x$dpp_cv_TestSuite_cppunit_lib" = "xyes" ; then
         ifelse([$2], , :, [$2])
      else
         ifelse([$3], , :, [$3])
      fi

   dnl UNIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   else
      dpp_save_LIBS="$LIBS"

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      CPPFLAGS="$CPPFLAGS"
      INCLUDES="$INCLUDES"

      dnl AC_CHECK_LIB(cppunit, CppUnit::NotEqualException::type(),
      AC_CHECK_HEADER([cppunit/NotEqualException.h],
                      [dpp_have_cppunit='yes'], $3)
      dnl     AC_MSG_WARN(*** CppUnit will not be available (cppunit library not found) ***),
      dnl -lm)

      dnl This is necessary because AC_CHECK_LIB() adds -lopenal to
      dnl $LIBS.  We want to do that ourselves later.
      LIBS="$dpp_save_LIBS"
        
      if test "x$dpp_have_cppunit" = "xyes" ; then
         ifelse([$2], , :, [$2])
      fi

      DPP_LANG_RESTORE
   fi

   dnl If CppUnit API files were found, define this extra stuff that may be
   dnl helpful in some Makefiles.
   if test "x$dpp_have_cppunit" = "xyes" ; then
      if test "x$OS_TYPE" = "xUNIX" ;  then
         LIBCPPUNIT="-lcppunit -lm"
      else
         LIBCPPUNIT='cppunit.lib'
      fi

      if test "x$CPPUNITROOT" != "x/usr" ; then
         CPPUNIT_INCLUDES="-I$CPPUNITROOT/include"
         CPPUNIT_LDFLAGS="-L$CPPUNITROOT/lib$LIBBITSUF"
      fi
        
      CPPUNIT='yes'
   fi

   dnl Restore all the variables now that we are done testing.
   CFLAGS="$dpp_save_CFLAGS"
   CPPFLAGS="$dpp_save_CPPFLAGS"
   INCLUDES="$dpp_save_INCLUDES"
   LDFLAGS="$dpp_save_LDFLAGS"

   dnl Export all of the output vars for use by makefiles and configure
   dnl script.
   AC_SUBST(CPPUNITROOT)
   AC_SUBST(LIBCPPUNIT)
   AC_SUBST(CPPUNIT_INCLUDES)
   AC_SUBST(CPPUNIT_LDFLAGS)
])
