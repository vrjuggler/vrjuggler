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
dnl File:          subsynth.m4,v
dnl Date modified: 2002/08/15 15:37:16
dnl Version:       1.1.2.8
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's Subsynth installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_SUBSYNTH - Determine if the target system has Subsynth installed.
dnl
dnl Command-line options added:
dnl     --with-synroot - Give the root directory of the Subsynth implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     SUBSYNTH      - do we have subsynth on the system?
dnl     SYNROOT     - The Subsynth installation directory.
dnl     LIBSUBSYNTH   - The list of libraries to link for Subsynth appliations.
dnl     SYN_INCLUDES - Extra include path for the Subsynth header directory.
dnl     SYN_LDFLAGS  - Extra linker flags for the Subsynth library directory.
dnl ===========================================================================

dnl subsynth.m4,v 1.1.2.8 2002/08/15 15:37:16 subatomic Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has Subsynth installed.  This
dnl adds command-line arguments --with-synroot.
dnl
dnl Usage:
dnl     DPP_HAVE_SUBSYNTH(synroot [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     synroot             - The default directory where the Subsynth
dnl                           installation is rooted.  This directory should
dnl                           contain an include/AL directory with the AL
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the AL libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-synroot command-line argument.
dnl     action-if-found     - The action to take if an Subsynth implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an Subsynth implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_SUBSYNTH,
[
   dnl initialize returned data...
   SUBSYNTH='no'
   LIBSUBSYNTH=''
   SYN_INCLUDES=''
   SYN_LDFLAGS=''
   dpp_have_subsynth='no'

   AC_REQUIRE([DPP_SYSTEM_SETUP])
   DPP_HAVE_PORTAUDIO( /usr, AC_MSG_RESULT(+ Found PortAudio), AC_MSG_RESULT(+ PortAudio not found, skipping use of Subsynth), )

   dnl Define the root directory for the Subsynth installation.
   AC_ARG_WITH(synroot,
               [  --with-synroot=<PATH>   Subsynth installation directory   [default=$1]],
               SYNROOT="$withval", SYNROOT=$1)

   dnl Only continue checking for subsynth if we found port audio
   if test "x$PORTAUDIO" == "xyes"; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CFLAGS="$CFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_INCLUDES="$INCLUDES"
      dpp_save_LDFLAGS="$LDFLAGS"

      dnl Add the user-specified Subsynth installation directory to these
      dnl paths.  Ensure that /usr/include and /usr/lib are not included
      dnl multiple times if $SYNROOT is "/usr".
      if test "x$SYNROOT" != "x/usr" ; then
         CPPFLAGS="$CPPFLAGS -I$SYNROOT/include $PA_INCLUDES"
         INCLUDES="$INCLUDES -I$SYNROOT/include $PA_INCLUDES"
         if test -d "$SYNROOT/lib$LIBBITSUF" ; then
            LDFLAGS="-L$SYNROOT/lib$LIBBITSUF $PA_LDFLAGS $LDFLAGS"
         fi
      fi

      dnl debugging output...
      dnl echo "+ setting CPPFLAGS == $CPPFLAGS"
      dnl echo "+ setting INCLUDES == $INCLUDES"
      dnl echo "+ setting LDFLAGS == $LDFLAGS"

      CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

      echo "checking for Subsynth in  $SYNROOT/lib$LIBBITSUF and $SYNROOT/include"

      dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      if test "x$dpp_os_type" = "xWin32" ; then
         AC_LANG_SAVE
         AC_LANG_C

         LIBS="$LIBS subsynth.lib"

         dpp_cv_synisLittle_subsynth_lib='no'

         AC_CACHE_CHECK([for syn::isLittle in subsynth.lib],
                        [dpp_cv_synisLittle_subsynth_lib],
                        [AC_TRY_LINK([#include <windows.h>
#include <syn/Util/Endian.h>],
                           [syn::isLittle();],
                           [dpp_cv_synisLittle_subsynth_lib='yes'],
                           [dpp_cv_synisLittle_subsynth_lib='no'])])

         if test "x$dpp_cv_synisLittle_subsynth_lib" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi

         LIBS="$dpp_save_LIBS"

         if test "x$dpp_cv_synisLittle_subsynth_lib" = "xyes" ; then
            ifelse([$2], , :, [$2])
         else
            ifelse([$3], , :, [$3])
         fi

         AC_LANG_RESTORE

      dnl UNIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      else
         dnl LDFLAGS="$LDFLAGS"

         dpp_save_LIBS="$LIBS"

         AC_LANG_SAVE
         AC_LANG_C

         CPPFLAGS="$CPPFLAGS"
         INCLUDES="$INCLUDES"

         dpp_have_subsynth='no'

         # no C symbols in subsynth,...  there has to be a better way,
         # but this is good enough for me...
         echo "checking for Subsynth in $SYNROOT/lib$LIBBITSUF/libsubsynth.[a|so]..."
         if test -e "$SYNROOT/lib$LIBBITSUF/libsubsynth.a" && test -e "$SYNROOT/lib$LIBBITSUF/libsubsynth.so" ; then
            echo "checking for Subsynth in $SYNROOT/include/syn/Util/Endian.h..."
            if test -e "$SYNROOT/include/syn/Util/Endian.h" ; then
               dpp_have_subsynth='yes'
            fi
         fi

         #AC_CHECK_LIB([subsynth], [sin],
         #   [AC_CHECK_HEADER([syn/Util/Endian.h], [dpp_have_subsynth='yes'],
         #      [AC_MSG_WARN([*** Subsynth will not be used (syn/Util/Endian.h not found) ***])])],
         #   [AC_MSG_WARN([*** Subsynth will not be used (subsynth library not found) ***])],
         #   [-lm])

         dnl This is necessary because AC_CHECK_LIB() adds -lsubsynth to
         dnl $LIBS.  We want to do that ourselves later.
         LIBS="$dpp_save_LIBS"

         if test "x$dpp_have_subsynth" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
         if test "x$dpp_have_subsynth" = "xyes" ; then
            ifelse([$2], , :, [$2])
         fi

         AC_LANG_RESTORE
      fi

      dnl If Subsynth API files were found, define this extra stuff that may be
      dnl helpful in some Makefiles.
      if test "x$dpp_have_subsynth" = "xyes" ; then
         if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBSUBSYNTH="-lsubsynth $LIBPORTAUDIO -lm"
         else
            LIBSUBSYNTH='subsynth.lib $LIBPORTAUDIO'
         fi

         if test "x$SYNROOT" != "x/usr" ; then
            SYN_INCLUDES="-I$SYNROOT/include $PA_INCLUDES"
            SYN_LDFLAGS="-L$SYNROOT/lib$LIBBITSUF $PA_LDFLAGS"
         fi

         SUBSYNTH='yes'
      fi

      dnl Restore all the variables now that we are done testing.
      CFLAGS="$dpp_save_CFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      INCLUDES="$dpp_save_INCLUDES"
      LDFLAGS="$dpp_save_LDFLAGS"
      
      if test "x$PLATFORM" = "xIRIX" ; then
         SYN_INCLUDES="-DNO_METAPROG $SYN_INCLUDES"
      fi

   fi

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(SUBSYNTH)
   AC_SUBST(SYNROOT)
   AC_SUBST(LIBSUBSYNTH)
   AC_SUBST(SYN_INCLUDES)
   AC_SUBST(SYN_LDFLAGS)
])
