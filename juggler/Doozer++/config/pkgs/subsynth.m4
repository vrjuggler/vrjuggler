dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
dnl File:          subsynth.m4,v
dnl Date modified: 2005/01/08 22:44:41
dnl Version:       1.14
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's Subsynth installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_SUBSYNTH - Determine if the target system has Subsynth
dnl                         installed.
dnl
dnl Command-line options added:
dnl     --with-subsynth   - Give the root directory of the Subsynth
dnl                         implementation installation.
dnl
dnl Variables defined:
dnl     SUBSYNTH          - do we have subsynth on the system?
dnl     SUBSYNTH_ROOT     - The Subsynth installation directory.
dnl     LIBSUBSYNTH       - The list of libraries to link for Subsynth
dnl                         appliations.
dnl     SUBSYNTH_INCLUDES - Extra include path for the Subsynth header
dnl                         directory.
dnl     SUBSYNTH_LDFLAGS  - Extra linker flags for the Subsynth library
dnl                         directory.
dnl ===========================================================================

dnl subsynth.m4,v 1.14 2005/01/08 22:44:41 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has Subsynth installed.  This adds the
dnl command-line argument --with-subsynth.
dnl
dnl Usage:
dnl     DPP_HAVE_SUBSYNTH(subsynth-root [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     subsynth-root       - The default directory where the Subsynth
dnl                           installation is rooted.  This directory should
dnl                           contain an include/AL directory with the AL
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the AL libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-subsynth command-line argument.
dnl     action-if-found     - The action to take if an Subsynth implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an Subsynth implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_SUBSYNTH],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl initialize returned data...
   SUBSYNTH='no'
   LIBSUBSYNTH=''
   SUBSYNTH_INCLUDES=''
   SUBSYNTH_LDFLAGS=''
   dpp_have_subsynth='no'

   dnl XXX: This isn't valid because the caller of this macro has no way to
   dnl specify where to look for PortAudio by default.  The default (/usr) is
   dnl hard coded, which is undesireable.
   DPP_HAVE_PORTAUDIO([/usr], ,
      [AC_MSG_WARN([*** Subsynth requires PortAudio ***])])

   dnl Define the root directory for the Subsynth installation.
   AC_ARG_WITH(subsynth,
               [  --with-subsynth=<PATH>  Subsynth installation dir.      [default=$1]],
               SUBSYNTH_ROOT="$withval", SUBSYNTH_ROOT=$1)

   if test "x$SUBSYNTH_ROOT" != "xno" ; then
      dnl Only continue checking for subsynth if we found port audio
      if test "x$PORTAUDIO" = "xyes"; then
         dnl Save these values in case they need to be restored later.
         dpp_save_CFLAGS="$CFLAGS"
         dpp_save_CPPFLAGS="$CPPFLAGS"
         dpp_save_LDFLAGS="$LDFLAGS"

         dnl Add the user-specified Subsynth installation directory to these
         dnl paths.  Ensure that /usr/include and /usr/lib are not included
         dnl multiple times if $SUBSYNTH_ROOT is "/usr".
         if test "x$SUBSYNTH_ROOT" != "x/usr" ; then
            CPPFLAGS="$CPPFLAGS -I$SUBSYNTH_ROOT/include $PA_INCLUDES"

            if test -d "$SUBSYNTH_ROOT/lib$LIBBITSUF" ; then
               LDFLAGS="-L$SUBSYNTH_ROOT/lib$LIBBITSUF $PA_LDFLAGS $LDFLAGS"
            fi
         fi

         CFLAGS="$CFLAGS $ABI_FLABS"

         dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

         if test "x$dpp_os_type" = "xWin32" ; then
            DPP_LANG_SAVE
            DPP_LANG_CPLUSPLUS

            LIBS="$LIBS subsynth.lib"

            dpp_cv_synisLittle_subsynth_lib='no'

            AC_CACHE_CHECK([for syn::isLittle in subsynth.lib],
                           [dpp_cv_synisLittle_subsynth_lib],
                           [AC_TRY_LINK([#include <windows.h>
#include <syn/Util/Endian.h>],
                              [syn::isLittle();],
                              [dpp_cv_synisLittle_subsynth_lib='yes'],
                              [dpp_cv_synisLittle_subsynth_lib='no'])])

            LIBS="$dpp_save_LIBS"
            dpp_have_subsynth="$dpp_cv_synisLittle_subsynth_lib"

            DPP_LANG_RESTORE

         dnl UNIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         else
            dpp_save_LIBS="$LIBS"

            DPP_LANG_SAVE
            DPP_LANG_C

            dpp_have_subsynth='no'

            # XXX: Fix this crummy test.
            if test -e "$SUBSYNTH_ROOT/lib$LIBBITSUF/libsubsynth.a" && test -e "$SUBSYNTH_ROOT/lib$LIBBITSUF/libsubsynth.so"
            then
               if test -e "$SUBSYNTH_ROOT/include/syn/Util/Endian.h" ; then
                  dpp_have_subsynth='yes'
               fi
            fi

dnl            AC_CHECK_LIB([subsynth], [sin],
dnl               [AC_CHECK_HEADER([syn/Util/Endian.h], [dpp_have_subsynth='yes'],
dnl                  [AC_MSG_WARN([*** Subsynth will not be used (syn/Util/Endian.h not found) ***])])],
dnl               [AC_MSG_WARN([*** Subsynth will not be used (subsynth library not found) ***])],
dnl               [-lm])

            dnl This is necessary because AC_CHECK_LIB() adds -lsubsynth to
            dnl $LIBS.  We want to do that ourselves later.
            LIBS="$dpp_save_LIBS"

            DPP_LANG_RESTORE
         fi

         if test "x$dpp_have_subsynth" = "xyes" ; then
            ifelse([$2], , :, [$2])
         else
            ifelse([$3], , :, [$3])
         fi

         dnl If Subsynth API files were found, define this extra stuff that may
         dnl be helpful in some Makefiles.
         if test "x$dpp_have_subsynth" = "xyes" ; then
            if test "x$OS_TYPE" = "xUNIX" ;  then
               LIBSUBSYNTH="-lsubsynth $LIBPORTAUDIO -lm"
            else
               LIBSUBSYNTH='subsynth.lib $LIBPORTAUDIO'
            fi

            if test "x$SUBSYNTH_ROOT" != "x/usr" ; then
               SUBSYNTH_INCLUDES="-I$SUBSYNTH_ROOT/include $PA_INCLUDES"
               SUBSYNTH_LDFLAGS="-L$SUBSYNTH_ROOT/lib$LIBBITSUF $PA_LDFLAGS"
            fi

            SUBSYNTH='yes'
         fi

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"

         if test "x$PLATFORM" = "xIRIX" ; then
            SUBSYNTH_INCLUDES="-DNO_METAPROG $SUBSYNTH_INCLUDES"
         fi

      dnl Failure case when PortAudio is not available.
      else
         ifelse([$3], , :, [$3])
      fi
   fi

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(SUBSYNTH)
   AC_SUBST(SUBSYNTH_ROOT)
   AC_SUBST(LIBSUBSYNTH)
   AC_SUBST(SUBSYNTH_INCLUDES)
   AC_SUBST(SUBSYNTH_LDFLAGS)
])
