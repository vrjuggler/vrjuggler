# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2010 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Base configure.in for VRJ plug-ins.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([VR Juggler Plug-Ins], [2.3], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2010 by Iowa State University])
AC_CONFIG_SRCDIR([Makefile.in])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../../share/config])
DPP_PREREQ([2.2.2])
DPP_INIT

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

DPP_STD_CPP(yes)

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Allow forcing the use of GCC as the compiler.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_SYSTEM_SETUP

# Depending on the target operating system, set various command options and
# such.
case $target_os in
    # SGI running IRIX 6.*.
    irix6*)
        if test "x$USE_GCC" != "xyes" ; then
            DBG_FLAGS="$DBG_FLAGS -gslim"
        fi
        ;;
esac

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $PLUGIN_ROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
cd "$srcdir"
PLUGIN_ROOT_ABS=`pwd`

cd "$PLUGIN_ROOT_ABS/../../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   JUGGLERROOT_ABS=`cygpath -w "$UNIX_JUGGLERROOT_ABS" | sed -e 's/\\\\/\\//g'`
else
   JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"
fi

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP
VJ_PROG_CC_PROF_FLAG(yes)
VJ_PROG_CXX_PROF_FLAG(yes)

# Ensure that the C++ compiler we've found is capable of compiling the newer
# newer C++ features that we need.
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** VRJ plug-ins require C++ namespace support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** VRJ plug-ins require C++ static_cast<> ***])])
DPP_CXX_DYNAMIC_CAST([AC_MSG_ERROR([*** VRJ Juggler plug-ins require C++ dynamic_cast<> ***])])

# Check for the JDK.
DPP_PATH_JAVA

# If there is still no value in $JAVAC, then warn that the GUI will not be
# built.
if test "x$JAVAC" = "xno" ; then
    AC_MSG_WARN([*** Performance Monitor JavaBean requires javac -- it will not be built ***])
    BUILD_JAVA='N'
else
    BUILD_JAVA='Y'
fi

AC_CHECK_PROG([ANT],
    [ant],
    [JAVA_HOME=\"$JDK_HOME\" \"$UNIX_JUGGLERROOT_ABS/external/ant/bin/ant\" --noconfig])
AC_ARG_VAR([ANT], [Path of ant build tool])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_HAVE_PERL5( , , [AC_MSG_ERROR([*** Perl is required ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
DPP_PROG_LINKER

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
vrj_version='2.1.15'
jccl_version='1.1.5'
VRJUGGLER_PATH([$vrj_version], [test_tweek_cxx='Y'],
   [AC_MSG_WARN([*** VRJ C++ API required for VRJ C++ plug-ins ***])
    test_tweek_cxx='N'
   ])

VPR_PATH([1.1.23], , [AC_MSG_ERROR([*** VPR required for plugins ***])])

min_tweek_version='1.3.4'
if test "x$test_tweek_cxx" = "xY" ; then
   TWEEK_PATH_CXX([$min_tweek_version], [HAVE_TWEEK_CXX='Y'],
      [AC_MSG_WARN([*** Tweek C++ API required for performance monitoring ***])
       HAVE_TWEEK_CXX='N'
      ])

   if test "x$HAVE_TWEEK_CXX" = "xY" ; then
      JCCL_PATH_CXX([$jccl_version], [HAVE_TWEEK_CXX='Y'],
         [AC_MSG_WARN([*** JCCL C++ API required for performance monitoring ***])
          HAVE_TWEEK_CXX='N'
         ])
   fi
fi

if test "x$BUILD_JAVA" = "xY" ; then
   JCCL_PATH_JAVA([$jccl_version], [test_tweek_java='Y'],
      [AC_MSG_WARN([*** JCCL Java API required for VRJ Java plug-ins ***])
       test_tweek_java='N'
      ])

   if test "x$test_tweek_java" = "xY" ; then
      TWEEK_PATH_JAVA([$min_tweek_version], [HAVE_TWEEK_JAVA='Y'],
         [AC_MSG_WARN([*** Tweek Java required for performance monitor JavaBean ***])
          HAVE_TWEEK_JAVA='N'
         ])
   fi
fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------
DATA_SUBDIR="$VRJ_DATA_SUBDIR"
PLUGIN_SUBDIR="$VRJ_PLUGIN_SUBDIR"

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VRJ_CXXFLAGS"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$VRJ_CXXFLAGS"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" = "xno" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
#    CXXFLAGS="$CXXFLAGS -woff 3322,1388 -w2"
    CXXFLAGS="$CXXFLAGS -w2"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -J4 -all"
elif test "x$PLATFORM" = "xDarwin" ; then
    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CFLAGS_DYNLIB="$CFLAGS_DYNLIB -fexport-coalesced"
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
    fi

    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -Wl,-bind_at_load"
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
    vc_cflags='/QIfdiv /QI0f'
    vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
    CFLAGS="$CFLAGS /W3 $vc_cflags"
    CXXFLAGS="$CXXFLAGS /W3 $vc_cxxflags"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
    DBG_FLAGS="$DBG_FLAGS /MDd"
    OPT_FLAGS="$OPT_FLAGS /MD"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# $DSO_PLUGIN_DEPS must list all the libraries required to link a VRJ plug-in.
DSO_PLUGIN_DEPS="$VRJ_LIBS"
DSO_PLUGIN_PROF_DEPS="$VRJ_PROF_LIBS"
TWEEK_PLUGIN_DEPS="$TWEEK_LIBS"
TWEEK_PROF_PLUGIN_DEPS="$TWEEK_PROF_LIBS"

# Remove duplicated arguments from each of the following.  This can help speed
# up compile times, and it reduces the (perceived) complexity of the commands.
# These steps are performed here because no further additions are made to any
# of these variables after this point.
VJ_STRIP_DUPLICATE_ARGS(CXXFLAGS, [$CXXFLAGS])
VJ_STRIP_DUPLICATE_ARGS(DEPEND_FLAGS, [$DEPEND_FLAGS])
VJ_STRIP_DUPLICATE_ARGS_REVERSE([DSO_PLUGIN_DEPS], [$DSO_PLUGIN_DEPS])
VJ_STRIP_DUPLICATE_ARGS_REVERSE([DSO_PLUGIN_PROF_DEPS],
                                [$DSO_PLUGIN_PROF_DEPS])
VJ_BUILD_LIB_FILE_LIST([$DSO_PLUGIN_DEPS], [DSO_PLUGIN_DEP_FILES])
VJ_BUILD_LIB_FILE_LIST([$DSO_PLUGIN_PROF_DEPS], [DSO_PLUGIN_PROF_DEP_FILES])

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(PLUGIN_ROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)

AC_SUBST(DATA_SUBDIR)
AC_SUBST(PLUGIN_SUBDIR)

AC_SUBST(HAVE_TWEEK_CXX)
AC_SUBST(HAVE_TWEEK_JAVA)

AC_SUBST(ANT)
AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(BUILD_JAVA)

AC_SUBST(DSO_PLUGIN_DEPS)
AC_SUBST(DSO_PLUGIN_DEP_FILES)
AC_SUBST(DSO_PLUGIN_PROF_DEPS)
AC_SUBST(DSO_PLUGIN_PROF_DEP_FILES)
AC_SUBST(TWEEK_PLUGIN_DEPS)
AC_SUBST(TWEEK_PLUGIN_PROF_DEPS)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------
AC_CONFIG_FILES([
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   plugin.defs.mk
   corba_perf_mon/build.xml
   corba_perf_mon/Makefile
   ])

AC_OUTPUT
