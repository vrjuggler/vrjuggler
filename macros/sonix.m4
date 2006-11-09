dnl **************** <SNX heading BEGIN do not edit this line> ****************
dnl
dnl sonix
dnl
dnl Original Authors:
dnl   Kevin Meinert, Carolina Cruz-Neira
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ***************** <SNX heading END do not edit this line> *****************

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
dnl     SNX_CXXFLAGS_MIN
dnl     SNX_INCLUDES
dnl     SNX_INCLUDES_MIN
dnl     SNX_LIBS_CC
dnl     SNX_LIBS_LD
dnl     SNX_PROF_LIBS_CC
dnl     SNX_PROF_LIBS_LD
dnl     SNX_LIBS_STATIC_CC
dnl     SNX_LIBS_STATIC_LD
dnl     SNX_PROF_LIBS_STATIC_CC
dnl     SNX_PROF_LIBS_STATIC_LD
dnl     SNX_EXTRA_LIBS_CC
dnl     SNX_EXTRA_LIBS_LD
dnl ---------------------------------------------------------------------------
AC_DEFUN([SNX_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from the sonix-config script
    AC_ARG_WITH(sonix,
                [  --with-sonix=<PATH>     Directory where Sonix is
                          installed                       [No default]],
                sonix_config_prefix="$withval", sonix_config_prefix="")
    AC_ARG_WITH(sonix-exec-prefix,
                [  --with-sonix-exec-prefix=<PATH>
                          Prefix where Sonix executables
                          are installed (optional)        [No default]],
                sonix_config_exec_prefix="$withval", sonix_config_exec_prefix="")

    if test "x$sonix_config_exec_prefix" != "x" ; then
        sonix_config_args="$sonix_config_args --exec-prefix=$sonix_config_exec_prefix"

        if test x${SNX_CONFIG+set} != xset ; then
            SNX_CONFIG="$sonix_config_exec_prefix/bin/sonix-config"
        fi
    fi

    if test "x$sonix_config_prefix" != "x" ; then
        sonix_config_args="$sonix_config_args --prefix=$sonix_config_prefix"

        if test x${SNX_CONFIG+set} != xset ; then
            SNX_CONFIG="$sonix_config_prefix/bin/sonix-config"
        fi
    fi

    if test "x$SNX_BASE_DIR" != "x" ; then
        sonix_config_args="$sonix_config_args --prefix=$SNX_BASE_DIR"

        if test x${SNX_CONFIG+set} != xset ; then
            SNX_CONFIG="$SNX_BASE_DIR/bin/sonix-config"
        fi
    fi

    AC_PATH_PROG(SNX_CONFIG, sonix-config, no)
    min_sonix_version=ifelse([$1], ,0.0.1,[$1])

    dnl Do a sanity check to ensure that $SNX_CONFIG actually works.
    if ! (eval $SNX_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        SNX_CONFIG='no'
    fi

    no_sonix=''
    if test "x$SNX_CONFIG" = "xno" ; then
        no_sonix=yes
    else
        SNX_CXXFLAGS=`$SNX_CONFIG $sonix_config_args --cxxflags $ABI`
        SNX_INCLUDES=`$SNX_CONFIG $sonix_config_args --includes`
        SNX_LIBS_CC="`$SNX_CONFIG $sonix_config_args --libs $ABI`"
        SNX_LIBS_LD="`$SNX_CONFIG $sonix_config_args --libs $ABI --linker`"
        SNX_PROF_LIBS_CC="`$SNX_CONFIG $sonix_config_args --libs $ABI --profiled`"
        SNX_PROF_LIBS_LD="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --profiled`"
        SNX_LIBS_STATIC_CC="`$SNX_CONFIG $sonix_config_args --libs $ABI --static`"
        SNX_LIBS_STATIC_LD="`$SNX_CONFIG $sonix_config_args --libs $ABI --linker --static`"
        SNX_PROF_LIBS_STATIC_CC="`$SNX_CONFIG $sonix_config_args --libs $ABI --static --profiled`"
        SNX_PROF_LIBS_STATIC_LD="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --static --profiled`"
        SNX_EXTRA_LIBS_CC=`$SNX_CONFIG $sonix_config_args --extra-libs $ABI`
        SNX_EXTRA_LIBS_LD=`$SNX_CONFIG $sonix_config_args --extra-libs $ABI --linker`
        SNX_VERSION=`$SNX_CONFIG --version`

        SNX_CXXFLAGS_MIN=`$SNX_CONFIG $sonix_config_args --cxxflags $ABI --min`
        SNX_INCLUDES_MIN=`$SNX_CONFIG $sonix_config_args --includes --min`
        SNX_LIBS_CC_MIN="`$SNX_CONFIG $sonix_config_args --libs $ABI --min`"
        SNX_LIBS_LD_MIN="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --min`"
        SNX_PROF_LIBS_CC_MIN="`$SNX_CONFIG $sonix_config_args --libs $ABI --min --profiled`"
        SNX_PROF_LIBS_LD_MIN="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --min --profiled`"
        SNX_LIBS_STATIC_CC_MIN="`$SNX_CONFIG $sonix_config_args --libs $ABI --static --min`"
        SNX_LIBS_STATIC_LD_MIN="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --static --min`"
        SNX_PROF_LIBS_STATIC_CC_MIN="`$SNX_CONFIG $sonix_config_args --libs $ABI --static --profiled --min`"
        SNX_PROF_LIBS_STATIC_LD_MIN="`$SNX_CONFIG $sonix_config_args --linker --libs $ABI --static --profiled --min`"
        SNX_EXTRA_LIBS_CC_MIN=`$SNX_CONFIG $sonix_config_args --extra-libs $ABI --min`
        SNX_EXTRA_LIBS_LD_MIN=`$SNX_CONFIG $sonix_config_args --extra-libs $ABI --min --linker`

        DPP_VERSION_CHECK_MSG_NO_CACHE([Sonix], [$SNX_VERSION],
                                       [$min_sonix_version], [$2], [$3])
    fi

    if test "x$no_sonix" != x ; then
        if test "$SNX_CONFIG" = "no" ; then
            echo "*** The sonix-config script installed by SNX could not be found"
            echo "*** If SNX was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the SNX_CONFIG environment variable to the"
            echo "*** full path to sonix-config."
        fi
        SNX_CXXFLAGS=""
        SNX_INCLUDES=""
        SNX_LIBS_CC=""
        SNX_LIBS_LD=""
        SNX_PROF_LIBS_CC=""
        SNX_PROF_LIBS_LD=""
        SNX_LIBS_STATIC_CC=""
        SNX_LIBS_STATIC_LD=""
        SNX_PROF_LIBS_STATIC_CC=""
        SNX_PROF_LIBS_STATIC_LD=""
        SNX_EXTRA_LIBS_CC=""
        SNX_EXTRA_LIBS_LD=""
        SNX_VERSION="-1"

        SNX_CXXFLAGS_MIN=""
        SNX_INCLUDES_MIN=""
        SNX_LIBS_CC_MIN=""
        SNX_LIBS_LD_MIN=""
        SNX_PROF_LIBS_CC_MIN=""
        SNX_PROF_LIBS_LD_MIN=""
        SNX_LIBS_STATIC_CC_MIN=""
        SNX_LIBS_STATIC_LD_MIN=""
        SNX_PROF_LIBS_STATIC_CC_MIN=""
        SNX_PROF_LIBS_STATIC_LD_MIN=""
        SNX_EXTRA_LIBS_CC_MIN=""
        SNX_EXTRA_LIBS_LD_MIN=""
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(SNX_CXXFLAGS)
    AC_SUBST(SNX_INCLUDES)
    AC_SUBST(SNX_LIBS_CC)
    AC_SUBST(SNX_LIBS_LD)
    AC_SUBST(SNX_PROF_LIBS_CC)
    AC_SUBST(SNX_PROF_LIBS_LD)
    AC_SUBST(SNX_LIBS_STATIC_CC)
    AC_SUBST(SNX_LIBS_STATIC_LD)
    AC_SUBST(SNX_PROF_LIBS_STATIC_CC)
    AC_SUBST(SNX_PROF_LIBS_STATIC_LD)
    AC_SUBST(SNX_EXTRA_LIBS_CC)
    AC_SUBST(SNX_EXTRA_LIBS_LD)
    AC_SUBST(SNX_VERSION)

    AC_SUBST(SNX_CXXFLAGS_MIN)
    AC_SUBST(SNX_INCLUDES_MIN)
    AC_SUBST(SNX_LIBS_CC_MIN)
    AC_SUBST(SNX_LIBS_LD_MIN)
    AC_SUBST(SNX_PROF_LIBS_CC_MIN)
    AC_SUBST(SNX_PROF_LIBS_LD_MIN)
    AC_SUBST(SNX_LIBS_STATIC_CC_MIN)
    AC_SUBST(SNX_LIBS_STATIC_LD_MIN)
    AC_SUBST(SNX_PROF_LIBS_STATIC_CC_MIN)
    AC_SUBST(SNX_PROF_LIBS_STATIC_LD_MIN)
    AC_SUBST(SNX_EXTRA_LIBS_CC_MIN)
    AC_SUBST(SNX_EXTRA_LIBS_LD_MIN)
])
