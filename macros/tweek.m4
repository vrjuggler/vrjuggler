dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for Tweek and then define the following variables:
dnl     TWEEK_CXXFLAGS
dnl     TWEEK_LIBS_LD
dnl     TWEEK_LIBS_STATIC_LD
dnl     TWEEK_LIBS_CC
dnl     TWEEK_LIBS_STATIC_CC
dnl     TWEEK_CXX_IDL_OPTS
dnl     TWEEK_CXX_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL_OPTS
dnl     TWEEK_JAVA_IDL_GENDIR_OPT
dnl     TWEEK_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(TWEEK_PATH,
[
    dnl Get the cflags and libraries from the tweek-config script
    AC_ARG_WITH(tweek-prefix,
                [  --with-tweek-prefix=<PATH>
                          Prefix where Tweek is installed
                          (optional)                      [No default]],
                tweek_config_prefix="$withval", tweek_config_prefix="")
    AC_ARG_WITH(tweek-exec-prefix,
                [  --with-tweek-exec-prefix=<PATH>
                          Exec prefix where Tweek is
                          installed (optional)            [No default]],
                tweek_config_exec_prefix="$withval",
                tweek_config_exec_prefix="")
    AC_ARG_ENABLE(tweektest,
                  [  --disable-tweektest     Do not try to compile and run a
                          test Tweek program], , enable_tweektest=yes)

    if test "x$tweek_config_exec_prefix" != "x" ; then
        tweek_config_args="$tweek_config_args --exec-prefix=$tweek_config_exec_prefix"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$tweek_config_exec_prefix/bin/tweek-config"
        fi
    fi

    if test "x$tweek_config_prefix" != "x" ; then
        tweek_config_args="$tweek_config_args --prefix=$tweek_config_prefix"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$tweek_config_prefix/bin/tweek-config"
        fi
    fi

    if test "x$TWEEK_BASE_DIR" != "x" ; then
        tweek_config_args="$tweek_config_args --prefix=$TWEEK_BASE_DIR"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$TWEEK_BASE_DIR/bin/tweek-config"
        fi
    fi

    AC_PATH_PROG(TWEEK_CONFIG, tweek-config, no)
    min_tweek_version=ifelse([$1], , 0.0.1, $1)

    dnl Do a sanity check to ensure that $TWEEK_CONFIG actually works.
    if ! (eval $TWEEK_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        TWEEK_CONFIG='no'
    fi

    no_tweek=''
    if test "x$TWEEK_CONFIG" = "xno" ; then
        no_tweek=yes
    else
        TWEEK_CXXFLAGS=`$TWEEK_CONFIG $tweek_config_args --cxxflags $ABI`
        TWEEK_EXTRA_LIBS=`$TWEEK_CONFIG $tweek_config_args --extra-libs $ABI`
        TWEEK_LIBS_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker` $TWEEK_EXTRA_LIBS"
        TWEEK_LIBS_STATIC_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --static` $TWEEK_EXTRA_LIBS"
        TWEEK_LIBS_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI` $TWEEK_EXTRA_LIBS"
        TWEEK_LIBS_STATIC_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --static` $TWEEK_EXTRA_LIBS"
        TWEEK_CXX_IDL="`$TWEEK_CONFIG $tweek_config_args --idl cxx`"
        TWEEK_CXX_IDL_OPTS="`$TWEEK_CONFIG $tweek_config_args --idlflags cxx`"
        TWEEK_CXX_IDL_GENDIR_OPT="`$TWEEK_CONFIG $tweek_config_args --idlgendir cxx`"
        TWEEK_JAVA_IDL="`$TWEEK_CONFIG $tweek_config_args --idl java`"
        TWEEK_JAVA_IDL_OPTS="`$TWEEK_CONFIG $tweek_config_args --idlflags java`"
        TWEEK_JAVA_IDL_GENDIR_OPT="`$TWEEK_CONFIG $tweek_config_args --idlgendir java`"
        TWEEK_JARS="`$TWEEK_CONFIG $tweek_config_args --jars`"

        TWEEK_VERSION=`$TWEEK_CONFIG --version`
        DPP_VERSION_CHECK_MSG(Tweek, $TWEEK_VERSION, $min_tweek_version,
                              tweek_cv_tweek_version, $2, $3)
    fi

    if test "x$no_tweek" != x ; then
        if test "$TWEEK_CONFIG" = "no" ; then
            echo "*** The tweek-config script installed by Tweek could not be found"
            echo "*** If Tweek was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the TWEEK_CONFIG environment variable to the"
            echo "*** full path to tweek-config."
        fi
        TWEEK_CXXFLAGS=""
        TWEEK_LIBS_LD=""
        TWEEK_LIBS_STATIC_LD=""
        TWEEK_LIBS_CC=""
        TWEEK_LIBS_STATIC_CC=""
        TWEEK_CXX_IDL=""
        TWEEK_CXX_IDL_OPTS=""
        TWEEK_CXX_IDL_GENDIR_OPT=""
        TWEEK_JAVA_IDL=""
        TWEEK_JAVA_IDL_OPTS=""
        TWEEK_JAVA_IDL_GENDIR_OPT=""
        TWEEK_JARS=""
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(TWEEK_CXXFLAGS)
    AC_SUBST(TWEEK_LIBS_LD)
    AC_SUBST(TWEEK_LIBS_STATIC_LD)
    AC_SUBST(TWEEK_LIBS_CC)
    AC_SUBST(TWEEK_LIBS_STATIC_CC)
    AC_SUBST(TWEEK_CXX_IDL)
    AC_SUBST(TWEEK_JAVA_IDL)
    AC_SUBST(TWEEK_CXX_IDL_OPTS)
    AC_SUBST(TWEEK_CXX_IDL_GENDIR_OPT)
    AC_SUBST(TWEEK_JAVA_IDL_OPTS)
    AC_SUBST(TWEEK_JAVA_IDL_GENDIR_OPT)
    AC_SUBST(TWEEK_JARS)
])
