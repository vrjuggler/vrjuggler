# ***************** <SNX heading BEGIN do not edit this line> *****************
#
# sonix
#
# Original Authors:
#   Kevin Meinert
#
# ****************** <SNX heading END do not edit this line> ******************

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

AC_INIT([Sonix], [1.1], [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([snx/snxDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2006 by Iowa State University])
AC_CONFIG_SRCDIR([snx/snxConfig.h])
AC_REVISION([$Revision$])
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.1.2])
DPP_INIT

# make the nice reconfig script.
DPP_GEN_RECONFIG

DPP_STD_CPP(yes)
# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

# Enable versioning by default
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Override the C compiler that configure would otherwise find itself.
AC_ARG_WITH(cc, [  --with-cc=<PATH>        Alternate C compiler],
            [_alt_cc="$withval"], [_alt_cc='none'])

# Override the C++ compiler that configure would otherwise find itself.
AC_ARG_WITH(cxx, [  --with-cxx=<PATH>       Alternate C++ compiler],
            [_alt_cxx="$withval"], [_alt_cxx='none'])


# Define the binary format.
DPP_ABI_SETUP

# Allow forcing the use of GCC as the compiler.
DPP_WITH_GCC(no)

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP
DPP_SYSTEM_SETUP

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the SNX source tree.  To get a value
# for $VPRROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
cd "$srcdir"
SNXROOT_ABS=`pwd`

cd "$SNXROOT_ABS/../.."
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
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** Sonix requires C++ namespace support ***])])
DPP_CXX_TEMPLATES([AC_MSG_ERROR([*** Sonix requires C++ template support ***])])
DPP_CXX_INLINE([AC_MSG_ERROR([*** Sonix requires C++ inline support ***])])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER(5.004, , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree], [\$(PERL) \$(scriptdir)/mtree.pl])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
VPR_PATH([1.1.23], , [AC_MSG_ERROR([*** VPR required for Sonix ***])])

# find and append GMTL flags...
snx_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $STDFLAGS"
GMTL_PATH(0.1.9, [/usr/local], , ,
          [AC_MSG_ERROR([*** GMTL required for Sonix ***])])
CPPFLAGS="$snx_save_CPPFLAGS"

DPP_CHECK_DYNLOAD_LIB
DPP_HAVE_CPPUNIT([1.9.10])

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------

# Macros:
#     DPP_INSTALLER - Set up the installer configuration.
# Variables defined:
#     OWNER_NAME - The owner of everything installed.
#     GROUP_NAME - The group of everything installed.
#     FILE_PERMS - The UNIX-style octal permissions of normal files.
#     EXEC_PERMS - The UNIX-style octal permissions of executable files.
#     DIR_PERMS  - The UNIX-style octal permissions of directories.
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

   HEADER_VERSION="sonix-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="sonix-$VERSION_DOT"

   AC_DEFINE([SNX_USE_VERSIONING], ,
             [Define if full package versioning capabilities are enabled])
   AC_DEFINE_UNQUOTED([SNX_VERSION_DOT], ["$VERSION_DOT"],
                      [Sonix version string])

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Its use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   DATA_VERSION="sonix"
fi


CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VPR_CXXFLAGS $GMTL_INCLUDES"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$VPR_CXXFLAGS $GMTL_INCLUDES"
snx_cxxflags="$STDFLAGS"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#   CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
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
   CFLAGS="$CFLAGS $vc_cflags"
   CXXFLAGS="$CXXFLAGS $vc_cxxflags"
   BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
   DBG_FLAGS="$DBG_FLAGS /MDd"
   OPT_FLAGS="$OPT_FLAGS /MD"
   snx_cxxflags="$vc_cxxflags $snx_cxxflags"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" -a "x$USE_GCC" != "xyes" ; then
   _aCC_ROOT="/opt/aCC"
   DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
   DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Define the base path to the source directory using $SNXROOT_ABS as an
# alternative to using $srcdir.
UNIX_SNXROOT_ABS="$SNXROOT_ABS"

# $APP_EXTRA_LIBS must list all the libraries need to link a Sonix-based
# application.
if test "x$OS_TYPE" = "xWin32" ; then
   APP_EXTRA_LIBS="$LDFLAGS $VPR_LIBS $LIBS $VPR_EXTRA_LIBS"
   APP_EXTRA_PROF_LIBS="$APP_EXTRA_LIBS"
else
   APP_EXTRA_LIBS="$LDFLAGS $VPR_LIBS $LIBS $VPR_EXTRA_LIBS"
   APP_EXTRA_PROF_LIBS="$LDFLAGS $VPR_PROF_LIBS $LIBS $VPR_EXTRA_LIBS"
fi

# Put together the basic information needed to compile Sonix applications.
VJ_APP_COMPILER([$CC], [$CFLAGS], [$CXX], [$CXXFLAGS], [$DBG_FLAGS],
                [$OPT_FLAGS], [SNX_BASE_DIR], [$DEFS], [$INCLUDES],
                [${_EXTRA_FLAGS}])
VJ_APP_LINKER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG], [$LDOPTS_OPT],
              [SNX_BASE_DIR], [snx], [$APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(SNX_BASE_DIR)'

DYLIB_DEPS="$APP_EXTRA_LIBS"
DYLIB_PROF_DEPS="$APP_EXTRA_PROF_LIBS"

VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS], [DYLIB_DEP_FILES])
VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS], [DYLIB_PROF_DEP_FILES])

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
   SNXROOT_ABS=`dospath -p "$SNXROOT_ABS"`
   JUGGLERROOT_ABS=`dospath -p "$JUGGLERROOT_ABS"`

   if test "x$DEPEND_EXTRAS" != "x" ; then
      DEPEND_EXTRAS=`dospath "$DEPEND_EXTRAS"`
   fi

   if test "x$INCLUDES" != "x" ; then
      INCLUDES=`dospath "$INCLUDES"`
   fi

   if test "x$LIBS" != "x" ; then
      LIBS=`dospath "$LIBS"`
   fi

   LN_S='cp -r'
   RM_LN='rm -rf'
   MTREE_CMD=`dospath "$MTREE_CMD"`

#   APP_EXTRA_LIBS=`dospath "$APP_EXTRA_LIBS"`
    APP_EXTRA_LIBS_BASIC=`dospath "$APP_EXTRA_LIBS_BASIC"`
else
   RM_LN='rm -f'
   SNXROOT_ABS="$SNXROOT_ABS"
   JUGGLERROOT_ABS="$JUGGLERROOT_ABS"
fi

# Set up some sed expressions that will help separate the share directory from
# the full installation prefix.
if test "x$prefix" = "xNONE" ; then
   _prefix_exp="s|^$ac_default_prefix/||"
else
   _prefix_exp="s|^$prefix/||"
fi

# Make a best-effort attempt to extract the actual share directory from the
# full installation prefix.  This is not foolproof, but it will work in almost
# all cases.  A person would have to be doing something malicious or foolish
# to make it break.
data_subdir=`echo "$datadir" | sed -e 's|^${prefix}/||' -e "${_prefix_exp}"`
SNX_SHARE_DIR="$data_subdir/$DATA_VERSION"

snx_lib_name="sonix$LIBRARY_VERSION"
snx_prof_lib_name="sonix_p$LIBRARY_VERSION"

# Information needed to generate sonix-config.
case $OS_TYPE in
    UNIX)
        if test "x$GNU_LD" = "xyes" ; then
            static_begin="-Wl,-Bstatic"
            static_end="-Wl,-Bdynamic"
            snx_ldflags_compiler="-L\$prefix/lib\$LIBBITSUF -Wl,--export-dynamic"
            snx_ldflags_linker="-L\$prefix/lib\$LIBBITSUF --export-dynamic"
        else
            static_begin="-B static"
            static_end="-B dynamic"
            snx_ldflags_compiler="-L\$prefix/lib\$LIBBITSUF"
            snx_ldflags_linker="$snx_ldflags_compiler"
        fi

        snx_libs="-l$snx_lib_name"
        snx_prof_libs="-l$snx_prof_lib_name"
        snx_extra_libs="$LDFLAGS $LIBS"

        Sonix_lib='-lsonix'
        ;;
    Win32)
        snx_ldflags_compiler="/link /libpath:\$prefix/lib/debug "
        snx_ldflags_linker="/libpath:\$prefix/lib/debug "
        snx_libs="$snx_lib_name.lib"
        snx_prof_libs="$snx_prof_lib_name.lib"
        snx_extra_libs="$LDFLAGS $LIBS"

        Sonix_lib='sonix.lib'
        ;;
esac

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
    snx32_flags='-n32'
    snx64_flags='-64'
elif test "x$GCC" = "xyes" ; then
    snx32_flags='-m32'
    snx64_flags='-m64'
fi

snx_extra_ldflags_compiler="$APP_EXTRA_LIBS_BEGIN"
snx_extra_ldflags_linker="$snx_extra_ldflags_compiler"

includedir="$includedir/$HEADER_VERSION"

DPP_SUBST
DPP_GET_PROJECTDIR

AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_SNXROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(SNXROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(SNX_SHARE_DIR)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(DATA_VERSION)


AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS)
AC_SUBST(DYLIB_DEP_FILES)
AC_SUBST(DYLIB_PROF_DEPS)
AC_SUBST(DYLIB_PROF_DEP_FILES)

AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(snx_cxxflags)
AC_SUBST(snx_extra_cxxflags)
AC_SUBST(snx_ldflags_compiler)
AC_SUBST(snx_ldflags_linker)
AC_SUBST(snx_libs)
AC_SUBST(snx_prof_libs)
AC_SUBST(Sonix_lib)
AC_SUBST(snx_extra_ldflags_compiler)
AC_SUBST(snx_extra_ldflags_linker)
AC_SUBST(snx_extra_libs)
AC_SUBST(snx32_flags)
AC_SUBST(snx64_flags)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(SNX, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   sonix.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   snx/Makefile
   snx/Util/Makefile
   samples/simple/Makefile
   samples/opengl/Makefile
   mtree/SNX.data.dist
   mtree/SNX.include.dist
   mtree/SNX.install.dist
   mtree/SNX.samples.dist
   VARS.pl
   ]
   )

AC_OUTPUT
