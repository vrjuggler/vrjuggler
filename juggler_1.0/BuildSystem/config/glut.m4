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
dnl File:          glut.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.3
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's GLUT installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_GLUT - Determine if the target system has GLUT.
dnl
dnl Command-line options added:
dnl     --with-glutroot - Give the root directory of the GLUT installation.
dnl
dnl Variables defined:
dnl     GLUTROOT      - The GLUT installation directory.
dnl     LIBGLUT       - The list of libraries to link for GLUT appliations.
dnl     GLUT_INCLUDES - Extra include path for the GLUT header directory.
dnl     GLUT_LDFLAGS  - Extra linker flags for the GLUT library directory.
dnl ===========================================================================

dnl glut.m4,v 1.3 2001/06/29 23:47:52 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has GLUT installed.  This adds the
dnl command-line argument --with-glutroot.
dnl
dnl Usage:
dnl     DPP_HAVE_GLUT(glutroot[, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     glutroot            - The default directory where the GLUT
dnl                           installation is rooted.  This directory should
dnl                           contain an include/GL directory with the GLUT
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the GLUT libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-glutroot command-line argument.
dnl     action-if-found     - The action to take if an GLUT implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an GLUT implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_GLUT,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Define the root directory for the GLUT installation.
    AC_ARG_WITH(glutroot,
                [  --with-glutroot=<PATH>  GLUT installation directory     [default=$1]],
                GLUTROOT="$withval", GLUTROOT=$1)

    dnl Get the path to the X-Window libraries and header files.  We will need
    dnl these for all UNIX applications.
    AC_REQUIRE([AC_PATH_X])
    AC_REQUIRE([DPP_HAVE_OPENGL])

    dpp_have_glut='no'
    LIBGLUT=''

    dnl Save these values in case they need to be restored later.
    dpp_save_CFLAGS="$CFLAGS"
    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_INCLUDES="$INCLUDES"
    dpp_save_LDFLAGS="$LDFLAGS"

    dnl Add the user-specified GLUT installation directory to these
    dnl paths.  Ensure that /usr/include and /usr/lib are not included
    dnl multiple times if $GLUTROOT is "/usr".
    if test "x$GLUTROOT" != "x/usr" ; then
        CPPFLAGS="$CPPFLAGS -I$GLUTROOT/include"
        INCLUDES="$INCLUDES -I$GLUTROOT/include"
        LDFLAGS="-L$GLUTROOT/lib$LIBBITSUF $LDFLAGS"
    fi

    CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

    dnl On HP-UX, we have to compile the test code with the C++ compiler
    dnl because the HP-UX GLUT 1.1 implementation mandates this.
    if test "x$dpp_os_type" = "xWin32" ; then
        AC_MSG_WARN(Assuming that GLUT is available in a standard location)
        dpp_have_glut='yes'
        ifelse([$3], , :, [$3])
    else
        X_LDFLAGS=''

        if test "x$x_libraries" != "xNONE" -a "x$x_libraries" != "x" ; then
            X_LDFLAGS="-L$x_libraries"
        fi

        LDFLAGS="$X_LDFLAGS $LDFLAGS"

        dpp_save_LIBS="$LIBS"
        LIBS="$LIBS -lglut -l${MESA}GL -l${MESA}GLU -lX11 -lXext -lXmu -lm"

        AC_LANG_SAVE
        AC_LANG_C

        dnl Check the cache in case this test was run previously and if
        dnl not, compile the given code and try to link it against the GL
        dnl library.  We don't need to test for the existence of GL/gl.h
        dnl after linking because the test program already includes it.
        AC_CACHE_CHECK(for glutMainLoop in -lglut,
            dpp_cv_glutMainLoop_available,
            AC_TRY_LINK([#include <GL/glut.h>], [glutMainLoop()],
                dpp_cv_glutMainLoop_available='yes',
                dpp_cv_glutMainLoop_available='no'))

        LIBS="$dpp_save_LIBS"

        if test "x$dpp_cv_glutMainLoop_available" = "xyes" ; then
            dpp_have_glut='yes'
            ifelse([$3], , :, [$3])
        else
            ifelse([$4], , :, [$4])
        fi

        AC_LANG_RESTORE
    fi

    dnl If GLUT API files were found, define this extra stuff that may be
    dnl helpful in some Makefiles.
    if test "x$dpp_have_glut" = "xyes" ; then
        if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBGLUT="-lglut"
        else
            LIBGLUT='glut.lib'
        fi

        if test "x$GLUTROOT" != "x/usr" ; then
            GLUT_INCLUDES="-I$GLUTROOT/include"
            GLUT_LDFLAGS="-L$GLUTROOT/lib\$(LIBBITSUF)"
        fi
    fi

    dnl Restore all the variables now that we are done testing.
    CFLAGS="$dpp_save_CFLAGS"
    CPPFLAGS="$dpp_save_CPPFLAGS"
    INCLUDES="$dpp_save_INCLUDES"
    LDFLAGS="$dpp_save_LDFLAGS"

    AC_SUBST(GLUTROOT)
])
