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
dnl File:          audioworks.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.3
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's AudioWorks installation if one exists.
dnl NOTE: only works on UNIX type systems... (does AW exist on Win32?)
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_AUDIOWORKS - Determine if the target system has AudioWorks installed.
dnl
dnl Command-line options added:
dnl     --with-awroot - Give the root directory of the AudioWorks implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     AUDIOWORKS - does the system have audioworks?
dnl     AWROOT     - The AudioWorks installation directory.
dnl     LIBAUDIOWORKS   - The list of libraries to link for AudioWorks appliations.
dnl     AW_INCLUDES - Extra include path for the AudioWorks header directory.
dnl     AW_LDFLAGS  - Extra linker flags for the AudioWorks library directory.
dnl ===========================================================================

dnl audioworks.m4,v 1.3 2001/06/29 23:47:52 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has AudioWorks installed.  This
dnl adds command-line arguments --with-awroot.
dnl
dnl Usage:
dnl     DPP_HAVE_AUDIOWORKS(awroot [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     awroot             - The default directory where the AudioWorks
dnl                           installation is rooted.  This directory should
dnl                           contain an include/PSI directory with the PSI
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the PSI libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-awroot command-line argument.
dnl     action-if-found     - The action to take if an AudioWorks implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an AudioWorks implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_AUDIOWORKS,
[
    dnl initialize returned data...
    AUDIOWORKS='no'
    LIBAUDIOWORKS=''
    AW_INCLUDES=''
    AW_LDFLAGS=''
    dpp_have_audioworks='no'
   
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Define the root directory for the AudioWorks installation.
    AC_ARG_WITH(awroot,
                [  --with-awroot=<PATH>   AudioWorks installation directory   [default=$1]],
                AWROOT="$withval", AWROOT=$1)

    dnl Save these values in case they need to be restored later.
    dpp_save_CFLAGS="$CFLAGS"
    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_INCLUDES="$INCLUDES"
    dpp_save_LDFLAGS="$LDFLAGS"

    dnl Add the user-specified AudioWorks installation directory to these
    dnl paths.  Ensure that /usr/include and /usr/lib are not included
    dnl multiple times if $AWROOT is "/usr".
    if test "x$AWROOT" != "x/usr" ; then
        CPPFLAGS="$CPPFLAGS -I$AWROOT/include"
        INCLUDES="$INCLUDES -I$AWROOT/include"
        LDFLAGS="-L$AWROOT/lib$LIBBITSUF $LDFLAGS"
    fi
    CPPFLAGS="$CPPFLAGS -I$AWROOT/include/PSI"
    INCLUDES="$INCLUDES -I$AWROOT/include/PSI"
    LDFLAGS="-L$AWROOT/lib$LIBBITSUF/PSI $LDFLAGS"
   
   

    dnl debugging output...
    dnl echo "+ setting CPPFLAGS == $CPPFLAGS"
    dnl echo "+ setting INCLUDES == $INCLUDES"
    dnl echo "+ setting LDFLAGS == $LDFLAGS"

   
    CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

    echo "+ looking for AudioWorks in $AWROOT/lib and $AWROOT/include"


        dnl This is necessary because AC_CHECK_LIB() adds -laudioworks to
        dnl $LIBS.  We want to do that ourselves later.
        dpp_save_LIBS="$LIBS"

        AC_LANG_SAVE
        AC_LANG_CPLUSPLUS

        AC_CHECK_LIB(aw, awInitSys,
        AC_CHECK_HEADER(aw.h,
            dpp_have_audioworks='yes',
            AC_MSG_WARN(*** AudioWorks will not be used (aw.h not found) ***)),
        AC_MSG_WARN(*** AudioWorks will not be used (library not found) ***),
        -lawhwi -lpsi -laudiofile -lm)

        dnl This is necessary because AC_CHECK_LIB() adds -laudioworks to
        dnl $LIBS.  We want to do that ourselves later.
        LIBS="$dpp_save_LIBS"

        if test "x$dpp_have_audioworks" = "xyes" ; then
            ifelse([$2], , :, [$2])
        fi

        AC_LANG_RESTORE

    dnl If AudioWorks API files were found, define this extra stuff that may be
    dnl helpful in some Makefiles.
    if test "x$dpp_have_audioworks" = "xyes" ; then
        LIBAUDIOWORKS="-law -lawhwi -lpsi -laudiofile"
        AW_INCLUDES="-I$AWROOT/include/PSI"
        AW_LDFLAGS="-L$AWROOT/lib$LIBBITSUF/PSI"
        AUDIOWORKS='yes'
    fi

    dnl Restore all the variables now that we are done testing.
    CFLAGS="$dpp_save_CFLAGS"
    CPPFLAGS="$dpp_save_CPPFLAGS"
    INCLUDES="$dpp_save_INCLUDES"
    LDFLAGS="$dpp_save_LDFLAGS"

   # export all of the output vars for use by makefiles and configure script
    AC_SUBST(AUDIOWORKS)
    AC_SUBST(AWROOT)
    AC_SUBST(LIBAUDIOWORKS)
    AC_SUBST(AW_INCLUDES)
    AC_SUBST(AW_LDFLAGS)
])
