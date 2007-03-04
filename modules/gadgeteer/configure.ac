# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Base configure.in for Gadgeteer.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([Gadgeteer], [1.1], [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([gadget/gadgetDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2007 by Iowa State University])
AC_CONFIG_SRCDIR([gadget/gadgetConfig.h])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.1.2])
DPP_INIT

# Generate the reconfig script first so that it's easy to run configure again
# if it fails later on.
DPP_GEN_RECONFIG

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

AC_ARG_ENABLE([xwindows],
              [  --enable-xwindows       Use X11 for windowing code ],
              [XWIN_UI="$enableval"], [XWIN_UI='yes'])

DPP_STD_CPP(yes)

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

# Allow forcing the use of GCC as the compiler.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP
DPP_SYSTEM_SETUP

# Templates for Autoheader.
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
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
   # HP PA-RISC machine running HP-UX 11.x.
   hpux11*)
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
esac

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $GADGETROOT_ABS, we cd there and save the value of running pwd.  Then return
# to the directory where configure is being run ($topdir).
cd "$srcdir"
GADGETROOT_ABS=`pwd`

cd "$GADGETROOT_ABS/../.."
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

# If we are not using X11 on Mac OS X, then we need to make sure that we can
# build Objective-C and Objective-C++ code.
if test "x$PLATFORM" = "xDarwin" -a "x$XWIN_UI" != "xyes" ; then
   DPP_PROG_OBJC
   DPP_PROG_OBJCXX
fi

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
DPP_PERL_VER(5.004, , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG(MTREE_CMD, mtree, mtree, [\$(PERL) \$(scriptdir)/mtree.pl])
DPP_HAVE_GNU_MAKE(3.78, ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS($PLATFORM, $OS_TYPE)
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
MIN_VPR_VERSION='1.1.42'
VPR_PATH([$MIN_VPR_VERSION], ,
         [AC_MSG_ERROR([*** VPR required for Gadgeteer ***])])
#VPR_SUBSYSTEM=`$VPR_CONFIG --subsystem`
MIN_JCCL_VERSION='1.1.5'
JCCL_PATH_CXX([$MIN_JCCL_VERSION], ,
              [AC_MSG_ERROR([*** JCCL C++ API required for Gadgeteer ***])])

MIN_GMTL_VERSION='0.4.11'
GMTL_PATH([$MIN_GMTL_VERSION], ,
          [AC_MSG_ERROR([*** GMTL required for Gadgeteer ***])])

AC_PATH_XTRA

gadget_use_x11='no'
gadget_use_cocoa='no'

if test "x$have_x" = "xyes" ; then
   AH_TEMPLATE([GADGET_USE_X11], [Define if the X Window System will be used])

   if test "x$PLATFORM" = "xDarwin" ; then
      if test "x$XWIN_UI" = "xyes" ; then
         gadget_use_x11='yes'
         AC_DEFINE([GADGET_USE_X11],)
      else
         gadget_use_cocoa='yes'
         AC_DEFINE([GADGET_USE_COCOA], , [Define if Cocoa will be used])
      fi
   elif test "x$OS_TYPE" = "xUNIX" ; then
      gadget_use_x11='yes'
      AC_DEFINE([GADGET_USE_X11],)
   fi
# If --enable-xwindows was passed on the command line but X11 was not found,
# then we have a problem.
elif test "x$XWIN_UI" = "xyes" ; then
   AC_MSG_ERROR([*** Use of the X Window System requested, but it was not found ***])
fi

# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/time.h unistd.h])

# -----------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# -----------------------------------------------------------------------------
AC_TYPE_SIZE_T

# -----------------------------------------------------------------------------
# Checks for library functions.
# -----------------------------------------------------------------------------
AC_CHECK_FUNCS(strdup strerror)

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------
VJ_VERSION_GROK(VERSION)

# Set the form of the version number used for versioned directory and file
# naming. Unstable builds use all three version numbers; stable builds use
# only the major and minor version numbers.
if test "x$ENABLE_VERSIONING" = "xyes" ; then
   VERSION_DOT="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
   VERSION_UNDERSCORE="${MAJOR_VERSION}_${MINOR_VERSION}_${MICRO_VERSION}"
   HEADER_VERSION="gadgeteer-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="gadgeteer-$VERSION_DOT"

   AC_DEFINE([GADGET_USE_VERSIONING], ,
             [Define if full package versioning capabilities are enabled])
   AC_DEFINE_UNQUOTED([GADGET_VERSION_DOT], ["$VERSION_DOT"],
                      [Gadgeteer version string])

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Their use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   DATA_VERSION="gadgeteer"
fi

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VPR_CXXFLAGS $JCCL_CXXFLAGS $GMTL_INCLUDES"
OBJCXXFLAGS="$CXXFLAGS -fobjc-exceptions"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$VPR_CXXFLAGS $JCCL_CXXFLAGS $GMTL_INCLUDES"
gadget_cxxflags="$STDFLAGS"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
    CXXFLAGS="$CXXFLAGS -w2"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -J4 -all"
elif test "x$PLATFORM" = "xDarwin" ; then
    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
       OBJCXXFLAGS_DYNLIB="$OBJCXXFLAGS_DYNLIB -fexport-coalesced"
    fi

    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -Wl,-bind_at_load"
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
    vc_cflags='/QIfdiv /QI0f'
    vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
    CFLAGS="$CFLAGS $vc_cflags"
    CXXFLAGS="$CXXFLAGS $vc_cxxflags"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
    DBG_FLAGS="$DBG_FLAGS /MDd"
    OPT_FLAGS="$OPT_FLAGS /MD"
    gadget_cxxflags="$vc_cxxflags $gadget_cxxflags"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Define the base path to the source directory using $(GADGETROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_GADGETROOT_ABS="$GADGETROOT_ABS"

# $APP_EXTRA_LIBS must list all the libraries need to link a Gadgeteer-based
# application.
if test "x$OS_TYPE" = "xWin32" ; then
    APP_EXTRA_LIBS="$LDFLAGS $JCCL_LIBS $LIBS comctl32.lib gdi32.lib user32.lib"
    APP_EXTRA_PROF_LIBS="$APP_EXTRA_LIBS"
else
    APP_EXTRA_LIBS="$LDFLAGS $JCCL_LIBS $LIBS $DTK_LDFLAGS"
    APP_EXTRA_PROF_LIBS="$LDFLAGS $JCCL_PROF_LIBS $LIBS $DTK_LDFLAGS"

    if test "x$gadget_use_x11" = "xyes" ; then
       APP_EXTRA_LIBS="$APP_EXTRA_LIBS $X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
       APP_EXTRA_PROF_LIBS="$APP_EXTRA_PROF_LIBS $X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
    elif test "x$PLATFORM" = "xDarwin" ; then
       APP_EXTRA_LIBS="$APP_EXTRA_LIBS -framework Cocoa"
       APP_EXTRA_PROF_LIBS="$APP_EXTRA_PROF_LIBS -framework Cocoa"
    fi
fi

if test "x$OS_TYPE" = "xUNIX" ; then
   if test "x$gadget_use_x11" = "xyes" ; then
      INCLUDES="$INCLUDES $X_CFLAGS"
      gadget_cxxflags="$gadget_cxxflags $X_CFLAGS"
      APP_EXTRA_LIBS_X11="$X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
   fi
fi

gadget_lib_name="gadget$LIBRARY_VERSION"
gadget_prof_lib_name="gadget_p$LIBRARY_VERSION"

# Remove duplicated arguments from each of the following.  This can help speed
# up compile times, and it reduces the (perceived) complexity of the commands.
# These steps are performed here because no further additions are made to any
# of these variables after this point.
VJ_STRIP_DUPLICATE_ARGS(CFLAGS, [$CFLAGS])
VJ_STRIP_DUPLICATE_ARGS(CXXFLAGS, [$CXXFLAGS])
VJ_STRIP_DUPLICATE_ARGS(OBJCXXFLAGS, [$OBJCXXFLAGS])
VJ_STRIP_DUPLICATE_ARGS(DEPEND_FLAGS, [$DEPEND_FLAGS])
VJ_STRIP_DUPLICATE_ARGS(DEPEND_EXTRAS, [$DEPEND_EXTRAS])
VJ_STRIP_DUPLICATE_ARGS(INCLUDES, [$INCLUDES])
VJ_STRIP_DUPLICATE_ARGS(gadget_cxxflags, [$gadget_cxxflags])

# Put together the basic information needed to compile Gadgeteer applications.
VJ_APP_COMPILER($CC, $CFLAGS, $CXX, $CXXFLAGS, $DBG_FLAGS, $OPT_FLAGS,
                GADGET_BASE_DIR, $DEFS, $INCLUDES, ${_EXTRA_FLAGS},
                $HEADER_VERSION)
VJ_APP_LINKER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG], [$LDOPTS_OPT],
              [GADGET_BASE_DIR], [$gadget_lib_name], [$APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(GADGET_BASE_DIR)'

DYLIB_DEPS="$APP_EXTRA_LIBS"
DYLIB_PROF_DEPS="$APP_EXTRA_PROF_LIBS"

VJ_STRIP_DUPLICATE_ARGS([CXXFLAGS], [$CXXFLAGS])
VJ_STRIP_DUPLICATE_ARGS([DEPEND_FLAGS], [$DEPEND_FLAGS])
#VJ_STRIP_DUPLICATE_ARGS_REVERSE([DYLIB_DEPS], [$DYLIB_DEPS])
#VJ_STRIP_DUPLICATE_ARGS_REVERSE([DYLIB_PROF_DEPS], [$DYLIB_PROF_DEPS])

VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS], [DYLIB_DEP_FILES])
VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS], [DYLIB_PROF_DEP_FILES])

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    GADGETROOT_ABS=`dospath -p "$GADGETROOT_ABS"`
    JUGGLERROOT_ABS=`dospath -p "$JUGGLERROOT_ABS"`

    DEPEND_EXTRAS=`dospath "$DEPEND_EXTRAS"`
    INCLUDES=`dospath "$INCLUDES"`
    LIBS=`dospath "$LIBS"`
    LN_S='cp -r'
    RM_LN='rm -rf'
    MTREE_CMD=`dospath "$MTREE_CMD"`

#    APP_EXTRA_LIBS=`dospath "$APP_EXTRA_LIBS"`
else
    RM_LN='rm -f'
    GADGETROOT_ABS="$GADGETROOT_ABS"
    JUGGLERROOT_ABS="$JUGGLERROOT_ABS"
fi

# Set up some sed expressions that will help separate the share directory from
# the full installation prefix.
if test "x$prefix" = "xNONE" ; then
   _prefix_exp="s|^$ac_default_prefix/||"
else
   _prefix_exp="s|^$prefix/||"
fi

# In Autoconf 2.60, datadir has been renamed to datarootdir.
if test "x$datarootdir" != "x" ; then
   data_var="$datarootdir"
else
   data_var="$datadir"
fi

# Make a best-effort attempt to extract the actual share directory from the
# full installation prefix.  This is not foolproof, but it will work in almost
# all cases.  A person would have to be doing something malicious or foolish
# to make it break.
data_subdir=`echo "$data_var" | sed -e 's|^${prefix}/||' -e "${_prefix_exp}"`
GADGET_SHARE_DIR="$data_subdir/$DATA_VERSION"

# Information needed to generate gadgeteer.fpc.
if test "x$GNU_LD" = "xyes" ; then
   static_begin="-Wl,-Bstatic"
   static_end="-Wl,-Bdynamic"
else
   static_begin="-B static"
   static_end="-B dynamic"
fi

gadget_ldflags="-L\$prefix/lib\$LIBBITSUF"
gadget_libs="-l$gadget_lib_name"
gadget_prof_libs="-l$gadget_prof_lib_name"
gadget_extra_libs="$LDFLAGS $LIBS $DTK_LDFLAGS $APP_EXTRA_LIBS_X11"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
    gadget32_flags='-n32'
    gadget64_flags='-64'
elif test "x$GCC" = "xyes" ; then
    gadget32_flags='-m32'
    gadget64_flags='-m64'
fi

gadget_extra_ldflags="$APP_EXTRA_LIBS_BEGIN"

includedir="$includedir/$HEADER_VERSION"

DPP_SUBST

AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_GADGETROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(GADGETROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(GADGET_SHARE_DIR)

AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(DATA_VERSION)

AC_SUBST(MIN_VPR_VERSION)
AC_SUBST(MIN_JCCL_VERSION)
AC_SUBST(MIN_GMTL_VERSION)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS)
AC_SUBST(DYLIB_DEP_FILES)
AC_SUBST(DYLIB_PROF_DEPS)
AC_SUBST(DYLIB_PROF_DEP_FILES)

AC_SUBST(APP_EXTRA_LIBS_X11)

AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(gadget_cxxflags)
AC_SUBST(gadget_extra_cxxflags)
AC_SUBST(gadget_ldflags)
AC_SUBST(gadget_libs)
AC_SUBST(gadget_prof_libs)
AC_SUBST(gadget_extra_ldflags)
AC_SUBST(gadget_extra_libs)
AC_SUBST(gadget32_flags)
AC_SUBST(gadget64_flags)
AC_SUBST(gadget_use_x11)
AC_SUBST(gadget_use_cocoa)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(GADGET, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   gadgeteer.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   cluster/Makefile
   cluster/Packets/Makefile
   gadget/Makefile
   gadget/Devices/Makefile
   gadget/Devices/KeyboardMouseDevice/Makefile
   gadget/Devices/Sim/Makefile
   gadget/Filter/Makefile
   gadget/Filter/Position/Makefile
   gadget/Type/Makefile
   gadget/Type/KeyboardMouse/Makefile
   gadget/Type/Position/Makefile
   gadget/Util/Makefile
   test/Makefile
   mtree/GADGET.data.dist
   mtree/GADGET.include.dist
   mtree/GADGET.install.dist
   mtree/GADGET.samples.dist
   mtree/GADGET.test.dist
   mtree/GADGET.tools.dist
   VARS.pl
   ]
   )

AC_OUTPUT
