dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
dnl
dnl VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
dnl ************* <auto-copyright.pl END do not edit this line> *************

dnl ---------------------------------------------------------------------------
dnl VRJUGGLER_PATH([minimum-version, [action-if-found [, action-if-not-found [, modules]]]])
dnl
dnl Test for VR Juggler and then define the following variables:
dnl     VRJ_CXXFLAGS
dnl     VRJ_LIBS
dnl     VRJ_PROF_LIBS
dnl     VRJ_LIBS_STATIC
dnl     VRJ_PROF_LIBS_STATIC
dnl     VRJ_EXTRA_LIBS
dnl ---------------------------------------------------------------------------
AC_DEFUN([VRJUGGLER_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from the vrjuggler-config script
    AC_ARG_WITH(vrjuggler-meta-file,
                [  --with-vrjuggler-meta-file=<PATH> Flagpoll metadata file
                         for VR Juggler                       [No default]],
                vrj_config_prefix="$withval", vrj_config_prefix="")

    dnl See if the user specified where to find the meta file 
    dnl if they didn't take a guess for them
    if test "x$vrj_meta_file" != "x" ; then
        vrj_flagpoll_args="--from-file=$vrj_meta_file"
    else
        vrj_flagpoll_args="--from-file=$instlinks/share/flagpoll/vrjuggler.fpc"
    fi

    vrj_flagpoll_args="vrjuggler $vrj_flagpoll_args --no-deps"

    AC_PATH_PROG(FLAGPOLL, flagpoll, no)
    min_vrj_version=ifelse([$1], ,0.0.1,$1)

    dnl Do a sanity check to ensure that $FLAGPOLL actually works.
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
    fi

    no_vrj=''
    if test "x$FLAGPOLL" = "xno" ; then
        no_vrj=yes
    else
        VRJ_CXXFLAGS=`$FLAGPOLL $vrj_flagpoll_args -cflags`
        VRJ_LIBS="`$FLAGPOLL $vrj_flagpoll_args --get-libs`"
        VRJ_PROF_LIBS="`$FLAGPOLL $vrj_flagpoll_args --get-profiled-libs`"
        VRJ_LIBS_STATIC="`$FLAGPOLL $vrj_flagpoll_args --get-static-libs`"
        VRJ_PROF_LIBS_STATIC="`$FLAGPOLL $vrj_flagpoll_args --get-profiled-static-libs`"
        VRJ_EXTRA_LIBS=`$FLAGPOLL $vrj_flagpoll_args --get-extra-libs`
        VRJ_VERSION=`$FLAGPOLL $vrj_flagpoll_args --modversion`

        DPP_VERSION_CHECK_MSG_NO_CACHE([VR Juggler], [$VRJ_VERSION],
                                       [$min_vrj_version], [$2], [$3])
    fi

    if test "x$no_vrj" != x ; then
        if test "$FLAGPOLL" = "no" ; then
           echo "*** Flagpoll is required to build VR Juggler."
           echo "*** Please check that the PATH variable is set to "
           echo "*** include the proper path to flagpoll."
        fi
        VRJ_CXXFLAGS=""
        VRJ_LIBS=""
        VRJ_PROF_LIBS=""
        VRJ_LIBS_STATIC=""
        VRJ_PROF_LIBS_STATIC=""
        VRJ_EXTRA_LIBS=""
        VRJ_VERSION="-1"
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(VRJ_CXXFLAGS)
    AC_SUBST(VRJ_LIBS)
    AC_SUBST(VRJ_PROF_LIBS)
    AC_SUBST(VRJ_LIBS_STATIC)
    AC_SUBST(VRJ_PROF_LIBS_STATIC)
    AC_SUBST(VRJ_EXTRA_LIBS)
    AC_SUBST(VRJ_VERSION)
])
