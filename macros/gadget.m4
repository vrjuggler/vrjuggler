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
dnl     GADGET_CXXFLAGS_MIN
dnl     GADGET_INCLUDES
dnl     GADGET_INCLUDES_MIN
dnl     GADGET_LIBS_CC
dnl     GADGET_LIBS_LD
dnl     GADGET_PROF_LIBS_CC
dnl     GADGET_PROF_LIBS_LD
dnl     GADGET_LIBS_STATIC_CC
dnl     GADGET_LIBS_STATIC_LD
dnl     GADGET_PROF_LIBS_STATIC_CC
dnl     GADGET_PROF_LIBS_STATIC_LD
dnl     GADGET_EXTRA_LIBS_CC
dnl     GADGET_EXTRA_LIBS_LD
dnl     GADGET_USE_X11
dnl ---------------------------------------------------------------------------
AC_DEFUN([GADGETEER_PATH],
[
    DPP_PREREQ([2.0.1])

    dnl Get the cflags and libraries from the gadgeteer-config script
    AC_ARG_WITH(gadgeteer,
                [  --with-gadgeteer=<PATH> Directory where Gadgeteer is
                          installed                       [No default]],
                gadget_config_prefix="$withval", gadget_config_prefix="")
    AC_ARG_WITH(gadgeteer-exec-prefix,
                [  --with-gadgeteer-exec-prefix=<PATH>
                          Exec prefix where Gadgeteer is
                          installed (optional)            [No default]],
                gadget_config_exec_prefix="$withval",
                gadget_config_exec_prefix="")

    if test "x$gadget_config_exec_prefix" != "x" ; then
        gadget_config_args="$gadget_config_args --exec-prefix=$gadget_config_exec_prefix"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$gadget_config_exec_prefix/bin/gadgeteer-config"
        fi
    fi

    if test "x$gadget_config_prefix" != "x" ; then
        gadget_config_args="$gadget_config_args --prefix=$gadget_config_prefix"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$gadget_config_prefix/bin/gadgeteer-config"
        fi
    fi

    if test "x$GADGET_BASE_DIR" != "x" ; then
        gadget_config_args="$gadget_config_args --prefix=$GADGET_BASE_DIR"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$GADGET_BASE_DIR/bin/gadgeteer-config"
        fi
    fi

    AC_PATH_PROG(GADGETEER_CONFIG, gadgeteer-config, no)
    min_gadget_version=ifelse([$1], , 0.0.1, $1)

    dnl Do a sanity check to ensure that $GADGETEER_CONFIG actually works.
    if ! (eval $GADGETEER_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        GADGETEER_CONFIG='no'
    fi

    no_gadgeteer=''
    if test "x$GADGETEER_CONFIG" = "xno" ; then
        no_gadgeteer=yes
    else
        GADGET_CXXFLAGS=`$GADGETEER_CONFIG $gadget_config_args --cxxflags $ABI`
        GADGET_INCLUDES=`$GADGETEER_CONFIG $gadget_config_args --includes`
        GADGET_LIBS_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI`"
        GADGET_LIBS_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI`"
        GADGET_PROF_LIBS_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --profiled`"
        GADGET_PROF_LIBS_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --profiled`"
        GADGET_LIBS_STATIC_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static`"
        GADGET_LIBS_STATIC_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static`"
        GADGET_PROF_LIBS_STATIC_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --profiled`"
        GADGET_PROF_LIBS_STATIC_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --profiled`"
        GADGET_EXTRA_LIBS_CC=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI`
        GADGET_EXTRA_LIBS_LD=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --linker`
        GADGET_VERSION=`$GADGETEER_CONFIG --version`

        GADGET_CXXFLAGS_MIN=`$GADGETEER_CONFIG $gadget_config_args --cxxflags $ABI --min`
        GADGET_INCLUDES_MIN=`$GADGETEER_CONFIG $gadget_config_args --includes --min`
        GADGET_LIBS_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --min`"
        GADGET_LIBS_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --min`"
        GADGET_PROF_LIBS_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --min --profiled`"
        GADGET_PROF_LIBS_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --min --profiled`"
        GADGET_LIBS_STATIC_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --min`"
        GADGET_LIBS_STATIC_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --min`"
        GADGET_PROF_LIBS_STATIC_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --profiled --min`"
        GADGET_PROF_LIBS_STATIC_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --profiled --min`"
        GADGET_EXTRA_LIBS_CC_MIN=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --min`
        GADGET_EXTRA_LIBS_LD_MIN=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --min --linker`
        GADGET_USE_X11=`$GADGETEER_CONFIG $gadget_config_args --use-x11`

        DPP_VERSION_CHECK_MSG_NO_CACHE([Gadgeteer], [$GADGET_VERSION],
                                       [$min_gadget_version], [$2], [$3])
    fi

    if test "x$no_gadgeteer" != x ; then
        if test "$GADGETEER_CONFIG" = "no" ; then
            echo "*** The gadgeteer-config script installed by Gadgeteer could not be found"
            echo "*** If Gadgeteer was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the GADGETEER_CONFIG environment variable to the"
            echo "*** full path to gadgeteer-config."
        fi
        GADGET_CXXFLAGS=""
        GADGET_INCLUDES=""
        GADGET_LIBS_CC=""
        GADGET_LIBS_LD=""
        GADGET_PROF_LIBS_CC=""
        GADGET_PROF_LIBS_LD=""
        GADGET_LIBS_STATIC_CC=""
        GADGET_LIBS_STATIC_LD=""
        GADGET_PROF_LIBS_STATIC_CC=""
        GADGET_PROF_LIBS_STATIC_LD=""
        GADGET_EXTRA_LIBS_CC=""
        GADGET_EXTRA_LIBS_LD=""
        GADGET_VERSION="-1"

        GADGET_CXXFLAGS_MIN=""
        GADGET_INCLUDES_MIN=""
        GADGET_LIBS_CC_MIN=""
        GADGET_LIBS_LD_MIN=""
        GADGET_PROF_LIBS_CC_MIN=""
        GADGET_PROF_LIBS_LD_MIN=""
        GADGET_LIBS_STATIC_CC_MIN=""
        GADGET_LIBS_STATIC_LD_MIN=""
        GADGET_PROF_LIBS_STATIC_CC_MIN=""
        GADGET_PROF_LIBS_STATIC_LD_MIN=""
        GADGET_EXTRA_LIBS_CC_MIN=""
        GADGET_EXTRA_LIBS_LD_MIN=""
        GADGET_USE_X11=''

        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(GADGET_CXXFLAGS)
    AC_SUBST(GADGET_INCLUDES)
    AC_SUBST(GADGET_LIBS_CC)
    AC_SUBST(GADGET_LIBS_LD)
    AC_SUBST(GADGET_PROF_LIBS_CC)
    AC_SUBST(GADGET_PROF_LIBS_LD)
    AC_SUBST(GADGET_LIBS_STATIC_CC)
    AC_SUBST(GADGET_LIBS_STATIC_LD)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_CC)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_LD)
    AC_SUBST(GADGET_EXTRA_LIBS_CC)
    AC_SUBST(GADGET_EXTRA_LIBS_LD)
    AC_SUBST(GADGET_VERSION)

    AC_SUBST(GADGET_CXXFLAGS_MIN)
    AC_SUBST(GADGET_INCLUDES_MIN)
    AC_SUBST(GADGET_LIBS_CC_MIN)
    AC_SUBST(GADGET_LIBS_LD_MIN)
    AC_SUBST(GADGET_PROF_LIBS_CC_MIN)
    AC_SUBST(GADGET_PROF_LIBS_LD_MIN)
    AC_SUBST(GADGET_LIBS_STATIC_CC_MIN)
    AC_SUBST(GADGET_LIBS_STATIC_LD_MIN)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_CC_MIN)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_LD_MIN)
    AC_SUBST(GADGET_EXTRA_LIBS_CC_MIN)
    AC_SUBST(GADGET_EXTRA_LIBS_LD_MIN)

    AC_SUBST(GADGET_USE_X11)
])
