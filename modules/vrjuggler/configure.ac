# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
# Base configure.in for VR Juggler.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([VR Juggler], [2.3], [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([vrj/vrjDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2008 by Iowa State University])
AC_CONFIG_SRCDIR([vrj/vrjConfig.h])
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

DPP_STD_CPP(yes)

# Enable compilation of the OpenGL API.  Possible values are "yes" or "no".
# "yes" is set with --enable-gl-api and "no" is set with --disable-gl-api.
# The default is "yes".
AC_ARG_ENABLE(gl-api,
              [  --enable-gl-api         Use OpenGL API                  [default=yes]],
              _enable_gl_api="$enableval", _enable_gl_api='yes')

# Enable compilation of the Performer API.  Possible values are "yes" or "no".
# "yes" is set with --enable-pf-api and "no" is set with --disable-pf-api.
# The default is "yes".
AC_ARG_ENABLE(pf-api,
              [  --enable-pf-api         Use Perfromer API               [default=yes]],
              _enable_pf_api="$enableval", _enable_pf_api='yes')

# Enable versioning by default
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])
# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #
AC_ARG_WITH([boost-po-lib],
            [  --with-boost-po-lib=<LIB NAME>
                          The name of the Boost.program_options
                          library suitable for the -l
                          compiler option                 [default=Auto-detect]],
            [given_boost_po_libname="$withval"], [given_boost_po_libname=''])

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is partially
# usable on Windows.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP
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

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $VJROOT_ABS, we cd there and save the value of running pwd.  Then return to
# the directory where configure is being run ($topdir).
cd "$srcdir"
VJROOT_ABS=`pwd`
cd "$VJROOT_ABS/../.."
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
DPP_CXX_NAMESPACE(AC_MSG_ERROR(*** The library requires C++ namesapce support ***))
DPP_CXX_HAVE_STD
DPP_CXX_INLINE(AC_MSG_ERROR(*** The library requires C++ inline support ***))
DPP_CXX_RTTI(AC_MSG_ERROR(*** The library requires C++ RTTI support ***))
DPP_CXX_STATIC_CAST(AC_MSG_ERROR(*** The library requires C++ static_cast<> ***))
DPP_CXX_DYNAMIC_CAST(AC_MSG_ERROR(*** The library requires C++ dynamic_cast<> ***))

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER(5.004, , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree], [\$(PERL) \$(scriptdir)/mtree.pl])

DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS($PLATFORM, $OS_TYPE)
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
AC_HEADER_STDC
AC_CHECK_HEADERS(strings.h sys/time.h unistd.h)

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS
AC_CHECK_HEADERS([ext/functional])
DPP_LANG_RESTORE

DPP_HAVE_BOOST([1.31.0], [/usr/local], , ,
               [AC_MSG_ERROR([*** Boost is required for VR Juggler ***])])
BOOST_VERSION_DOT="$dpp_boost_version"

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
VPR_PATH([2.1.4], , [AC_MSG_ERROR(*** VPR required for VR Juggler ***)])

JCCL_PATH_CXX([1.3.5], ,
              [AC_MSG_ERROR(*** JCCL C++ API required for VR Juggler ***)])
GADGETEER_PATH([1.3.15], ,
               [AC_MSG_ERROR(*** Gadgeteer required for VR Juggler ***)])
SNX_PATH([1.3.6], , [AC_MSG_ERROR(*** Sonix required for VR Juggler ***)])

VJ_GET_BOOST_LIB_SUFFIX_LIST([$dpp_boost_major], [$dpp_boost_minor],
                             [$BOOST_LIB_VERSION], [boost_suffixes])

if test "x$given_boost_po_libname" != "x" ; then
   boost_po_name="$given_boost_po_libname"
fi

# Ensure that a compiled version of the Boost.program_options library is available.
vrjsave_CXXFLAGS="$CXXFLAGS"
vrjsave_CPPFLAGS="$CPPFLAGS"
vrjsave_LIBS="$LIBS"
vrjsave_LDFLAGS="$LDFLAGS"

CXXFLAGS="$CXXFLAGS $ABI_FLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_INCLUDES"

if test "lib$LIBBITSUF" != "lib" ; then
   libdirs="lib$LIBBITSUF lib"
else
   libdirs="lib"
fi

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

testBoostProgramOptions ( )
{
   libname="$1"
   orig_LIBS="$2"

   # NOTE: We do not need $ABI_FLAGS in $LDFLAGS since $CXXFLAGS and $LDFLAGS
   # are used together by AC_TRY_LINK().
   LIBS="-l$libname $orig_LIBS"

   AC_MSG_CHECKING([for boost::program_options::options_description() in $libname in $cur_boost_libdir])
   AC_TRY_LINK([#include <boost/program_options.hpp>],
               [boost::program_options::options_description desc("Allowed options");desc.add_options()("help", "help");],
               [vrj_have_boost_po='yes'], [vrj_have_boost_po='no'])
   rm -rf ./ii_files
   AC_MSG_RESULT([$vrj_have_boost_po])

   LIBS="$orig_LIBS"
}

for l in $libdirs ; do
   cur_boost_libdir="$BOOST_ROOT/$l"
   LDFLAGS="-L$cur_boost_libdir $vrjsave_LDFLAGS"

   if test "x$boost_po_name" != "x" ; then
      testBoostProgramOptions $boost_po_name $vrjsave_LIBS
   else
      # The extra empty string is to allow for no suffix to be used on the
      # boost_program_options library name.
      for s in $boost_suffixes "" ; do
         cur_boost_po_name="boost_program_options$s"

         testBoostProgramOptions $cur_boost_po_name $vrjsave_LIBS

         if test "$vrj_have_boost_po" = "yes" ; then
            boost_po_name="$cur_boost_po_name"
            break
         fi
      done
   fi

   if test "$vrj_have_boost_po" = "yes" ; then
      BOOST_LIBDIR="$cur_boost_libdir"
      break
   fi
done

if test "x$vrj_have_boost_po" != "xyes" ; then
   AC_MSG_ERROR([*** Boost.program_options required by VR Juggler ***])
else
   BOOST_PO_LINK="-l$boost_po_name"
fi

DPP_LANG_RESTORE

CXXFLAGS="$vrjsave_CXXFLAGS"
CPPFLAGS="$vrjsave_CPPFLAGS"
LIBS="$vrjsave_LIBS"
LDFLAGS="$vrjsave_LDFLAGS"

VRJ_USE_X11="$GADGET_USE_X11"

if test "x$VRJ_USE_X11" = "xyes" ; then
   AC_PATH_XTRA

   if test "x$PLATFORM" = "xDarwin" ; then
      vjsave_CPPFLAGS="$CPPFLAGS"
      AC_MSG_CHECKING([for extra flags needed to include GL/glx.h])
      AC_TRY_CPP([#include <GL/glx.h>], [AC_MSG_RESULT([none])],
         [CPPFLAGS="$CPPFLAGS -I/usr/X11R6/include"
          AC_TRY_CPP([#include <GL/glx.h>],
             [X_CFLAGS="$X_CFLAGS -I/usr/X11R6/include"
              AC_MSG_RESULT([-I/usr/X11R6/include])],
             [AC_MSG_RESULT([failed to preprocess GL/glx.h])
              AC_MSG_ERROR([*** GLX header required ***])]
         )]
      )
      CPPFLAGS="$vjsave_CPPFLAGS"
   fi

   AC_DEFINE([VRJ_USE_X11], , [Define if the X Window System will be used.])
# IF we are not using X11 on Mac OS X, then we need to make sure that we can
# build Objective-C and Objective-C++ code.
elif test "x$PLATFORM" = "xDarwin" ; then
   DPP_PROG_OBJC
   DPP_PROG_OBJCXX
   AC_DEFINE([VRJ_USE_COCOA], , [Define if Cocoa will be used.])
fi

GMTL_PATH([0.4.11], , [AC_MSG_ERROR([*** GMTL required for VR Juggler ***])])

# ------------
# OpenGL API.
# ------------
OPENGL_API=''			# 'Y' then we are using OpenGL

# Test for the OpenGL library if the OpenGL API was enabled.
if test "x${_enable_gl_api}" = "xyes" ; then
    DPP_HAVE_OPENGL(/usr, no,
        [OPENGL_API='Y'],
        [AC_MSG_ERROR(*** OpenGL API will not be used (GL files not found) ***)])
fi

# The following determines whether we have to link against libXxf86vm
# explicitly.  This is a little tricky because the Mesa libGL is always
# linked against it (either statically or implicitly), but the NVidia libGL
# implementation is not.
# We only need to worry about this on non-Darwin UNIX-based platforms.
if test "x$OS_TYPE" = "xUNIX" -a "x$PLATFORM" != "xDarwin" ; then
   DPP_LANG_SAVE
   DPP_LANG_C

   vjsave_CFLAGS="$CFLAGS"
   vjsave_LIBS="$LIBS"
   vjsave_LDFLAGS="$LDFLAGS"

   CFLAGS="$CFLAGS $X_CFLAGS"
   LDFLAGS="$LDFLAGS $X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"

   # First, make sure that libXxf86vm even exists and that the function that
   # we'll use for testing below is available.  If this test fails, we assume
   # that we are not using XFree86 or X.org.
   AC_CHECK_LIB([Xxf86vm], [XF86VidModeQueryExtension], [using_xf86='yes'],
                [using_xf86='no'])

   if test "x$using_xf86" = "xyes" ; then
      CFLAGS="$vjsave_CFLAGS $OGL_INCLUDES"
      LIBS="$GL_LIB $vjsave_LIBS"
      LDFLAGS="$vjsave_LDFLAGS $OGL_LDFLAGS"

      AC_CACHE_CHECK([if we need to link against libXxf86vm],
                     [vj_cv_need_libXxf86vm],
                     [AC_TRY_LINK([#include <stdlib.h>
#include <X11/Xlib.h>],
                        [XF86VidModeQueryExtension(NULL, NULL, NULL);],
                        [vj_cv_need_libXxf86vm='no'],
                        [vj_cv_need_libXxf86vm='yes'])])

      # If we have to link against libXxf86vm, we can do that most easily by
      # adding it to the list of existing OpenGL libraries we'll be using.
      if test "x$vj_cv_need_libXxf86vm" = "xyes" ; then
         LIBOPENGL="$LIBOPENGL -lXxf86vm"
      fi
   fi

   CFLAGS="$vjsave_CFLAGS"
   LIBS="$vjsave_LIBS"
   LDFLAGS="$vjsave_LDFLAGS"

   DPP_LANG_RESTORE
fi

# ---------------------
# OpenGL Performer API.
# ---------------------
PERFORMER_API=''		# 'Y' then we are using Performer

# Test for the Performer library if the Performer API was enabled.
if test "x${_enable_pf_api}" = "xyes" ; then
    DPP_HAVE_PERFORMER(/usr, 2.4,
        [PERFORMER_API='Y'],
        [AC_MSG_WARN(*** Performer API will not be used (files not found) ***)])
fi

DPP_HAVE_CPPUNIT([1.9.10])

# -----------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# -----------------------------------------------------------------------------
AC_TYPE_SIZE_T

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Finalize substitution settings.
# -----------------------------------------------------------------------------

VJ_VERSION_GROK(VERSION)

# Set the form of the version number used for versioned directory and file
# naming. Unstable builds use all three version numbers; stable builds use
# only the major and minor version numbers.
if test "x$ENABLE_VERSIONING" = "xyes" ; then
   VERSION_DOT="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
   VERSION_UNDERSCORE="${MAJOR_VERSION}_${MINOR_VERSION}_${MICRO_VERSION}"

   HEADER_VERSION="vrjuggler-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="vrjuggler-$VERSION_DOT"
   PLUGIN_VERSION="vrjuggler-$VERSION_DOT"

   AC_DEFINE([VRJ_USE_VERSIONING], ,
             [Define if full package versioning capabilities are enabled])
   AC_DEFINE_UNQUOTED([VRJ_VERSION_DOT], ["$VERSION_DOT"],
                      [VR Juggler version string])

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION and $PLUGIN_VERSION. Their use is different
# than the others. The important thing to note is that the directory name is
# unversioned in this case.
else
   DATA_VERSION="vrjuggler"
   PLUGIN_VERSION="vrjuggler"
fi

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $GADGET_CXXFLAGS $SNX_CXXFLAGS"
OBJCXXFLAGS="$CXXFLAGS -fobjc-exceptions"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$GADGET_CXXFLAGS $SNX_CXXFLAGS"
vrj_cxxflags="$STDFLAGS"

# Compiler-specific settings when GCC is not being used.
if test "x$USE_GCC" != "xyes" ; then
   if test "x$PLATFORM" = "xIRIX" ; then
#      CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
      CXXFLAGS="$CXXFLAGS -w2"
      BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -J4 -all"
   elif test "x$OS_TYPE" = "xWin32" ; then
      vc_cflags='/QIfdiv /QI0f'
      vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
      CFLAGS="$CFLAGS $vc_cflags"
      CXXFLAGS="$CXXFLAGS $vc_cxxflags"
      BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
      DBG_FLAGS="$DBG_FLAGS /MDd"
      OPT_FLAGS="$OPT_FLAGS /MD"
      vrj_cxxflags="$vc_cxxflags $vrj_cxxflags"
   fi
fi

if test "x$PLATFORM" = "xDarwin" ; then
   changequote(<<, >>)
   vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
   changequote([, ])

   # GCC 4.0 in Darwin removed -fexport-coalesced.
   if test $vpr_gcc_major -lt 4 ; then
      CFLAGS_DYNLIB="$CFLAGS_DYNLIB -fexport-coalesced"
      CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
      OBJCXXFLAGS_DYNLIB="$OBJCXXFLAGS_DYNLIB -fexport-coalesced"
   fi

   BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -Wl,-bind_at_load"
fi

if test "x$x_includes" != "xNONE" -a "x$x_includes" != "x" ; then
   INCLUDES="$INCLUDES -I$x_includes"
   vrj_cxxflags="$vrj_cxxflags -I$x_includes"
fi

if test "x$OGL_INCLUDES" != "x" ; then
   INCLUDES="$INCLUDES $OGL_INCLUDES"
   vrj_cxxflags="$vrj_cxxflags $OGL_INCLUDES"
fi

if test "x$PF_INCLUDES" != "x" ; then
   INCLUDES="$INCLUDES $PF_INCLUDES"
   vrj_cxxflags="$vrj_cxxflags $PF_INCLUDES"
fi

if test "x$PLATFORM" = "xDarwin" -a "x$VRJ_USE_X11" = "xyes" ; then
   vrj_cxxflags="$vrj_cxxflags $X_CFLAGS"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

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
VJ_STRIP_DUPLICATE_ARGS(vrj_cxxflags, [$vrj_cxxflags])

VJ_APP_CXXFLAGS='$(VRJ_CXX_FLAGS)'
VJ_APP_EXTRA_LIBS='$(VRJ_EXTRA_LIBS_STATIC)'
VJ_APP_EXTRA_LIBS_INST='$(VRJ_EXTRA_LIBS)'

# Enable warnings when building applications with GCC.
if test "x$GXX" = "xyes" ; then
   VJ_APP_CXXFLAGS="$VJ_APP_CXXFLAGS -Wall"
fi

# Variables set in Makefiles found in the application trees.  All variables
# listed are for use only in the Makefile.in's found in that tree unless
# otherwise noted.  They are used in the following way:
#
#     * APP_EXTRA_LIBS_GL: The OpenGL libraries needed for compiling with the
#       OpenGL API.
#     * APP_EXTRA_LIBS_PF: The Performer libraries needed for compiling with
#       the Performer API.
#     * APP_EXTRA_LIBS_OSG: The OpenSceneGraph libraries needed for compiling
#       with the Open Scene Graph.

# Put together the basic information needed to compile VR Juggler applications.
VJ_APP_COMPILER($CC, $CFLAGS, $CXX, $VJ_APP_CXXFLAGS, $DBG_FLAGS, $OPT_FLAGS,
                VJ_BASE_DIR, $DEFS, , ${_EXTRA_FLAGS}, $HEADER_VERSION)
VJ_APP_LINKER_DOOZER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG],
                     [$LDOPTS_OPT], ['$(VRJ_LIBS)'],
                     [$VJ_APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(VJ_BASE_DIR)'

vrj_lib_name="vrj$LIBRARY_VERSION"
vrj_prof_lib_name="vrj_p$LIBRARY_VERSION"

vrjogl_lib_name="vrj_ogl$LIBRARY_VERSION"
vrjogl_prof_lib_name="vrj_ogl_p$LIBRARY_VERSION"

vrjpf_lib_name="vrj_pf$LIBRARY_VERSION"
vrjpf_prof_lib_name="vrj_pf_p$LIBRARY_VERSION"

if test "$OS_TYPE" = "UNIX" ; then
    REQ_EXTRA_LIBS="$LDFLAGS $SNX_LIBS $GADGET_LIBS -lm -l$boost_po_name"
    DYLIB_DEPS_VJ="$REQ_EXTRA_LIBS"
    DYLIB_DEPS_GL="$REQ_EXTRA_LIBS"
    DYLIB_DEPS_PF="$PF_LDFLAGS $LIBPERFORMER $REQ_EXTRA_LIBS"

    osx_core_frameworks=''
    osx_gl_frameworks=''

    if test "x$VRJ_USE_X11" != "xyes" ; then
        osx_core_frameworks="-framework AppKit -framework Cocoa"
        osx_gl_frameworks="$osx_core_frameworks"
    fi

    REQ_EXTRA_PROF_LIBS="$LDFLAGS $SNX_PROF_LIBS $GADGET_PROF_LIBS $LIBS -lm -l$boost_po_name"
    DYLIB_PROF_DEPS_VJ="$REQ_EXTRA_PROF_LIBS"
    DYLIB_PROF_DEPS_GL="-L\$(LIBDIR) -l$vrj_prof_lib_name $OGL_LDFLAGS $LIBOPENGL $REQ_EXTRA_PROF_LIBS"
    DYLIB_PROF_DEPS_PF="-L\$(LIBDIR) -l$vrj_prof_lib_name $PF_LDFLAGS $LIBPERFORMER $REQ_EXTRA_PROF_LIBS"

    if test "x$PLATFORM" = "xDarwin" ; then
        DYLIB_DEPS_VJ="$DYLIB_DEPS_VJ $osx_core_frameworks"

        if test "x$VRJ_USE_X11" = "xyes" ; then
            # Deal with a bug in Mac OS X 10.5.0 and 10.5.1 (at lesat) when
            # linking against the X11 libGL.dylib.
            case $target_os in
               darwin9*)
                  gl_hack='-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib'
                  ;;
            esac

            CFLAGS="$CFLAGS $X_CFLAGS"
            CXXFLAGS="$CXXFLAGS $X_CFLAGS"
            DYLIB_DEPS_GL="$DYLIB_DEPS_GL -lGLU -lGL $X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS $osx_gl_frameworks $gl_hack"
            DYLIB_PROF_DEPS_GL="$DYLIB_PROF_DEPS_GL -lGLU -lGL $X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS $osx_gl_frameworks $gl_hack"
        else
            DYLIB_DEPS_GL="$DYLIB_DEPS_GL $OGL_LDFLAGS $LIBOPENGL $osx_gl_frameworks"
            DYLIB_PROF_DEPS_GL="$DYLIB_DEPS_GL $OGL_LDFLAGS $LIBOPENGL $osx_gl_frameworks"
        fi
    else
        DYLIB_DEPS_GL="$DYLIB_DEPS_GL $OGL_LDFLAGS $LIBOPENGL"
        DYLIB_PROF_DEPS_GL="$DYLIB_PROF_DEPS_GL $OGL_LDFLAGS $LIBOPENGL"
    fi

    if test "x$VRJ_USE_X11" = "xyes" ; then
        APP_EXTRA_LIBS_X11="$X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
    fi

    APP_EXTRA_LIBS_OSG='-L$(OSGHOME)/lib -losg -losgDB -losgGA -losgUtil -lOpenThreads'

    if test "x$OGL_LDFLAGS" != "x" ; then
        if test "x$PLATFORM" = "xDarwin" ; then
            if test "x$VRJ_USE_X11" = "xyes" ; then
                APP_EXTRA_LIBS_GL="-lGLU -lGL $osx_core_frameworks $gl_hack"
            else
                APP_EXTRA_LIBS_GL="$OGL_LDFLAGS $osx_core_frameworks"
            fi
        else
            APP_EXTRA_LIBS_GL="-L$OGLROOT/lib$LIBBITSUF $LIBOPENGL"
        fi
    else
        APP_EXTRA_LIBS_GL="$LIBOPENGL"
    fi

    if test "x$PF_LDFLAGS" != "x" ; then
        APP_EXTRA_LIBS_PF="-L$PFROOT/lib$LIBBITSUF $LIBPERFORMER"
    else
        APP_EXTRA_LIBS_PF="$LIBPERFORMER"
    fi

    VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS_VJ], [DYLIB_DEP_FILES_VJ])
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS_GL], [DYLIB_DEP_FILES_GL])
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS_PF], [DYLIB_DEP_FILES_PF])
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS_VJ], [DYLIB_PROF_DEP_FILES_VJ])
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS_GL], [DYLIB_PROF_DEP_FILES_GL])
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS_PF], [DYLIB_PROF_DEP_FILES_PF])

    # This is done after the call to VJ_BUILD_LIB_FILE_LIST() to prevent the
    # build from getting confused and using a path to the wrong libvrj.
    # Unfortunately, this means that libvrj will not be identified as a
    # dependency of libvrj_ogl or of libvrj_pf, but that generally shouldn't
    # be a problem since the regular header file dependency stuff will cause
    # object files to be rebuilt appropriately.
    # XXX: This is, of cousre, a hack.
    DYLIB_DEPS_GL="-L\$(LIBDIR) -l$vrj_lib_name $DYLIB_DEPS_GL"
    DYLIB_DEPS_PF="-L\$(LIBDIR) -l$vrj_lib_name $DYLIB_DEPS_PF"
elif test "$OS_TYPE" = "Win32" ; then
    REQ_EXTRA_LIBS="$LDFLAGS $GADGET_LIBS $SNX_LIBS $LIBS"
    BASIC_WIN32_LIBS='comctl32.lib user32.lib gdi32.lib'
    DYLIB_DEPS_BASIC="$REQ_EXTRA_LIBS $BASIC_WIN32_LIBS"
    DYLIB_DEPS_VJ="/libpath:\$(LIBDIR) $DYLIB_DEPS_BASIC"
    DYLIB_DEPS_GL="/libpath:\$(LIBDIR) vrj.lib $DYLIB_DEPS_BASIC $OGL_LDFLAGS $LIBOPENGL"
    DYLIB_DEPS_PF="/libpath:\$(LIBDIR) vrj.lib $PF_LDFLAGS_LINK_EXE $LIBPERFORMER_LINK_EXE $OGL_LDFLAGS $LIBOPENGL $REQ_EXTRA_LIBS"
    DYLIB_PROF_DEPS_VJ="$DYLIB_DEPS_VJ"
    DYLIB_PROF_DEPS_GL="$DYLIB_DEPS_GL"
    DYLIB_PROF_DEPS_PF="$DYLIB_DEPS_PF"

    APP_EXTRA_LIBS_GL="$OGL_LDFLAGS $LIBOPENGL"
    APP_EXTRA_LIBS_PF="$PF_LDFLAGS_LINK_EXE $LIBPERFORMER_LINK_EXE"
    APP_EXTRA_LIBS_OSG='/libpath:$(OSGHOME)/lib osg.lib osgDB.lib osgGA.lib osgUtil.lib'
fi


# Define the base path to the source directory using $(VJROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_VJROOT_ABS="$VJROOT_ABS"

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    VJROOT_ABS=`dospath -p "$VJROOT_ABS"`
    JUGGLERROOT_ABS=`dospath -p "$JUGGLERROOT_ABS"`

#    DEPEND_EXTRAS=`dospath "$DEPEND_EXTRAS"`
#    INCLUDES=`dospath "$INCLUDES"`
#    LIBS=`dospath "$LIBS"`
    LN_S='cp -r'
    RM_LN='rm -rf'
    MTREE_CMD=`dospath "$MTREE_CMD"`

    JDK_HOME=`dospath -p "$JDK_HOME"`

#    APP_EXTRA_LIBS=`dospath "$APP_EXTRA_LIBS"`
#    APP_EXTRA_LIBS_GL=`dospath "$APP_EXTRA_LIBS_GL"`
#    APP_EXTRA_LIBS_PF=`dospath "$APP_EXTRA_LIBS_PF"`
else
    RM_LN='rm -f'
    VJROOT_ABS="$VJROOT_ABS"
    JUGGLERROOT_ABS="$JUGGLERROOT_ABS"
fi

# Set up some sed expressions that will help separate the share directory from
# the full installation prefix.
if test "x$prefix" = "xNONE" ; then
    _prefix_exp="s|^$ac_default_prefix/||"
else
    _prefix_exp="s|^$prefix/||"
fi

VJ_CLEAN_LINKER_ARGS(LDFLAGS, [$LDFLAGS])
VJ_CLEAN_LINKER_ARGS(LIBS, [$LIBS])
VJ_CLEAN_LINKER_ARGS(DYLIB_DEPS_VJ, [$DYLIB_DEPS_VJ])
VJ_CLEAN_LINKER_ARGS(DYLIB_DEPS_GL, [$DYLIB_DEPS_GL])

# XXX: This will mangle the default Performer installation path on Windows.
# Having to deal with spaces in path names sucks.
if test "x$OS_TYPE" != "xWin32" ; then
   VJ_CLEAN_LINKER_ARGS(DYLIB_DEPS_PF, [$DYLIB_DEPS_PF])
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
VJ_SHARE_DIR="$data_subdir/$DATA_VERSION"

# Information needed to generate vrjuggler-config.
# $vrj_extra_libs only lists the external (non-Juggler) libraries needed by
# VR Juggler.
vrj_ldflags="-L\$prefix/lib\$LIBBITSUF"
vrj_extra_libs="$LDFLAGS $APP_EXTRA_LIBS_GL $APP_EXTRA_LIBS_X11 $LIBS"

vrj_libs="-l$vrj_lib_name"
vrj_prof_libs="-l$vrj_prof_lib_name"
vrj_ogl_lib="-l$vrjogl_lib_name"
vrj_ogl_prof_lib="-l$vrjogl_prof_lib_name"

if test "x$PERFORMER_API" = "xY" ; then
   vrj_pf_lib="-l$vrjpf_lib_name"
   vrj_pf_prof_lib="-l$vrjpf_prof_lib_name"
   vrj_pf_extra_libs="$APP_EXTRA_LIBS_PF"
fi

# XXX: Ideally, -lm would not be added "manually" like this.
vrj_extra_libs="$LDFLAGS $APP_EXTRA_LIBS_X11 $LIBS -lm"
vrj_ogl_extra_libs="$APP_EXTRA_LIBS_GL"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
    vrj32_flags='-n32'
    vrj64_flags='-64'
elif test "x$GCC" = "xyes" ; then
    vrj32_flags='-m32'
    vrj64_flags='-m64'
fi

vrj_extra_ldflags="$APP_EXTRA_LIBS_BEGIN"

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------
includedir="$includedir/$HEADER_VERSION"
vrj_header_subdir="$HEADER_VERSION"
vrj_data_subdir="$DATA_VERSION"
vrj_plugin_subdir="$PLUGIN_VERSION"

DPP_SUBST

AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_VJROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(VJROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(VJ_SHARE_DIR)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(DATA_VERSION)
AC_SUBST(PLUGIN_VERSION)

AC_SUBST(ANT)
AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)
AC_SUBST(VPR_SUBSYSTEM)

AC_SUBST(OPENGL_API)
AC_SUBST(PERFORMER_API)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS_VJ)
AC_SUBST(DYLIB_DEPS_GL)
AC_SUBST(DYLIB_DEPS_PF)
AC_SUBST(DYLIB_DEP_FILES_VJ)
AC_SUBST(DYLIB_DEP_FILES_GL)
AC_SUBST(DYLIB_DEP_FILES_PF)
AC_SUBST(DYLIB_PROF_DEPS_VJ)
AC_SUBST(DYLIB_PROF_DEPS_GL)
AC_SUBST(DYLIB_PROF_DEPS_PF)
AC_SUBST(DYLIB_PROF_DEP_FILES_VJ)
AC_SUBST(DYLIB_PROF_DEP_FILES_GL)
AC_SUBST(DYLIB_PROF_DEP_FILES_PF)

AC_SUBST(VJ_APP_EXTRA_LIBS_INST)
AC_SUBST(APP_EXTRA_LIBS_GL)
AC_SUBST(APP_EXTRA_LIBS_OSG)
AC_SUBST(APP_EXTRA_LIBS_PF)

AC_SUBST(vrj_cxxflags)
AC_SUBST(vrj_extra_cxxflags)
AC_SUBST(vrj_ldflags)
AC_SUBST(vrj_extra_ldflags)
AC_SUBST(vrj_extra_libs)
AC_SUBST(vrj32_flags)
AC_SUBST(vrj64_flags)

AC_SUBST(vrj_libs)
AC_SUBST(vrj_ogl_lib)
AC_SUBST(vrj_pf_lib)
AC_SUBST(vrj_prof_libs)
AC_SUBST(vrj_ogl_prof_lib)
AC_SUBST(vrj_pf_prof_lib)
AC_SUBST(vrj_extra_libs)
AC_SUBST(vrj_ogl_extra_libs)
AC_SUBST(vrj_pf_extra_libs)
AC_SUBST(vrj_header_subdir)
AC_SUBST(vrj_data_subdir)
AC_SUBST(vrj_plugin_subdir)

AC_SUBST(BOOST_VERSION_DOT)
AC_SUBST(BOOST_LIBDIR)
AC_SUBST(BOOST_PO_LINK)
AC_SUBST(boost_po_name)

AC_SUBST(tweek_java_api_jars_bundle)
AC_SUBST(tweek_ext_java_api_jars_bundle)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(VJ, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   vrjuggler.fpc
   vrjuggler-opengl.fpc
   vrjuggler-performer.fpc
   boost_program_options.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   vrj/Makefile
   vrj/Display/Makefile
   vrj/Draw/Makefile
   vrj/Draw/OGL/Makefile
   vrj/Draw/OSG/Makefile
   vrj/Draw/OpenGL/Makefile
   vrj/Draw/OpenSG/Makefile
   vrj/Draw/Pf/Makefile
   vrj/Kernel/Makefile
   vrj/Performance/Makefile
   vrj/Sound/Makefile
   vrj/Test/Makefile
   vrj/Util/Makefile
   vrj.appdefs.mk
   samples/Makefile
   samples/OGL/Makefile
   samples/OGL/advanced/Makefile
   samples/OGL/advanced/glove/Makefile
   samples/OGL/advanced/navgrab/Makefile
   samples/OGL/simple/Makefile
   samples/OGL/simple/ConfigApp/Makefile
   samples/OGL/simple/MPApp/Makefile
   samples/OGL/simple/SimpleApp/Makefile
   samples/OGL/simple/contextApp/Makefile
   samples/OGL/simple/simpleInput/Makefile
   samples/OGL/ext/Makefile
   samples/OGL/ext/proceduralApp/Makefile
   samples/OSG/Makefile
   samples/OSG/advanced/Makefile
   samples/OSG/advanced/OSGNavGrab/Makefile
   samples/OSG/simple/Makefile
   samples/OSG/simple/osgNav/Makefile
   samples/OpenSG/Makefile
   samples/OpenSG/advanced/Makefile
   samples/OpenSG/advanced/OpenSGNavGrab/Makefile
   samples/OpenSG/advanced/OpenSGViewer/Makefile
   samples/OpenSG/simple/Makefile
   samples/OpenSG/simple/OpenSGNav/Makefile
   samples/Pf/Makefile
   samples/Pf/advanced/Makefile
   samples/Pf/advanced/pfConfigNav/Makefile
   samples/Pf/advanced/pfNav/Makefile
   samples/Pf/simple/Makefile
   samples/Pf/simple/simplePf/Makefile
   samples/sound/Makefile
   samples/sound/simple/Makefile
   samples/sound/simple/sonix/Makefile
   samples/sound/simple/soundManager/Makefile
   samples/speech/Makefile
   samples/speech/simple/Makefile
   samples/speech/simple/SpeechRecogApp/Makefile
   test/Makefile 
   test/Cluster/Makefile
   test/Cluster/applicationData/Makefile
   test/Draw/Makefile
   test/Draw/OGL/Makefile
   test/Draw/OGL/analog/Makefile
   test/Draw/OGL/combo/Makefile
   test/Draw/OGL/cubes/Makefile
   test/Draw/OGL/digitalGlove/Makefile
   test/Draw/OGL/logger/Makefile
   test/Draw/OGL/simpleGlove/Makefile
   test/Draw/OGL/swapSync/Makefile   
   test/Draw/OGL/texture/Makefile
   test/Draw/OGL/torus/Makefile
   test/Draw/OGL/WallTest/Makefile
   test/Draw/OGL/wand/Makefile
   test/PerfMon/Makefile
   test/PerfMon/collideApp/Makefile
   test/RTRC/Makefile
   test/TestSuite/Makefile
   test/TestSuite/test.defs.mk
   test/TestSuite/TestCases/Makefile
   test/TestSuite/TestCases/Pf/Makefile
   tools/Makefile
   mtree/VJ.data.dist
   mtree/VJ.include.dist
   mtree/VJ.install.dist
   mtree/VJ.java.dist
   mtree/VJ.samples.dist
   mtree/VJ.test.dist
   mtree/VJ.tools.dist
   VARS.pl
   ]
   )

AC_OUTPUT
