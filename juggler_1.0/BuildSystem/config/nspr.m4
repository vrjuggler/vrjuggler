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
dnl File:          nspr.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.20
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
dnl     --with-nspr     - Define the path to the NSPR installation.
dnl     --with-nsprver  - Define the NSPR version number.
dnl
dnl Variables defined:
dnl     NSPR_ROOT       - The root of the NSPR installation.
dnl     NSPR_INCLUDES   - The compiler option giving the NSPR include path.
dnl     NSPR_LIB        - The linker option for the basic NSPR library.
dnl     PLC_LIB         - The linker option for the NSPR PLC library.
dnl     PLDS_LIB        - The linker option for the NSPR PLDS library.
dnl     NSPR_LDFLAGS    - The linker option for finding the NSPR libraries.
dnl     NSPR_LIB_STATIC - Full path to the static NSPR base library.
dnl     PLC_LIB_STATIC  - Full path to the static NSPR PLC library.
dnl     PLDS_LIB_STATIC - Full path to the static NSPR PLDS library.
dnl ===========================================================================

dnl nspr.m4,v 1.20 2001/06/29 23:47:52 patrickh Exp

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
AC_DEFUN(DPP_ENABLE_NSPR_THREADS,
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
AC_DEFUN(DPP_HAVE_NSPR,
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
    dnl The default is /usr/local.
    dnl -----------------------------------------------------------------------
    AC_ARG_WITH(nspr,
                [  --with-nspr=<PATH>      NSPR installation directory     [default=/usr/local]],
                NSPR_ROOT="$withval", NSPR_ROOT='/usr/local')

    if test "x$NSPR_ROOT" != "xno" ; then
        AC_LANG_SAVE
        AC_LANG_C

        dnl Check the NSPR version if a version number was given.
        if test "x$1" != "x" ; then
            DPP_NSPR_VER($NSPR_ROOT, $1, $3)
        else
            AC_MSG_ERROR(*** No NSPR version given! ***)
        fi

        dpp_save_CPPFLAGS="$CPPFLAGS"
        dpp_save_LDFLAGS="$LDFLAGS"
        dpp_save_LIBS="$LIBS"

        CPPFLAGS="$CPPFLAGS -I$NSPR_ROOT/include"
        LDFLAGS="$PTHREAD_ARG -L$NSPR_ROOT/lib $LDFLAGS"

        if test "x$dpp_os_type" = "xWin32" ; then
            NSPR_LIB_NAME_PREFIX='lib'
        fi

        dpp_nspr_lib="${NSPR_LIB_NAME_PREFIX}nspr$NSPR_VER"
        dpp_plc_lib="${NSPR_LIB_NAME_PREFIX}plc$NSPR_VER"
        dpp_plds_lib="${NSPR_LIB_NAME_PREFIX}plds$NSPR_VER"

        AC_CHECK_LIB($dpp_nspr_lib, PR_CreateThread,
                     AC_CHECK_HEADER(nspr.h, dpp_nspr_inc="$NSPR_ROOT/include",
                         AC_CHECK_HEADER(nspr/nspr.h,
                             dpp_nspr_inc="$NSPR_ROOT/include/nspr", $3)),
                     $3, [$PTHREAD_LIB $SEM_LIB])

        CPPFLAGS="$dpp_save_CPPFLAGS"
        LDFLAGS="$dpp_save_LDFLAGS"
        LIBS="$dpp_save_LIBS"

        AC_LANG_RESTORE

        NSPR_INCLUDES="-I$dpp_nspr_inc"
        NSPR_LIB="-l$dpp_nspr_lib"
        PLC_LIB="-l$dpp_plc_lib"
        PLDS_LIB="-l$dpp_plds_lib"
        NSPR_LDFLAGS="-L$NSPR_ROOT/lib"

        NSPR_LIB_STATIC="\$(NSPR_ROOT)/lib/libnspr$NSPR_VER.a"
        PLC_LIB_STATIC="\$(NSPR_ROOT)/lib/libplc$NSPR_VER.a"
        PLDS_LIB_STATIC="\$(NSPR_ROOT)/lib/libplds$NSPR_VER.a"
    fi

    dnl -----------------------------------------------------------------------
    dnl Do the substition step for $NSPR_ROOT.
    dnl -----------------------------------------------------------------------
    AC_SUBST(NSPR_ROOT)
    AC_SUBST(NSPR_LIB_STATIC)
    AC_SUBST(PLC_LIB_STATIC)
    AC_SUBST(PLDS_LIB_STATIC)
])

dnl ---------------------------------------------------------------------------
dnl Make sure that the installed NSPR is at least the given version.
dnl
dnl Usage:
dnl     DPP_NSPR_VER(nspr-root, version [, action-if-not-found])
dnl
dnl Arguments:
dnl     nspr-root           - Root directory of the NSPR installation.
dnl     version             - Minimum required version.
dnl     action-if-not-found - Action to take if the version requirement is not
dnl                           met.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_NSPR_VER,
[
    AC_REQUIRE([DPP_HAVE_NSPR])

    dnl -----------------------------------------------------------------------
    dnl Define the version number of the NSPR installation.  This is the number
    dnl at the end of the library names (e.g., libnspr4.so).
    dnl -----------------------------------------------------------------------
    AC_ARG_WITH(nsprver,
                [  --with-nsprver=<VER>    Minimum NSPR version            [default=$2]],
                NSPR_VER="$withval", NSPR_VER=$2)

    dpp_save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS -I$1/include"

    AC_CHECK_HEADER(prinit.h, dpp_include_path="$1/include",
        AC_CHECK_HEADER(nspr/prinit.h, dpp_include_path="$1/include/nspr",
            dpp_prinit_found='no'))

    CPPFLAGS="$dpp_save_CPPFLAGS"

    dnl If prinit.h was not found, we're out of luck.
    if test "x$dpp_prinit_found" = "xno" ; then
        ifelse([$3], , :, [$3])
    dnl Otherwise, use prinit.h to determine the NSPR version being used.
    else
        dpp_ver_num_exp=['s/.*\([0-9][0-9]*\)$/\1/']
        dpp_nspr_ver_major=`grep PR_VMAJOR $dpp_include_path/prinit.h | sed -e $dpp_ver_num_exp` ;
        dpp_nspr_ver_minor=`grep PR_VMINOR $dpp_include_path/prinit.h | sed -e $dpp_ver_num_exp` ;
        dpp_nspr_ver_patch=`grep PR_VPATCH $dpp_include_path/prinit.h | sed -e $dpp_ver_num_exp` ;

        dpp_nspr_ver="${dpp_nspr_ver_major}.${dpp_nspr_ver_minor}.${dpp_nspr_ver_patch}"

        DPP_VERSION_CHECK_MSG(NSPR, $dpp_nspr_ver, $2,
                              dpp_cv_nspr_version_okay, , $3)

        NSPR_VER="$dpp_nspr_ver_major"
    fi
])
