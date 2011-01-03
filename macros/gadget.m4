dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
dnl
dnl VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
dnl Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl Boston, MA 02110-1301, USA.
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
dnl     GADGET_USE_COCOA
dnl ---------------------------------------------------------------------------
AC_DEFUN([GADGETEER_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from flagpoll
    AC_ARG_WITH(gadgeteer-meta-file,
                [  --with-gadgeteer-meta-file=<PATH> Flagpoll metadata file
                          for Gadgeteer (optional)      [No default]],
                [gadget_meta_file="$withval"], [gadget_meta_file=""])

    dnl See if the user specified where to find the Gadgeteer meta file. If
    dnl not, take a guess.
    if test "x$gadget_meta_file" != "x" ; then
        gadget_meta_dir=`dirname $gadget_meta_file`
        gadget_flagpoll_args="--extra-paths=$gadget_meta_dir gadgeteer --from-file=$gadget_meta_file"
    elif test -f "$instlinks/share/flagpoll/gadgeteer.fpc" ; then
        gadget_meta_dir="$instlinks/share/flagpoll"
        gadget_flagpoll_args="--extra-paths=$gadget_meta_dir gadgeteer --from-file=$gadget_meta_dir/gadgeteer.fpc"
    else
        gadget_flagpoll_args="gadgeteer"
    fi

    AM_PATH_FLAGPOLL([0.9.4], ,
                     [AC_MSG_ERROR(*** Flagpoll required for Gadgeteer flags ***)])
    min_gadget_version=ifelse([$1], ,0.0.1,[$1])

    dnl Do a sanity check to ensure that $FLAGPOLL actually works.
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
    fi

    no_gadgeteer=''
    if test "x$FLAGPOLL" = "xno" ; then
        no_gadgeteer=yes
    else
        if ! (eval $FLAGPOLL $gadget_flagpoll_args --modversion >/dev/null 2>&1)
        then
            AC_MSG_WARN([*** Flagpoll has no valid Gadgeteer configuration ***])
            no_gadgeteer=yes
        else
            GADGET_CXXFLAGS=`$FLAGPOLL $gadget_flagpoll_args --cflags`
            GADGET_LIBS=`$FLAGPOLL $gadget_flagpoll_args --libs`
            GADGET_PROF_LIBS=`$FLAGPOLL $gadget_flagpoll_args --get-profiled-libs`
            GADGET_LIBS_STATIC=`$FLAGPOLL $gadget_flagpoll_args --get-static-libs`
            GADGET_PROF_LIBS_STATIC=`$FLAGPOLL $gadget_flagpoll_args --get-profiled-static-libs`
            GADGET_EXTRA_LIBS_LD=`$FLAGPOLL $gadget_flagpoll_args --get-extra-libs`
            GADGET_VERSION=`$FLAGPOLL $gadget_flagpoll_args --modversion`

            GADGET_USE_X11=`$FLAGPOLL $gadget_flagpoll_args --get-use-x11`
            GADGET_USE_COCOA=`$FLAGPOLL $gadget_flagpoll_args --get-use-cocoa`
            GADGET_HEADER_SUBDIR=`$FLAGPOLL $gadget_flagpoll_args --get-gadget-header-subdir`
            GADGET_DATA_SUBDIR=`$FLAGPOLL $gadget_flagpoll_args --get-gadget-data-subdir`
            GADGET_PLUGIN_SUBDIR=`$FLAGPOLL $gadget_flagpoll_args --get-gadget-plugin-subdir`

            DPP_VERSION_CHECK_MSG_NO_CACHE([Gadgeteer], [$GADGET_VERSION],
                                           [$min_gadget_version], [$2], [$3])
        fi
    fi

    if test "x$no_gadgeteer" != x ; then
        if test "$FLAGPOLL" = "no" ; then
           echo "*** Flagpoll is required to build Gadgeteer."
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
        GADGET_USE_COCOA=''

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
    AC_SUBST(GADGET_USE_COCOA)
    AC_SUBST(GADGET_HEADER_SUBDIR)
    AC_SUBST(GADGET_DATA_SUBDIR)
    AC_SUBST(GADGET_PLUGIN_SUBDIR)
])
