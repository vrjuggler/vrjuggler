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
dnl File:          openal.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.2
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's OpenAL installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_OPENAL - Determine if the target system has OpenAL installed.
dnl
dnl Command-line options added:
dnl     --with-oalroot - Give the root directory of the OpenAL implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     OPENAL      - do we have openal on the system?
dnl     OALROOT     - The OpenAL installation directory.
dnl     LIBOPENAL   - The list of libraries to link for OpenAL appliations.
dnl     AL_INCLUDES - Extra include path for the OpenAL header directory.
dnl     AL_LDFLAGS  - Extra linker flags for the OpenAL library directory.
dnl ===========================================================================

dnl openal.m4,v 1.2 2001/06/29 23:47:52 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has OpenAL installed.  This
dnl adds command-line arguments --with-oalroot.
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
dnl                           --with-oalroot command-line argument.
dnl     action-if-found     - The action to take if an OpenAL implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an OpenAL implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_OPENAL,
[
    dnl initialize returned data...
    OPENAL='n'
    LIBOPENAL=''
    AL_INCLUDES=''
    AL_LDFLAGS=''
    dpp_have_openal='no'
    
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Define the root directory for the OpenAL installation.
    AC_ARG_WITH(oalroot,
                [  --with-oalroot=<PATH>   OpenAL installation directory   [default=$1]],
                OALROOT="$withval", OALROOT=$1)

    dnl Save these values in case they need to be restored later.
    dpp_save_CFLAGS="$CFLAGS"
    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_INCLUDES="$INCLUDES"
    dpp_save_LDFLAGS="$LDFLAGS"

    dnl Add the user-specified OpenAL installation directory to these
    dnl paths.  Ensure that /usr/include and /usr/lib are not included
    dnl multiple times if $OALROOT is "/usr".
    if test "x$OALROOT" != "x/usr" ; then
        CPPFLAGS="$CPPFLAGS -I$OALROOT/include"
        INCLUDES="$INCLUDES -I$OALROOT/include"
        if test -d "$OALROOT/lib" ; then
            LDFLAGS="-L$OALROOT/lib $LDFLAGS"
        fi
    fi
   
   

    dnl debugging output...
    dnl echo "+ setting CPPFLAGS == $CPPFLAGS"
    dnl echo "+ setting INCLUDES == $INCLUDES"
    dnl echo "+ setting LDFLAGS == $LDFLAGS"

   
    CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

    echo "+ looking for OpenAL in $OALROOT/lib and $OALROOT/include"

    dnl WIN32 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if test "x$dpp_os_type" = "xWin32" ; then
        AC_LANG_SAVE
        AC_LANG_C

        LIBS="$LIBS ALut.lib OpenAL32.lib"

        AC_CACHE_CHECK(for alEnable in Openal32.lib,
                       dpp_cv_alEnable_openal_lib,
                       AC_TRY_LINK([#include <windows.h>
#include <AL/al.h>],
                           [alEnable(DISTANCE_ATTENUATION);],
                           dpp_cv_alEnable_openal_lib='yes',
                           dpp_cv_alEnable_openal_lib='no'))

        if test "x$dpp_cv_alEnable_openal_lib" = "xno" ; then
            ifelse([$3], , :, [$3])
        fi

        LIBS="$LIBS"

        LIBS="$dpp_save_LIBS"

        if test "x$dpp_cv_alEnable_openal_lib" = "xyes" ; then
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

        AC_CHECK_LIB( openal, alEnable, , $3, -lm )

        dnl This is necessary because AC_CHECK_LIB() adds -lopenal to
        dnl $LIBS.  We want to do that ourselves later.
        LIBS="$dpp_save_LIBS"

        CPPFLAGS="$CPPFLAGS"
        INCLUDES="$INCLUDES"

        AC_CHECK_HEADER(AL/al.h, dpp_have_openal='yes', $3)

        if test "x$dpp_have_openal" = "xyes" ; then
            ifelse([$2], , :, [$2])
        fi

        AC_LANG_RESTORE
    fi

    dnl If OpenAL API files were found, define this extra stuff that may be
    dnl helpful in some Makefiles.
    if test "x$dpp_have_openal" = "xyes" ; then
        if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBOPENAL="-lopenal -lm"
        else
            LIBOPENAL='openal.lib'
        fi

        if test "x$OALROOT" != "x/usr" ; then
            AL_INCLUDES="-I$OALROOT/include"
            AL_LDFLAGS="-L$OALROOT/lib"
        fi
        
        OPENAL='yes'
    fi

    dnl Restore all the variables now that we are done testing.
    CFLAGS="$dpp_save_CFLAGS"
    CPPFLAGS="$dpp_save_CPPFLAGS"
    INCLUDES="$dpp_save_INCLUDES"
    LDFLAGS="$dpp_save_LDFLAGS"

   # export all of the output vars for use by makefiles and configure script
    AC_SUBST(OALROOT)
    AC_SUBST(LIBOPENAL)
    AC_SUBST(AL_INCLUDES)
    AC_SUBST(AL_LDFLAGS)
])
