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
dnl File:          portaudio.m4,v
dnl Date modified: 2005/01/08 22:44:41
dnl Version:       1.14
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's PortAudio installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_PORTAUDIO - Determine if the target system has PortAudio
dnl                          installed.
dnl
dnl Command-line options added:
dnl     --with-portaudio - Give the root directory of the PortAudio
dnl                        implementation installation.
dnl
dnl Variables defined:
dnl     PORTAUDIO          - Do we have PortAudio on the system?
dnl     PORTAUDIO_ROOT     - The PortAudio installation directory.
dnl     LIBPORTAUDIO       - The list of libraries to link for PortAudio
dnl                          appliations.
dnl     PORTAUDIO_INCLUDES - Extra include path for the PortAudio header
dnl                          directory.
dnl     PORTAUDIO_LDFLAGS  - Extra linker flags for the PortAudio library
dnl                          directory.
dnl ===========================================================================

dnl portaudio.m4,v 1.14 2005/01/08 22:44:41 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has PortAudio installed.  This
dnl adds command-line arguments --with-portaudio.
dnl
dnl Usage:
dnl     DPP_HAVE_PORTAUDIO(portaudio-root [, action-if-pa-found [, action-if-not-pa-found [, action-if-pablio-found [, action-if-pablio-not-found]]]])
dnl
dnl Arguments:
dnl     portaudio-root             - The default directory where the PortAudio
dnl                                  installation is rooted.  This directory
dnl                                  should contain an include directory with
dnl                                  the AL headers and a lib (with
dnl                                  appropriate bit suffix) directory with
dnl                                  the PortAudio libraries.  The value
dnl                                  given is used as the default value of the
dnl                                  --with-portaudio command-line argument.
dnl     action-if-pa-found         - The action to take if a PortAudio
dnl                                  installation is found.  This argument is
dnl                                  optional.
dnl     action-if-pa-not-found     - The action to take if a PortAudio
dnl                                  installation is not found.  This argument
dnl                                  is optional.
dnl     action-if-pablio-found     - The action to take if the Pablio library
dnl                                  is found.  This argument is optional.
dnl     action-if-pablio-not-found - The action to take if the Pablio library
dnl                                  is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_PORTAUDIO],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl initialize returned data...
   PORTAUDIO='no'
   LIBPORTAUDIO=''
   PORTAUDIO_INCLUDES=''
   PORTAUDIO_LDFLAGS=''
   dpp_have_portaudio='no'

   dnl Define the root directory for the PortAudio installation.
   AC_ARG_WITH(portaudio,
               [  --with-portaudio=<PATH> PortAudio installation dir.     [default=$1]],
               PORTAUDIO_ROOT="$withval", PORTAUDIO_ROOT=$1)

   if test "x$PORTAUDIO_ROOT" != "xno" ; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CFLAGS="$CFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"

      dnl Add the user-specified PortAudio installation directory to these
      dnl paths.  Ensure that /usr/include and /usr/lib are not included
      dnl multiple times if $PORTAUDIO_ROOT is "/usr".
      if test "x$PORTAUDIO_ROOT" != "x/usr" ; then
         CPPFLAGS="$CPPFLAGS -I$PORTAUDIO_ROOT/include"

         if test -d "$PORTAUDIO_ROOT/lib$LIBBITSUF" ; then
            pa_ldflags="-L$PORTAUDIO_ROOT/lib$LIBBITSUF"
         else
            pa_ldflags="-L$PORTAUDIO_ROOT/lib"
         fi

         LDFLAGS="$pa_ldflags $LDFLAGS"
      fi

      CFLAGS="$CFLAGS $ABI_FLABS"

      dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if test "x$dpp_os_type" = "xWin32" ; then
         AC_LANG_SAVE
         AC_LANG_C

         LIBS="$LIBS PAStaticDS.lib"

         dpp_cv_Pa_Initialize_pastaticds_lib='no'

         AC_CACHE_CHECK([for Pa_Initialize in PAStaticDS.lib],
                        [dpp_cv_Pa_Initialize_pastaticds_lib],
                        [AC_TRY_LINK([#include <windows.h>
#include <portaudio.h>],
                           [Pa_Initialize();],
                           [dpp_cv_Pa_Initialize_pastaticds_lib='yes'],
                           [dpp_cv_Pa_Initialize_pastaticds_lib='no'])])

         if test "x$dpp_cv_Pa_Initialize_pastaticds_lib" = "xyes" ; then
            ifelse([$2], , :, [$2])

            LIBS="pablio.lib $LIBS"

            AC_CACHE_CHECK([for GetAudioStreamReadable in pablio.lib],
                           [dpp_cv_GetAudioStreamReadable_pablio_lib],
                           [AC_TRY_LINK([#include <windows.h>
#include <stdlib.h>
#include <pablio.h>],
                              [GetAudioStreamReadable(NULL);],
                              [dpp_cv_GetAudioStreamReadable_pablio_lib='yes'],
                              [dpp_cv_GetAudioStreamReadable_pablio_lib='no'])])

            if test "x$dpp_cv_GetAudioStreamReadable_pablio_lib" = "xyes"
            then
               ifelse([$4], , :, [$4])
            else
               ifelse([$5], , :, [$5])
            fi
         else
            ifelse([$3], , :, [$3])
         fi

         LIBS="$dpp_save_LIBS"

         AC_LANG_RESTORE

      dnl UNIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      else
         dpp_save_LIBS="$LIBS"

         DPP_LANG_SAVE
         DPP_LANG_C

         dpp_have_portaudio='no'

         AC_CHECK_LIB([portaudio], [Pa_Initialize],
            [AC_CHECK_HEADER([portaudio.h], [dpp_have_portaudio='yes'])])

         DPP_LANG_RESTORE

         LIBS="$dpp_save_LIBS"

         if test "x$dpp_have_portaudio" = "xyes" ; then
            ifelse([$2], , :, [$2])

            # Pablio is optional.
            # TODO: make an m4 for this...
            dpp_have_pablio='no'

            DPP_LANG_SAVE
            DPP_LANG_C

            AC_CHECK_LIB([pablio], [WriteAudioStream],
               [AC_CHECK_HEADER([pablio.h], [dpp_have_pablio='yes'])])

            DPP_LANG_RESTORE

            LIBS="$dpp_save_LIBS"

            if test "x$dpp_have_pablio" = "xyes" ; then
               ifelse([$4], , :, [$4])
            else
               ifelse([$5], , :, [$5])
            fi
         else
            ifelse([$3], , :, [$3])
         fi
      fi

      dnl If PortAudio API files were found, define this extra stuff that may
      dnl be helpful in some Makefiles.
      if test "x$dpp_have_portaudio" = "xyes" ; then
         if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBPORTAUDIO="-lportaudio -lm"
            if test "x$dpp_have_pablio" = "xyes" ; then
               LIBPORTAUDIO="$LIBPORTAUDIO -lpablio"
            fi
         elif test "x$OS_TYPE" = "xWin32"; then
            LIBPORTAUDIO='PAStaticDS.lib'
            if test "x$dpp_have_pablio" = "xyes" ; then
               LIBPORTAUDIO="$LIBPORTAUDIO pablio.lib"
            fi
         fi

         if test "x$PORTAUDIO_ROOT" != "x/usr" ; then
            PORTAUDIO_INCLUDES="-I$PORTAUDIO_ROOT/include"
            PORTAUDIO_LDFLAGS="$pa_ldflags"
         fi

         PORTAUDIO='yes'
      fi

      dnl Restore all the variables now that we are done testing.
      CFLAGS="$dpp_save_CFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      LDFLAGS="$dpp_save_LDFLAGS"
   fi

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(PORTAUDIO)
   AC_SUBST(PORTAUDIO_ROOT)
   AC_SUBST(LIBPORTAUDIO)
   AC_SUBST(PORTAUDIO_INCLUDES)
   AC_SUBST(PORTAUDIO_LDFLAGS)
])
