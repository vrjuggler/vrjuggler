dnl **************** <JCCL heading BEGIN do not edit this line> ****************
dnl
dnl Juggler Application Configuration & Control Library
dnl
dnl Original Authors:
dnl   Christopher Just, Carolina Cruz-Neira
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ***************** <JCCL heading END do not edit this line> *****************

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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl JCCL_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for JCCL and then define JCCL_CXXFLAGS, JCCL_LIBS, and JCCL_LIBS_STATIC.
dnl ---------------------------------------------------------------------------
AC_DEFUN(JCCL_PATH,
[
    dnl Get the cflags and libraries from the jccl-config script
    AC_ARG_WITH(jccl-prefix,
                [  --with-jccl-prefix=<PATH> Prefix where JCCL is installed
                          (optional)                      [No default]],
                jccl_config_prefix="$withval", jccl_config_prefix="")
    AC_ARG_WITH(jccl-exec-prefix,
                [  --with-jccl-exec-prefix=<PATH>
                          Exec prefix where JCCL is
                          installed (optional)            [No default]],
                jccl_config_exec_prefix="$withval", jccl_config_exec_prefix="")
    AC_ARG_ENABLE(jccltest,
                  [  --disable-jccltest       Do not try to compile and run a
                          test JCCL program], , enable_jccltest=yes)

    if test "x$jccl_config_exec_prefix" != "x" ; then
        jccl_config_args="$jccl_config_args --exec-prefix=$jccl_config_exec_prefix"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$jccl_config_exec_prefix/bin/jccl-config"
        fi
    fi

    if test "x$jccl_config_prefix" != "x" ; then
        jccl_config_args="$jccl_config_args --prefix=$jccl_config_prefix"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$jccl_config_prefix/bin/jccl-config"
        fi
    fi

    if test "x$JCCL_BASE_DIR" != "x" ; then
        jccl_config_args="$jccl_config_args --prefix=$JCCL_BASE_DIR"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$JCCL_BASE_DIR/bin/jccl-config"
        fi
    fi

    AC_PATH_PROG(JCCL_CONFIG, jccl-config, no)
    min_jccl_version=ifelse([$1], ,0.0.1,$1)

    dnl Do a sanity check to ensure that $JCCL_CONFIG actually works.
    if ! eval `$JCCL_CONFIG --cxxflags >/dev/null 2>&1` ; then
        JCCL_CONFIG='no'
    fi

    no_jccl=''
    if test "x$JCCL_CONFIG" = "xno" ; then
        no_jccl=yes
    else
        JCCL_CXXFLAGS=`$JCCL_CONFIG $jccl_config_args --cxxflags $ABI`
        JCCL_EXTRA_LIBS=`$JCCL_CONFIG $jccl_config_args --extra-libs $ABI`
        JCCL_LIBS_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker` $JCCL_EXTRA_LIBS"
        JCCL_LIBS_STATIC_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --static` $JCCL_EXTRA_LIBS"
        JCCL_LIBS_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI` $JCCL_EXTRA_LIBS"
        JCCL_LIBS_STATIC_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI --static` $JCCL_EXTRA_LIBS"
        JCCL_VERSION=`$JCCL_CONFIG --version`
        DPP_VERSION_CHECK_MSG(JCCL, $JCCL_VERSION, $min_jccl_version,
                              jccl_cv_jccl_version, $2, $3)
    fi

    if test "x$no_jccl" != x ; then
        if test "$JCCL_CONFIG" = "no" ; then
            echo "*** The jccl-config script installed by JCCL could not be found"
            echo "*** If JCCL was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the JCCL_CONFIG environment variable to the"
            echo "*** full path to jccl-config."
        fi
        JCCL_CXXFLAGS=""
        JCCL_LIBS=""
        JCCL_LIBS_STATIC=""
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(JCCL_CXXFLAGS)
    AC_SUBST(JCCL_LIBS_LD)
    AC_SUBST(JCCL_LIBS_STATIC_LD)
    AC_SUBST(JCCL_LIBS_CC)
    AC_SUBST(JCCL_LIBS_STATIC_CC)
])
