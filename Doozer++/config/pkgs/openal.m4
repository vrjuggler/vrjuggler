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
dnl File:          openal.m4,v
dnl Date modified: 2006/12/31 16:55:00
dnl Version:       1.25
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's OpenAL installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_OPENAL - Determine if the target system has OpenAL installed.
dnl
dnl Command-line options added:
dnl     --with-openal   - Give the root directory of the OpenAL implementation
dnl                       installation.
dnl
dnl Variables defined:
dnl     OPENAL      - do we have openal on the system?
dnl     OALROOT     - The OpenAL installation directory.
dnl     LIBOPENAL   - The list of libraries to link for OpenAL appliations.
dnl     AL_INCLUDES - Extra include path for the OpenAL header directory.
dnl     AL_LDFLAGS  - Extra linker flags for the OpenAL library directory.
dnl     AL_LIBDIR   - The directory containing the OpenAL library. On Mac OS X,
dnl                   this is the directory containing the OpenAL framework.
dnl ===========================================================================

dnl openal.m4,v 1.25 2006/12/31 16:55:00 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has OpenAL installed.  This
dnl adds command-line arguments --with-openal.
dnl
dnl Usage:
dnl     DPP_HAVE_OPENAL(alroot [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     alroot              - The default directory where the OpenAL
dnl                           installation is rooted.  This directory should
dnl                           contain an include/AL directory with the AL
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the AL libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-openal command-line argument.
dnl     action-if-found     - The action to take if an OpenAL implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an OpenAL implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_OPENAL],
[
   dnl initialize returned data...
   OPENAL='no'
   LIBOPENAL=''
   AL_INCLUDES=''
   AL_LDFLAGS=''
   dpp_have_openal='no'

   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl The UNIX version of OpenAL requires the use of pthreads.
   if test "x$OS_TYPE" = "xUNIX" ; then
      AC_REQUIRE([DPP_CC_PTHREAD_ARG])
      AC_REQUIRE([DPP_CC_PTHREADS_ARG])
      AC_REQUIRE([DPP_GET_PTHREAD_LIB])

      dnl This is here just to be safe
      true
   fi

   AC_REQUIRE([DPP_CHECK_DYNLOAD_LIB])

   dnl Define the root directory for the OpenAL installation.
   AC_ARG_WITH(openal,
               [  --with-openal=<PATH>    OpenAL installation directory   [default=$1]],
               [OALROOT="$withval"], [OALROOT=$1])

   if test "x$OALROOT" != "xno" ; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CFLAGS="$CFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"
      dpp_save_LIBS="$LIBS"

      dnl Add the user-specified OpenAL installation directory to these
      dnl paths.  Ensure that /usr/include and /usr/lib are not included
      dnl multiple times if $OALROOT is "/usr".
      if test "x$OALROOT" != "x/usr" ; then
         CPPFLAGS="$CPPFLAGS -I$OALROOT/include"
      fi

      CFLAGS="$CFLAGS $ABI_FLAGS"

      dnl Win32 test.
      if test "x$OS_TYPE" = "xWin32" ; then
         DPP_LANG_SAVE
         DPP_LANG_C

         dnl XXX: I am assuming we'll be using Visual C++.  Worse yet, this may
         dnl just be a nasty hack to deal with unexpected behavior when spaces
         dnl appear in path names ...
dnl         LDFLAGS="/link /libpath:\"$OALROOT/libs\" $LDFLAGS"
         LDFLAGS="-L\"$OALROOT/libs\" $LDFLAGS"
         LIBS="$LIBS OpenAL32.lib $DYN_LOAD_LIB"

         AC_CACHE_CHECK([for alEnable in OpenAL32.lib],
                        [dpp_cv_alEnable_openal_lib],
                        [AC_TRY_LINK([#include <windows.h>
#include <al.h>],
                           [alEnable(0);],
                           [dpp_cv_alEnable_openal_lib='yes'],
                           [dpp_cv_alEnable_openal_lib='no'])])

         DPP_LANG_RESTORE

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         dnl Success.
         if test "x$dpp_cv_alEnable_openal_lib" = "xyes" ; then
            dpp_have_openal='yes'
            AL_LIBDIR="$OALROOT/libs"
            ifelse([$2], , :, [$2])
         dnl Failure.
         else
            dpp_have_openal='no'
            ifelse([$3], , :, [$3])
         fi
      elif test "x$PLATFORM" = "xDarwin" ; then
         DPP_LANG_SAVE
         DPP_LANG_C

         CPPFLAGS="-I$OALROOT/OpenAL.framework/Headers $CPPFLAGS"
         LDFLAGS="-F$OALROOT $LDFLAGS"
         LIBS="$LIBS -framework OpenAL"

         AC_CACHE_CHECK([for alEnable in OpenAL framework],
                        [dpp_cv_alEnable_openal_lib],
                        [AC_TRY_LINK([#include <al.h>], [alEnable(0);],
                           [dpp_cv_alEnable_openal_lib='yes'],
                           [dpp_cv_alEnable_openal_lib='no'])])

         DPP_LANG_RESTORE

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         dnl Success.
         if test "x$dpp_cv_alEnable_openal_lib" = "xyes" ; then
            dpp_have_openal='yes'
            AL_LIBDIR="$OALROOT"
            ifelse([$2], , :, [$2])
         dnl Failure.
         else
            dpp_have_openal='no'
            ifelse([$3], , :, [$3])
         fi
      dnl Other platforms.
      else
         if test "lib$LIBBITSUF" != "lib" ; then
            libdirs="lib$LIBBITSUF lib"
         else
            libdirs="lib"
         fi

         LIBS="-lopenal $LIBS $DYN_LOAD_LIB -lm"

         DPP_LANG_SAVE
         DPP_LANG_C

         for l in $libdirs ; do
            cur_al_libdir="$OALROOT/$l"

            dnl The pthreads-related macros will set only one of
            dnl $PTHREAD_ARG or $PTHREAD_LIB, so it's safe (and simpler)
            dnl for us to use both here.
            LDFLAGS="-L$cur_al_libdir $dpp_save_LDFLAGS $PTHREAD_ARG $PTHREAD_LIB"

            AC_MSG_CHECKING([for alEnable in -lopenal in $cur_al_libdir])
            AC_TRY_LINK([#include <AL/al.h>], [alEnable(0);],
                        [dpp_have_openal='yes'], [dpp_have_openal='no'])
            AC_MSG_RESULT([$dpp_have_openal])

            if test "$dpp_have_openal" = "yes" ; then
               AL_LIBDIR="$cur_al_libdir"
               break
            fi
         done

         DPP_LANG_RESTORE

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         dnl Success.
         if test "x$dpp_have_openal" = "xyes" ; then
            ifelse([$2], , :, [$2])
         dnl Failure.
         else
            ifelse([$3], , :, [$3])
         fi
      fi

      dnl If OpenAL API files were found, define this extra stuff that may be
      dnl helpful in some Makefiles.
      if test "x$dpp_have_openal" = "xyes" ; then
         if test "x$OS_TYPE" = "xWin32" ;  then
            LIBOPENAL='OpenAL32.lib'
         elif test "x$PLATFORM" = "xDarwin" ; then
            LIBOPENAL='-framework OpenAL'
         else
            LIBOPENAL="-lopenal -lm"
         fi

         if test "x$PLATFORM" = "xDarwin" ; then
            if test "x$OALROOT" != "x" ; then
               AL_INCLUDES="-I$OALROOT/OpenAL.framework/Headers"
               AL_LDFLAGS="-F$OALROOT"
            fi
         else
            if test "x$OALROOT" != "x/usr" ; then
               AL_INCLUDES="-I$OALROOT/include"

               if test "x$OS_TYPE" = "xWin32" ; then
                  AL_LDFLAGS="-L$OALROOT/libs"
               else
                  AL_LDFLAGS="-L$OALROOT/lib"
               fi
            fi
         fi

         OPENAL='yes'
      fi
   fi

   dnl Export all of the output vars for use by makefiles and configure script.
   AC_SUBST(OPENAL)
   AC_SUBST(OALROOT)
   AC_SUBST(LIBOPENAL)
   AC_SUBST(AL_INCLUDES)
   AC_SUBST(AL_LDFLAGS)
   AC_SUBST(AL_LIBDIR)
])
