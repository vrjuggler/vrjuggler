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
dnl File:          portaudio.m4,v
dnl Date modified: 2003/02/22 03:23:18
dnl Version:       1.1.2.9
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

dnl portaudio.m4,v 1.1.2.9 2003/02/22 03:23:18 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has PortAudio installed.  This
dnl adds command-line arguments --with-portaudio.
dnl
dnl Usage:
dnl     DPP_HAVE_PORTAUDIO(portaudio-root [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     portaudio-root      - The default directory where the PortAudio
dnl                           installation is rooted.  This directory should
dnl                           contain an include/AL directory with the AL
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the AL libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-portaudio command-line argument.
dnl     action-if-found     - The action to take if a PortAudio installation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if a PortAudio installation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_PORTAUDIO,
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

   dnl Save these values in case they need to be restored later.
   dpp_save_CFLAGS="$CFLAGS"
   dpp_save_CPPFLAGS="$CPPFLAGS"
   dpp_save_INCLUDES="$INCLUDES"
   dpp_save_LDFLAGS="$LDFLAGS"

   dnl Add the user-specified PortAudio installation directory to these
   dnl paths.  Ensure that /usr/include and /usr/lib are not included
   dnl multiple times if $PORTAUDIO_ROOT is "/usr".
   if test "x$PORTAUDIO_ROOT" != "x/usr" ; then
      CPPFLAGS="$CPPFLAGS -I$PORTAUDIO_ROOT/include"
      INCLUDES="$INCLUDES -I$PORTAUDIO_ROOT/include"
      if test -d "$PORTAUDIO_ROOT/lib$LIBBITSUF" ; then
         LDFLAGS="-L$PORTAUDIO_ROOT/lib$LIBBITSUF $LDFLAGS"
      fi
   fi

   CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

   dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   if test "x$dpp_os_type" = "xWin32" ; then
      AC_LANG_SAVE
      AC_LANG_C

      LIBS="$LIBS PAStaticDS.lib"

      dpp_cv_paisLittle_portaudio_lib='no'

      echo "checking for PortAudio in $PORTAUDIO_ROOT/lib$LIBBITSUF/PAStaticDS.lib..."
      if test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/PAStaticDS.lib" ; then
         echo "checking for PortAudio in $PORTAUDIO_ROOT/include/portaudio.h..."
         if test -e "$PORTAUDIO_ROOT/include/portaudio.h" ; then
            dpp_cv_paisLittle_portaudio_lib='yes'
         fi
      fi
      
      # pablio is optional, todo, make an m4 for this...
      pablio_found='no'
      echo "checking for Pablio in $PORTAUDIO_ROOT/lib$LIBBITSUF/pablio.lib..."
      if test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/pablio.lib" ; then
         echo "checking for Pablio in $PORTAUDIO_ROOT/include/pablio.h..."
         if test -e "$PORTAUDIO_ROOT/include/pablio.h" ; then
            pablio_found='yes'
         fi
      fi

      if test "x$dpp_cv_paisLittle_portaudio_lib" = "xno" ; then
         ifelse([$3], , :, [$3])
      fi

      LIBS="$dpp_save_LIBS"

      if test "x$dpp_cv_paisLittle_portaudio_lib" = "xno" ; then
         ifelse([$3], , :, [$3])
      fi
      if test "x$dpp_cv_paisLittle_portaudio_lib" = "xyes" ; then
         ifelse([$2], , :, [$2])
      else
         ifelse([$3], , :, [$3])
      fi

      AC_LANG_RESTORE
        
   dnl UNIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   else
      dpp_save_LIBS="$LIBS"

      AC_LANG_SAVE
      AC_LANG_C

      dpp_have_portaudio='no'
      
      # no C symbols in portaudio,...  there has to be a better way, 
      # but this is good enough for me... 
      echo "checking for PortAudio in $PORTAUDIO_ROOT/lib$LIBBITSUF/libportaudio.[a|so]..."
      if test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/libportaudio.a" || test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/libportaudio.so" ; then
         echo "checking for PortAudio in $PORTAUDIO_ROOT/include/portaudio.h..."
         if test -e "$PORTAUDIO_ROOT/include/portaudio.h" ; then
            dpp_have_portaudio='yes'
         fi
      fi
      
      # pablio is optional, todo, make an m4 for this...
      pablio_found='no'
      echo "checking for Pablio in $PORTAUDIO_ROOT/lib$LIBBITSUF/libpablio.[a|so]..."
      if test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/libpablio.a" || test -e "$PORTAUDIO_ROOT/lib$LIBBITSUF/libpablio.so" ; then
         echo "checking for Pablio in $PORTAUDIO_ROOT/include/pablio.h..."
         if test -e "$PORTAUDIO_ROOT/include/pablio.h" ; then
            pablio_found='yes'
         fi
      fi

      dnl This is necessary because AC_CHECK_LIB() adds -lportaudio to
      dnl $LIBS.  We want to do that ourselves later.
      LIBS="$dpp_save_LIBS"
        
      if test "x$dpp_have_portaudio" = "xno" ; then
         ifelse([$3], , :, [$3])
      fi
      if test "x$dpp_have_portaudio" = "xyes" ; then
         ifelse([$2], , :, [$2])
      else
         ifelse([$3], , :, [$3])
      fi

      AC_LANG_RESTORE
   fi

   dnl If PortAudio API files were found, define this extra stuff that may be
   dnl helpful in some Makefiles.
   if test "x$dpp_have_portaudio" = "xyes" ; then
      if test "x$OS_TYPE" = "xUNIX" ;  then
         LIBPORTAUDIO="-lportaudio -lm"
         if test "x$pablio_found" = "xyes" ; then
            LIBPORTAUDIO="$LIBPORTAUDIO -lpablio"
         fi
      else
         LIBPORTAUDIO='PAStaticDS.lib'
         if test "x$pablio_found" = "xyes" ; then
            LIBPORTAUDIO="$LIBPORTAUDIO pablio.lib"
         fi
      fi

      if test "x$PORTAUDIO_ROOT" != "x/usr" ; then
         PORTAUDIO_INCLUDES="-I$PORTAUDIO_ROOT/include"
         PORTAUDIO_LDFLAGS="-L$PORTAUDIO_ROOT/lib$LIBBITSUF"
      fi
        
      PORTAUDIO='yes'
   fi

   dnl Restore all the variables now that we are done testing.
   CFLAGS="$dpp_save_CFLAGS"
   CPPFLAGS="$dpp_save_CPPFLAGS"
   INCLUDES="$dpp_save_INCLUDES"
   LDFLAGS="$dpp_save_LDFLAGS"

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(PORTAUDIO)
   AC_SUBST(PORTAUDIO_ROOT)
   AC_SUBST(LIBPORTAUDIO)
   AC_SUBST(PORTAUDIO_INCLUDES)
   AC_SUBST(PORTAUDIO_LDFLAGS)
])
