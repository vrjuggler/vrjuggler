# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

AC_INIT([PyJuggler], [0.8], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling])
AC_REVISION([$Revision$])
AC_CONFIG_SRCDIR([src/gadgeteer-wrappers.h])
AC_CONFIG_AUX_DIR([Doozer++/scripts])
DPP_PREREQ([1.5.1])
DPP_INIT

DPP_GEN_RECONFIG

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

AC_ARG_WITH([python],
            [  --with-python=<PATH>    Path to Python root             [default=/usr/local]],
            [PYTHON_ROOT="$withval"], [PYTHON_ROOT='/usr/local'])

AC_ARG_WITH([bpldir],
            [  --with-bpldir=<PATH>    Directory containing a built
                          version of Boost.Python v2      [default=/usr/local/lib]],
            [BOOST_PYTHON_LIBDIR="$withval"], [BOOST_PYTHON_LIBDIR='/usr/local/lib'])

AC_ARG_WITH([pyste],
            [  --with-pytse=<PATH>     Full path to pyste.py script   [default=none]],
            [PYSTE="$withval"], [PYSTE='none'])

AC_ARG_WITH([osg],
            [ --with-osg=<PATH>        Open Scene Graph installation  [default=$OSGHOME then /usr/local]],
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
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the PyJuggler source tree.  To get a value
# for $PYJROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
cd "$srcdir"
PYJROOT_ABS=`pwd`
cd "$PYJROOT_ABS/../.."
UNIX_PYJUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   PYJUGGLERROOT_ABS=`cygpath -w "$UNIX_PYJUGGLERROOT_ABS" | sed -e 's/\\\\/\\//g'`
else
   PYJUGGLERROOT_ABS="$UNIX_PYJUGGLERROOT_ABS"
fi

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
_vpr_save_LIBS="$LIBS"
AC_CHECK_FUNC([dlopen], [DL_LOAD_LIB=''],
    [AC_CHECK_LIB([dl], [dlopen], [DL_LOAD_LIB='-ldl'])])
LIBS="${_vpr_save_LIBS}"

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

if test "x$OS_TYPE" = "xUNIX" ; then
   PYTHON_LIB="-l$PYTHON_LIB"
fi

CPPFLAGS="${_pyj_save_CPPFLAGS}"
LIBS="${_pyj_save_LIBS}"
LDFLAGS="${_pyj_save_LDFLAGS}"

BOOST_PYTHON_LIB="boost_python.$DYNAMICLIB_EXT"

if test "x$OS_TYPE" = "xUNIX" ; then
   BOOST_PYTHON_LIB="lib$BOOST_PYTHON_LIB"
fi

# XXX: This is pathetic.
AC_CHECK_FILE([$BOOST_PYTHON_LIBDIR/$BOOST_PYTHON_LIB], ,
              [AC_MSG_ERROR([*** Boost.Python v2 is required for PyJuggler ***])])

if test "x$OS_TYPE" = "xWin32" ; then
   BOOST_PYTHON_LDFLAGS="/libpath:$BOOST_PYTHON_LIBDIR boost_python.lib"

   # XXX: This is a little strange.  I cannot get the modules to link 
   BOOST_PYTHON_LDFLAGS="/libpath:$PYTHON_ROOT/libs $BOOST_PYTHON_LDFLAGS"
else
   BOOST_PYTHON_LDFLAGS="-L$BOOST_PYTHON_LIBDIR -lboost_python"
fi

VPR_PATH([0.27], , [AC_MSG_ERROR([*** VPR is required for PyJuggler ***])])
JCCL_PATH([0.12], ,
          [AC_MSG_ERROR([*** JCCL is required for PyJuggler ***])])
GADGETEER_PATH([0.13], ,
               [AC_MSG_ERROR([*** Gadgeteer is required for PyJuggler ***])])
VRJUGGLER_PATH([1.1.144], ,
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

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

CFLAGS="$CFLAGS ${_EXTRA_FLAGS}"
CPPFLAGS="$CPPFLAGS $OSG_INCLUDES $STDFLAGS"
LDFLAGS="$LDFLAGS $OSG_LDFLAGS"

AC_CHECK_LIB([osg], [osgGetVersion],
   [AC_CHECK_HEADER([osg/Vec3], [HAVE_OSG='Y'], [HAVE_OSG='N'])],
   [HAVE_OSG='N'], [-lX11 -lXext])

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
   PYJUTIL_LDFLAGS='/libpath:$(topdir)/lib pyjutil.lib'
else
   PYJUTIL_LDFLAGS='-L$(topdir)/lib -lpyjutil'
fi

DPP_PTHREAD_FINISH

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

DPP_SUBST

AC_SUBST(EXTRA_LINK_FLAGS)
AC_SUBST(PYJUTIL_LDFLAGS)

AC_SUBST(HAVE_OSG)
AC_SUBST(OSG_INCLUDES)
AC_SUBST(OSG_LDFLAGS)
AC_SUBST(OSG_LIBS)

AC_SUBST(PYTHON_ROOT)
AC_SUBST(PYTHON_INCLUDES)
AC_SUBST(PYTHON_LIB)
AC_SUBST(PYMOD_EXT)
AC_SUBST(UTIL_LIB)
AC_SUBST(PYTHON_LIB_DEPS)
AC_SUBST(PYTHON_LDFLAGS)
AC_SUBST(PYTHON_VERSION)

AC_SUBST(PYSTE)

AC_SUBST(BOOST_PYTHON_LDFLAGS)

# -----------------------------------------------------------------------------
# Finish up.
# -----------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                 VARS.pl
                 pyjutil/Makefile
                 src/Makefile
                 examples/Makefile
                 examples/cxx/Makefile
                 examples/cxx/PyAppLoader/Makefile
                 examples/cxx/PyExtApp/Makefile])

AC_OUTPUT
