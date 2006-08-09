dnl **************** <SNX heading BEGIN do not edit this line> ****************
dnl
dnl sonix
dnl
dnl Original Authors:
dnl   Kevin Meinert, Carolina Cruz-Neira
dnl
dnl ***************** <SNX heading END do not edit this line> *****************

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl SNX_PATH([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Test for SNX and then define the following variables:
dnl     SNX_CXXFLAGS
dnl     SNX_LIBS
dnl     SNX_PROF_LIBS
dnl     SNX_LIBS_STATIC
dnl     SNX_PROF_LIBS_STATIC
dnl     SNX_EXTRA_LIBS
dnl     SNX_VERSION
dnl ---------------------------------------------------------------------------
AC_DEFUN([SNX_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from the sonix-config script
    AC_ARG_WITH(sonix-meta-file,
                [  --with-sonix-meta-file=<PATH>     Flagpoll metadata file
                         for Sonix (optional)           [No default]],
                sonix_meta_file="$withval", sonix_meta_file="")

    dnl See if the user specified where to find the meta file 
    dnl if they didn't take a guess for them
    if test "x$sonix_meta_file" != "x" ; then
        sonix_flagpoll_args="--from-file=$sonix_meta_file"
    else
        sonix_flagpoll_args="--from-file=$instlinks/share/flagpoll/sonix.fpc"
    fi

    sonix_flagpoll_args="sonix $sonix_flagpoll_args --no-deps"

    AM_PATH_FLAGPOLL([0.5.0], , [AC_MSG_ERROR(*** Flagpoll required for Sonix Flags ***)])
    min_sonix_version=ifelse([$1], ,0.0.1, [$1])

    dnl Do a sanity check to ensure that $FLAGPOLL actually works.
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
    fi

    no_sonix=''
    if test "x$FLAGPOLL" = "xno" ; then
        no_sonix=yes
    else
        SNX_CXXFLAGS=`$FLAGPOLL $sonix_flagpoll_args --cflags`
        SNX_LIBS="`$FLAGPOLL $sonix_flagpoll_args --get-libs`"
        SNX_PROF_LIBS="`$FLAGPOLL $sonix_flagpoll_args --get-profiled-libs`"
        SNX_LIBS_STATIC="`$FLAGPOLL $sonix_flagpoll_args --get-static-libs`"
        SNX_PROF_LIBS_STATIC="`$FLAGPOLL $sonix_flagpoll_args --get-profiled-static-libs`"
        SNX_EXTRA_LIBS=`$FLAGPOLL $sonix_flagpoll_args --get-extra-libs`
        SNX_VERSION=`$FLAGPOLL $sonix_flagpoll_args --modversion`

        DPP_VERSION_CHECK_MSG_NO_CACHE([Sonix], [$SNX_VERSION],
                                       [$min_sonix_version], [$2], [$3])
    fi

    if test "x$no_sonix" != x ; then
        if test "$FLAGPOLL" = "no" ; then
           echo "*** Flagpoll is required to build sonix."
           echo "*** Please check that the PATH variable is set to "
           echo "*** include the proper path to flagpoll."
        fi
        SNX_CXXFLAGS=""
        SNX_LIBS=""
        SNX_PROF_LIBS=""
        SNX_LIBS_STATIC=""
        SNX_PROF_LIBS_STATIC=""
        SNX_EXTRA_LIBS=""
        SNX_VERSION="-1"

        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(SNX_CXXFLAGS)
    AC_SUBST(SNX_LIBS)
    AC_SUBST(SNX_PROF_LIBS)
    AC_SUBST(SNX_LIBS_STATIC)
    AC_SUBST(SNX_PROF_LIBS_STATIC)
    AC_SUBST(SNX_EXTRA_LIBS)
    AC_SUBST(SNX_VERSION)

])
