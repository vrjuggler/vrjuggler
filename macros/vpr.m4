dnl **************** <VPR heading BEGIN do not edit this line> ****************
dnl
dnl VR Juggler Portable Runtime
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
dnl
dnl ***************** <VPR heading END do not edit this line> *****************

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
dnl VPR_PATH([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Test for VPR and then define VPR_CXXFLAGS, VPR_LIBS, and VPR_LIBS_STATIC.
dnl ---------------------------------------------------------------------------
AC_DEFUN([VPR_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from flagpoll
    AC_ARG_WITH(vpr-meta-file,
                [  --with-vpr-meta-file=<PATH>
                          Flagpoll metadata file for VPR
                          (optional)                      [No default]],
                [vpr_meta_file="$withval"], [vpr_meta_file=""])

    dnl See if the user specified where to find the VPR meta file. If not,
    dnl take a guess.
    if test "x$vpr_meta_file" != "x" ; then
        vpr_meta_dir=`dirname $vpr_meta_file`
        vpr_flagpoll_args="--extra-paths=$vpr_meta_dir vpr --from-file=$vpr_meta_file"
    elif test -f "$instlinks/share/flagpoll/vpr.fpc" ; then
        vpr_meta_dir="$instlinks/share/flagpoll"
        vpr_flagpoll_args="--extra-paths=$vpr_meta_dir vpr --from-file=$vpr_meta_dir/vpr.fpc"
    else
        vpr_flagpoll_args="vpr"
    fi

    AM_PATH_FLAGPOLL([0.9.1], ,
                     [AC_MSG_ERROR(*** Flagpoll required for VPR flags ***)])
    min_vpr_version=ifelse([$1], ,0.0.1,$1)

    dnl Do a sanity check to ensure that all is well
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
    fi

    no_vpr=''
    if test "x$FLAGPOLL" = "xno" ; then
        no_vpr=yes
    else
        if ! (eval $FLAGPOLL $vpr_flagpoll_args --modversion >/dev/null 2>&1)
        then
           AC_MSG_WARN([*** Flagpoll has no valid VPR configuration ***])
           no_vpr=yes
        else
           VPR_CXXFLAGS=`$FLAGPOLL $vpr_flagpoll_args --cflags`
           VPR_LIBS=`$FLAGPOLL $vpr_flagpoll_args --libs`
           VPR_PROF_LIBS=`$FLAGPOLL $vpr_flagpoll_args --get-profiled-libs`
           VPR_LIBS_STATIC=`$FLAGPOLL $vpr_flagpoll_args --get-static-libs`
           VPR_PROF_LIBS_STATIC=`$FLAGPOLL $vpr_flagpoll_args --get-profiled-static-libs`
           VPR_EXTRA_LIBS=`$FLAGPOLL $vpr_flagpoll_args --get-extra-libs`

           VPR_SUBSYSTEM=`$FLAGPOLL $vpr_flagpoll_args --get-subsystem`
           VPR_VERSION=`$FLAGPOLL $vpr_flagpoll_args --modversion`

           DPP_VERSION_CHECK_MSG_NO_CACHE([VPR], [$VPR_VERSION],
                                          [$min_vpr_version], [$2], [$3])
        fi
    fi

    if test "x$no_vpr" != x ; then
        if test "$FLAGPOLL" = "no" ; then
           echo "*** Flagpoll is required to build VPR."
           echo "*** Please check that the PATH variable is set to "
           echo "*** include the proper path to flagpoll."
        fi

        VPR_CXXFLAGS=''
        VPR_LIBS=''
        VPR_PROF_LIBS=''
        VPR_LIBS_STATIC=''
        VPR_PROF_LIBS_STATIC=''
        VPR_EXTRA_LIBS=''

        VPR_SUBSYSTEM=''
        VPR_VERSION='-1'
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(VPR_CXXFLAGS)
    AC_SUBST(VPR_LIBS)
    AC_SUBST(VPR_PROF_LIBS)
    AC_SUBST(VPR_LIBS_STATIC)
    AC_SUBST(VPR_PROF_LIBS_STATIC)
    AC_SUBST(VPR_EXTRA_LIBS)
    AC_SUBST(VPR_BUILD_INCLUDES)

    AC_SUBST(VPR_SUBSYSTEM)
    AC_SUBST(VPR_VERSION)
])
