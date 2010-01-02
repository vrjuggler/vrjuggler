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
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# ************** <auto-copyright.pl END do not edit this line> ****************

# -----------------------------------------------------------------------------
# Base configure.in for Tweek.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([Tweek], [1.3], [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([tweek/tweekDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2010 by Iowa State University])
AC_CONFIG_SRCDIR([tweek/tweekConfig.h])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.1.2])
DPP_INIT

DPP_GEN_RECONFIG

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

DPP_STD_CPP(yes)

#AC_ARG_ENABLE(jdk-orb,
#              [  --enable-jdk-orb        Use Java IDL (requires JDK 1.4) [default=no]],
#              _enable_jdk_orb="$enable_val", _enable_jdk_orb='no')

AC_ARG_ENABLE(extensions,
              [  --enable-extensions     Build in Tweek extensions       [default=yes]],
              use_tweek_extensions="$enableval", use_tweek_extensions='yes')

# Enable versioning by default
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Override the C compiler that configure would otherwise find itself.
AC_ARG_WITH(cc, [  --with-cc=<PATH>        Alternate C compiler ],
            _alt_cc="$withval", _alt_cc='none')

# Override the C++ compiler that configure would otherwise find itself.
AC_ARG_WITH(cxx, [  --with-cxx=<PATH>       Alternate C++ compiler ],
            _alt_cxx="$withval", _alt_cxx='none')

AC_ARG_WITH(cxx-orb,
            [  --with-cxx-orb=<NAME>   Name of C++ ORB to use          [default=omniORB4]
                          Allowed values are:
                            omniORB4, TAO],
            cxx_orb_name="$withval", cxx_orb_name='omniORB4')

AC_ARG_WITH(cxx-orb-root,
            [  --with-cxx-orb-root=<PATH>                              [default=/usr]
                          Root directory of C++ ORB
                          installation],
            cxx_orb_root="$withval", cxx_orb_root='/usr')

AC_ARG_WITH(cxx-orb-bin,
            [  --with-cxx-orb-bin=<PATH>                               [default=/usr/bin]
                          Path to C++ ORB executables],
            cxx_orb_bin_dir="$withval", cxx_orb_bin_dir='none')

AC_ARG_WITH(cxx-orb-lib,
            [  --with-cxx-orb-lib=<PATH>                               [default=/usr/lib]
                          Path to C++ ORB libraries],
            cxx_orb_lib_dir="$withval", cxx_orb_lib_dir='none')

AC_ARG_WITH(python,
            [  --with-python=<PATH>    Path to Python installation     [No default]],
            python_root="$withval", python_root='none')

AC_ARG_WITH(python-orb,
            [  --with-python-orb=<NAME> Name of Python ORB to use      [default=omniORBpy]
                          Allowed values are:
                            omniORBpy],
            python_orb_name="$withval", python_orb_name='omniORBpy')

AC_ARG_WITH(python-orb-root,
            [  --with-python-orb-root=<PATH>                           [default=/usr]
                          Root directory of Python ORB
                          installation],
            python_orb_root="$withval", python_orb_root='/usr')

AC_ARG_WITH(python-orb-bin,
            [  --with-python-orb-bin=<PATH>                            [default=/usr/bin]
                          Path to Python ORB executables],
            python_orb_bin_dir="$withval", python_orb_bin_dir='none')

AC_ARG_WITH(python-orb-lib,
            [  --with-python-orb-lib=<PATH>                            [default=/usr/lib/pythonX.Y/site-packages]
                          Path to Python ORB package],
            python_orb_lib_dir="$withval", python_orb_lib_dir='none')

AC_ARG_WITH(java-orb,
            [  --with-java-orb=<NAME>  Name of Java ORB to use         [default=JDK]
                          Allowed values are:
                            JDK, OpenORB, JacORB],
            java_orb_name="$withval", java_orb_name='JDK')

AC_ARG_WITH(java-orb-jar,
            [  --with-java-orb-jar=<PATH>
                          Path to Java ORB JAR file(s)],
            JAVA_ORB_JAR="$withval", JAVA_ORB_JAR='none')

AC_ARG_WITH(idl-java,
            [  --with-idl-java=<PATH>  Path to Java IDL compiler ],
            _idl_java="$withval", _idl_java='no')

AC_ARG_WITH(junit, [  --with-junit=<PATH>     Path to JUnit JAR file(s) ],
            JUNIT_JAR="$withval", JUNIT_JAR='none')

DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------

# Define macros and helpers useful when working in a Win32/DOS-based
# environment.
DPP_WIN32_SETUP
DPP_SYSTEM_SETUP

# Templates for Autoheader.
AH_TEMPLATE([__OSVERSION__],
            [Define to the major number of the target operating system.])
AH_TEMPLATE([__alpha__], [Define to 1 if building on the Alpha architecture.])
AH_TEMPLATE([__hppa__], [Define to 1 if building on HP PA-RISC architecture.])
AH_TEMPLATE([__mips__], [Define to 1 if building on MIPS architecture.])
AH_TEMPLATE([__powerpc__], [Define to 1 if building on PowerPC architecture.])
AH_TEMPLATE([__sparc__], [Define to 1 if building on the SPARC architecture.])
AH_TEMPLATE([__x86__], [Define to 1 if building on Intel x86 architecture.])
AH_TEMPLATE([__ia64__],
            [Define to 1 if building on Intel Itanium architecture.])

# Depending on the target operating system, set various command options and
# such.
case $target_os in
   # IBM RS/6000 running AIX.
   aix4*)
      AC_DEFINE(__aix__, 1, [Define to 1 if building on AIX.])
      AC_DEFINE(__powerpc__)
      AC_DEFINE(__OSVERSION__, 4)
      EXTRA_DEFS="$EXTRA_DEFS -D__aix__ -D__powerpc__ -D__OSVERSION__=4"
      ;;
   # i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
   cygwin)
      AC_DEFINE(__WIN32__, 1, [Define to 1 if building on a Win32 platform.])
      AC_DEFINE(__x86__)
      EXTRA_DEFS="$EXTRA_DEFS -D__WIN32__ -D__x86__"

      if test "x$PLATFORM" = "xWinNT" ; then
         AC_DEFINE(__NT__, 1, [Define only on Windows NT/2000/XP.])
         EXTRA_DEFS="$EXTRA_DEFS -D__NT__"

         _sys_name=`uname -s`
         case "${_sys_name}" in
            *NT-4*)
               AC_DEFINE(__OSVERSION__, 4)
               EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=4"
               ;;
            *NT-5*)
               AC_DEFINE(__OSVERSION__, 5)
               EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=5"
               ;;
         esac
      fi
      ;;
   # Darwin (aka MacOS X).
   darwin*)
      AC_DEFINE(__darwin__, 1, [Define to 1 if building on Darwin/Mac OS X.])
      EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=1 -D__darwin__"

      case $target_cpu in
         powerpc)
            AC_DEFINE(__powerpc__)
            EXTRA_DEFS="$EXTRA_DEFS -D__powerpc__"
            ;;
         *86)
            AC_DEFINE(__x86__)
            EXTRA_DEFS="$EXTRA_DEFS -D__x86__"
            ;;
      esac
      ;;
   # A machine running FreeBSD or DragonFlyBSD.
   freebsd*|dragonfly*)
      AC_DEFINE(__freebsd__, 1,
                [Define to 1 if building on FreeBSD or DragonFly.])
      EXTRA_DEFS="$EXTRA_DEFS -D__freebsd__"

      case $target_cpu in
         *86)
            AC_DEFINE(__x86__)
            EXTRA_DEFS="$EXTRA_DEFS -D__x86__"
            ;;
         ia64)
            AC_DEFINE(__ia64__)
            EXTRA_DEFS="$EXTRA_DEFS -D__ia64__"
            ;;
         alpha)
            AC_DEFINE(__alpha__)
            EXTRA_DEFS="$EXTRA_DEFS -D__alpha__"
            ;;
         sparc)
            AC_DEFINE(__sparc__)
            EXTRA_DEFS="$EXTRA_DEFS -D__sparc__"
            ;;
      esac

      changequote(<<, >>)
      # This will match $target_os of the form freebsdX.Y... or dragonflyX.Y...
      tweek_bsd_ver=`echo $target_os | sed -e 's/^.*[dy]\([1-9][0-9]*\)\..*/\1/'`
      changequote([, ])
      AC_DEFINE_UNQUOTED(__OSVERSION__, $tweek_bsd_ver)
      EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=$tweek_bsd_ver"

      ;;
   # HP PA-RISC machine running HP-UX.20.
   hpux*)
      AC_DEFINE(__hpux__, 1, [Define to 1 if building on HP-UX.])
      AC_DEFINE(__hppa__)
      EXTRA_DEFS="$EXTRA_DEFS -D__hpux__ -D__hppa__"

      case $target_os in
         # HP-UX 10.20.
         hpux10.20)
            AC_DEFINE(__OSVERSION__, 10)
            EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=10"
         ;;
         # HP-UX 11.
         hpux11*)
            AC_DEFINE(__OSVERSION__, 11)
            EXTRA_DEFS="$EXTRA_DEFS -D__OSVERSION__=11"
         ;;
      esac

      ;;
   # SGI running IRIX 6.*.
   irix6*)
      AC_DEFINE(__irix__, 1, [Define to 1 if building on IRIX.])
      AC_DEFINE(__mips__)
      AC_DEFINE(__OSVERSION__, 6)
      EXTRA_DEFS="$EXTRA_DEFS -D__irix__ -D__mips__ -D__OSVERSION__=6"
      ;;
   # A machine running Linux.
   linux*)
      AC_DEFINE(__linux__, 1, [Define to 1 if building on Linux.])
      AC_DEFINE(__OSVERSION__, 2)
      EXTRA_DEFS="$EXTRA_DEFS -D__linux__ -D__OSVERSION__=2"

      case $target_cpu in
         *86)
            AC_DEFINE(__x86__)
            EXTRA_DEFS="$EXTRA_DEFS -D__x86__"
            ;;
         ia64)
            AC_DEFINE(__ia64__)
            EXTRA_DEFS="$EXTRA_DEFS -D__ia64__"
            ;;
         alpha)
            AC_DEFINE(__alpha__)
            EXTRA_DEFS="$EXTRA_DEFS -D__alpha__"
            ;;
         sparc)
            AC_DEFINE(__sparc__)
            EXTRA_DEFS="$EXTRA_DEFS -D__sparc__"
            ;;
         mips)
            AC_DEFINE(__mips__)
            EXTRA_DEFS="$EXTRA_DEFS -D__mips__"
            ;;
      esac
      ;;
   # DEC Alpha running Digital UNIX 4.x.
   osf4*)
      AC_DEFINE(__osf1__, 1,
                [Define to 1 if building on OSF1/Digital UNIX/TRU64.])
      AC_DEFINE(__alpha__)
      AC_DEFINE(__OSVERSION__, 4)
      EXTRA_DEFS="$EXTRA_DEFS -D__osf1__ -D__alpha__ -D__OSVERSION__=4"
      ;;
   # A machine running Solaris 2.x (aka SunOS 5.x).
   solaris2*)
      AC_DEFINE(__sunos__, 1, [Define if building on SunOS/Solaris.])
      AC_DEFINE(__OSVERSION__, 5)
      EXTRA_DEFS="$EXTRA_DEFS -D__sunos__ -D__OSVERSION__=5"

      case $target_cpu in
         *86)
            AC_DEFINE(__x86__)
            EXTRA_DEFS="$EXTRA_DEFS -D__x86__"
            ;;
         sparc)
            AC_DEFINE(__sparc__)
            EXTRA_DEFS="$EXTRA_DEFS -D__sparc__"
            ;;
      esac
      ;;
esac

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the juggler source tree.  To get a value
# for $TWEEKROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
cd "$srcdir"
UNIX_TWEEKROOT_ABS=`pwd`

cd "$UNIX_TWEEKROOT_ABS/../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   TWEEKROOT_ABS=`dospath -p "$UNIX_TWEEKROOT_ABS"`
   JUGGLERROOT_ABS=`dospath -p "$UNIX_JUGGLERROOT_ABS"`
else
   TWEEKROOT_ABS="$UNIX_TWEEKROOT_ABS"
   JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"
fi

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP
VJ_PROG_CC_PROF_FLAG(yes)
VJ_PROG_CXX_PROF_FLAG(yes)
DPP_CC_PTHREAD_ARG
DPP_CC_PTHREADS_ARG
DPP_GET_PTHREAD_LIB

if test "x$DPP_USING_MSVCCC" = "xyes" ; then
   AR='$(CXX) /link /lib'
   LD='$(CXX) /link /dll'
fi

# Ensure that the C++ compiler we've found is capable of compiling the newer
# newer C++ features that we need.
DPP_CXX_NAMESPACE(AC_MSG_ERROR(*** Tweek requires C++ namesapce support ***))
DPP_CXX_EXCEPTIONS(AC_MSG_ERROR(*** Tweek requires C++ exception support ***))
DPP_CXX_HAVE_STD

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER([5.004], , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree], [\$(PERL) \$(scriptdir)/mtree.pl])
DPP_HAVE_GNU_MAKE([3.78], ,
   [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
VJ_LINKER_SETUP

BUILD_JAVA='Y'

# Check for the JDK.
DPP_PATH_JAVA( , [tweek_have_java='Y'],
              [tweek_have_java='N'
               BUILD_JAVA='N'])

if test "x$tweek_have_java" = "xY" ; then
   DPP_JDK_VERSION([$JDK_HOME], [1.4], ,
      [ BUILD_JAVA='N'
        AC_MSG_WARN([*** JDK 1.4 or newer required for Tweek Java API ***])
      ])

   DPP_CHECK_JNI([$JDK_HOME], [BUILD_JNI='Y'],
      [BUILD_JNI='N'
       AC_MSG_WARN([*** NOTE: Not building JNI extensions to Tweek Java API ***])
      ])

   # If we are building the Tweek Java code, we should try to enable building
   # the Java test suite too.
   if test "x$BUILD_JAVA" = "xY" ; then
      DPP_CHECK_JAVA_STATIC_METHOD(junit.framework.Assert, assertTrue(true), ,
         BUILD_JAVA_TEST_SUITE='Y',
         [ AC_MSG_WARN([*** JUnit required for Tweek Java test suite ***])
           BUILD_JAVA_TEST_SUITE='N' ],
         $JUNIT_JAR)
   fi
fi

ANT="JAVA_HOME=\"$JDK_HOME\" \"$UNIX_JUGGLERROOT_ABS/external/ant/bin/ant\" --noconfig"

if test "x$python_root" != "xnone" ; then
   python_bin_dir="$python_root/bin"

   if test "x$OS_TYPE" = "xWin32" ; then
      python_bin_dir_unix=`unixpath "$python_bin_dir"`
      python_search_path="${python_bin_dir_unix}:${PATH}"
   else
      python_search_path="${python_bin_dir}:${PATH}"
   fi
else
   python_search_path="$PATH"
fi

AC_PATH_PROG(PYTHON, python, no, [$python_search_path])
AC_PATH_PROG(PYUIC, pyuic, no)

if test "x$PYTHON" = "xno" ; then
   AC_MSG_WARN([*** Python is required to use the Python GUI ***])
   BUILD_PYTHON_IDL='N'
   BUILD_PYTHON_GUI='N'
elif test "x$PYUIC" = "xno" ; then
   AC_MSG_WARN([*** pyuic is required to build Python GUI ***])
   BUILD_PYTHON_IDL='Y'
   BUILD_PYTHON_GUI='N'
else
   BUILD_PYTHON_IDL='Y'
   BUILD_PYTHON_GUI='Y'
fi

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
MIN_VPR_VERSION='2.1.5'
VPR_PATH([$MIN_VPR_VERSION], [BUILD_CXX='Y'],
         [AC_MSG_WARN([*** VPR required for Tweek C++ API ***])
          BUILD_CXX='N'])

AH_TEMPLATE([TWEEK_INCLUDE_CORBA_H],
   [Define to include the main CORBA header file -- for example: <omniORB4/CORBA.h>])
AH_TEMPLATE([TWEEK_ORB_VER_STRING],
   [Define to an ORB identifier string used to initialized the ORB -- for example: "omniORB4"])

# Find omniORB.  This one is a beast.
setup_omniORB_cxx ( )
{
   OMNI_VER="$1"

   if test "x$cxx_orb_bin_dir" = "xnone" ; then
      OMNI_BIN_DIR="$cxx_orb_root/bin"
   else
      OMNI_BIN_DIR="$cxx_orb_bin_dir"
   fi

   if test "x$cxx_orb_lib_dir" = "xnone" ; then
      OMNI_LIB_DIR="$cxx_orb_root/lib"
   else
      OMNI_LIB_DIR="$cxx_orb_lib_dir"
   fi

   tweek_omni_lib="omniORB$OMNI_VER"

   tweek_save_CPPFLAGS="$CPPFLAGS"

   if test "x$cxx_orb_root" != "x/usr" ; then
      CPPFLAGS="$CPPFLAGS -I$cxx_orb_root/include"
   fi

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CHECK_HEADER(omniORB$OMNI_VER/CORBA.h, ,
      [AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
       BUILD_CXX='N'])

   DPP_LANG_RESTORE

   CPPFLAGS="$tweek_save_CPPFLAGS"

   if test "x$BUILD_CXX" != "xN" ; then
      tweek_save_LDFLAGS="$LDFLAGS"
      tweek_save_LIBS="$LIBS"

      if test "x$OMNI_LIB_DIR" != "x/usr/lib" ; then
         LDFLAGS="$LDFLAGS -L$OMNI_LIB_DIR"
      fi

      if test "x$OS_TYPE" = "xWin32" ; then
         # XXX: This is utterly pathetic.  What I really want is a way to get
         # the full omniORB version number and use it to figure out these file
         # names.
         orb_rt_file=`ls $OMNI_LIB_DIR/omniORB*_rt.lib`
         thread_rt_file=`ls $OMNI_LIB_DIR/omnithread*_rt.lib`
         dynamic_rt_file=`ls $OMNI_LIB_DIR/omniDynamic*_rt.lib`

         # XXX: More hackery.  This strips off the path to the .lib files
         # along with the .lib extension.  This is needed so that our wrapper
         # around MSVC++ can translate the -l<libname> argument correctly.
         orb_rt=`basename $orb_rt_file | sed -e 's/.lib$//'`
         thread_rt=`basename $thread_rt_file | sed -e 's/.lib$//'`
         dynamic_rt=`basename $dynamic_rt_file | sed -e 's/.lib$//'`

         omni_win32_libs="-l$orb_rt -l$thread_rt -l$dynamic_rt -lws2_32 -ladvapi32"
         changequote(<<, >>)
         omni_win32_link_args=`echo $omni_win32_libs | sed -e 's/-l\([^ ][^ ]*\)/\1.lib/g'`
         changequote([, ])
         LIBS="$LIBS $omni_win32_libs"
      else
         LIBS="$LIBS -l$tweek_omni_lib -lomnithread"
      fi

      # This is done to deal with omniORB 4.0.0 not compiling out of the box
      # on IRIX.
      if test "x$OMNI_VER" = "x4" -a "x$PLATFORM" = "xIRIX" ; then
         DPP_LANG_SAVE
         DPP_LANG_CPLUSPLUS

         tweek_save_CPPFLAGS="$CPPFLAGS"
         CPPFLAGS="$CPPFLAGS -I$cxx_orb_root/include $EXTRA_DEFS"

         AC_CACHE_CHECK([whether OMNI_REQUIRES_FQ_BASE_CTOR is defined],
            [tweek_cv_IRIX_ctor_defined],
            [AC_TRY_COMPILE([#include <omniORB$OMNI_VER/CORBA.h>],
               [
#ifndef OMNI_REQUIRES_FQ_BASE_CTOR
   crash
#endif
],
               [tweek_cv_IRIX_ctor_defined='yes'],
               [tweek_cv_IRIX_ctor_defined='no'])])

         CPPFLAGS="$tweek_save_CPPFLAGS"

         DPP_LANG_RESTORE

         # If OMNI_REQUIRES_FQ_BASE_CTOR is not defined, then we have to add
         # it manually.
         if test "x$tweek_cv_IRIX_ctor_defined" = "xno" ; then
            EXTRA_DEFS="$EXTRA_DEFS -DOMNI_REQUIRES_FQ_BASE_CTOR"
         fi
      fi

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      tweek_save_CPPFLAGS="$CPPFLAGS"
      tweek_save_CFLAGS="$CFLAGS"

      CPPFLAGS="$CPPFLAGS -I$cxx_orb_root/include $EXTRA_DEFS"
      CFLAGS="$CFLAGS $ABI_FLAGS"
      LDFLAGS="$LDFLAGS $PTHREAD_ARG $ABI_FLAGS"

      AC_CACHE_CHECK([for CORBA::ORB_init in -l$tweek_omni_lib],
         [tweek_cv_CORBA__ORB_init_in_omniORB],
         [AC_TRY_LINK([
#include <omniORB$OMNI_VER/CORBA.h>

int omni_main_i(int argc, char** const argv)
{
   CORBA::ORB_init(argc, argv, "omniORB$OMNI_VER");
   return 0;
}],
                     [omni_main_i(0, NULL);],
                     [ tweek_cv_CORBA__ORB_init_in_omniORB='yes'
                       rm -rf ./ii_files ],
                     [tweek_cv_CORBA__ORB_init_in_omniORB='no'])])

      # We're done testing things now, so we can restore the original flags.
      CFLAGS="$tweek_save_CFLAGS"
      CPPFLAGS="$tweek_save_CPPFLAGS"
      LDFLAGS="$tweek_save_LDFLAGS"
      LIBS="$tweek_save_LIBS"

      DPP_LANG_RESTORE

      if test "x$OS_TYPE" = "xWin32" ; then
         CXX_ORB_BIN_DIR=`unixpath "$OMNI_BIN_DIR"`
         CXX_ORB_LIB_DIR=`unixpath "$OMNI_LIB_DIR"`
         omniidl_search_path="${CXX_ORB_BIN_DIR}:${PATH}"
      else
         CXX_ORB_BIN_DIR="$OMNI_BIN_DIR"
         CXX_ORB_LIB_DIR="$OMNI_LIB_DIR"
         omniidl_search_path="${OMNI_BIN_DIR}:${PATH}"
      fi

      AC_PATH_PROG(CXX_IDL, omniidl, no, [$omniidl_search_path])

      if test "x$CXX_IDL" = "xno" ; then
         AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
         BUILD_CXX='N'
      else
         CXX_IDL_CMD="$CXX_IDL"
         CXX_IDL_OPTS='-bcxx -Wbh=.h,s=.cpp'
         CXX_IDL_GENDIR_OPT='-C'
         CXX_IDL_INCFLAG='-I'

         IDL_SERVER_EXT=''
         IDL_SERVER_TMPL_EXT=''
         IDL_CLIENT_EXT=''

         if test "x$tweek_cv_CORBA__ORB_init_in_omniORB" = "xyes" ; then
            BUILD_CXX='Y'

            CXX_ORB_DEPS="omniORB$OMNI_VER omnithread3 omniDynamic$OMNI_VER"

            if test "x$cxx_orb_root" != "x/usr" ; then
               CXX_ORB_INCLUDES="-I$cxx_orb_root/include"
            fi

            CXX_ORB_LIB="-l$tweek_omni_lib -lomnithread -lomniDynamic$OMNI_VER"
            CXX_ORB_LDFLAGS="-L$OMNI_LIB_DIR"

            CXX_ORB_LIB_MSVCCC="$omni_win32_libs"
            CXX_ORB_LDFLAGS_MSVCCC="-L$OMNI_LIB_DIR"

            CXX_ORB_LIB_LINK_EXE="$omni_win32_link_args"
            CXX_ORB_LDFLAGS_LINK_EXE="/libpath:$OMNI_LIB_DIR"

            AC_DEFINE_UNQUOTED([TWEEK_USE_OMNIORB], ,
               [Define if using omniORB as the C++ CORBA implementation.])
            AC_DEFINE_UNQUOTED(OMNIORB_VER, $OMNI_VER,
               [If using omniORB, define to the major version number (3, 4, etc.).])
            AC_DEFINE_UNQUOTED([TWEEK_INCLUDE_CORBA_H],
               [<omniORB$OMNI_VER/CORBA.h>])
            AC_DEFINE_UNQUOTED([TWEEK_ORB_VER_STRING], "omniORB$OMNI_VER")
         else
            AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
            BUILD_CXX='N'
         fi
      fi
   fi
}

# Find TAO.
setup_TAO ( )
{
   # Handle the case when $TAO_ROOT is set.  ($ACE_ROOT will probably be
   # set, too.)  This is a little tricky because $TAO_ROOT could refer to a
   # TAO installation or to the ACE+TAO source/build tree.
   if test "x$TAO_ROOT" != "x" -a "x$TAO_ROOT" != "x$cxx_orb_root" ; then
      cxx_orb_root="$TAO_ROOT"
      TAO_BIN_DIR="$cxx_orb_root/bin"

      if test -d "$TAO_ROOT/include" ; then
         tao_includes="-I$TAO_ROOT/include"
      else
         tao_includes="-I$TAO_ROOT -I$TAO_ROOT/orbsvcs"
      fi

      if test "x$ACE_ROOT" != "x" -a "x$TAO_ROOT" != "x$ACE_ROOT" ; then
         if test -d "$ACE_ROOT/include/ace" ; then
            tao_includes="-I$ACE_ROOT/include"
         else
            tao_extra_includes="-I$ACE_ROOT"
         fi
      fi

      if test "x$ACE_ROOT" != "x" ; then
         TAO_BIN_DIR="$ACE_ROOT/bin"
         TAO_LIB_DIR="$ACE_ROOT/lib"
      else
         TAO_BIN_DIR="$TAO_ROOT/bin"
         TAO_LIB_DIR="$TAO_ROOT/lib"
      fi
   else
      if test "x$cxx_orb_bin_dir" = "xnone" ; then
         TAO_BIN_DIR="$cxx_orb_root/bin"
      else
         TAO_BIN_DIR="$cxx_orb_bin_dir"
      fi

      if test "x$cxx_orb_lib_dir" = "xnone" ; then
         TAO_LIB_DIR="$cxx_orb_root/lib"
      else
         TAO_LIB_DIR="$cxx_orb_lib_dir"
      fi

      tao_includes="-I$TAO_ROOT/include"
   fi

   tweek_save_CPPFLAGS="$CPPFLAGS"
   CPPFLAGS="$CPPFLAGS $tao_extra_includes"

   if test "x$cxx_orb_root" != "x/usr" ; then
      CPPFLAGS="$CPPFLAGS $tao_includes"
   fi

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CHECK_HEADER(tao/corba.h, ,
      [AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
       BUILD_CXX='N'])

   DPP_LANG_RESTORE

   CPPFLAGS="$tweek_save_CPPFLAGS"

   if test "x$BUILD_CXX" != "xN" ; then
      tweek_save_LDFLAGS="$LDFLAGS"
      tweek_save_LIBS="$LIBS"

      if test "x$TAO_LIB_DIR" != "x/usr/lib" ; then
         LDFLAGS="$LDFLAGS -L$TAO_LIB_DIR"
      fi

      LIBS="$LIBS -lTAO -lACE"

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      tweek_save_CPPFLAGS="$CPPFLAGS"
      tweek_save_CFLAGS="$CFLAGS"

      CPPFLAGS="$CPPFLAGS $tao_extra_includes $tao_includes $EXTRA_DEFS"
      CFLAGS="$CFLAGS $ABI_FLAGS"
      LDFLAGS="$LDFLAGS $PTHREAD_ARG $ABI_FLAGS"

      AC_CACHE_CHECK([for CORBA::ORB_init in -lTAO],
         [tweek_cv_CORBA__ORB_init_in_TAO],
         [AC_TRY_LINK([
#include <tao/corba.h>

int tao_main_i(int argc, char** const argv)
{
   CORBA::ORB_init(argc, argv, "TAO");
   return 0;
}],
                     [omni_main_i(0, NULL);],
                     [ tweek_cv_CORBA__ORB_init_in_TAO='yes'
                       rm -rf ./ii_files ],
                     [tweek_cv_CORBA__ORB_init_in_TAO='no'])])

      # We're done testing things now, so we can restore the original flags.
      CFLAGS="$tweek_save_CFLAGS"
      CPPFLAGS="$tweek_save_CPPFLAGS"
      LDFLAGS="$tweek_save_LDFLAGS"
      LIBS="$tweek_save_LIBS"

      DPP_LANG_RESTORE

      CXX_ORB_BIN_DIR="$TAO_BIN_DIR"
      CXX_ORB_LIB_DIR="$TAO_LIB_DIR"
      tao_idl_search_path="${TAO_BIN_DIR}:${PATH}"

      AC_PATH_PROG(CXX_IDL, tao_idl, no, [$tao_idl_search_path])

      if test "x$CXX_IDL" = "xno" ; then
         AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
         BUILD_CXX='N'
      else
         CXX_IDL_CMD='tao_idl'
         CXX_IDL_OPTS='-hs .h -ss .cpp'
         CXX_IDL_GENDIR_OPT='-o '
         CXX_IDL_INCFLAG='-I'

         IDL_SERVER_EXT=''
         IDL_SERVER_TMPL_EXT='S_T'
         IDL_CLIENT_EXT='C'

         if test "x$tweek_cv_CORBA__ORB_init_in_TAO" = "xyes" ; then
            BUILD_CXX='Y'

            if test "x$cxx_orb_root" != "x/usr" ; then
               CXX_ORB_INCLUDES="$tao_extra_includes $tao_includes"
            fi

            CXX_ORB_LIB="-lTAO -lTAO_PortableServer -lTAO_CosNaming -lACE"
            CXX_ORB_LDFLAGS="-L$TAO_LIB_DIR"

            AC_DEFINE_UNQUOTED([TWEEK_USE_TAO], ,
               [Define if using TAO as the C++ CORBA implementation.])
            AC_DEFINE_UNQUOTED([TWEEK_INCLUDE_CORBA_H], [<tao/corba.h>])
            AC_DEFINE_UNQUOTED([TWEEK_ORB_VER_STRING], "TAO")
         else
            AC_MSG_WARN([*** $cxx_orb_name required for Tweek C++ API ***])
            BUILD_CXX='N'
         fi
      fi
   fi
}

# Find omniPython.
setup_omniORB_python ( )
{
#   OMNI_PY_VER="$1"

   if test "x$python_orb_bin_dir" = "xnone" ; then
      OMNI_PY_BIN_DIR="$python_orb_root/bin"
   else
      OMNI_PY_BIN_DIR="$python_orb_bin_dir"
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      OMNI_PY_BIN_DIR_UNIX=`unixpath "$OMNI_PY_BIN_DIR"`
      omniidl_search_path="${OMNI_PY_BIN_DIR_UNIX}:${PATH}"
   else
      omniidl_search_path="${OMNI_PY_BIN_DIR}:${PATH}"
   fi

   AC_PATH_PROG(PYTHON_IDL, omniidl, no, [$omniidl_search_path])

   if test "x$PYTHON_IDL" = "xno" ; then
      AC_MSG_WARN([*** $python_orb_name required for Tweek Python API ***])
      BUILD_PYTHON_IDL='N'
      BUILD_PYTHON_GUI='N'
   else
      PYTHON_ORB_MODULE='omniORB'
      PYTHON_IDL_OPTS='-bpython'
      PYTHON_IDL_GENDIR_OPT='-C'
      PYTHON_IDL_INCFLAG='-I'

      AC_MSG_CHECKING([for omniidl Python backend])
      if ! (eval $PYTHON_IDL $PYTHON_IDL_OPTS -u >/dev/null 2>&1) 2>&1 ; then
         AC_MSG_RESULT([no])
         AC_MSG_WARN([*** $python_orb_name required for Tweek Python API ***])
         BUILD_PYTHON_IDL='N'
         BUILD_PYTHON_GUI='N'
      else
         AC_MSG_RESULT([yes])
         BUILD_PYTHON_IDL='Y'
      fi
   fi
}

setup_fnorb ( )
{
   if test "x$python_orb_bin_dir" = "xnone" ; then
      FNORB_BIN_DIR="$python_orb_root/bin"
   else
      FNORB_BIN_DIR="$python_orb_bin_dir"
   fi

   if test "x$python_orb_lib_dir" = "xnone" ; then
      FNORB_LIB_DIR="$python_orb_root/lib/python$PYTHON_VER/site-packages"
   else
      FNORB_LIB_DIR="$python_orb_lib_dir"
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      FNORB_BIN_DIR_UNIX=`unixpath "$FNORB_BIN_DIR"`
      fnidl_search_path="${FNORB_BIN_DIR_UNIX}:${PATH}"
   else
      fnidl_search_path="${FNORB_BIN_DIR}:${PATH}"
   fi

   AC_PATH_PROG(PYTHON_IDL, fnidl, no, [$fnidl_search_path])
   
   if test "x$PYTHON_IDL" = "xno" ; then
      AC_MSG_WARN([*** $python_orb_name required for Tweek Python API ***])
      BUILD_PYTHON_IDL='N'
      BUILD_PYTHON_GUI='N'
   else
      fnidl_works='N'
      AC_MSG_CHECKING([for $PYTHON_IDL usability])
      if ! (eval $PYTHON_IDL --help >/dev/null 2>&1) 2>&1 ; then
         PYTHON_IDL="PYTHONPATH=$FNORB_LIB_DIR $PYTHON_IDL"

         if ! (eval $PYTHON_IDL --help >/dev/null 2>&1) 2>&1 ; then
            AC_MSG_RESULT([no])
            AC_MSG_WARN([*** $python_orb_name required for Tweek Python API ***])
         else
            AC_MSG_RESULT([yes])
            fnidl_works='Y'
         fi
      else
         AC_MSG_RESULT([yes])
         fnidl_works='Y'
      fi

      if test "x$fnidl_works" = "xY" ; then
         PYTHON_ORB_MODULE='Fnorb.orb'
         PYTHON_IDL_OPTS='--poa-skel'
         PYTHON_IDL_GENDIR_OPT='--directory='
         PYTHON_IDL_INCFLAG='-I'
         BUILD_PYTHON_IDL='Y'
      fi
   fi
}

setup_PyORBit ( )
{
   min_pyorbit_version='2.0.0'
   AC_MSG_CHECKING([for PyORBit version >= $min_pyorbit_version])

   # First test to see if PyORBit is even available.
   if ! (eval "$PYTHON -c \"import ORBit, CORBA\"" >/dev/null 2>&1) 2>&1 ; then
      # PyORBit is not available, so we won't try to build the PyQt GUI.
      AC_MSG_RESULT([no])
      AC_MSG_WARN([*** PyORBit is required for Tweek Python API ***])
      BUILD_PYTHON_GUI='N'

   # If PyORBit is available, then we can test its version.
   else
      # The Python code here causes frustrating sh syntactic issues relating
      # to when strings are resolved.
#      pyorbit_version=`$PYTHON -c '"import ORBit; print '.'.join([str(x) for x in ORBit.__version__])"'`
      pyorbit_version=`$PYTHON -c "import ORBit; print ORBit.__version__" | sed -e 's/, /./g' -e 's/(\(.*\))/\1/'`
      DPP_VERSION_CHECK([$pyorbit_version], [$min_pyorbit_version],
                        [AC_MSG_RESULT([yes])],
                        [AC_MSG_RESULT([no])
                         BUILD_PYTHON_GUI='N'])

      PYTHON_ORB_MODULE='ORBit'
   fi

   # This is always going to be 'N' because PyORBit performs IDL compilation
   # dynamically.  In other words, there is nothing to build.
   BUILD_PYTHON_IDL='N'
}

# --------------------
# Find the C++ ORB.
# --------------------
if test "x$BUILD_CXX" != "xN" ; then
   tweeksave_CPPFLAGS="$CPPFLAGS"
   tweeksave_LDFLAGS="$LDFLAGS"
   tweeksave_LIBS="$LIBS"

   case $cxx_orb_name in
      omniORB4)
         setup_omniORB_cxx 4
         ;;
      TAO)
         setup_TAO
         ;;
      *)
         AC_MSG_ERROR([Unsupported C++ ORB $cxx_orb_name!])
         ;;
   esac

   CPPFLAGS="$tweeksave_CPPFLAGS"
   LDFLAGS="$tweeksave_LDFLAGS"
   LIBS="$tweeksave_LIBS"
fi

# --------------------
# Find the Java ORB.
# --------------------
if test "x${_idl_java}" = "xno" ; then
   jacorb_dir='$(JUGGLERROOT_ABS)/external/JacORB'
   JAVA_IDL="JAVA_HOME=\"$JDK_HOME\" JACORB_PATH=\"$jacorb_dir\" \"$jacorb_dir/idl\""

   java_idl_name='JacORB'

   app_jacorb_dir='$(TWEEK_BASE_DIR)/bin'
   APP_JAVA_IDL="JAVA_HOME=$\"JDK_HOME\" JACORB_PATH=\"$app_jacorb_dir\" \"$app_jacorb_dir/idl\""
   APP_JAVA_IDL_INST="JAVA_HOME=\"$JDK_HOME\" JACORB_PATH=\"\$(TWEEK_BASE_DIR)/bin\" \"\$(TWEEK_BASE_DIR)/bin/idl\""

   config_jacorb_dir='$prefix/bin'

   # JAVA_HOME is not set the way that JACORB_PATH is set because we do not
   # know what version of the JDK/JRE the user will have installed, nor do we
   # know where it will be installed.
   config_java_idl="JACORB_PATH=\"$config_jacorb_dir\" \"$config_jacorb_dir/idl\""
else
   # XXX: Need to support more than one IDL compiler for Java by recognizing
   # the name of the compiler and defining options for it below.  Blah.
   if test "x${_idl_java}" != "x" ; then
      case ${_idl_java} in
         # Absolute path to IDL-to_Java compiler.
         /*)
            AC_MSG_CHECKING([execute bit on ${_idl_java}])

            if test -x "${_idl_java}" ; then
               JAVA_IDL="${_idl_java}"
               AC_MSG_RESULT(yes)
            else
               AC_MSG_RESULT(no)
            fi
            ;;
         *)
            AC_CHECK_PROG(JAVA_IDL, [${_idl_java}], [${_idl_java}], 'no')
            ;;
      esac

      java_idl_name="$JAVA_IDL"
      APP_JAVA_IDL="$JAVA_IDL"
      APP_JAVA_IDL_INST="$JAVA_IDL"
      config_java_idl="$JAVA_IDL"
   else
      JAVA_IDL='no'
   fi
fi

if test "x$JAVA_IDL" = "xno" ; then
   BUILD_JAVA='N'
   AC_MSG_WARN(*** Cannot perform IDL-to-Java conversion ***)
else
   case $java_idl_name in
      JacORB)
         JAVA_IDL_NAME="JacORB"
         JAVA_IDL_OPTS=''
         JAVA_IDL_GENDIR_OPT='-d '
         JAVA_IDL_INCFLAG='-I'
         JAVA_IDL_CLIENT_OPTS='-noskel'
         JAVA_IDL_SERVER_OPTS=''
         ;;
      *idlj)
         JAVA_IDL_NAME="IDLJ"
         JAVA_IDL_OPTS=''
         JAVA_IDL_GENDIR_OPT='-td '
         JAVA_IDL_INCFLAG='-i '
         JAVA_IDL_CLIENT_OPTS='-fclient'
         JAVA_IDL_SERVER_OPTS='-fallTIE'
         ;;
      *)
         JAVA_IDL_NAME="Unknown"
         AC_MSG_WARN([*** WARNING: Unknown IDL-to-Java compiler $java_idl_name **])
         ;;
   esac
fi

case $java_orb_name in
   JDK)
      # If we are going to use Java IDL, we have to have JDK 1.4 or newer so
      # that we get an ORB that supports INS.
      DPP_JDK_VERSION([$JDK_HOME], [1.4], ,
         [AC_MSG_ERROR([*** JDK 1.4 or newer required for Java IDL use ***])])
      ;;
   *)
      if test "x$JAVA_ORB_JAR" != "xnone" ; then
         DPP_CHECK_JAVA_STATIC_METHOD([org.omg.CORBA.ORB],
            [init()], ,
            [ ORB_CLASS_PROP='org.openorb.CORBA.ORB'
              ORB_SINGLETON_CLASS_PROP='org.openorb.CORBA.ORBSingleton'
            ],
            [ AC_MSG_WARN([*** $java_orb_name required for Tweek GUI execution ***])
              BUILD_JAVA='N'
            ],
            [$JAVA_ORB_JAR])
      else
         AC_MSG_WARN([*** $java_orb_name required for Tweek Java API ***])
         BUILD_JAVA='N'
      fi
      ;;
esac

# Check if we want to build the Tweek extensions
BUILD_EXTENSIONS='N'
if test "x$use_tweek_extensions" != "xno" -a "x$BUILD_JAVA" = "xY" ; then
   BUILD_EXTENSIONS='Y'
fi

# --------------------
# Find the Python ORB.
# --------------------
if test "x$BUILD_PYTHON_IDL" != "xN" ; then
   AC_MSG_CHECKING([for Python version])
   changequote(<<, >>)
   PYTHON_VER=`echo "import sys; print sys.version[:3]" | $PYTHON -`
   changequote([, ])
   AC_MSG_RESULT([$PYTHON_VER])

   case $python_orb_name in
      Fnorb|fnorb)
         setup_fnorb
         ;;
      omniORBpy|omniorbpy)
         setup_omniORB_python
         ;;
      PyORBit|pyorbit)
         setup_PyORBit
         ;;
      *)
         AC_MSG_ERROR([Unsupported Python ORB $python_orb_name!])
         ;;
   esac
fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------

DPP_INSTALLER(tweek, 0644, 0755, 0755)

# Makefile Substitutions

VJ_VERSION_GROK(VERSION)

# Set the form of the version number used for versioned directory and file
# naming. Unstable builds use all three version numbers; stable builds use
# only the major and minor version numbers.
if test "x$ENABLE_VERSIONING" = "xyes" ; then
   VERSION_DOT="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
   VERSION_UNDERSCORE="${MAJOR_VERSION}_${MINOR_VERSION}_${MICRO_VERSION}"

   HEADER_VERSION="tweek-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="tweek-$VERSION_DOT"

   AC_DEFINE([TWEEK_USE_VERSIONING], ,
             [Define if full package versioning capabilities are enabled])
   AC_DEFINE_UNQUOTED([TWEEK_VERSION_DOT], ["$VERSION_DOT"],
                      [Tweek version string])

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Its use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   DATA_VERSION="tweek"
fi

TWEEK_DATA_DIR="share/$DATA_VERSION"

includedir="$includedir/$HEADER_VERSION"
tweek_header_subdir="$HEADER_VERSION"
tweek_data_subdir="$DATA_VERSION"

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

# For use in linking applications.
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"

tweek_idlflags_cxx="$CXX_IDL_OPTS ${CXX_IDL_INCFLAG}\$prefix/include/$HEADER_VERSION"
tweek_idlflags_java="$JAVA_IDL_OPTS ${JAVA_IDL_INCFLAG}\$prefix/include/$HEADER_VERSION"
tweek_idlflags_python="$PYTHON_IDL_OPTS ${PYTHON_IDL_INCFLAG}\$prefix/include/$HEADER_VERSION"
tweek_idlgendir_cxx="$CXX_IDL_GENDIR_OPT"
tweek_idlgendir_java="$JAVA_IDL_GENDIR_OPT"
tweek_idlgendir_python="$PYTHON_IDL_GENDIR_OPT"
tweek_idl_incflag_cxx="$CXX_IDL_INCFLAG"
tweek_idl_incflag_java="$JAVA_IDL_INCFLAG"
tweek_idl_incflag_python="$PYTHON_IDL_INCFLAG"

case $cxx_orb_name in
   omniORB*)
      idl_gen_dirs="tweek/idl"
      ;;
   *)
      idl_gen_dirs=''
      ;;
esac

#idl_src_dirs=''

for dir in "$idl_gen_dirs" ; do
   IDL_GEN_INCLUDES="$IDL_GEN_INCLUDES -I\$(TWEEK_BASE_DIR)/include/$HEADER_VERSION/$dir"
   tweek_idl_gen_includes="$tweek_idl_gen_includes ${CXX_IDL_INCFLAG}\$prefix/include/$HEADER_VERSION/$dir"
done

#for dir in "$idl_src_dirs" ; do
#   tweek_idlflags_cxx="$tweek_idlflags_cxx ${CXX_IDL_INCFLAG}\${TWEEK_BASE_DIR}/include/$dir"
#   tweek_idlflags_java="$tweek_idlflags_java ${JAVA_IDL_INCFLAG}\${prefix}/include/$dir"
#done

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VPR_CXXFLAGS"
tweek_cxxflags="$STDFLAGS $EXTRA_DEFS $INCLUDES $tweek_idl_gen_includes"
INCLUDES="$INCLUDES $CXX_ORB_INCLUDES"
DEPEND_FLAGS="$VPR_CXXFLAGS"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#   CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
#   CXXFLAGS="$CXXFLAGS -woff 1234,1388,3303,3322 -w2"
   CXXFLAGS="$CXXFLAGS -woff 1174 -w2"
   BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -J4 -all"
elif test "x$PLATFORM" = "xDarwin" ; then
   changequote(<<, >>)
   vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
   changequote([, ])

   # GCC 4.0 in Darwin removed -fexport-coalesced.
   if test $vpr_gcc_major -lt 4 ; then
      CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
   fi

   BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -Wl,-bind_at_load"
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
   vc_cflags='/QIfdiv /QI0f'
   vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
   CFLAGS="$CFLAGS $vc_cflags"
   CXXFLAGS="$CXXFLAGS $vc_cxxflags"
   DBG_FLAGS="$DBG_FLAGS /MDd"
   OPT_FLAGS="$OPT_FLAGS /MD"
   BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
   tweek_cxxflags="$vc_cxxflags $tweek_cxxflags"
fi

if test "x$BUILD_CXX" = "xY" ; then
   tweek_cxxflags="-DTWEEK_HAVE_CXX=1 $tweek_cxxflags"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" -a "x$USE_GCC" != "xyes" ; then
   _aCC_ROOT="/opt/aCC"
   DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
   DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Dependencies for applications and for dynamic/shared libraries.
if test "x$OS_TYPE" = "xWin32" ; then
   APP_EXTRA_LIBS="$LDFLAGS $CXX_ORB_LDFLAGS_LINK_EXE $CXX_ORB_LIB_LINK_EXE $VPR_LIBS $LIBS $VPR_EXTRA_LIBS"
   DYLIB_DEPS="$LDFLAGS $CXX_ORB_LDFLAGS_LINK_EXE $CXX_ORB_LIB_LINK_EXE $LIBS"
   DYLIB_PROF_DEPS="$DYLIB_DEPS"
else
   base_libs="$LDFLAGS $CXX_ORB_LDFLAGS $CXX_ORB_LIB "
   APP_EXTRA_LIBS="$base_libs $VPR_LIBS $LIBS $VPR_EXTRA_LIBS"
   DYLIB_DEPS="$base_libs $VPR_LIBS $LIBS"
   DYLIB_PROF_DEPS="$base_libs $VPR_PROF_LIBS $LIBS"

   VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS], [DYLIB_DEP_FILES])
   VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS], [DYLIB_PROF_DEP_FILES])
fi

# Put together the basic information needed to compile Tweek applications.
VJ_APP_COMPILER($CC, $CFLAGS, $CXX, $CXXFLAGS $IDL_GEN_INCLUDES, $DBG_FLAGS,
                $OPT_FLAGS, TWEEK_BASE_DIR, $DEFS $EXTRA_DEFS, $INCLUDES,
                $ABI_FLAGS, $HEADER_VERSION)
VJ_APP_LINKER_DOOZER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG],
                     [$LDOPTS_OPT], ['$(TWEEK_LIBS_MIN)'],
                     [$APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(TWEEK_BASE_DIR)'

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
   TWEEKROOT_ABS=`dospath -p "$TWEEKROOT_ABS"`
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

   LN_S='cp -pr'
   RM_LN='rm -rf'
   MTREE_CMD=`dospath "$MTREE_CMD"`

#   APP_EXTRA_LIBS_BASIC=`dospath "$APP_EXTRA_LIBS_BASIC"`
else
   RM_LN='rm -f'
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
TWEEK_SHARE_DIR="$data_subdir/$DATA_VERSION"

tweek_lib_name="tweek$LIBRARY_VERSION"
tweek_prof_lib_name="tweek_p$LIBRARY_VERSION"

# Information needed to generate tweek.fpc.
if test "x$PLATFORM" != "xDarwin" ; then
   if test "x$GXX" = "xyes" ; then
      static_begin="-Wl,-Bstatic"
      static_end="-Wl,-Bdynamic"
   else
      static_begin="-B static"
      static_end="-B dynamic"
   fi
fi

tweek_ldflags="-L\$prefix/lib\$LIBBITSUF"
tweek_libs="-l$tweek_lib_name"
tweek_prof_libs="-l$tweek_prof_lib_name"

tweek_cxx_orb_root="$cxx_orb_root"
tweek_extra_ldflags="$EXTRA_LDFLAGS"

jars='Tweek.jar TweekBeans.jar TweekEvents.jar TweekNet.jar TweekBeanDelivery.jar TweekServices.jar kunststoff-mod.jar metouia.jar liquidlnf.jar looks.jar jaxen-core.jar jaxen-jdom.jar jdom.jar saxpath.jar xalan.jar xerces.jar xml-apis.jar'
ext_jars='wizard.jar ui.jar'

if test "x$JAVA_ORB_JAR" != "xnone" ; then
   VJ_STRIP_JAR_PATH(strip_java_orb, [$JAVA_ORB_JAR])
   jars="$jars $strip_java_orb"
fi

share_dir="tweek$HEADER_VERSION"

# The following takes a space-separated list of JAR files and creates a new
# list separated by $JCPS.  Each file in the list is named by an absolute path
# of ${prefix}/$TWEEK_SHARE_DIR/java/<file>.jar.  The variable ${prefix} is expanded
# when tweek-config is executed.
changequote(<<, >>)
tweek_java_api_jars=`echo "$jars" | sed -e "s|\([A-Za-z0-9_][A-Za-z0-9_.-]*\.jar\)|\\\${prefix}/$TWEEK_SHARE_DIR/java/\1|g" -e "s/ /$JCPS/g"`
tweek_java_api_jars_bundle=`echo "$jars" | sed -e 's/\([A-Za-z0-9_][A-Za-z0-9_.-]*\.jar\)/$JAVAROOT\/\1/g' -e "s/ /$JCPS/g"`
tweek_ext_jars=`echo "$ext_jars" | sed -e "s|\([A-Za-z0-9_][A-Za-z0-9_.-]*\.jar\)|\\\${prefix}/$TWEEK_SHARE_DIR/java/\1|g" -e "s/ /$JCPS/g"`
changequote([, ])

DPP_SUBST

# Export variables defined above.
AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_TWEEKROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(TWEEKROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)

AC_SUBST(TWEEK_SHARE_DIR)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(DATA_VERSION)
AC_SUBST(TWEEK_DATA_DIR)

AC_SUBST(MIN_VPR_VERSION)

AC_SUBST(ANT)
AC_SUBST(BUILD_CXX)
AC_SUBST(CXX_ORB_DEPS)
AC_SUBST(CXX_IDL_CMD)
AC_SUBST(CXX_IDL_OPTS)
AC_SUBST(CXX_IDL_GENDIR_OPT)
AC_SUBST(CXX_IDL_INCFLAG)
AC_SUBST(IDL_SERVER_EXT)
AC_SUBST(IDL_SERVER_TMPL_EXT)
AC_SUBST(IDL_CLIENT_EXT)
AC_SUBST(EXTRA_DEFS)
AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(BUILD_JAVA)
AC_SUBST(BUILD_JNI)
AC_SUBST(JAVA_IDL_NAME)
AC_SUBST(JAVA_IDL)
AC_SUBST(JAVA_IDL_OPTS)
AC_SUBST(JAVA_IDL_GENDIR_OPT)
AC_SUBST(JAVA_IDL_INCFLAG)
AC_SUBST(JAVA_IDL_CLIENT_OPTS)
AC_SUBST(JAVA_IDL_SERVER_OPTS)
AC_SUBST(JAVA_ORB_JAR)

AC_SUBST(BUILD_JAVA_TEST_SUITE)
AC_SUBST(JUNIT_JAR)

AC_SUBST(BUILD_EXTENSIONS)

AC_SUBST(ORB_CLASS_PROP)
AC_SUBST(ORB_SINGLETON_CLASS_PROP)

AC_SUBST(config_java_idl)

AC_SUBST(PYTHON_VER)
AC_SUBST(BUILD_PYTHON_IDL)
AC_SUBST(BUILD_PYTHON_GUI)
AC_SUBST(PYTHON_ORB_MODULE)
AC_SUBST(PYTHON_IDL)
AC_SUBST(PYTHON_IDL_OPTS)
AC_SUBST(PYTHON_IDL_GENDIR_OPT)
AC_SUBST(PYTHON_IDL_INCFLAG)

AC_SUBST(APP_JAVA_IDL)
AC_SUBST(APP_JAVA_IDL_INST)
AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)
AC_SUBST(APP_EXTRA_FLAGS)
AC_SUBST(APP_TWEEK_LIBS_BEGIN)
AC_SUBST(APP_TWEEK_LIBS_END)
AC_SUBST(APP_TWEEK_LIBS_BASIC)
AC_SUBST(APP_TWEEK_LIBS_BEGIN_INST)
AC_SUBST(APP_LIBS_BASIC)
AC_SUBST(APP_EXTRA_LIBS_BEGIN)
AC_SUBST(APP_EXTRA_LIBS_END)
AC_SUBST(APP_EXTRA_LIBS_BASIC)

AC_SUBST(DYLIB_DEPS)
AC_SUBST(DYLIB_DEP_FILES)
AC_SUBST(DYLIB_PROF_DEPS)
AC_SUBST(DYLIB_PROF_DEP_FILES)

AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(tweek_cxx_orb_root)
AC_SUBST(tweek_cxxflags)
AC_SUBST(tweek_ldflags)
AC_SUBST(tweek_ldflags)
AC_SUBST(tweek_libs)
AC_SUBST(tweek_prof_libs)
AC_SUBST(tweek_extra_cxxflags)
AC_SUBST(tweek_extra_include_dirs)
AC_SUBST(tweek_extra_ldflags)
AC_SUBST(tweek32_flags)
AC_SUBST(tweek64_flags)
AC_SUBST(tweek_idlflags_cxx)
AC_SUBST(tweek_idl_incflag_cxx)
AC_SUBST(tweek_idlgendir_cxx)
AC_SUBST(tweek_idlflags_java)
AC_SUBST(tweek_idl_incflag_java)
AC_SUBST(tweek_idlgendir_java)
AC_SUBST(tweek_java_api_jars)
AC_SUBST(tweek_java_api_jars_bundle)
AC_SUBST(tweek_ext_jars)
AC_SUBST(tweek_idlflags_python)
AC_SUBST(tweek_idl_incflag_python)
AC_SUBST(tweek_idlgendir_python)
AC_SUBST(tweek_header_subdir)
AC_SUBST(tweek_data_subdir)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(TWEEK, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   tweek.fpc
   tweek-java.fpc
   tweek-python.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   java.defs.mk
   make.defs.mk
   tweek.appdefs.mk
   extensions/Makefile
   extensions/java/build.xml
   extensions/java/Makefile
   extensions/java/Makefile.inc
   java/build.xml
   java/Info.plist
   java/Makefile
   java/Makefile.inc
   java/tweek-base.sh
   java/tweek.sh
   python/Makefile
   python/Makefile.inc
   tweek/Makefile
   tweek/CORBA/Makefile
   tweek/Client/Makefile
   tweek/Util/Makefile
   tweek/idl/Makefile
   test/Makefile
   test/BeanDeliveryTest/Makefile
   test/CxxClient/Makefile
   test/DirectConnectTestBean/Makefile
   test/FileOpenTestBean/Makefile
   test/NetworkTestBean/Makefile
   test/PrefsTestBean/Makefile
   test/TestBean/Makefile
   test/TestSuite/Makefile
   test/TestSuite/java/Makefile
   test/corba/Makefile
   mtree/TWEEK.data.dist
   mtree/TWEEK.include.dist
   mtree/TWEEK.install.dist
   mtree/TWEEK.java.dist
   mtree/TWEEK.python.dist
   mtree/TWEEK.test.dist
   VARS.pl
   ])

   AC_CONFIG_COMMANDS([permission], [chmod a+x java/tweek.sh])

AC_OUTPUT
