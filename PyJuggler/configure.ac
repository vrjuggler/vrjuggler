# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

AC_INIT([PyJuggler], [0.9], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling])
AC_REVISION([$Revision$])
AC_CONFIG_SRCDIR([src/gadgeteer-wrappers.h])
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

AC_ARG_ENABLE([boost-hacks],
              [  --disable-boost-hacks   Turn off workarounds for Boost
                          versioning madness],
              [BOOST_HACKS="$withval"], [BOOST_HACKS='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

AC_ARG_WITH([python],
            [  --with-python=<PATH>    Path to Python root             [default=/usr/local]],
            [PYTHON_ROOT="$withval"], [PYTHON_ROOT='/usr/local'])

AC_ARG_WITH([boost-python],
            [  --with-boost-python=<PATH>
                          Path to a Boost.Python V2
                          installation                    [default=/usr/local]],
            [BOOST_PYTHON_DIR="$withval"], [BOOST_PYTHON_DIR='/usr/local'])

AC_ARG_WITH([boost-version],
            [  --with-boost-version=<VER>
                          Boost version number (for
                          example, 1.31)                  [default=1.31]],
            [BOOST_VERSION="$withval"], [BOOST_VERSION='1.31'])

AC_ARG_WITH([pyste],
            [  --with-pyste=<PATH>     Full path to pyste.py script    [No default]],
            [PYSTE="$withval"], [PYSTE='pyste.py'])

AC_ARG_WITH([osg],
            [  --with-osg=<PATH>       Open Scene Graph installation   [default=$OSGHOME then /usr/local]],
            [osg_base_dir="$withval"], [osg_base_dir=${OSGHOME-/usr/local}])

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

# We use AC_CANONICAL SYSTEM so that we can find out information about the
# build, target and host platforms rather than only the host.
AC_CANONICAL_SYSTEM
DPP_SYSTEM_SETUP

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP

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

if test "x$OS_TYPE" = "xWin32" ; then
   python_root_cyg=`$CYGPATH -u $PYTHON_ROOT`
   python_path="$python_root_cyg:$python_root_cyg/bin:$PATH"
else
   python_path="$PYTHON_ROOT/bin:$PATH"
fi

USE_OBJCOPY='no'
if test "x$GXX" = "xyes" ; then
   AC_PATH_PROG(OBJCOPY, objcopy, no)

   if test "x$OBJCOPY" != "xno" ; then
      USE_OBJCOPY='yes'
   fi
fi

AC_PATH_PROG(PYTHON, python, no, [$python_path])

if test "x$PYTHON" = "xno" ; then
   AC_MSG_ERROR([*** Python is required for building PyJuggler ***])
fi

AC_CHECK_FILE([$PYSTE], , [AC_MSG_WARN([*** pyste.py does not exist ***])])

AC_MSG_CHECKING([for Python version])
changequote(<<, >>)
PYTHON_VERSION=`$PYTHON -c "import sys; print sys.version[:3]"`
changequote([, ])
AC_MSG_RESULT([$PYTHON_VERSION])

AC_MSG_CHECKING([for suffix of Python extension modules])
changequote(<<, >>)

PYMOD_EXT=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_config_var('SO')"`

# This is a fallback in case Python fails to tell us the file extension it
# prefers.
if test "x" = "x$PYMOD_EXT"; then
   PYMOD_EXT=".$DYNAMICLIB_EXT"
fi

changequote([, ])
AC_MSG_RESULT([$PYMOD_EXT])

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

DPP_GET_PTHREAD_LIB

# We do this first so that we can pass the $DL_LOAD_LIB in as a dependency of
# Python.
_pyj_save_LIBS="$LIBS"
AC_CHECK_FUNC([dlopen], [DL_LOAD_LIB=''],
    [AC_CHECK_LIB([dl], [dlopen], [DL_LOAD_LIB='-ldl'])])
LIBS="${_pyj_save_LIBS}"

_pyj_save_LIBS="$LIBS"
AC_CHECK_FUNC([openpty], [UTIL_LIB=''],
    [AC_CHECK_LIB([util], [openpty], [UTIL_LIB='-lutil'])])
LIBS="${_pyj_save_LIBS}"

if test "x$OS_TYPE" = "xWin32" ; then
   py_ver=`echo $PYTHON_VERSION | sed -e 's/\.//g'`
   PYTHON_INCLUDES="-I$PYTHON_ROOT/include"
   PYTHON_LDFLAGS="-L$PYTHON_ROOT/libs"
   PYTHON_LIB="python$py_ver"
else
   PYTHON_INCLUDES="-I$PYTHON_ROOT/include/python$PYTHON_VERSION"
   PYTHON_LDFLAGS="-L$PYTHON_ROOT/lib/python$PYTHON_VERSION/config"
   PYTHON_LIB="python$PYTHON_VERSION"
fi

_pyj_save_CPPFLAGS="$CPPFLAGS"
_pyj_save_LIBS="$LIBS"
_pyj_save_LDFLAGS="$LDFLAGS"
CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"

PYTHON_LIB_DEPS="$PTHREAD_ARG $DL_LOAD_LIB $PTHREAD_LIB"

if test "x$OS_TYPE" = "xUNIX" ; then
   PYTHON_LIB_DEPS="$PYTHON_LIB_DEPS -lm"
fi

LDFLAGS="$LDFLAGS $PYTHON_LDFLAGS"

DPP_LANG_SAVE
DPP_LANG_C

AC_CHECK_LIB([$PYTHON_LIB], [Py_Initialize],
   [AC_CHECK_HEADER([Python.h], ,
      [AC_MSG_ERROR([*** Python is required for PyJuggler ***])])],
   [AC_MSG_ERROR([*** Python is required for PyJuggler ***])],
   [$UTIL_LIB $PYTHON_LIB_DEPS])

DPP_LANG_RESTORE

# Convert the string given to AC_CHECK_LIB() as a library to something that
# the linker can use later on.
if test "x$OS_TYPE" = "xWin32" ; then
   PYTHON_LIB="$PYTHON_LIB.lib"
   PYTHON_LIB_OPT="$PYTHON_LIB.lib"
   PYTHON_LIB_DBG="${PYTHON_LIB_OPT}_d.lib"
else
   PYTHON_LIB="-l$PYTHON_LIB"
   PYTHON_LIB_OPT="$PYTHON_LIB"
   PYTHON_LIB_DBG="$PYTHON_LIB_OPT"
fi

boost_ver_exp=`echo $BOOST_VERSION | sed -e 's/\./ /g'`
boost_major_ver=`echo $boost_ver_exp | awk '{ print $1 }' -`
boost_minor_ver=`echo $boost_ver_exp | awk '{ print $2 }' -`

if test $boost_minor_ver -ge 31; then
   boost_debug_suffix='-d'
else
   boost_debug_suffix='_debug'
fi

if test "x$BOOST_HACKS" = "xyes" -a $boost_minor_ver -ge 31; then
   boost_ver_str=`echo $BOOST_VERSION | sed -e 's/\./_/g'`
   boost_extra_inc_dir="/boost-$boost_ver_str"

   case $CC in
      gcc)
         toolset='gcc'
         ;;
      cygcl|msvccc|cl*|CL*)
         toolset=${BOOST_TOOL:='vc7'}
         ;;
      cc)
         if test "x$GCC" = "xyes" ; then
            toolset='gcc'
         else
            case $PLATFORM in
               IRIX)
                  toolset='mp'
                  ;;
               Solaris)
                     toolset='sw'
               ;;
            esac
         fi
         ;;
   esac

   # We can only use multi-threading versions of Boost libraries if we are not
   # using SPROC threads.  Boost does not support SPROC.
   if test "x$VPR_SUBSYSTEM" != "xSPROC" ; then
      threading='-mt'
   fi

   BOOST_PYTHON_LIB="boost_python-$toolset$threading"
else
   BOOST_PYTHON_LIB="boost_python"
fi

CPPFLAGS="${_pyj_save_CPPFLAGS}"
LIBS="${_pyj_save_LIBS}"
LDFLAGS="${_pyj_save_LDFLAGS}"

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

if test "x$OS_TYPE" = "xWin32" ; then
   bpl_link_option="$BOOST_PYTHON_LIB.lib"
else
   bpl_link_option="-l$BOOST_PYTHON_LIB"
fi

_pyj_save_CPPFLAGS="$CPPFLAGS"
_pyj_save_LIBS="$LIBS"
_pyj_save_LDFLAGS="$LDFLAGS"

CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES -I$BOOST_PYTHON_DIR/include$boost_extra_inc_dir"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
   CPPFLAGS="$CPPFLAGS -I$BOOST_PYTHON_DIR/include$boost_extra_inc_dir/boost/compatibility/cpp_c_headers"
fi

LIBS="$LIBS $bpl_link_option $PYTHON_LIB $UTIL_LIB $PYTHON_LIB_DEPS"
LDFLAGS="$LDFLAGS -L$BOOST_PYTHON_DIR/lib $PYTHON_LDFLAGS"

AC_CACHE_CHECK([for boost::python::handle<> in $bpl_link_option],
   ac_cv_boost__python__handle_in_boost_python,
   [AC_TRY_LINK([
#include <boost/python.hpp>
using namespace boost::python;],
                [handle<> main_module(borrowed(PyImport_AddModule("__main__")));],
                [ac_cv_boost__python__handle_in_boost_python='yes'
                 rm -rf ./ii_files],
                [ac_cv_boost__python__handle_in_boost_python='no'])])

DPP_LANG_RESTORE

CPPFLAGS="${_pyj_save_CPPFLAGS}"
LIBS="${_pyj_save_LIBS}"
LDFLAGS="${_pyj_save_LDFLAGS}"

if test "x$ac_cv_boost__python__handle_in_boost_python" = "xyes" ; then
   BOOST_PYTHON_INCLUDES="-I$BOOST_PYTHON_DIR/include$boost_extra_inc_dir"

   if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
      BOOST_PYTHON_INCLUDES="$BOOST_PYTHON_INCLUDES -I$BOOST_PYTHON_DIR/include$boost_extra_inc_dir/boost/compatibility/cpp_c_headers"
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      bpl_dir="/libpath:$PYTHON_ROOT/libs /libpath:$BOOST_PYTHON_DIR/lib"
      BOOST_PYTHON_LDFLAGS_OPT="$bpl_dir $BOOST_PYTHON_LIB.lib"
      BOOST_PYTHON_LDFLAGS_DBG="$bpl_dir $BOOST_PYTHON_LIB$boost_debug_suffix.lib"
   else
      bpl_dir="-L$BOOST_PYTHON_DIR/lib"
      BOOST_PYTHON_LDFLAGS_OPT="$bpl_dir -l$BOOST_PYTHON_LIB"
      BOOST_PYTHON_LDFLAGS_DBG="$bpl_dir -l$BOOST_PYTHON_LIB$boost_debug_suffix"
   fi
else
   AC_MSG_ERROR([*** Boost.Python v2 is required for PyJuggler ***])
fi

VPR_PATH([0.27], , [AC_MSG_ERROR([*** VPR is required for PyJuggler ***])])
JCCL_PATH([0.12], ,
          [AC_MSG_ERROR([*** JCCL is required for PyJuggler ***])])
GADGETEER_PATH([0.13], ,
               [AC_MSG_ERROR([*** Gadgeteer is required for PyJuggler ***])])
VRJUGGLER_PATH([1.1.148], ,
               [AC_MSG_ERROR([*** VR Juggler is required for PyJuggler ***])])

pyjsave_CFLAGS="$CFLAGS"
pyjsave_CPPFLAGS="$CPPFLAGS"
pyjsave_LDFLAGS="$LDFLAGS"
pyjsave_LIBS="$LIBS"

# Ensure that /usr/include and /usr/lib are not included multiple times
# if $OSGROOT is "/usr".
if test "x$osg_base_dir" != "x/usr" ; then
   OSG_INCLUDES="-I$osg_base_dir/include"

   if test "x$OS_TYPE" = "xWin32" ; then
      OSG_LDFLAGS="/libpath:$osg_base_dir/lib"
      OSG_LIBS='osg.lib osgProducer.lib osgUtil.lib osgDB.lib'
   else
      OSG_LDFLAGS="-L$osg_base_dir/lib"
      OSG_LIBS='-losg -losgProducer -losgUtil -losgDB'
   fi
fi

AC_PATH_X

if test "x$x_libraries" != "xNONE" -a "x$x_libraries" != "x" ; then
   X_LDFLAGS="-L$x_libraries"
fi

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"
CPPFLAGS="$CPPFLAGS $OSG_INCLUDES $STDFLAGS"
LDFLAGS="$LDFLAGS $OSG_LDFLAGS"

AC_CHECK_LIB([osg], [osgGetVersion],
   [AC_CHECK_HEADER([osg/Vec3], [HAVE_OSG='Y'], [HAVE_OSG='N'])],
   [HAVE_OSG='N'], [$X_LDFLAGS -lX11 -lXext])

DPP_LANG_RESTORE

CFLAGS="$pyjsave_CFLAGS"
CPPFLAGS="$pyjsave_CPPFLAGS"
LDFLAGS="$pyjsave_LDFLAGS"
LIBS="$pyjsave_LIBS"

if test "x$HAVE_OSG" = "xY" ; then
   AC_DEFINE([PYJUGGLER_HAVE_OSG], 1,
   	     [Define if the Open Scene Graph is available.])
fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Finalize substitution settings.
# -----------------------------------------------------------------------------

# Compiler-specific settings when GCC is not being used.
if test "x$USE_GCC" != "xyes" ; then
   if test "x$PLATFORM" = "xIRIX" ; then
      EXTRA_LINK_FLAGS="$EXTRA_LINK_FLAGS -J4 -all"
   fi
fi

if test "x$OS_TYPE" = "xWin32" ; then
   PYJUTIL_LDFLAGS_OPT='/libpath:$(topdir)/lib pyjutil.lib'
   PYJUTIL_LDFLAGS_DBG='/libpath:$(topdir)/lib pyjutil_d.lib'
else
   PYJUTIL_LDFLAGS_OPT='-L$(topdir)/lib -lpyjutil'
   PYJUTIL_LDFLAGS_DBG="$PYJUTIL_LDFLAGS_OPT"
fi

DPP_PTHREAD_FINISH

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

DPP_SUBST

AC_SUBST(USE_OBJCOPY)
AC_SUBST(EXTRA_LINK_FLAGS)
AC_SUBST(PYJUTIL_LDFLAGS_DBG)
AC_SUBST(PYJUTIL_LDFLAGS_OPT)

AC_SUBST(HAVE_OSG)
AC_SUBST(OSG_INCLUDES)
AC_SUBST(OSG_LDFLAGS)
AC_SUBST(OSG_LIBS)

AC_SUBST(PYTHON_ROOT)
AC_SUBST(PYTHON_INCLUDES)
AC_SUBST(PYTHON_LIB_OPT)
AC_SUBST(PYTHON_LIB_DBG)
AC_SUBST(PYMOD_EXT)
AC_SUBST(UTIL_LIB)
AC_SUBST(PYTHON_LIB_DEPS)
AC_SUBST(PYTHON_LDFLAGS)
AC_SUBST(PYTHON_VERSION)

AC_SUBST(PYSTE)

AC_SUBST(BOOST_PYTHON_INCLUDES)
AC_SUBST(BOOST_PYTHON_LDFLAGS_OPT)
AC_SUBST(BOOST_PYTHON_LDFLAGS_DBG)

# -----------------------------------------------------------------------------
# Finish up.
# -----------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                 VARS.pl
                 pyjutil/Makefile
                 src/Makefile
                 src/common.mk
                 src/common-targets.mk
                 src/gadget/Makefile
                 src/jccl/Makefile
                 src/osgapp/Makefile
                 src/vpr/Makefile
                 src/vrj/Makefile
                 examples/Makefile
                 examples/cxx/Makefile
                 examples/cxx/PyAppLoader/Makefile
                 examples/cxx/PyExtApp/Makefile])

AC_OUTPUT
