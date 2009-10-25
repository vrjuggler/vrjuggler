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
dnl File:          alut.m4,v
dnl Date modified: 2009/10/25 20:06:11
dnl Version:       1.5
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's OpenAL installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_ALUT - Determine if the target system has ALUT installed.
dnl
dnl Command-line options added:
dnl     --with-alut   - Give the root directory of the ALUT implementation
dnl                     installation.
dnl
dnl Variables defined:
dnl     ALUT          - Do we have ALUT on the system?
dnl     ALUT_ROOT     - The ALUT installation directory.
dnl     LIBALUT       - The list of libraries to link for ALUT appliations.
dnl     ALUT_INCLUDES - Extra include path for the ALUT header directory.
dnl     ALUT_LDFLAGS  - Extra linker flags for the ALUT library directory.
dnl     ALUT_LIBDIR   - The directory containing the ALUT library. On Mac OS X,
dnl                     this is the directory containing the OpenAL framework
dnl                     (since OpenAL and ALUT are not separated).
dnl ===========================================================================

dnl alut.m4,v 1.5 2009/10/25 20:06:11 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has ALUT installed.  This adds the
dnl command-line argument --with-alut.
dnl
dnl Usage:
dnl     DPP_HAVE_ALUT(alutroot [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     alutroot            - The default directory where the ALUT
dnl                           installation is rooted. This directory should
dnl                           contain an include/AL directory with the ALUT
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the ALUT libraries. The value
dnl                           given is used as the default value of the
dnl                           --with-alut command-line argument.
dnl     action-if-found     - The action to take if an ALUT implementation is
dnl                           found. This argument is optional.
dnl     action-if-not-found - The action to take if an ALUT implementation is
dnl                           not found. This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_ALUT],
[
   ALUT='no'
   LIBALUT=''
   ALUT_INCLUDES=''
   ALUT_LDFLAGS=''
   dpp_have_alut='no'

   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl If no test has been performed to find OpenAL yet (see openal.m4), then
   dnl do it now. We'll just assume that OpenAL and ALUT are installed in the
   dnl same place since we have no better fallback path to use.
   if test "x$OPENAL" = "x" ; then
      DPP_HAVE_OPENAL([$1], , 
                      [AC_MSG_WARN([*** OpenAL is required for ALUT ***])])
   fi

   dnl Define the root directory for the OpenAL installation.
   AC_ARG_WITH(alut,
               [  --with-alut=<PATH>      ALUT installation directory     [default=$1]],
               [ALUT_ROOT="$withval"], [ALUT_ROOT=$1])

   if test "x$OPENAL" != "xyes" ; then
      AC_MSG_WARN([*** OpenAL is required for ALUT ***])
      ifelse([$3], , :, [$3])
   else
      if test "x$ALUT_ROOT" != "xno" ; then
         dnl Save these values in case they need to be restored later.
         dpp_save_CFLAGS="$CFLAGS"
         dpp_save_CPPFLAGS="$CPPFLAGS"
         dpp_save_LDFLAGS="$LDFLAGS"
         dpp_save_LIBS="$LIBS"

         dnl Add the user-specified ALUT installation directory to these paths.
         dnl Ensure that /usr/include and /usr/lib are not included multiple
         dnl multiple times if $ALUT_ROOT is "/usr".
         if test "x$ALUT_ROOT" != "x/usr" ; then
            CPPFLAGS="$CPPFLAGS -I$ALUT_ROOT/include"
         fi

         CFLAGS="$CFLAGS $ABI_FLAGS"
         CPPFLAGS="$CPPFLAGS $AL_INCLUDES"

         dnl On Mac OS X prior to 10.6 (Snow Leopard), the version of OpenAL
         dnl available from Creative predates the separation of ALUT from
         dnl OpenAL. As such, if OpenAL is available, then ALUT is available.
         if test "x$PLATFORM" = "xDarwin" -a "$OS_REL_NUM_MAJOR" -lt 10 ; then
            ALUT="$OPENAL"
            ALUT_LIBDIR="$AL_LIBDIR"
            dpp_have_alut="$dpp_have_openal"
         dnl Other platforms.
         else
            if test "lib$LIBBITSUF" != "lib" ; then
               libdirs="lib$LIBBITSUF lib"
            else
               libdirs="lib"
            fi

            LIBS="-lalut $DYN_LOAD_LIB $LIBOPENAL $LIBS"

            DPP_LANG_SAVE
            DPP_LANG_C

            for l in $libdirs ; do
               cur_alut_libdir="$ALUT_ROOT/$l"

               dnl The pthreads-related macros will set only one of
               dnl $PTHREAD_ARG or $PTHREAD_LIB, so it's safe (and simpler)
               dnl for us to use both here.
               LDFLAGS="-L$cur_alut_libdir $dpp_saveLDFLAGS $AL_LDFLAGS $PTHREAD_ARG $PTHREAD_LIB"

               AC_MSG_CHECKING([for alutInit in -lalut in $cur_alut_libdir])
               AC_TRY_LINK([#include <AL/alut.h>], [alutInit(0, 0);],
                           [dpp_have_alut='yes'], [dpp_have_alut='no'])
               AC_MSG_RESULT([$dpp_have_alut])

               if test "x$dpp_have_alut" = "xyes" ; then
                  ALUT_LIBDIR="$cur_alut_libdir"
                  break
               fi
            done

            DPP_LANG_RESTORE
         fi

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         dnl Success.
         if test "x$dpp_have_alut" = "xyes" ; then
            ifelse([$2], , :, [$2])
         dnl Failure.
         else
            ifelse([$3], , :, [$3])
         fi

         dnl If ALUT API files were found, define this extra stuff that may be
         dnl helpful in some Makefiles.
         if test "x$dpp_have_alut" = "xyes" ; then
            if test "x$PLATFORM" = "xDarwin" -a "$OS_REL_NUM_MAJOR" -lt 10 ; then
               LIBALUT=''
            else
               LIBALUT="-lalut"
            fi

            if test "x$PLATFORM" != "xDarwin" -o "$OS_REL_NUM_MAJOR" -ge 10 ; then
               if test "x$ALUT_ROOT" != "x/usr" ; then
                  ALUT_INCLUDES="-I$ALUT_ROOT/include"
                  ALUT_LDFLAGS="-L$ALUT_ROOT/lib"
               fi
            fi

            ALUT='yes'
         fi
      fi
   fi

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(ALUT)
   AC_SUBST(ALUT_ROOT)
   AC_SUBST(LIBALUT)
   AC_SUBST(ALUT_INCLUDES)
   AC_SUBST(ALUT_LDFLAGS)
   AC_SUBST(ALUT_LIBDIR)
])
