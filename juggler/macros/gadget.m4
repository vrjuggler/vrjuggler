dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
dnl
dnl VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ************* <auto-copyright.pl END do not edit this line> *************

dnl ---------------------------------------------------------------------------
dnl GADGETEER_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for Gadgeteer and then define the following variables:
dnl     GADGET_CXXFLAGS
dnl     GADGET_CXXFLAGS_ALL
dnl     GADGET_LIBS_LD
dnl     GADGET_LIBS_LD_ALL
dnl     GADGET_LIBS_STATIC_LD
dnl     GADGET_LIBS_STATIC_LD_ALL
dnl     GADGET_LIBS_CC
dnl     GADGET_LIBS_CC_ALL
dnl     GADGET_LIBS_STATIC_CC
dnl     GADGET_LIBS_STATIC_CC_ALL
dnl     GADGET_CXX_IDL_OPTS
dnl     GADGET_CXX_IDL_GENDIR_OPT
dnl     GADGET_JAVA_IDL_OPTS
dnl     GADGET_JAVA_IDL_GENDIR_OPT
dnl     GADGET_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(GADGETEER_PATH,
[
    dnl Get the cflags and libraries from the gadgeteer-config script
    AC_ARG_WITH(gadget-prefix,
                [  --with-gadget-prefix=<PATH>
                          Prefix where Gadgeteer is
                          installed (optional)            [No default]],
                gadget_config_prefix="$withval", gadget_config_prefix="")
    AC_ARG_WITH(gadget-exec-prefix,
                [  --with-gadget-exec-prefix=<PATH>
                          Exec prefix where Gadgeteer is
                          installed (optional)            [No default]],
                gadget_config_exec_prefix="$withval",
                gadget_config_exec_prefix="")
dnl    AC_ARG_ENABLE(gadgettest,
dnl                  [  --disable-gadgettest    Do not try to compile and run a
dnl                          test Gadgeteer program], , enable_gadgettest=yes)

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
        GADGET_CXXFLAGS_ALL=`$GADGETEER_CONFIG $gadget_config_args --cxxflags $ABI --all`
        GADGET_LIBS_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI`"
        GADGET_LIBS_STATIC_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static`"
        GADGET_LIBS_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI`"
        GADGET_LIBS_STATIC_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static`"
        GADGET_EXTRA_LIBS=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI`
        GADGET_EXTRA_LIBS_ALL=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --all`
        GADGET_VERSION=`$GADGETEER_CONFIG --version`
        DPP_VERSION_CHECK_MSG(Gadgeteer, $GADGET_VERSION, $min_gadget_version,
                              gadget_cv_gadget_version, $2, $3)
    fi

    if test "x$no_gadgeteer" != x ; then
        if test "$GADGETEER_CONFIG" = "no" ; then
            echo "*** The gadgeteer-config script installed by Gadgeteer could not be found"
            echo "*** If Gadgeteer was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the GADGETEER_CONFIG environment variable to the"
            echo "*** full path to gadgeteer-config."
        fi
        GADGET_CXXFLAGS=""
        GADGET_CXXFLAGS_ALL=""
        GADGET_LIBS_LD=""
        GADGET_LIBS_STATIC_LD=""
        GADGET_LIBS_CC=""
        GADGET_LIBS_STATIC_CC=""
        GADGET_EXTRA_LIBS=""
        GADGET_EXTRA_LIBS_ALL=""
        GADGET_VERSION="-1"
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(GADGET_CXXFLAGS)
    AC_SUBST(GADGET_CXXFLAGS_ALL)
    AC_SUBST(GADGET_LIBS_LD)
    AC_SUBST(GADGET_LIBS_STATIC_LD)
    AC_SUBST(GADGET_LIBS_CC)
    AC_SUBST(GADGET_LIBS_STATIC_CC)
    AC_SUBST(GADGET_EXTRA_LIBS)
    AC_SUBST(GADGET_EXTRA_LIBS_ALL)
    AC_SUBST(GADGET_VERSION)
])
