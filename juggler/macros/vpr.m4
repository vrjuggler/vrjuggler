dnl **************** <VPR heading BEGIN do not edit this line> ****************
dnl
dnl VR Juggler Portable Runtime
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ***************** <VPR heading END do not edit this line> *****************

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
dnl VPR_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for VPR and then define VPR_CXXFLAGS, VPR_LIBS, and VPR_LIBS_STATIC.
dnl ---------------------------------------------------------------------------
AC_DEFUN(VPR_PATH,
[
    dnl Get the cflags and libraries from the vpr-config script
    AC_ARG_WITH(vpr-prefix,
                [  --with-vpr-prefix=<PATH> Prefix where VPR is installed
                          (optional)                      [No default]],
                vpr_config_prefix="$withval", vpr_config_prefix="")
    AC_ARG_WITH(vpr-exec-prefix,
                [  --with-vpr-exec-prefix=<PATH>
                          Exec prefix where VPR is
                          installed (optional)            [No default]],
                vpr_config_exec_prefix="$withval", vpr_config_exec_prefix="")
    AC_ARG_ENABLE(vprtest,
                  [  --disable-vprtest       Do not try to compile and run a
                          test VPR program], , enable_vprtest=yes)

    if test "x$vpr_config_exec_prefix" != "x" ; then
        vpr_config_args="$vpr_config_args --exec-prefix=$vpr_config_exec_prefix"

        if test x${VPR_CONFIG+set} != xset ; then
            VPR_CONFIG="$vpr_config_exec_prefix/bin/vpr-config"
        fi
    fi

    if test "x$vpr_config_prefix" != "x" ; then
        vpr_config_args="$vpr_config_args --prefix=$vpr_config_prefix"

        if test x${VPR_CONFIG+set} != xset ; then
            VPR_CONFIG="$vpr_config_prefix/bin/vpr-config"
        fi
    fi

    if test "x$VPR_BASE_DIR" != "x" ; then
        vpr_config_args="$vpr_config_args --prefix=$VPR_BASE_DIR"

        if test x${VPR_CONFIG+set} != xset ; then
            VPR_CONFIG="$VPR_BASE_DIR/bin/vpr-config"
        fi
    fi

    AC_PATH_PROG(VPR_CONFIG, vpr-config, no)
    min_vpr_version=ifelse([$1], ,0.0.1,$1)

    dnl Do a sanity check to ensure that $VPR_CONFIG actually works.
    if ! eval `$VPR_CONFIG --cxxflags >/dev/null 2>&1` ; then
        VPR_CONFIG='no'
    fi

    no_vpr=''
    if test "x$VPR_CONFIG" = "xno" ; then
        no_vpr=yes
    else
        VPR_CXXFLAGS=`$VPR_CONFIG $vpr_config_args --cxxflags $ABI`
        VPR_EXTRA_LIBS=`$VPR_CONFIG $vpr_config_args --extra-libs $ABI`
        VPR_LIBS="`$VPR_CONFIG $vpr_config_args --libs $ABI` $VPR_EXTRA_LIBS"
        VPR_LIBS_STATIC="`$VPR_CONFIG $vpr_config_args --libs $ABI --static` $VPR_EXTRA_LIBS"
        VPR_VERSION=`$VPR_CONFIG --version`
        DPP_VERSION_CHECK_MSG(VPR, $VPR_VERSION, $min_vpr_version,
                              vpr_cv_vpr_version, $2, $3)
    fi

    if test "x$no_vpr" != x ; then
        if test "$VPR_CONFIG" = "no" ; then
            echo "*** The vpr-config script installed by VPR could not be found"
            echo "*** If VPR was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the VPR_CONFIG environment variable to the"
            echo "*** full path to vpr-config."
        fi
        VPR_CXXFLAGS=""
        VPR_LIBS=""
        VPR_LIBS_STATIC=""
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(VPR_CXXFLAGS)
    AC_SUBST(VPR_LIBS)
    AC_SUBST(VPR_LIBS_STATIC)
])
