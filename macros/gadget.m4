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
dnl GADGETEER_PATH([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Test for Gadgeteer and then define the following variables:
dnl     GADGET_CXXFLAGS
dnl     GADGET_LIBS
dnl     GADGET_PROF_LIBS
dnl     GADGET_LIBS_STATIC
dnl     GADGET_PROF_LIBS_STATIC
dnl     GADGET_EXTRA_LIBS
dnl     GADGET_USE_X11
dnl ---------------------------------------------------------------------------
AC_DEFUN([GADGETEER_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from the gadgeteer-config script
    AC_ARG_WITH(gadgeteer-meta-file,
                [  --with-gadgeteer-meta-file=<PATH> Flagpoll metadata file
                          for Gadgeteer                       [No default]],
                gadget_meta_file="$withval", gadget_meta_file="")

    dnl See if the user specified where to find the meta file
    dnl if they didn't take a guess for them
    if test "x$gadget_meta_file" != "x" ; then
        gadget_flagpoll_args="--from-file=$gadget_meta_file"
    else
        gadget_flagpoll_args="--from-file=$instlinks/share/flagpoll/gadgeteer.fpc"
    fi

    gadget_flagpoll_args="gadgeteer $gadget_flagpoll_args --no-deps"

    AM_PATH_FLAGPOLL([0.7.0], , [AC_MSG_ERROR(*** Flagpoll required for Gadgeteer Flags ***)])
    min_gadget_version=ifelse([$1], , 0.0.1, $1)

    dnl Do a sanity check to ensure that $FLAGPOLL actually works.
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
    fi

    no_gadgeteer=''
    if test "x$FLAGPOLL" = "xno" ; then
        no_gadgeteer=yes
    else
        GADGET_CXXFLAGS=`$FLAGPOLL $gadget_flagpoll_args --cflags`
        GADGET_LIBS="`$FLAGPOLL $gadget_flagpoll_args --get-libs`"
        GADGET_PROF_LIBS="`$FLAGPOLL $gadget_flagpoll_args --get-profiled-libs`"
        GADGET_LIBS_STATIC="`$FLAGPOLL $gadget_flagpoll_args --get-static-libs`"
        GADGET_PROF_LIBS_STATIC="`$FLAGPOLL $gadget_flagpoll_args --get-profiled-static-libs`"
        GADGET_EXTRA_LIBS_LD=`$FLAGPOLL $gadget_flagpoll_args --get-extra-libs`
        GADGET_VERSION=`$FLAGPOLL $gadget_flagpoll_args --modversion`

        GADGET_USE_X11=`$FLAGPOLL $gadget_flagpoll_args --get-use-x11`

        DPP_VERSION_CHECK_MSG_NO_CACHE([Gadgeteer], [$GADGET_VERSION],
                                       [$min_gadget_version], [$2], [$3])
    fi

    if test "x$no_gadgeteer" != x ; then
        if test "$FLAGPOLL" = "no" ; then
           echo "*** Flagpoll is required to build gadgeteer."
           echo "*** Please check that the PATH variable is set to "
           echo "*** include the proper path to flagpoll."
        fi
        GADGET_CXXFLAGS=""
        GADGET_INCLUDES=""
        GADGET_LIBS_LD=""
        GADGET_PROF_LIBS_LD=""
        GADGET_LIBS_STATIC_LD=""
        GADGET_PROF_LIBS_STATIC_LD=""
        GADGET_EXTRA_LIBS_LD=""
        GADGET_VERSION="-1"

        GADGET_USE_X11=''

        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(GADGET_CXXFLAGS)
    AC_SUBST(GADGET_LIBS)
    AC_SUBST(GADGET_PROF_LIBS)
    AC_SUBST(GADGET_LIBS_STATIC)
    AC_SUBST(GADGET_PROF_LIBS_STATIC)
    AC_SUBST(GADGET_EXTRA_LIBS)
    AC_SUBST(GADGET_VERSION)

    AC_SUBST(GADGET_USE_X11)
])
