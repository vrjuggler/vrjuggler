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
dnl File:          opengl.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.19
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's OpenGL installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_OPENGL - Determine if the target system has OpenGL (or
dnl                       Mesa3D) installed.
dnl
dnl Command-line options added:
dnl     --enable-mesa  - State that the OpenGL implementation on the target
dnl                      system is Mesa3D and that libMesaGL* should be used
dnl                      for linking.
dnl     --with-oglroot - Give the root directory of the OpenGL implementation
dnl                      installation.
dnl
dnl Variables defined:
dnl     USE_MESA     - Either 'yes' or 'no' depending on whether Mesa3D is
dnl                    being used.
dnl     OGLROOT      - The OpenGL installation directory.
dnl     LIBOPENGL    - The list of libraries to link for OpenGL appliations.
dnl     GL_LIB       - The basic OpenGL library.
dnl     GLU_LIB      - The OpenGL utilities library.
dnl     OGL_INCLUDES - Extra include path for the OpenGL header directory.
dnl     OGL_LDFLAGS  - Extra linker flags for the OpenGL library directory.
dnl     X_INCLUDES   - Extra include path for the X11 header directory.
dnl     X_LDFLAGS    - Extra linker flags for the X11 library directory.
dnl ===========================================================================

dnl opengl.m4,v 1.19 2001/06/29 23:47:52 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has OpenGL (or Mesa3D) installed.  This
dnl adds command-line arguments --with-oglroot and --enable-mesa.
dnl
dnl Usage:
dnl     DPP_HAVE_OPENGL(glroot, use-mesa [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     glroot              - The default directory where the OpenGL
dnl                           installation is rooted.  This directory should
dnl                           contain an include/GL directory with the GL
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the GL libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-oglroot command-line argument.
dnl     use-mesa            - This is a 'yes' or 'no' setting for the default
dnl                           use of Mesa3D as the OpenGL implementation.  The
dnl                           value given is used as the default value of the
dnl                           --enable-mesa command-line argument.
dnl     action-if-found     - The action to take if an OpenGL implementation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an OpenGL implementation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_OPENGL,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Use the Mesa OpenGL implementation.  That is, link with libMesaGL and
    dnl libMesaGLU.
    AC_ARG_ENABLE(mesa,
                [  --enable-mesa           Use Mesa3D for OpenGL           [default=$2]],
                USE_MESA="$enableval", USE_MESA=$2)

    dnl Define the root directory for the OpenGL installation.
    AC_ARG_WITH(oglroot,
                [  --with-oglroot=<PATH>   OpenGL installation directory   [default=$1]],
                OGLROOT="$withval", OGLROOT=$1)

    dnl Get the path to the X-Window libraries and header files.  We will need
    dnl these for all UNIX applications.
    AC_REQUIRE([AC_PATH_X])

    if test "x$USE_MESA" = "xyes" ; then
        MESA='Mesa'
    else
        MESA=''
    fi

    dpp_have_opengl='no'
    LIBOPENGL=''

    dnl Save these values in case they need to be restored later.
    dpp_save_CFLAGS="$CFLAGS"
    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_INCLUDES="$INCLUDES"
    dpp_save_LDFLAGS="$LDFLAGS"

    if test "x$dpp_platform" = "xDarwin" ; then
        OGLROOT='/System/Library/Frameworks/OpenGL.framework'
    fi

    dnl Add the user-specified OpenGL installation directory to these
    dnl paths.  Ensure that /usr/include and /usr/lib are not included
    dnl multiple times if $OGLROOT is "/usr".
    if test "x$OGLROOT" != "x/usr" -a "x$dpp_platform" != "xDarwin" ; then
        CPPFLAGS="$CPPFLAGS -I$OGLROOT/include"
        INCLUDES="$INCLUDES -I$OGLROOT/include"
        LDFLAGS="-L$OGLROOT/lib$LIBBITSUF $LDFLAGS"
    fi

    CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"

    dnl On HP-UX, we have to compile the test code with the C++ compiler
    dnl because the HP-UX OpenGL 1.1 implementation mandates this.
    if test "x$dpp_platform" = "xHP" ; then
        X_LDFLAGS=''
        X_INCLUDES=''

        if test "x$x_libraries" != "xNONE" -a "x$x_libraries" != "x" ; then
            X_LDFLAGS="-L$x_libraries"
        fi

        if test "x$x_includes" != "xNONE" -a "x$x_includes" != "x" ; then
            X_INCLUDES="-I$x_includes"
        fi

        DPP_LANG_SAVE
        DPP_LANG_CPLUSPLUS

        dpp_save_LIBS="$LIBS"
        LIBS="$LIBS -l${MESA}GL"

        dnl Check the cache in case this test was run previously and if
        dnl not, compile the given code and try to link it against the GL
        dnl library.  We don't need to test for the existence of GL/gl.h
        dnl after linking because the test program already includes it.
        AC_CACHE_CHECK(for glEnable in -l${MESA}GL, ac_cv_glEnable_available,
            AC_TRY_LINK([#include <GL/gl.h>], [glEnable(GL_CULL_FACE)],
                ac_cv_glEnable_available='yes',
                LIBS="$dpp_save_LIBS"))

        if test "x$ac_cv_glEnable_available" = "xyes" ; then
            dpp_have_opengl='yes'
            ifelse([$3], , :, [$3])
        else
            ifelse([$4], , :, [$4])
        fi

        dnl This is necessary again because AC_CHECK_LIB() adds -l${MESA}GLU
        dnl to $LIBS.
        LIBS="$dpp_save_LIBS"

        DPP_LANG_RESTORE
    elif test "x$dpp_platform" = "xDarwin" ; then
        AC_LANG_SAVE
        AC_LANG_C

        CPPFLAGS="$CPPFLAGS -F$OGLROOT"
        LDFLAGS="$LDFLAGS -framework OpenGL"

        AC_CACHE_CHECK(for glEnable in OpenGL framework,
                       dpp_cv_glEnable_framework_OpenGL,
                       AC_TRY_LINK([#include <OpenGL/gl.h>],
                           [glEnable(GL_LIGHTING)],
                           dpp_cv_glEnable_framework_OpenGL='yes',
                           dpp_cv_glEnable_framework_OpenGL='no'))

        if test "x$dpp_cv_glEnable_framework_OpenGL" = "xno" ; then
            ifelse([$4], , :, [$4])
        fi

        AC_CACHE_CHECK(for gluNewQuadric in OpenGL framework,
                       dpp_cv_gluNewQuadric_framework_OpenGL,
                       AC_TRY_LINK([
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>],
                           [gluNewQuadric()],
                           dpp_cv_gluNewQuadric_framework_OpenGL='yes',
                           dpp_cv_gluNewQuadric_framework_OpenGL='no'))

        if test "x$dpp_cv_gluNewQuadric_framework_OpenGL" = "xno" ; then
            ifelse([$4], , :, [$4])
        else
            OGL_INCLUDES="-F$OGLROOT"
            OGL_LDFLAGS="-framework OpenGL"
            ifelse([$3], , :, [$3])
        fi

        DPP_LANG_RESTORE
    elif test "x$dpp_os_type" = "xWin32" ; then
        AC_LANG_SAVE
        AC_LANG_C

        LIBS="$LIBS opengl32.lib"

        AC_CACHE_CHECK(for glEnable in opengl32.lib,
                       dpp_cv_glEnable_opengl32_lib,
                       AC_TRY_LINK([#include <windows.h>
#include <GL/gl.h>],
                           [glEnable(GL_LIGHTING);],
                           dpp_cv_glEnable_opengl32_lib='yes',
                           dpp_cv_glEnable_opengl32_lib='no'))

        if test "x$dpp_cv_glEnable_opengl32_lib" = "xno" ; then
            ifelse([$4], , :, [$4])
        fi

        LIBS="$LIBS glu32.lib"

        AC_CACHE_CHECK(for gluNewQuadric in glu32.lib,
                       dpp_cv_gluNewQuadric_glu32_lib,
                       AC_TRY_LINK([#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>],
                           [gluNewQuadric();],
                           dpp_cv_gluNewQuadric_glu32_lib='yes',
                           dpp_cv_gluNewQuadric_glu32_lib='no'))

        LIBS="$dpp_save_LIBS"

        if test "x$dpp_cv_glEnable_opengl32_lib" = "xyes" ; then
            dpp_have_opengl='yes'
            ifelse([$3], , :, [$3])
        else
            ifelse([$4], , :, [$4])
        fi

        AC_LANG_RESTORE
    else
        X_LDFLAGS=''

        if test "x$x_libraries" != "xNONE" -a "x$x_libraries" != "x" ; then
            X_LDFLAGS="-L$x_libraries"
        fi

        LDFLAGS="$X_LDFLAGS $LDFLAGS"

        dpp_save_LIBS="$LIBS"

        AC_LANG_SAVE
        AC_LANG_C

        AC_CHECK_LIB(${MESA}GL, glEnable, , $4, -lX11 -lXext -lm)

        dnl This is necessary because AC_CHECK_LIB() adds -l${MESA}GL to
        dnl $LIBS.  We want to do that ourselves later.
        LIBS="$dpp_save_LIBS"

        dnl If we have reached this point, the previous test succeeded.  Now
        dnl we need to ensure that the compiler can find the GL headers.
        dnl The goal here is to provide a fallback to the directory in
        dnl $x_includes since that is a reasonable location for them to be
        dnl (if they are not in /usr/include).  However, we do not want to
        dnl bother with this if the user gave us a value for $OGLROOT.
        if test "x$OGLROOT" = "x/usr" -a "x$x_includes" != "x" -a	\
                "x$x_includes" != "xNONE"
        then
            X_INCLUDES="-I$x_includes"
        fi

        CPPFLAGS="$CPPFLAGS $X_INCLUDES"
        INCLUDES="$INCLUDES $X_INCLUDES"

        AC_CHECK_HEADER(GL/gl.h, , $4)

        AC_CHECK_LIB(${MESA}GLU, gluNewQuadric, , $4,
            -l${MESA}GL -lm -lX11 -lXext -lXt)

        dnl This is necessary again because AC_CHECK_LIB() adds -l${MESA}GLU
        dnl to $LIBS.
        LIBS="$dpp_save_LIBS"

        AC_CHECK_HEADER(GL/glu.h, dpp_have_opengl='yes', $4)

        if test "x$dpp_have_opengl" = "xyes" ; then
            ifelse([$3], , :, [$3])
        fi

        AC_LANG_RESTORE
    fi

    dnl If OpenGL API files were found, define this extra stuff that may be
    dnl helpful in some Makefiles.
    if test "x$dpp_have_opengl" = "xyes" ; then
        if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBOPENGL="-l${MESA}GLU -l${MESA}GL $X_LDFLAGS -lX11 -lXext"
            GL_LIB="-l${MESA}GL"
            GLU_LIB="-l${MESA}GLU"
        else
            LIBOPENGL='opengl32.lib glu32.lib'
            GL_LIB='opengl32.lib'
            GLU_LIB='glu32.lib'
        fi

        if test "x$OGLROOT" != "x/usr" ; then
            OGL_INCLUDES="-I$OGLROOT/include"
            OGL_LDFLAGS="-L$OGLROOT/lib\$(LIBBITSUF)"
        fi
    fi

    dnl Restore all the variables now that we are done testing.
    CFLAGS="$dpp_save_CFLAGS"
    CPPFLAGS="$dpp_save_CPPFLAGS"
    INCLUDES="$dpp_save_INCLUDES"
    LDFLAGS="$dpp_save_LDFLAGS"

    AC_SUBST(OGLROOT)
])
