# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
# Base configure.ac for Gadgeteer cluster plug-ins.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([Gadgeteer Plugins], [2.1], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2011 by Iowa State University])
AC_CONFIG_SRCDIR([Makefile.in])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../../share/config])
DPP_PREREQ([2.2.2])
DPP_INIT

DPP_STD_CPP(yes)

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is partially
# usable on Windows.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff._CMA_NOWRAPPERS_
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP

DPP_SYSTEM_SETUP

# Templates for Autoheader.
AH_TEMPLATE([_INCLUDE_TERMIO], [Define if building on HP-UX.])
AH_TEMPLATE([_CMA_NOWRAPPERS_], [Define if building on HP-UX.])

# Depending on the target operating system, set various command options and
# such.
case $target_os in
   # SGI running IRIX 6.*.
   irix6*)
      if test "x$USE_GCC" != "xyes" ; then
         DBG_FLAGS="$DBG_FLAGS -gslim"
      fi
      ;;
   # HP PA-RISC machine running HP-UX 10.20.
   hpux10.20)
      AC_DEFINE(_INCLUDE_TERMIO,)
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
   # HP PA-RISC machine running HP-UX 11.x.
   hpux11*)
      AC_DEFINE(_INCLUDE_TERMIO,)
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
esac

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $PLUGIN_ROOT_ABS, we cd there and save the value of running pwd.  Then return
# to the directory where configure is being run ($topdir).
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
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** The library requires C++ namesapce support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_INLINE([AC_MSG_ERROR([*** The library requires C++ inline support ***])])
DPP_CXX_RTTI([AC_MSG_ERROR([*** The library requires C++ RTTI support ***])])
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** The library requires C++ static_cast<> ***])])
DPP_CXX_DYNAMIC_CAST([AC_MSG_ERROR([*** The library requires C++ dynamic_cast<> ***])])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_HAVE_PERL5( , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree],
              [\$(PERL5) \$(scriptdir)/mtree.pl])
DPP_HAVE_GNU_MAKE(3.78, ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS($PLATFORM, $OS_TYPE)
DPP_PROG_INSTALL
DPP_PROG_LINKER

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

VPR_PATH([1.1.42], , [AC_MSG_ERROR([*** VPR required for Device Drivers ***])])
GADGETEER_PATH([2.1.19], ,
               [AC_MSG_ERROR(*** Gadgeteer required for Plugins ***)])
JCCL_PATH_CXX([1.1.5], ,
              [AC_MSG_ERROR([*** JCCL C++ API required for Plugins ***])])

# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
#AC_HEADER_STDC
#AC_CHECK_HEADERS([fcntl.h sys/ioctl.h sys/time.h termios.h unistd.h      \
#                  sys/z8530.h sys/stdsyms.h])

# -----------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# -----------------------------------------------------------------------------
AC_TYPE_SIZE_T

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

HEADER_SUBDIR="$GADGET_HEADER_SUBDIR"
DATA_SUBDIR="$GADGET_DATA_SUBDIR"
PLUGIN_SUBDIR="$GADGET_PLUGIN_SUBDIR"

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $GADGET_CXXFLAGS"
DEPEND_FLAGS="$GADGET_CXXFLAGS"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
    CXXFLAGS="$CXXFLAGS -w2"
elif test "x$PLATFORM" = "xDarwin" ; then
    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
    fi
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
    vc_cflags='/QIfdiv /QI0f'
    vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
    CFLAGS="$CFLAGS $vc_cflags"
    CXXFLAGS="$CXXFLAGS $vc_cxxflags"
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

# Define the base path to the source directory using $(PLUGIN_ROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_PLUGIN_ROOT_ABS="$PLUGIN_ROOT_ABS"

DSO_PLUGIN_DEPS="$LDFLAGS $GADGET_LIBS"

if test "x$OS_TYPE" = "xWin32" ; then
   DSO_PLUGIN_PROF_DEPS="$DSO_PLUGIN_DEPS"
else
   DSO_PLUGIN_PROF_DEPS="$LDFLAGS $GADGET_PROF_LIBS"

   VJ_BUILD_LIB_FILE_LIST([$DSO_PLUGIN_DEPS], [DSO_PLUGIN_DEP_FILES])
   VJ_BUILD_LIB_FILE_LIST([$DSO_PLUGIN_PROF_DEPS], [DSO_PLUGIN_PROF_DEP_FILES])
fi

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    PLUGIN_ROOT_ABS=`dospath -p "$PLUGIN_ROOT_ABS"`
    JUGGLERROOT_ABS=`dospath -p "$JUGGLERROOT_ABS"`

    DEPEND_EXTRAS=`dospath "$DEPEND_EXTRAS"`
    INCLUDES=`dospath "$INCLUDES"`
    LIBS=`dospath "$LIBS"`
    LN_S='cp -r'
    RM_LN='rm -rf'
    MTREE_CMD=`dospath "$MTREE_CMD"`

    JDK_HOME=`dospath -p "$JDK_HOME"`
else
    RM_LN='rm -f'
    PLUGIN_ROOT_ABS="$PLUGIN_ROOT_ABS"
    JUGGLERROOT_ABS="$JUGGLERROOT_ABS"
fi

includedir="$includedir/$HEADER_SUBDIR"

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_PLUGIN_ROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(PLUGIN_ROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(VPR_SUBSYSTEM)

AC_SUBST(HEADER_SUBDIR)
AC_SUBST(DATA_SUBDIR)
AC_SUBST(PLUGIN_SUBDIR)

AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(DSO_PLUGIN_DEPS)
AC_SUBST(DSO_PLUGIN_DEP_FILES)
AC_SUBST(DSO_PLUGIN_PROF_DEPS)
AC_SUBST(DSO_PLUGIN_PROF_DEP_FILES)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------
AC_CONFIG_FILES([
   ApplicationDataManager/Makefile
   common.defs.mk
   EventPlugin/Makefile
   gadget.plugin.mk
   make.defs.mk
   Makefile
   Makefile.inc
   plugin.defs.mk
   RIMPlugin/Makefile
   ApplicationBarrierManager/Makefile
   VARS.pl
   ])

AC_OUTPUT
