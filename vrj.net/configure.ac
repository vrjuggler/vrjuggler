# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

AC_INIT([VRJ.NET], [0.0], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VRJ.NET is (C) Copyright 2004 by Patrick Hartling])
AC_REVISION([$Revision$])
AC_CONFIG_SRCDIR([src/sharppy.h])
AC_CONFIG_AUX_DIR([Doozer++/scripts])
DPP_PREREQ([1.5.1])
DPP_INIT

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

DPP_STD_CPP([yes])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

AC_ARG_WITH([cli],
            [  --with-cli=<NAME>       Name of the CLI implementation  [default=clr]
                          Valid values are: mono, clr, pnet],
            [cli_impl="$withval"], [cli_impl='clr'])

AC_ARG_WITH([cli-root],
            [  --with-cli-root=<PATH>  Path to CLI implementation root [default=/usr/local]],
            [CLI_ROOT="$withval"], [CLI_ROOT='/usr/local'])

AC_ARG_WITH([sharppy],
            [  --with-sharppy=<PATH>   Full path to sharppy.py script  [No default]],
            [SHARPPY="$withval"], [SHARPPY='sharppy.py'])

#AC_ARG_WITH([osg],
#            [  --with-osg=<PATH>       Open Scene Graph installation   [default=$OSGHOME then /usr/local]],
#            [osg_base_dir="$withval"], [osg_base_dir=${OSGHOME-/usr/local}])

# Force the use of GCC as the compiler.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP

# We use AC_CANONICAL SYSTEM so that we can find out information about the
# build, target and host platforms rather than only the host.
AC_CANONICAL_SYSTEM
DPP_SYSTEM_SETUP

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP

case $cli_impl in
   mono)
      csc_prog='mcs'
      AC_DEFINE([USE_MONO], , [Define if we are compiling against Mono])
      ;;
   clr)
      csc_prog='csc'
      AC_DEFINE([USE_CLR], ,
                [Define if we are compiling against Microsoft's CLR])
      ;;
   pnet)
      csc_prog='cscc'
      AC_DEFINE([USE_PNET], ,
                [Define if we are compiling against Portable.NET])
      ;;
   *)
      AC_MSG_ERROR([Unrecognized CLI implementation named $cli_impl])
      ;;
esac

csc_path="$CLI_ROOT/bin:$PATH"
AC_CHECK_PROG([CSC], [$csc_prog], [$csc_prog], [no], [$csc_path])

if test "x$csc_prog" = "xno" ; then
   AC_MSG_ERROR([*** Cannot find C sharp compiler for $cli_impl ***])
fi

DPP_CC_PTHREAD_ARG

if test "x$CC_ACCEPTS_PTHREAD" = "xno" ; then
   DPP_CC_PTHREADS_ARG
fi

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER([5.004], , , [AC_MSG_ERROR([*** Perl is required ***])])
DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL

AC_CHECK_FILE([$SHARPPY], , [AC_MSG_WARN([*** sharppy.py does not exist ***])])

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

case $cli_impl in
   mono)
      AC_CHECK_PROG([PKGCONFIG], [pkg-config], [pkg-config], [no])
      if test "x$PKGCONFIG" = "xno" ; then
         AC_MSG_ERROR([*** pkg-config required for configure to complete ***])
      fi

      CLI_CFLAGS=`PKG_CONFIG_PATH="$CLI_ROOT/lib/pkgconfig" pkg-config --cflags mono`
      CLI_LDFLAGS=`PKG_CONFIG_PATH="$CLI_ROOT/lib/pkgconfig" pkg-config --libs-only-L mono`
      CLI_LIBS=`PKG_CONFIG_PATH="$CLI_ROOT/lib/pkgconfig" pkg-config --libs-only-l mono`
      ;;
esac

GMTL_PATH([0.3.4], , , , [AC_MSG_ERROR([*** GMTL is required for VRJ.NET ***])])
VPR_PATH([0.34.2], , [AC_MSG_ERROR([*** VPR is required for VRJ.NET ***])])
JCCL_PATH([0.15], ,
          [AC_MSG_ERROR([*** JCCL is required for VRJ.NET ***])])
GADGETEER_PATH([0.15.1], ,
               [AC_MSG_ERROR([*** Gadgeteer is required for VRJ.NET ***])])
VRJUGGLER_PATH([1.1.152], ,
               [AC_MSG_ERROR([*** VR Juggler is required for VRJ.NET ***])])

#vrjnet_save_CFLAGS="$CFLAGS"
#vrjnet_save_CPPFLAGS="$CPPFLAGS"
#vrjnet_save_LDFLAGS="$LDFLAGS"
#vrjnet_save_LIBS="$LIBS"
#
## Ensure that /usr/include and /usr/lib are not included multiple times
## if $OSGROOT is "/usr".
#if test "x$osg_base_dir" != "x/usr" ; then
#   OSG_INCLUDES="-I$osg_base_dir/include"
#
#   if test "x$OS_TYPE" = "xWin32" ; then
#      OSG_LDFLAGS="/libpath:$osg_base_dir/lib"
#      OSG_LIBS='osg.lib osgProducer.lib osgUtil.lib osgDB.lib'
#   else
#      OSG_LDFLAGS="-L$osg_base_dir/lib"
#      OSG_LIBS='-losg -losgProducer -losgUtil -losgDB'
#   fi
#fi
#
#AC_PATH_X
#
#if test "x$x_libraries" != "xNONE" -a "x$x_libraries" != "x" ; then
#   X_LDFLAGS="-L$x_libraries"
#fi
#
#DPP_LANG_SAVE
#DPP_LANG_CPLUSPLUS
#
#CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"
#CPPFLAGS="$CPPFLAGS $OSG_INCLUDES $STDFLAGS"
#LDFLAGS="$LDFLAGS $OSG_LDFLAGS"
#
#AC_CHECK_LIB([osg], [osgGetVersion],
#   [AC_CHECK_HEADER([osg/Vec3], [HAVE_OSG='Y'], [HAVE_OSG='N'])],
#   [HAVE_OSG='N'], [$X_LDFLAGS -lX11 -lXext])
#
#DPP_LANG_RESTORE
#
#CFLAGS="$vrjnet_save_CFLAGS"
#CPPFLAGS="$vrjnet_save_CPPFLAGS"
#LDFLAGS="$vrjnet_save_LDFLAGS"
#LIBS="$vrjnet_save_LIBS"
#
#if test "x$HAVE_OSG" = "xY" ; then
#   AC_DEFINE([VRJNET_HAVE_OSG], 1,
#   	     [Define if the Open Scene Graph is available.])
#fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Finalize substitution settings.
# -----------------------------------------------------------------------------

if test "x$OS_TYPE" = "xWin32" ; then
   PLATFORM_TYPE='WIN32'
else
   PLATFORM_TYPE='UNIX'
fi

# Compiler-specific settings when GCC is not being used.
if test "x$USE_GCC" != "xyes" ; then
   if test "x$PLATFORM" = "xIRIX" ; then
      EXTRA_LINK_FLAGS="$EXTRA_LINK_FLAGS -J4 -all"
   fi
fi

if test "x$OS_TYPE" = "xWin32" ; then
   VRJNET_UTIL_LDFLAGS_OPT='/libpath:$(topdir)/lib vrjnet_util.lib'
   VRJNET_UTIL_LDFLAGS_DBG='/libpath:$(topdir)/lib vrjnet_util_d.lib'
else
   VRJNET_UTIL_LDFLAGS_OPT='-L$(topdir)/lib -lvrjnet_util'
   VRJNET_UTIL_LDFLAGS_DBG="$VRJNET_UTIL_LDFLAGS_OPT"
fi

DPP_PTHREAD_FINISH

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

DPP_SUBST

AC_SUBST(PLATFORM_TYPE)

AC_SUBST(EXTRA_LINK_FLAGS)
AC_SUBST(VRJNET_UTIL_LDFLAGS_DBG)
AC_SUBST(VRJNET_UTIL_LDFLAGS_OPT)

#AC_SUBST(HAVE_OSG)
#AC_SUBST(OSG_INCLUDES)
#AC_SUBST(OSG_LDFLAGS)
#AC_SUBST(OSG_LIBS)

AC_SUBST(SHARPPY)

AC_SUBST(CLI_CFLAGS)
AC_SUBST(CLI_LDFLAGS)
AC_SUBST(CLI_LIBS)

# -----------------------------------------------------------------------------
# Finish up.
# -----------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                 VARS.pl
                 vrjnet_util/Makefile
                 src/Makefile
                 src/common.mk
                 src/common-csharp.mk
                 src/common-cxx.mk
                 src/gadget_bridge_cpp/Makefile
                 src/gadget_bridge_cs/Makefile
                 src/gmtl_bridge_cpp/Makefile
                 src/gmtl_bridge_cs/Makefile
                 src/jccl_bridge_cpp/Makefile
                 src/jccl_bridge_cs/Makefile
                 src/vpr_bridge_cpp/Makefile
                 src/vpr_bridge_cs/Makefile
                 src/vrj_bridge_cpp/Makefile
                 src/vrj_bridge_cs/Makefile])

AC_OUTPUT
