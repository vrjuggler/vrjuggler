dnl *************** <Plexus heading BEGIN do not edit this line> **************
dnl
dnl Plexus
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Patrick Hartling, Kevin Meinert
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl **************** <Plexus heading END do not edit this line> ***************

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
dnl PLEXUS_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for Plexus and then define the following variables:
dnl     PLEXUS_CXXFLAGS
dnl     PLEXUS_CXXFLAGS_MIN
dnl     PLEXUS_INCLUDES
dnl     PLEXUS_INCLUDES_MIN
dnl     PLEXUS_LIBS_LD
dnl     PLEXUS_LIBS_LD_MIN
dnl     PLEXUS_LIBS_STATIC_LD
dnl     PLEXUS_LIBS_STATIC_LD_MIN
dnl     PLEXUS_LIBS_CC
dnl     PLEXUS_LIBS_CC_MIN
dnl     PLEXUS_LIBS_STATIC_CC
dnl     PLEXUS_LIBS_STATIC_CC_MIN
dnl ---------------------------------------------------------------------------
AC_DEFUN(PLEXUS_PATH,
[
    dnl Get the cflags and libraries from the plexus-config script
    AC_ARG_WITH(plexus-prefix,
                [  --with-plexus-prefix=<PATH>
                          Prefix where Plexus is installed
                          (optional)                      [No default]],
                plexus_config_prefix="$withval", plexus_config_prefix="")
    AC_ARG_WITH(plexus-exec-prefix,
                [  --with-plexus-exec-prefix=<PATH>
                          Exec prefix where Plexus is
                          installed (optional)            [No default]],
                plexus_config_exec_prefix="$withval",
                plexus_config_exec_prefix="")
dnl    AC_ARG_ENABLE(plexustest,
dnl                  [  --disable-plexustest    Do not try to compile and run a
dnl                          test Plexus program], , enable_plexustest=yes)

    if test "x$plexus_config_exec_prefix" != "x" ; then
        plexus_config_args="$plexus_config_args --exec-prefix=$plexus_config_exec_prefix"

        if test x${PLEXUS_CONFIG+set} != xset ; then
            PLEXUS_CONFIG="$plexus_config_exec_prefix/bin/plexus-config"
        fi
    fi

    if test "x$plexus_config_prefix" != "x" ; then
        plexus_config_args="$plexus_config_args --prefix=$plexus_config_prefix"

        if test x${PLEXUS_CONFIG+set} != xset ; then
            PLEXUS_CONFIG="$plexus_config_prefix/bin/plexus-config"
        fi
    fi

    if test "x$PLX_BASE_DIR" != "x" ; then
        plexus_config_args="$plexus_config_args --prefix=$PLX_BASE_DIR"

        if test x${PLEXUS_CONFIG+set} != xset ; then
            PLEXUS_CONFIG="$PLX_BASE_DIR/bin/plexus-config"
        fi
    fi

    AC_PATH_PROG(PLEXUS_CONFIG, plexus-config, no)
    min_plexus_version=ifelse([$1], ,0.0.1,$1)

    dnl Do a sanity check to ensure that $PLEXUS_CONFIG actually works.
    if ! (eval $PLEXUS_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        PLEXUS_CONFIG='no'
    fi

    no_plexus=''
    if test "x$PLEXUS_CONFIG" = "xno" ; then
        no_plexus=yes
    else
        PLEXUS_CXXFLAGS=`$PLEXUS_CONFIG $plexus_config_args --cxxflags $ABI`
        PLEXUS_INCLUDES=`$PLEXUS_CONFIG $plexus_config_args --includes`
        PLEXUS_EXTRA_LIBS_CC=`$PLEXUS_CONFIG $plexus_config_args --extra-libs $ABI`
        PLEXUS_EXTRA_LIBS_LD=`$PLEXUS_CONFIG $plexus_config_args --extra-libs $ABI --linker`
        PLEXUS_LIBS_CC="`$PLEXUS_CONFIG $plexus_config_args --libs $ABI` $PLEXUS_EXTRA_LIBS_CC"
        PLEXUS_LIBS_LD="`$PLEXUS_CONFIG $plexus_config_args --linker --libs $ABI` $PLEXUS_EXTRA_LIBS_LD"
        PLEXUS_LIBS_STATIC_CC="`$PLEXUS_CONFIG $plexus_config_args --libs $ABI --static` $PLEXUS_EXTRA_LIBS_CC"
        PLEXUS_LIBS_STATIC_LD="`$PLEXUS_CONFIG $plexus_config_args --linker --libs $ABI --static` $PLEXUS_EXTRA_LIBS_LD"
        PLEXUS_VERSION=`$PLEXUS_CONFIG --version`

        PLEXUS_CXXFLAGS_MIN=`$PLEXUS_CONFIG $plexus_config_args --cxxflags $ABI --min`
        PLEXUS_INCLUDES_MIN=`$PLEXUS_CONFIG $plexus_config_args --includes --min`
        PLEXUS_LIBS_CC_MIN="`$PLEXUS_CONFIG $plexus_config_args --libs $ABI --min` $PLEXUS_EXTRA_LIBS_CC"
        PLEXUS_LIBS_LD_MIN="`$PLEXUS_CONFIG $plexus_config_args --linker --libs $ABI --min` $PLEXUS_EXTRA_LIBS_LD"
        PLEXUS_LIBS_STATIC_CC_MIN="`$PLEXUS_CONFIG $plexus_config_args --libs $ABI --static --min` $PLEXUS_EXTRA_LIBS_CC"
        PLEXUS_LIBS_STATIC_LD_MIN="`$PLEXUS_CONFIG $plexus_config_args --linker --libs $ABI --static --min` $PLEXUS_EXTRA_LIBS_LD"

        AC_MSG_CHECKING([whether Plexus version is >= $min_plexus_version])
        AC_MSG_RESULT([$PLEXUS_VERSION])
        DPP_VERSION_CHECK([$PLEXUS_VERSION], [$min_plexus_version], $2, $3)
    fi

    if test "x$no_plexus" != x ; then
        if test "$PLEXUS_CONFIG" = "no" ; then
            echo "*** The plexus-config script installed by Plexus could not be found"
            echo "*** If Plexus was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the PLEXUS_CONFIG environment variable to the"
            echo "*** full path to plexus-config."
        fi
        PLEXUS_CXXFLAGS=""
        PLEXUS_CXXFLAGS_MIN=""
        PLEXUS_INCLUDES=""
        PLEXUS_INCLUDES_MIN=""
        PLEXUS_LIBS_LD=""
        PLEXUS_LIBS_LD_MIN=""
        PLEXUS_LIBS_STATIC_LD=""
        PLEXUS_LIBS_STATIC_LD_MIN=""
        PLEXUS_LIBS_CC=""
        PLEXUS_LIBS_CC_MIN=""
        PLEXUS_LIBS_STATIC_CC=""
        PLEXUS_LIBS_STATIC_CC_MIN=""
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(PLEXUS_CXXFLAGS)
    AC_SUBST(PLEXUS_INCLUDES)
    AC_SUBST(PLEXUS_LIBS_LD)
    AC_SUBST(PLEXUS_LIBS_STATIC_LD)
    AC_SUBST(PLEXUS_LIBS_CC)
    AC_SUBST(PLEXUS_LIBS_STATIC_CC)

    AC_SUBST(PLEXUS_CXXFLAGS_MIN)
    AC_SUBST(PLEXUS_INCLUDES_MIN)
    AC_SUBST(PLEXUS_LIBS_CC_MIN)
    AC_SUBST(PLEXUS_LIBS_LD_MIN)
    AC_SUBST(PLEXUS_LIBS_STATIC_CC_MIN)
    AC_SUBST(PLEXUS_LIBS_STATIC_LD_MIN)
])
