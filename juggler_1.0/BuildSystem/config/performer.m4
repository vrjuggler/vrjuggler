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
dnl File:          performer.m4,v
dnl Date modified: 2001/06/29 23:47:52
dnl Version:       1.8
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Find the target host's IRIS/OpenGL Performer installation if one exists.
dnl This supports Performer 2.2, 2.3, and 2.4.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_ERFORMER - Determine if the target system has IRIS/OpenGL
dnl                         Performer installed.
dnl
dnl Command-line options added:
dnl     --with-pfroot - Give the root directory of the Perfromer installation.
dnl     --with-pfver  - Give the Performer version installed on the target
dnl                     system.
dnl
dnl Variables defined:
dnl     PFROOT      - The Performer installation directory.
dnl     PF_VER      - The Performer version being used.
dnl     LIBERFORMER - The list of libraries to link for Performer appliations.
dnl     PF_INCLUDES - Extra include path for the Performer header directory.
dnl     PF_LDFLAGS  - Extra linker flags for the Performer library directory.
dnl ===========================================================================

dnl performer.m4,v 1.8 2001/06/29 23:47:52 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has IRIS/OpenGL Performer installed.  This
dnl adds command-line arguments --with-pfroot and --with-pfver.
dnl
dnl Usage:
dnl     DPP_HAVE_PERFORMER(pfroot [, version, [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     pfroot              - The default directory where the Performer
dnl                           installation is rooted.  This directory should
dnl                           contain an include/Performer directory with the
dnl                           Performer headers and a lib (with appropriate
dnl                           bit suffix) directory with the Performer
dnl                           libraries.  The value given is used as the
dnl                           default value of the --with-prfroot command-line
dnl                           argument.
dnl     version             - Specify which version of Performer will be used.
dnl                           The value given is used as the default value of
dnl                           the --with-pfver command-line argument.  This
dnl                           argument is optional and if not specified, it
dnl                           assumes version 2.4.
dnl     action-if-found     - The action to take if Performer is found.  This
dnl                           argument is optional.
dnl     action-if-not-found - The action to take if Performer is not found.
dnl                           This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_PERFORMER,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Define the root directory for the Performer installation.
    AC_ARG_WITH(pfroot,
                [  --with-pfroot=<PATH>    Performer installation
                          directory                       [default=$1]],
                PFROOT="$withval", PFROOT=$1)

    dnl Define the Performer version installed..
    AC_ARG_WITH(pfver,
                [  --with-pfver=<VER>      Performer version to use        [default=$2]],
                PF_VER="$withval", PF_VER=$2)

    dpp_save_CFLAGS="$CFLAGS"
    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_INCLUDES="$INCLUDES"
    dpp_save_LDFLAGS="$LDFLAGS"
    dpp_save_LIBS="$LIBS"

    dnl Ensure that /usr/include and /usr/lib are not included multiple times
    dnl if $PFROOT is "/usr".
    if test "x$PFROOT" != "x/usr" ; then
        CPPFLAGS="-I$PFROOT/include $CPPFLAGS"
        INCLUDES="-I$PFROOT/include $INCLUDES"
        LDFLAGS="-L$PFROOT/lib$LIBBITSUF $LDFLAGS"
    fi

    CFLAGS="$CFLAGS $INCLUDES ${_EXTRA_FLAGS}"

    dnl Default to OpenGL Performer 2.4 if nothing was given for the default
    dnl argument.
    if test "x$PF_VER" = "x" ; then
        PF_VER='2.4'
    fi

    dnl We are using Performer 2.[23] as defined by the second argument.
    if test "x$2" = "x2.2" -o "x$2" = "x2.3" ; then
        pf_ext='_ogl'
    dnl We are using Performer 2.[23] as defined by the --with-pfver
    dnl command-line argument.
    elif test "x$PF_VER" = "x2.2" -o "x$PF_VER" = "x2.3" ; then
        pf_ext='_ogl'
    dnl We are using Performer 2.4 or newer.
    else
        pf_ext=''
    fi

    dnl On Linux, we have to link all the Performer libraries AND libdl to
    dnl get the check for pfInit() in libpf to compile.
    if test "x$dpp_platform" = "xLinux" ; then
        LIBS="$LIBS -lpf$pf_ext -lpfdu$pf_ext -lpfui -lpfutil$pf_ext -ldl"
    fi

    dpp_have_performer='no'

    dnl If the Performer library was found, add the API object files to the
    dnl files to be compiled and enable the Performer API compile-time
    dnl option.
    AC_CHECK_LIB(pf, pfInit,
        AC_CHECK_HEADER(Performer/pf.h, dpp_have_performer='yes', $4),
        $4)

    if test "x$dpp_have_performer" = "xyes" ; then
        ifelse([$3], , :, [$3])
    fi

    LIBS="$dpp_save_LIBS"

    dnl Restore $LIBS to its original value (it was modified before the
    dnl previous check block).
    if test "x$dpp_have_performer" = "xyes" ; then
        LIBPERFORMER="-lpf$pf_ext -lpfdu$pf_ext -lpfui -lpfutil$pf_ext"

        case $dpp_platform in
            dnl Under IRIX users will need -limage to load .flt files
            dnl referencing .sgi images.
            IRIX)
                LIBPERFORMER="$LIBPERFORMER -limage"
                ;;
            dnl Append -ldl to $LIBPERFORMER on Linux.
            Linux)
                LIBPERFORMER="$LIBPERFORMER -ldl"
                ;;
        esac

        if test "x$PFROOT" != "x/usr" ; then
            PF_INCLUDES="-I$PFROOT/include"
            PF_LDFLAGS="-L$PFROOT/lib\$(LIBBITSUF)"
        fi
    fi

    dnl Restore all the variables now that we are done testing.
    CFLAGS="$dpp_save_CFLAGS"
    CPPFLAGS="$dpp_save_CPPFLAGS"
    INCLUDES="$dpp_save_INCLUDES"
    LDFLAGS="$dpp_save_LDFLAGS"

    AC_SUBST(PFROOT)
])
