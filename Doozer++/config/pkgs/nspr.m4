dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2008 by Iowa State University
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
dnl File:          nspr.m4,v
dnl Date modified: 2008/01/01 15:29:21
dnl Version:       1.45
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide command-line functions and checks for including the NSPR library
dnl in the configuration process.  This does the substitution of the variable
dnl defined here (described below).
dnl
dnl If the NSPR was compiled to use POSIX threads, the macros
dnl DPP_CC_PTHREAD_ARG() and DPP_GET_PTHREAD_LIB() must have been executed
dnl before any macro here is called.  See pthreads.m4 for more information.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_NSPR - Determine if the NSPR is on the system.
dnl     DPP_NSPR_VER  - Make sure that the installed NSPR version is at least
dnl                     the given version number.
dnl
dnl Command-line options added:
dnl     --with-nspr          - Define the path to the NSPR installation.
dnl     --with-nspr-includes - Define the path to the NSPR headers.
dnl     --with-nspr-libs     - Define the path to the NSPR libraries.
dnl     --with-nsprver       - Define the NSPR version number.
dnl
dnl Variables defined:
dnl     NSPR_ROOT             - The root of the NSPR installation.
dnl     NSPR_INCLUDE_DIR      - The directory containing the root of the NSPR
dnl                             header files. This may contain a subdirectory
dnl                             called 'nspr'.
dnl     NSPR_INCLUDES         - The compiler option giving the NSPR include
dnl                             path.
dnl     NSPR_LIB              - The linker option for the basic NSPR library
dnl                             (safe for use with the msvccc shell script).
dnl     PLC_LIB               - The linker option for the NSPR PLC library
dnl                             (safe for use with the msvccc shell script).
dnl     PLDS_LIB              - The linker option for the NSPR PLDS library
dnl                             (safe for use with the msvccc shell script).
dnl     NSPR_LDFLAGS          - The linker option for finding the NSPR
dnl                             libraries (safe for use with the msvccc shell
dnl                             script).
dnl     NSPR_LIBDIR           - The directory containing the NSPR libraries.
dnl     NSPR_LIB_MSVCCC       - The linker option for the basic NSPR library
dnl                             for use with the msvccc shell script.
dnl     PLC_LIB_MSVCCC        - The linker option for the NSPR PLC library
dnl                             for use with the msvccc shell script.
dnl     PLDS_LIB_MSVCCC       - The linker option for the NSPR PLDS library
dnl                             for use with the msvccc shell script.
dnl     NSPR_LDFLAGS_MSVCCC   - The linker option for finding the NSPR
dnl                             libraries for use with the msvccc shell
dnl                             script.
dnl     NSPR_LIB_LINK_EXE     - The linker option for the basic NSPR library
dnl                             for use with MS Visual C++ LINK.EXE.
dnl     PLC_LIB_LINK_EXE      - The linker option for the NSPR PLC library
dnl                             for use with MS Visual C++ LINK.EXE.
dnl     PLDS_LIB_LINK_EXE     - The linker option for the NSPR PLDS library
dnl                             for use with MS Visual C++ LINK.EXE.
dnl     NSPR_LDFLAGS_LINK_EXE - The linker option for finding the NSPR
dnl                             libraries for use with MS Visual C++ LINK.EXE.
dnl     NSPR_LIB_STATIC       - Full path to the static NSPR base library.
dnl     PLC_LIB_STATIC        - Full path to the static NSPR PLC library.
dnl     PLDS_LIB_STATIC       - Full path to the static NSPR PLDS library.
dnl ===========================================================================

dnl nspr.m4,v 1.45 2008/01/01 15:29:21 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl State that NSPR threads are in use within NSPR.
dnl
dnl Usage:
dnl     DPP_ENABLE_NSPR_THREADS([default-nspr-thread-enableval])
dnl
dnl Arguments:
dnl     default-nspr-thread-enableval - The default value for enabling or
dnl                                     disabling pthreads.  This should be
dnl                                     either "yes" or "no".  This argument
dnl                                     is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ENABLE_NSPR_THREADS],
[
   AC_ARG_ENABLE(nspr-threads,
                 [  --enable-nspr-threads   Use NSPR threads                [default=$1]],
                 NSPR_THREADS_ENABLED="$enableval", NSPR_THREADS_ENABLED=$1)
])

dnl ---------------------------------------------------------------------------
dnl Verify that the NSPR library and headers are where they are expected to
dnl be.
dnl
dnl Usage:
dnl     DPP_HAVE_NSPR(version [, pthreads-enabled [, action-if-not-found]])
dnl
dnl Arguments:
dnl     version             - Minimum required NSPR version.
dnl     pthreads-enabled    - Set to "yes" if the NSPR was compiled to use
dnl                           POSIX threads.
dnl     action-if-not-found - Action to take if the NSPR could not be found or
dnl                           if the NSPR version requirement is not met.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_NSPR],
[
   if test "x$2" = "x" ; then
      AC_REQUIRE([DPP_ENABLE_NSPR_THREADS])
      true
   fi

   if test "x$PTHREADS_ENABLED" = "xyes" -o "x$2" = "xyes" ; then
      AC_REQUIRE([DPP_CC_PTHREAD_ARG])
      AC_REQUIRE([DPP_CC_PTHREADS_ARG])
      AC_REQUIRE([DPP_GET_POSIX_SEMAPHORE_LIB])
      true
   fi

   dnl -----------------------------------------------------------------------
   dnl Define the root directory for the NSPR API installation.
   dnl The default is /usr.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(nspr,
               [  --with-nspr=<PATH>      NSPR installation directory     [default=/usr]],
               NSPR_ROOT="$withval", NSPR_ROOT='/usr')

   AC_ARG_WITH(nspr-includes,
               [  --with-nspr-includes=<DIR>
                          NSPR headers are in DIR         [default=/usr/include]],
               _with_nspr_inc="$withval", _with_nspr_inc="$NSPR_ROOT/include")

   AC_ARG_WITH(nspr-libs,
               [  --with-nspr-libs=<DIR>  NSPR libraries are in DIR       [default=/usr/lib]],
               _with_nspr_lib="$withval", _with_nspr_lib="$NSPR_ROOT/lib")

   if test "x$NSPR_ROOT" != "xno" ; then
      DPP_LANG_SAVE
      DPP_LANG_C

      dnl Add the user-specified NSPR installation directory to the
      dnl preprocessor arguments.  Ensure that /usr/include is not included
      dnl multiple times if $NSPR_ROOT is "/usr".  The goal here is to let
      dnl the user specify either the NSPR root directory, the NSPR include
      dnl directory, or both.
      if test "x$NSPR_ROOT" != "x/usr" -o "x${_with_nspr_inc}" != "x/usr/include"
      then
         dnl No NSPR include directory, so append "/include" to $NSPR_ROOT.
         if test "x${_with_nspr_inc}" = "x/usr/include" ; then
            dpp_nspr_incdir="$NSPR_ROOT/include"
         dnl We have a NSPR include directory, so we'll use it and hope for
         dnl the best.
         else
            dpp_nspr_incdir="${_with_nspr_inc}"
         fi
      else
         dpp_nspr_incdir="$NSPR_ROOT/include"
      fi

      dnl Basically repeat the above for the library directory.
      if test "x$NSPR_ROOT" != "x/usr" -o "x${_with_nspr_lib}" != "x/usr/lib"
      then
         dnl No NSPR library directory, so append "/lib" to $NSPR_ROOT.
         if test "x${_with_nspr_lib}" = "x/usr/lib" ; then
            if test "lib$LIBBITSUF" != "lib" ; then
               libdirs="$NSPR_ROOT/lib$LIBBITSUF $NSPR_ROOT/lib"
            else
               libdirs="$NSPR_ROOT/lib"
            fi
         dnl We have a NSPR library directory, so we'll use it and hope for
         dnl the best.
         else
            libdirs="${_with_nspr_lib}"
         fi
      else
         if test "lib$LIBBITSUF" != "lib" ; then
            libdirs="$NSPR_ROOT/lib$LIBBITSUF $NSPR_ROOT/lib"
         else
            libdirs="$NSPR_ROOT/lib"
         fi
      fi

      dnl Check the NSPR version if a version number was given.
      if test "x$1" != "x" ; then
         DPP_NSPR_VER([$NSPR_ROOT], [$dpp_nspr_incdir], $1, $3)
      else
         AC_MSG_ERROR([*** No NSPR version given! ***])
      fi

      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"
      dpp_save_LIBS="$LIBS"

      CPPFLAGS="$CPPFLAGS -I$dpp_nspr_incdir"

      AC_CHECK_HEADERS([nspr.h nspr/nspr.h])

      if test "x$dpp_os_type" = "xWin32" ; then
         NSPR_LIB_NAME_PREFIX='lib'
      fi

      libname="${NSPR_LIB_NAME_PREFIX}nspr4"
      LIBS="-l$libname $PTHREAD_LIB $SEM_LIB"

      for l in $libdirs ; do
         cur_nspr_libdir="$l"
         LDFLAGS="$PTHREAD_ARG -L$cur_nspr_libdir $dpp_save_LDFLAGS"

         AC_MSG_CHECKING([for PR_Initialized in $libname in $cur_nspr_libdir])
         AC_TRY_LINK([
#if defined(HAVE_NSPR_H)
#include <nspr.h>
#else
#include <nspr/nspr.h>
#endif
],
            [PR_Initialized();], [dpp_have_nspr='yes'], [dpp_have_nspr='no'])
         AC_MSG_RESULT([$dpp_have_nspr])

         if test "x$dpp_have_nspr" = "xyes" ; then
            NSPR_LIBDIR="$cur_nspr_libdir"
            break
         fi
      done

      DPP_LANG_RESTORE

      dpp_nspr_lib="${NSPR_LIB_NAME_PREFIX}nspr$NSPR_VER"
      dpp_plc_lib="${NSPR_LIB_NAME_PREFIX}plc$NSPR_VER"
      dpp_plds_lib="${NSPR_LIB_NAME_PREFIX}plds$NSPR_VER"

      CPPFLAGS="$dpp_save_CPPFLAGS"
      LDFLAGS="$dpp_save_LDFLAGS"
      LIBS="$dpp_save_LIBS"

      if test "x$dpp_have_nspr" = "xyes" ; then
         NSPR_INCLUDE_DIR="$dpp_include_path"
         NSPR_INCLUDES="-I$dpp_include_path"
         NSPR_LIB="-l$dpp_nspr_lib"
         PLC_LIB="-l$dpp_plc_lib"
         PLDS_LIB="-l$dpp_plds_lib"
         NSPR_LDFLAGS="-L$dpp_nspr_libdir"

         NSPR_LIB_MSVCCC="-l$dpp_nspr_lib"
         PLC_LIB_MSVCCC="-l$dpp_plc_lib"
         PLDS_LIB_MSVCCC="-l$dpp_plds_lib"
         NSPR_LDFLAGS_MSVCCC="-L$dpp_nspr_libdir"

         NSPR_LIB_LINK_EXE="$dpp_nspr_lib.lib"
         PLC_LIB_LINK_EXE="$dpp_plc_lib.lib"
         PLDS_LIB_LINK_EXE="$dpp_plds_lib.lib"
         NSPR_LDFLAGS_LINK_EXE="/libpath:\"$dpp_nspr_libdir\""

         NSPR_LIB_STATIC="\$(NSPR_ROOT)/lib/libnspr$NSPR_VER.a"
         PLC_LIB_STATIC="\$(NSPR_ROOT)/lib/libplc$NSPR_VER.a"
         PLDS_LIB_STATIC="\$(NSPR_ROOT)/lib/libplds$NSPR_VER.a"
      else
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST(NSPR_ROOT)
   AC_SUBST(NSPR_INCLUDE_DIR)
   AC_SUBST(NSPR_INCLUDES)
   AC_SUBST(NSPR_LIB)
   AC_SUBST(PLC_LIB)
   AC_SUBST(PLDS_LIB)
   AC_SUBST(NSPR_LIB_STATIC)
   AC_SUBST(PLC_LIB_STATIC)
   AC_SUBST(PLDS_LIB_STATIC)
   AC_SUBST(NSPR_LIBDIR)
   AC_SUBST(NSPR_LDFLAGS)
   AC_SUBST(NSPR_LDFLAGS_LINK_EXE)
])

dnl ---------------------------------------------------------------------------
dnl Make sure that the installed NSPR is at least the given version.
dnl
dnl Usage:
dnl     DPP_NSPR_VER(nspr-root, nspr-inc, required-version [, action-if-not-found])
dnl
dnl Arguments:
dnl     nspr-root           - Root directory of the NSPR installation.
dnl     nspr-inc            - Directory with NSPR headers.
dnl     required-version    - Minimum required version.
dnl     action-if-not-found - Action to take if the version requirement is not
dnl                           met.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_NSPR_VER],
[
   dnl -----------------------------------------------------------------------
   dnl Define the version number of the NSPR installation.  This is the number
   dnl at the end of the library names (e.g., libnspr4.so).
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(nsprver,
               [  --with-nsprver=<VER>    Minimum NSPR version            [default=$3]],
               NSPR_VER="$withval", NSPR_VER=$3)

   dpp_save_CPPFLAGS="$CPPFLAGS"

   dnl Figure out what to do for the NSPR base directory and the NSPR header
   dnl directory.  This first conditional tests whether we have a user-defined
   dnl base or a user-defined header directory.  With either one, we can get a
   dnl possible header path.
   if test "x$1" != "x" -o "x$2" != "x" ; then
      dpp_nspr_base=$1

      dnl If we have a user-specified header directory, use it.
      if test "x$2" != "x" ; then
         dpp_nspr_headerdir="$2"
         CPPFLAGS="$CPPFLAGS -I$2 -I/usr/include"
      dnl Otherwise, fall back on the basic include sub-directory of the
      dnl base NSPR directory.
      else
         dpp_nspr_headerdir="$1/include"
         CPPFLAGS="$CPPFLAGS -I$1/include -I/usr/include"
      fi
   dnl If we have neither a base directory nor an include directory, fall back
   dnl on /usr.  Doing this allows us to check for NSPR headers without
   dnl extending $CPPFLAGS.
   else
      dpp_nspr_base='/usr'
   fi

   AC_CHECK_HEADER([prerr.h], [dpp_include_path="$dpp_nspr_headerdir"],
      [AC_CHECK_HEADER([nspr/prerr.h],
         [dpp_include_path="$dpp_nspr_headerdir/nspr"],
         [dpp_prerr_found='no'])])

   CPPFLAGS="$dpp_save_CPPFLAGS"

   dnl If prerr.h was not found, we're out of luck.
   if test "x$dpp_prerr_found" = "xno" ; then
      ifelse([$4], , :, [$4])
   dnl Otherwise, use prinit.h to determine the NSPR version being used.
   else
      dpp_ver_num_exp=['s/.*\([0-9][0-9]*\)$/\1/']
      dpp_nspr_ver_major=`grep PR_VMAJOR "$dpp_include_path/prinit.h" | sed -e $dpp_ver_num_exp`
      dpp_nspr_ver_minor=`grep PR_VMINOR "$dpp_include_path/prinit.h" | sed -e $dpp_ver_num_exp`
      dpp_nspr_ver_patch=`grep PR_VPATCH "$dpp_include_path/prinit.h" | sed -e $dpp_ver_num_exp`

      dpp_nspr_ver="${dpp_nspr_ver_major}.${dpp_nspr_ver_minor}.${dpp_nspr_ver_patch}"

      DPP_VERSION_CHECK_MSG_NO_CACHE([NSPR], [$dpp_nspr_ver], [$3], , [$4])

      NSPR_VER="$dpp_nspr_ver_major"
      NSPR_VERSION_DOT="$dpp_nspr_ver_major.$dpp_nspr_ver_minor.$dpp_nspr_ver_patch"
   fi

   AC_SUBST(NSPR_VERSION_DOT)
])
