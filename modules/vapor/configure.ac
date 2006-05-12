# ***************** <VPR heading BEGIN do not edit this line> *****************
#
# VR Juggler Portable Runtime
#
# Original Authors:
#   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
#
# ****************** <VPR heading END do not edit this line> ******************

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

# -----------------------------------------------------------------------------
# Base configure.in for VR Juggler Portable Runtime.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([VR Juggler Portable Runtime], [1.1],
        [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([vpr/vprDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2006 by Iowa State University])
AC_CONFIG_SRCDIR([vpr/vprConfig.h])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.1.0])
DPP_INIT

# Generate the reconfig script first so that it's easy to run configure again
# if it fails later on.
DPP_GEN_RECONFIG

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP

# $srcdir is the root directory of the juggler source tree.  To get a value
# for $VPRROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
cd "$srcdir"
UNIX_VPRROOT_ABS=`pwd`

cd "$UNIX_VPRROOT_ABS/../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   VPRROOT_ABS=`dospath -p "$UNIX_VPRROOT_ABS"`
   JUGGLERROOT_ABS=`dospath -p "$UNIX_JUGGLERROOT_ABS"`
else
   VPRROOT_ABS="$UNIX_VPRROOT_ABS"
   JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"
fi

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

DPP_STD_CPP(yes)

# Define which threading model to use.  Possible values are "SPROC", "POSIX",
# or "NSPR".
# The default is "SPROC".
AC_ARG_ENABLE([subsystem],
              [  --enable-subsystem=<SPROC|POSIX|NSPR>
                          Define OS abstraction subsystem],
              [SUBSYSTEM="$enableval"], [SUBSYSTEM='none'])

AC_ARG_ENABLE([simulator],
              [  --enable-simulator      Compile simulated sockets layer ],
              [SIMULATOR="$enableval"], [SIMULATOR='no'])

# Rudimentary spell checking based on the first letter in the $SUBSYSTEM name.
case $SUBSYSTEM in
    S*)
        SUBSYSTEM='SPROC'
        ;;
    P*)
        SUBSYSTEM='POSIX'
        ;;
    N*)
        SUBSYSTEM='NSPR'
        ;;
esac

# This enables the use of so-called "NSPR threads".  By default, NSPR is not
# compiled to use NSPR threads, so we do not want to use them by default
# either.
DPP_ENABLE_NSPR_THREADS(no)

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is
# partially usable on Windows.
DPP_WITH_GCC(no)

if test "x$SIMULATOR" != "xno" ; then
    SIMULATOR='Y'
    AC_DEFINE(VPR_SIMULATOR, , [Define if VPR simulated sockets will be used.])
else
    SIMULATOR='N'
fi

# Define the binary format.
DPP_ABI_SETUP

AC_ARG_WITH(cppdom,
            [  --with-cppdom=<PATH>    CppDOM installation directory   [default=/usr/local]],
            cppdom_root="$withval", cppdom_root="/usr/local")

AC_ARG_WITH([boost-fs-lib],
            [  --with-boost-fs-lib=<LIB NAME>
                          The name of the Boost.Filesystem
                          library binary for linking      [default=Auto-detect]
                          *** USE WITH CAUTION! ***],
            [given_boost_fs_libname="$withval"], [given_boost_fs_libname=''])

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
AC_CANONICAL_TARGET
DPP_SYSTEM_SETUP

# These are used on all platforms.
AC_DEFINE_UNQUOTED([VPR_OS_RELEASE], ["$OS_REL_STR"],
                   [Define to match the output of running 'uname -r'.])
AC_DEFINE_UNQUOTED([VPR_OS_RELEASE_MAJOR], [$OS_REL_NUM_MAJOR],
                   [Define to match the numeric major version number of the target OS.])
AC_DEFINE_UNQUOTED([VPR_OS_RELEASE_MINOR], [$OS_REL_NUM_MINOR],
                   [Define to match the numeric minor version number of the target OS.])

# Templates for Autoheader.
AH_TEMPLATE([_BSD_TYPES],
            [Define if building on Darwin or IRIX.])
AH_TEMPLATE([VPR_OS_HPUX], [Define if building on HP-UX.])
AH_TEMPLATE([VPR_HPUX_VERSION],
            [Define to the major version number of your HP-UX installation.])
AH_TEMPLATE([_INCLUDE_TERMIO], [Define if building on HP-UX.])
AH_TEMPLATE([_CMA_NOWRAPPERS_], [Define if building on HP-UX.])

AC_DEFUN([VPR_FORCE_SUBSYSTEM],
[
   if test "x$SUBSYSTEM" = "xnone" ; then
      AC_MSG_WARN([*** Forcing the use of the $1 subsystem ***])
      SUBSYSTEM='$1'
   fi
])

# Depending on the target operating system, set various command options and
# such.
case $target_os in
    # IBM RS/6000 running AIX.
    aix*)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_AIX, , [Define if building on AIX.])
        ;;
    # i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
    cygwin)
        VPR_FORCE_SUBSYSTEM([NSPR])
        AC_DEFINE(VPR_OS_Win32, , [Define if building on a Win32 platform.])
        ;;
    # Darwin, aka Mac OS X.
    darwin*)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_Darwin, , [Define if building on Darwin.])
        AC_DEFINE(_BSD_TYPES, 1)
        ;;
    # A machine running FreeBSD.  Currently only i386 is known to compile.
    freebsd*)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_FreeBSD, , [Define if building on FreeBSD.])
        ;;
    # HP PA-RISC machine running HP-UX 10.20.
    hpux10.20)
        # We have to use the NSPR subsystem on HP-UX 10.20.
        VPR_FORCE_SUBSYSTEM([NSPR])
        AC_DEFINE(VPR_OS_HPUX,)
        AC_DEFINE(VPR_HPUX_VERSION, 10)
        AC_DEFINE(_INCLUDE_TERMIO,)
        AC_DEFINE(_CMA_NOWRAPPERS_,)
        ;;
    # HP PA-RISC machine running HP-UX 11.x.
    hpux11*)
        AC_DEFINE(VPR_OS_HPUX,)
        AC_DEFINE(VPR_HPUX_VERSION, 11)
        AC_DEFINE(_INCLUDE_TERMIO,)
        AC_DEFINE(_CMA_NOWRAPPERS_,)
        ;;
    # SGI running IRIX 6.*.
    irix6*)
        if test "x$USE_GCC" != "xyes" ; then
            DBG_FLAGS="$DBG_FLAGS -gslim"
        fi

        VPR_FORCE_SUBSYSTEM([SPROC])
        AC_DEFINE(VPR_OS_IRIX, , [Define if building on IRIX.])
        AC_DEFINE(_BSD_TYPES,)
        ;;
    # A machine running Linux.  Currently only i386 is known to work.
    linux*)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_Linux, , [Define if building on Linux.])
        ;;
    # DEC Alpha running Digital UNIX 4.0.
    osf4.0)
        AC_DEFINE(VPR_OS_DUX, , [Define if building on Digital UNIX.])
        ;;
    # A machine running Solaris (aka SunOS).  Currently only i386 is known
    # to work.
    solaris*)
        VPR_FORCE_SUBSYSTEM([NSPR])
        AC_DEFINE(VPR_OS_Solaris, , [Define if building on Solaris/SunOS.])
        ;;
    unknown)
        AC_DEFINE(VPR_OS_GENERIC, ,
                  [Define if the target OS does not match anything else.])
        ;;
esac

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP
VJ_PROG_CC_PROF_FLAG(yes)
VJ_PROG_CXX_PROF_FLAG(yes)
DPP_PROG_CC_NOSTDINC

# Ensure that the C++ compiler we've found is capable of compiling the newer
# newer C++ features that we need.
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** The library requires C++ namesapce support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_INLINE([AC_MSG_ERROR([*** The library requires C++ inline support ***])])
DPP_CXX_RTTI([AC_MSG_ERROR([*** The library requires C++ RTTI support ***])])
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** The library requires C++ static_cast<> ***])])

# Now check to see if the compiler accepts the -pthread option.
if test "x$SUBSYSTEM" != "xSPROC" -a "x$NSPR_THREADS_ENABLED" = "xno" ; then
    DPP_CC_PTHREAD_ARG

    if test "x$CC_ACCEPTS_PTHREAD" = "xno" ; then
        DPP_CC_PTHREADS_ARG
    fi
fi

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER(5.004, , , [AC_MSG_ERROR([*** Perl is required ***])])

AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree], [\$(PERL) \$(scriptdir)/mtree.pl])

DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

# The $USE_DYLD stuff is for Darwin.
DPP_CHECK_DYNLOAD_LIB([USE_DYLD='N'], [USE_DYLD='Y'])
DPP_HAVE_CPPUNIT([1.9.10])

# Shared memory and synchronization primitives.
SEMAPHORE_SYS=''		# POSIX, SPROC, NSPR
MUTEX_SYS=''			# POSIX, SPROC, NSPR
SHMEM_SYS=''			# POSIX, SPROC, NSPR
THREAD_SYS=''			# POSIX, SPROC, NSPR

# Test for libpthread or for libcma if libpthread is not present.  If the
# library is found, check for pthread_kill().  If it is present, Draft 10 (the
# "final" draft) of the POSIX threads standard is in place.  If not, Draft 4
# is available.
if test "x$SUBSYSTEM" = "xPOSIX" ; then
    DPP_GET_PTHREAD_LIB

    # If $PTHREAD_LIB has a value at this point, then the pthread libraries
    # needed are in $LIBS, so now we make a check for the draft revision being
    # used.  This is done by determining if pthread_kill() is in the library.
    # If it is, Draft 10 is in use.  Otherwise, assume it is Draft 4.
    if test "$xPTHREAD_LIB" != "x" ; then
        AH_TEMPLATE([VPR_POSIX_C_SOURCE],
                    [Define if using POSIX threads.])
        DPP_GET_PTHREAD_VER([$PLATFORM], [VPR_POSIX_C_SOURCE])

        AH_TEMPLATE([VPR_THREAD_SCOPE],
                    [Define to be the POSIX threads constant for thread scoping.])
        # Define the default thread scope depending on the platform.  On IRIX,
        # only process scope is avaiable without special configuration options.
        # Other systems may support system scope.
#        if test "$PLATFORM" = "IRIX" ; then
#            AC_DEFINE(VPR_THREAD_SCOPE, PTHREAD_SCOPE_PROCESS)
#        else
            AC_DEFINE(VPR_THREAD_SCOPE, PTHREAD_SCOPE_SYSTEM)
#        fi

        USE_PTHREADS='yes'
        DPP_GET_POSIX_SEMAPHORE_LIB( ,
            AC_MSG_ERROR(*** POSIX semaphores are required for POSIX subsystem ***))
    else
        AC_MSG_WARN(*** POSIX threads will not be used (no library found) ***)
    fi

    # Do not define _POSIX_C_SOURCE here because it will cause problems when
    # compiling on IRIX.  Instead, define that in the source files where
    # appropriate.
    AC_DEFINE(VPR_USE_PTHREADS, , [Define if using POSIX threads.])
    AC_DEFINE(VPR_POSIX_SEMAPHORES, , [Define if using POSIX semaphores.])
    # Use POSIX threading, mutexes and shared memory.
    MUTEX_SYS='POSIX'
    SHMEM_SYS='POSIX'
    THREAD_SYS='POSIX'
    SEMPANORE_SYS='POSIX'
    HANDLE_ABS='UNIX'
    SERIAL_PORT_ABS='TERMIOS'
    SOCKET_ABS='BSD'
# If we are not using POSIX threads, we must be using the IRIX sproc(2) model,
# but we test $SUBSYSTEM just to be sure it is set right.
elif test "x$SUBSYSTEM" = "xSPROC" ; then
    AC_CHECK_HEADERS([sys/types.h])
    AC_CHECK_HEADERS([sys/prctl.h], ,
        [AC_MSG_ERROR([*** IRIX sproc(2) threading will not be used (no headers found) ***])],
[#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
])

    AC_DEFINE(VPR_USE_IRIX_SPROC, , [Define if using IRIX SPROC threads.])

    # Set to IRIX sproc threading, synchronization and shared memory.
    THREAD_SYS='SPROC'
    SEMAPHORE_SYS='SPROC'
    SHMEM_SYS='SPROC'
    MUTEX_SYS='SPROC'
    HANDLE_ABS='UNIX'
    SERIAL_PORT_ABS='TERMIOS'
    SOCKET_ABS='BSD'
# NSPR subsystem.
else
    if test "x$NSPR_THREADS_ENABLED" = "xno" -a "x$OS_TYPE" = "xUNIX" ; then
        DPP_GET_PTHREAD_LIB
        DPP_GET_POSIX_SEMAPHORE_LIB( ,
            [AC_MSG_ERROR([*** POSIX semaphores are required for NSPR subsystem ***])])
        USE_PTHREADS='yes'
    else
        USE_PTHREADS='no'
    fi

    _vpr_save_LIBS="$LIBS"
    LIBS="$LIBS $DYN_LOAD_LIB"
    DPP_HAVE_NSPR([4.0], ["$USE_PTHREADS"],
        [AC_MSG_ERROR([*** NSPR requried for compiling ***])])
    LIBS="${_vpr_save_LIBS}"

    AC_DEFINE(VPR_USE_NSPR, , [Define if using the Netscape Portable Runtime.])

    # Set to NSPR threading, synchronization and shared memory.
    SEMPANORE_SYS='NSPR'
    THREAD_SYS='NSPR'
    MUTEX_SYS='NSPR'
    SHMEM_SYS='NSPR'

    if test "x$OS_TYPE" = "xUNIX" ; then
        SERIAL_PORT_ABS='TERMIOS'
    else
        SERIAL_PORT_ABS='WIN32'
    fi

    HANDLE_ABS='NSPR'
    SOCKET_ABS='NSPR'
fi

if test "x$USE_PTHREADS" = "xyes" ; then
    if test "x$PTHREAD_ARG" = "x" ; then
       AC_DEFINE(_REENTRANT, 1, [Define if using POSIX threads.])
    fi

    AC_DEFINE(_THREAD_SAFE, , [Define if using POSIX threads.])
    AC_DEFINE(RWSTD_MULTI_THREAD, , [Define if using POSIX threads.])
    AC_DEFINE(RW_MULTI_THREAD, , [Define if using POSIX threads.])
fi

# -----------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# -----------------------------------------------------------------------------
if test "x$SUBSYTEM" = "xSPROC" ; then
   AC_TYPE_PID_T
fi

AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
DPP_TYPE_SOCKLEN_T

# The following checks for availability of the sigset_t type.  Since it is
# is included through signal.h, the usual AC_CHECK_TYPE method will not work.
AC_CACHE_CHECK([for sigset_t], [ac_cv_has_sigset_t],
    [AC_TRY_COMPILE([#include <signal.h> ],
                    [ sigset_t var; ],
                    [ac_cv_has_sigset_t='yes'],
                    [ac_cv_has_sigset_t='no'])])

if test "x$ac_cv_has_sigset_t" = "xno" ; then
    AC_DEFINE(sigset_t, unsigned int,
              [Define to unsigned int if the sigset_t type is not available.])
fi

# The following checks for availability of the timeval type.  Since it is
# is included through sys/time.h, the usual AC_CHECK_TYPE method will not work.
AC_CACHE_CHECK([for timeval], [ac_cv_has_timeval],
    [AC_TRY_COMPILE([#include <sys/time.h> ],
                    [ struct timeval var; ],
                    [ac_cv_has_timeval='yes'],
                    [ac_cv_has_timeval='no'])])

if test "x$ac_cv_has_timeval" = "xyes" ; then
    AC_DEFINE(HAVE_TIMEVAL, ,
              [Define if the timeval type is available.])
fi

# The following checks for availability of the timezone type.  Since it is
# is included through sys/time.h, the usual AC_CHECK_TYPE method will not work.
AC_CACHE_CHECK([for timezone], [ac_cv_has_timezone],
    [AC_TRY_COMPILE([#include <sys/time.h> ],
                    [ struct timezone var; ],
                    [ac_cv_has_timezone='yes'],
                    [ac_cv_has_timezone='no'])])

if test "x$ac_cv_has_timezone" = "xyes" ; then
    AC_DEFINE(HAVE_TIMEZONE, ,
              [Define if the timezone type is available.])
fi

if test "x$SUBSYSTEM" != "xNSPR" ; then
    # The following checks for availability of the in_addr_t type.  Since it is
    # supposed to be defined in netinet/in.h, the usual AC_CHECK_TYPE method
    # will not work.
    AC_CACHE_CHECK([for in_addr_t], [ac_cv_has_in_addr_t],
        [AC_TRY_COMPILE([#include <sys/types.h>
                         #include <netinet/in.h> ],
                        [ in_addr_t var; ],
                        [ac_cv_has_in_addr_t='yes'],
                        [ac_cv_has_in_addr_t='no'])])

    if test "x$ac_cv_has_in_addr_t" = "xno" ; then
        AC_DEFINE(in_addr_t, int,
                  [Define to int if the in_addr_t type is not available.])
    fi

    # Determine whether the sockaddr_in struct declares the sin_len member.
    # Some platforms (most notably, Linux) do not include this.
    # NOTE: If sockaddr_in does not declare sin_len, then sockaddr does not
    # declare sa_len.
    AC_CACHE_CHECK([whether struct sockaddr_in has sin_len],
        [ac_cv_have_sin_len],
        [AC_TRY_COMPILE([
#include <sys/types.h>
#include <netinet/in.h>],
                        [struct sockaddr_in addr; addr.sin_len = 0;],
                        [ac_cv_have_sin_len='yes'],
                        [ac_cv_have_sin_len='no'])])

    AH_TEMPLATE([HAVE_SIN_LEN], [Define if struct sockaddr_in has sin_len])

   if test "x$ac_cv_have_sin_len" = "xyes" ; then
        AC_DEFINE(HAVE_SIN_LEN)
   fi
fi

# If we are not using NSPR, we make typedefs in vprTypes.h based on the results
# of the following tests.
if test "x$SUBSYSTEM" != "xNSPR" ; then
    AC_CHECK_SIZEOF(char, 8)
    AC_CHECK_SIZEOF(short, 16)
    AC_CHECK_SIZEOF(int, 32)
    AC_CHECK_SIZEOF(long, 64)
    AC_CHECK_SIZEOF(long long, 64)
fi

# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([bstring.h fcntl.h limits.h signal.h strings.h sys/file.h \
                 sys/filio.h sys/ioctl.h sys/sysinfo.h sys/time.h \
                 termios.h unistd.h sys/capability.h sys/stdsyms.h \
                 uuid.h uuid/uuid.h sys/uuid.h])

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS
AC_CHECK_HEADERS([hash_map ext/hash_map hash_map.h])
AC_CHECK_HEADERS(hash_set ext/hash_set hash_set.h ext/numeric)
DPP_LANG_RESTORE

DPP_HAVE_BOOST([1.31.0], [/usr/local], , ,
               [AC_MSG_ERROR([*** Boost is required for VPR ***])])

# -----------------------------------------------------------------------------
# Checks for library functions.
# -----------------------------------------------------------------------------
AC_CHECK_FUNCS([gettimeofday sigaction sigemptyset sigfillset sigaddset \
                sigdelset sigismember snprintf strerror strerror_r \
                getifaddrs])

_vpr_save_LIBS="$LIBS"
AC_CHECK_FUNC([socket], [SOCKET_LIB=''],
   [AC_CHECK_LIB([socket], [socket], [SOCKET_LIB='-lsocket'])])
LIBS="${_vpr_save_LIBS}"

_vpr_save_LIBS="$LIBS"
AC_CHECK_FUNC([gethostbyname], [NS_LIB=''],
   [AC_CHECK_LIB([socket], [gethostbyname], [NS_LIB='-lsocket'],
      [AC_CHECK_LIB([nsl], [gethostbyname], [NS_LIB='-lnsl'])])])
LIBS="${_vpr_save_LIBS}"

if test "x$SOCKET_LIB" = "x$NS_LIB" ; then
   NS_LIB=''
fi

# Default to using the Leach UUID implementation found in
# juggler/external/leach-uuid.
USE_LEACH_UUID='Y'

AH_TEMPLATE([VPR_USE_DCE_1_1_UUID],
            [Define to 1 if you have the DCE 1.1 UUID functions (uuid_create(3), etc.).])
AH_TEMPLATE([VPR_USE_LIBUUID],
            [Define to 1 if you have the e2fsprogs libuuid funcs (uuid_generate(3), etc.).])

# Test for the DCE 1.1 UUID interface.
_vpr_save_LIBS="$LIBS"
AC_CHECK_FUNC([uuid_create],
   [AC_DEFINE([VPR_USE_DCE_1_1_UUID])
    UUID_LIB=''
    USE_LEACH_UUID='N'],
   [AC_CHECK_LIB([uuid], [uuid_create],
      [AC_DEFINE([VPR_USE_DCE_1_1_UUID])
       UUID_LIB='-luuid'
       USE_LEACH_UUID='N'])])
LIBS="${_vpr_save_LIBS}"

# If $USE_LEACH_UUID is still "Y", then we did not find uuid_create(3).
# Let's try uuid_generate(3) instead.  This comes from libuuid (only on
# Linux), typically.
if test "x$USE_LEACH_UUID" = "xY" ; then
   _vpr_save_LIBS="$LIBS"
   AC_CHECK_FUNC([uuid_generate],
      [AC_DEFINE([VPR_USE_LIBUUID])
       UUID_LIB=''
       USE_LEACH_UUID='N'],
      [AC_CHECK_LIB([uuid], [uuid_generate],
         [AC_DEFINE([VPR_USE_LIBUUID])
          UUID_LIB='-luuid'
          USE_LEACH_UUID='N'])])
   LIBS="${_vpr_save_LIBS}"
fi

# If we do not have the DCE 1.1 UUID interface or the libuuid stuff, we have
# to use the local Leach UUID implementation.
if test "x$USE_LEACH_UUID" = "xY" ; then
   AC_DEFINE([VPR_USE_LEACH_UUID], 1,
             [Define to 1 if neither uuid_create(3) nor uuid_generate(3) are available.])
fi

AH_TEMPLATE([HAVE_SCHED_YIELD],
            [Define to 1 if you have the sched_yield(2) function.])

# Ensure that we have sched_yield(3).  On Solaris, sched_yield(3) and its
# synchronization friends are in libposix4.
_vpr_save_LIBS="$LIBS"
AC_CHECK_FUNC([sched_yield], [AC_DEFINE([HAVE_SCHED_YIELD])],
              [AC_CHECK_LIB([posix4], [sched_yield],
                  [ _sched_lib='-lposix4'
                    AC_DEFINE([HAVE_SCHED_YIELD]) ])])
LIBS="${_vpr_save_LIBS}"

# Test to see if the function backtrace() (a GLIBC feature) is available.
# As of right now, we know of no other libc implementation with this feature,
# so this test is not very sophisticated.
AC_CACHE_CHECK([for backtrace], [ac_cv_has_backtrace],
   [AC_TRY_COMPILE([#include <execinfo.h> ],
                   [int size = backtrace(0, 0);],
                   [ac_cv_has_backtrace='yes'],
                   [ac_cv_has_backtrace='no'])])

if test "x$ac_cv_has_backtrace" = "xyes" ; then
   AC_DEFINE([HAVE_BACKTRACE], ,
             [Define if the GLIBC backtrace() function is available.])
fi

# Construct the Boost.Filesystem library name automatically based on Boost
# naming conventions if the user did not provide a library name.
if test "x$given_boost_fs_libname" = "x" ; then
   case $CC in
      gcc*|*/gcc*)
         toolset='-gcc'
         ;;
      icc*|*/icc*)
         if test "x$OS_TYPE" = "xWin32" ; then
            toolset='-iw'
         else
            toolset='-il'
         fi
         ;;
      cygcl|msvccc|cl*|CL*)
         toolset="-${BOOST_TOOL:=vc7}"
         ;;
      cc*|*/cc*)
         if test "x$GCC" = "xyes" ; then
            toolset='-gcc'
         else
            case $PLATFORM in
               IRIX)
                  toolset='-mp'
                  ;;
               Solaris)
                  toolset='-sw'
                  ;;
            esac
         fi
         ;;
   esac

   # A correct installation of Boost on Darwin does not include the toolset name
   # as part of the library file name.
   if test "x$PLATFORM" = "xDarwin" ; then
      toolset=''
      threading=''
   else
      # We can only use multi-threading versions of Boost libraries if we are
      # not using SPROC threads.  Boost does not support SPROC.
      if test "x$SUBSYSTEM" != "xSPROC" -a "x$ABI" != "x64" ; then
         threading='-mt'
      fi
   fi

   # Include the detected Boost version number in the library name.
   boost_fs_name="boost_filesystem$toolset$threading-$BOOST_LIB_VERSION"
else
   boost_fs_name="$given_boost_fs_libname"
fi

# Ensure that a compiled version of the Boost.Filesystem library is available.
vprsave_CXXFLAGS="$CXXFLAGS"
vprsave_CPPFLAGS="$CPPFLAGS"
vprsave_LIBS="$LIBS"
vprsave_LDFLAGS="$LDFLAGS"

CXXFLAGS="$CXXFLAGS $ABI_FLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_INCLUDES"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $ABI_FLAGS"
LIBS="$LIBS -l$boost_fs_name"

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS
AC_CACHE_CHECK([for boost::filesystem::exists() in $boost_fs_name],
   [ac_cv_has_boost_fs_exists],
   [AC_TRY_LINK([#include <boost/filesystem/operations.hpp> ],
                [boost::filesystem::path p("dir"); boost::filesystem::exists(p);],
                [ac_cv_has_boost_fs_exists='yes'],
                [ac_cv_has_boost_fs_exists='no'])])
rm -rf ./ii_files
DPP_LANG_RESTORE

if test "x$ac_cv_has_boost_fs_exists" = "xno" ; then
   AC_MSG_ERROR([*** Boost.Filesystem required by VPR ***])
fi

CXXFLAGS="$vprsave_CXXFLAGS"
CPPFLAGS="$vprsave_CPPFLAGS"
LIBS="$vprsave_LIBS"
LDFLAGS="$vprsave_LDFLAGS"

# -----------
# CppDOM
# -----------
_vpr_save_CXXFLAGS="$CXXFLAGS"
_vpr_save_CPPFLAGS="$CPPFLAGS"
_vpr_save_LDFLAGS="$LDFLAGS"

# Ensure that /usr/include and /usr/lib are not included multiple times
# if $cppdom_root is "/usr".
if test "x$cppdom_root" != "x" ; then
   cppdom_path="$cppdom_root/bin:$PATH"
else
   cppdom_path="$PATH"
fi

if test "x$OS_TYPE" = "xWin32" ; then
   CPPDOM_CXXFLAGS="-I$cppdom_root/include"
   CPPDOM_LDFLAGS="/libpath:$cppdom_root/lib"
   CPPDOM_LIBS="cppdom.lib"

   # Get the CppDOM version number components.
   cppdom_major_ver=`grep 'define CPPDOM_VERSION_MAJOR ' $cppdom_root/include/cppdom/version.h | awk '{ print $3 }' -`
   cppdom_minor_ver=`grep 'define CPPDOM_VERSION_MINOR ' $cppdom_root/include/cppdom/version.h | awk '{ print $3 }' -`
   cppdom_patch_ver=`grep 'define CPPDOM_VERSION_PATCH ' $cppdom_root/include/cppdom/version.h | awk '{ print $3 }' -`

   cppdom_version="$cppdom_major_ver.$cppdom_minor_ver.$cppdom_patch_ver"
else
   AC_PATH_PROG(CPPDOM_CONFIG, [cppdom-config], [no], [$cppdom_path])

   if test "x$CPPDOM_CONFIG" = "xno" ; then
      AC_MSG_ERROR([*** CppDOM is required for VPR ***])
   fi

   # XXX: This expression is not as robust as it ought to be.  It assumes
   # that cppdom-config is in a directory called 'bin'.  That is not
   # entirely invalid, but it is not flexible either.
   # Using `cppdom-config --prefix` would be simpler, but it wouldn't handle
   # the case when CppDOM was transplanted without updating cppdom-config.
   found_cppdom_root=`echo $CPPDOM_CONFIG | sed -e 's|\(.*\)/bin/cppdom-config$|\1|'`

   CPPDOM_CXXFLAGS=`CPPDOM_BASE_DIR=$found_cppdom_root $CPPDOM_CONFIG --cxxflags --basedir`
   cppdom_lib_output=`CPPDOM_BASE_DIR=$found_cppdom_root $CPPDOM_CONFIG --libs --basedir`

   changequote(<<, >>)
   # XXX: These sed(1) expressions could be improved.
   CPPDOM_LDFLAGS=`echo $cppdom_lib_output | sed -e 's/( |^)-l[^ ]*//g'`
   CPPDOM_LIBS=`echo $cppdom_lib_output | sed -e 's/( |^)-L[^ ]*//g'`
   changequote([, ])

   cppdom_version=`$CPPDOM_CONFIG --version`
fi

# Verify that we have a new enough version of CppDOM.
min_cppdom_version='0.3.1'
AC_MSG_CHECKING([whether CppDOM version is >= $min_cppdom_version])
DPP_VERSION_CHECK([$cppdom_version], [$min_cppdom_version],
   [AC_MSG_RESULT([yes (version $cppdom_version)])],
   [AC_MSG_RESULT([no (version $cppdom_version)])
    AC_MSG_ERROR([*** CppDOM version $min_cppdom_version or newer is requried for VPR ***])])

CXXFLAGS="$CPPDOM_CXXFLAGS $CXXFLAGS $STDFLAGS $ABI_FLAGS"
CPPFLAGS="$CPPDOM_CXXFLAGS $CPPFLAGS"
LDFLAGS="$CPPDOM_LDFLAGS $CPPDOM_LIBS $LDFLAGS"

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

# XXX: We should test for linking features too.
AC_CHECK_HEADER([cppdom/config.h], ,
    [AC_MSG_ERROR([*** CppDOM is required for VPR ***])])

DPP_LANG_RESTORE

CXXFLAGS="${_vpr_save_CXXFLAGS}"
CPPFLAGS="${_vpr_save_CPPFLAGS}"
LDFLAGS="${_vpr_save_LDFLAGS}"

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------

DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

vpr_cxxflags="$STDFLAGS"

# Add these C++ options when compiling with G++.
if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
   CXXFLAGS="$CXXFLAGS $PTHREAD_ARG"
   vpr_cxxflags="$vpr_cxxflags $PTHREAD_ARG"

   _vpr_gxx_ver=`$CXX -dumpversion`

   case ${_vpr_gxx_ver} in
      3.*)
         CXXFLAGS="$CXXFLAGS -fexceptions"
         vpr_cxxflags="$vpr_cxxflags -fexceptions"
         ;;
      2.9*)
         if test "x$PLATFORM" = "xLinux" ; then
            AC_DEFINE([_GNU_SOURCE], ,
                      [Define if compiling on Linux with GCC older than 3.0.])
         fi
         ;;
   esac
fi

# For use in linking applications.
BASE_APP_LINK_FLAGS="$STDFLAGS"

if test "x$ac_cv_header_sys_sysinfo_h" = "xyes" ; then
    UUID_DEFS='-DHAVE_SYS_SYSINFO_H'
fi

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
    CXXFLAGS="$CXXFLAGS -w2"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -J4"
elif test "x$PLATFORM" = "xDarwin" ; then
    CFLAGS="$CFLAGS -Wno-long-double"
    CXXFLAGS="$CXXFLAGS -Wno-long-double"

    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CFLAGS_DYNLIB="$CFLAGS_DYNLIB -fexport-coalesced"
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
    fi

    vpr_cxxflags="$vpr_cxxflags -Wno-long-double"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS -Wl,-bind_at_load"
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
    vc_cflags='/QIfdiv /QI0f'
    vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
    CFLAGS="$CFLAGS $vc_cflags"
    CXXFLAGS="$CXXFLAGS $vc_cxxflags"
    DBG_FLAGS="$DBG_FLAGS /MDd"
    OPT_FLAGS="$OPT_FLAGS /MD"
    UUID_DEFS="$UUID_DEFS -DWININC"
    vpr_cxxflags="$vc_cxxflags"
fi

# Add visibility flag if supported.
CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS"

# If we are using pthreads, add the library containing the pthread calls to
# $LIBS.
if test "x$USE_PTHREADS" = "xyes" ; then
    DPP_PTHREAD_FINISH
fi

subsystem_includes="$subsystem_includes $NSPR_INCLUDES"
INCLUDES="$INCLUDES $NSPR_INCLUDES"

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Define the base path to the source directory using $VPRROOT_ABS as an
# alternative to using $srcdir.
UNIX_VPRROOT_ABS="$VPRROOT_ABS"

# Put together the basic information needed to compile VPR applications.
if test "x$OS_TYPE" = "xWin32" ; then
    APP_EXTRA_LIBS="$LDFLAGS $LIBS $CPPDOM_LDFLAGS $CPPDOM_LIBS $BOOST_LDFLAGS_LINK_EXE $boost_fs_name.lib $NSPR_LDFLAGS_LINK_EXE $NSPR_LIB_LINK_EXE $PLC_LIB_LINK_EXE ws2_32.lib"
else
    APP_EXTRA_LIBS="$LDFLAGS $CPPDOM_LDFLAGS $CPPDOM_LIBS $BOOST_LDFLAGS -l$boost_fs_name $PTHREAD_ARG $LIBS $NSPR_LDFLAGS $NSPR_LIB $PLC_LIB $PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"
fi

VJ_APP_COMPILER($CC, $CFLAGS, $CXX, $CXXFLAGS, $DBG_FLAGS, $OPT_FLAGS,
                VPR_BASE_DIR, $DEFS, $INCLUDES, $ABI_FLAGS)
VJ_APP_LINKER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG], [$LDOPTS_OPT],
              [VPR_BASE_DIR], [vpr], [$APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(VPR_BASE_DIR)'

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    VPRROOT_ABS=`dospath -p "$VPRROOT_ABS"`
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

#    APP_EXTRA_LIBS=`dospath "$APP_EXTRA_LIBS"`
else
    RM_LN='rm -f'
fi

# Dependencies for dynamic/shared libraries.
if test "x$OS_TYPE" = "xWin32" ; then
    DYLIB_DEPS="$CPPDOM_LDFLAGS $CPPDOM_LIBS $BOOST_LDFLAGS_LINK_EXE $boost_fs_name.lib $NSPR_LDFLAGS_LINK_EXE $NSPR_LIB_LINK_EXE $PLC_LIB_LINK_EXE ws2_32.lib"
else
    DYLIB_DEPS="$APP_EXTRA_LIBS"
fi

# Information needed to generate vpr-config.
case $SUBSYSTEM in
    SPROC)
        subsystem_cxxflags="$subsystem_includes"
        subsystem_libs="$SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"
        static_begin="-B static"
        static_end="-B dynamic"
        vpr_ldflags_compiler="-L\${prefix}/lib\${LIBBITSUF}"
        vpr_ldflags_linker="$vpr_ldflags_compiler"
        vpr_extra_ldflags_compiler="$EXTRA_LDFLAGS $CPPDOM_LIBS -l$boost_fs_name"
        vpr_extra_ldflags_linker="$vpr_extra_ldflags_compiler"
        vpr_libs='-lvpr'
        vpr_prof_libs='-lvpr_p'
        ;;
    POSIX)
        subsystem_cxxflags="$subsystem_includes"
        subsystem_libs="$PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"

        if test "x$PLATFORM" != "xDarwin" ; then
            if test "x$GNU_LD" = "xyes" ; then
                static_begin="-Wl,-Bstatic"
                static_end="-Wl,-Bdynamic"
            else
                static_begin="-B static"
                static_end="-B dynamic"
            fi
        fi

        vpr_ldflags_compiler="-L\${prefix}/lib\${LIBBITSUF}"
        vpr_ldflags_linker="$vpr_ldflags_compiler"
        vpr_extra_ldflags_compiler="$EXTRA_LDFLAGS $CPPDOM_LIBS -l$boost_fs_name"
        vpr_extra_ldflags_linker="$vpr_extra_ldflags_compiler"
        vpr_libs='-lvpr'
        vpr_prof_libs='-lvpr_p'
        ;;
    NSPR)
        if test "x$OS_TYPE" = "xWin32" ; then
            subsystem_cxxflags="$subsystem_includes"
            subsystem_libs="$NSPR_LIB_LINK_EXE $PLC_LIB_LINK_EXE ws2_32.lib"
            vpr_ldflags_compiler="/link /libpath:\$prefix/lib/debug"
            vpr_ldflags_linker="/libpath:\$prefix/lib/debug"
            vpr_extra_ldflags_compiler="/link $CPPDOM_LIBS $BOOST_LDFLAGS_LINK_EXE $boost_fs_name.lib $NSPR_LDFLAGS_LINK_EXE"
            vpr_extra_ldflags_linker="$CPPDOM_LIBS $BOOST_LDFLAGS_LINK_EXE $boost_fs_name.lib $NSPR_LDFLAGS_LINK_EXE"
            vpr_libs='vpr.lib'
            vpr_prof_libs='vpr.lib'
        else
            subsystem_cxxflags="$subsystem_includes"
            subsystem_libs="$NSPR_LIB $PLC_LIB $PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"
            vpr_ldflags_compiler="-L\${prefix}/lib\${LIBBITSUF}"
            vpr_ldflags_linker="$vpr_ldflags_compiler"
            vpr_extra_ldflags_compiler="$EXTRA_LDFLAGS $CPPDOM_LIBS -l$boost_fs_name"
            vpr_extra_ldflags_linker="$vpr_extra_ldflags_compiler"
            vpr_libs='-lvpr'
            vpr_prof_libs='-lvpr_p'

            if test "x$PLATFORM" != "xDarwin" ; then
                if test "x$GNU_LD" = "xyes" ; then
                    static_begin="-Wl,-Bstatic"
                    static_end="-Wl,-Bdynamic"
                else
                    static_begin="-B static"
                    static_end="-B dynamic"
                fi
            fi
        fi
        ;;
esac

vpr_extra_cxxflags=''
vpr_extra_include_dirs=''

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
    vpr32_flags='-n32'
    vpr64_flags='-64'
    vpr_extra_include_dirs='boost/compatibility/cpp_c_headers'
elif test "x$GCC" = "xyes" ; then
    vpr32_flags='-m32'
    vpr64_flags='-m64'
fi

VJ_VERSION_GROK(VERSION)

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_VPRROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(VPRROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(VPR_SHARE_DIR)

AC_SUBST(RM_LN)

AC_SUBST(SUBSYSTEM)
AC_SUBST(SIMULATOR)
AC_SUBST(SEMAPHORE_SYS)
AC_SUBST(SHMEM_SYS)
AC_SUBST(MUTEX_SYS)
AC_SUBST(THREAD_SYS)
AC_SUBST(HANDLE_ABS)
AC_SUBST(SERIAL_PORT_ABS)
AC_SUBST(SOCKET_ABS)
AC_SUBST(USE_DYLD)

AC_SUBST(CPPDOM_CXXFLAGS)
AC_SUBST(CPPDOM_LIBS)
AC_SUBST(CPPDOM_LDFLAGS)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS)

AC_SUBST(UUID_DEFS)
AC_SUBST(USE_LEACH_UUID)

AC_SUBST(subsystem_cxxflags)
AC_SUBST(subsystem_libs)
AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(vpr_cxxflags)
AC_SUBST(vpr_ldflags_compiler)
AC_SUBST(vpr_ldflags_linker)
AC_SUBST(vpr_libs)
AC_SUBST(vpr_prof_libs)
AC_SUBST(vpr_extra_cxxflags)
AC_SUBST(vpr_extra_include_dirs)
AC_SUBST(vpr_extra_ldflags_compiler)
AC_SUBST(vpr_extra_ldflags_linker)
AC_SUBST(vpr_extra_libs)
AC_SUBST(vpr32_flags)
AC_SUBST(vpr64_flags)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(VPR, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   vpr-config
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   vpr/Makefile
   vpr/md/Makefile
   vpr/md/DARWIN/Makefile
   vpr/md/DARWIN/DynLoad/Makefile
   vpr/md/NSPR/Makefile
   vpr/md/NSPR/DynLoad/Makefile
   vpr/md/NSPR/IO/Makefile
   vpr/md/NSPR/IO/Socket/Makefile
   vpr/md/NSPR/Sync/Makefile
   vpr/md/NSPR/Thread/Makefile
   vpr/md/NSPR/Util/Makefile
   vpr/md/POSIX/Makefile
   vpr/md/POSIX/DynLoad/Makefile
   vpr/md/POSIX/IO/Makefile
   vpr/md/POSIX/IO/Port/Makefile
   vpr/md/POSIX/IO/Socket/Makefile
   vpr/md/POSIX/Sync/Makefile
   vpr/md/POSIX/Thread/Makefile
   vpr/md/POSIX/Util/Makefile
   vpr/md/SIM/Makefile
   vpr/md/SIM/IO/Makefile
   vpr/md/SIM/IO/Socket/Makefile
   vpr/md/SIM/Network/Makefile
   vpr/md/SPROC/Makefile
   vpr/md/SPROC/SharedMem/Makefile
   vpr/md/SPROC/Sync/Makefile
   vpr/md/SPROC/Thread/Makefile
   vpr/md/WIN32/Makefile
   vpr/md/WIN32/IO/Makefile
   vpr/md/WIN32/IO/Port/Makefile
   vpr/md/common/Makefile
   vpr/DynLoad/Makefile
   vpr/IO/Makefile
   vpr/IO/Port/Makefile
   vpr/IO/Socket/Makefile
   vpr/IO/Stats/Makefile
   vpr/Sync/Makefile
   vpr/Thread/Makefile
   vpr/Perf/Makefile
   vpr/Util/Makefile
   vpr/Util/detail/Makefile
   vpr/Util/md5/Makefile
   vpr/Util/uuid/Makefile
   test/Makefile
   test/TestSuite/Makefile
   test/TestSuite/test.defs.mk
   test/TestSuite/TestCases/Makefile
   test/TestSuite/TestCases/DynLoad/Makefile
   test/TestSuite/TestCases/DynLoad/modules/Makefile
   test/TestSuite/TestCases/Perf/Makefile
   test/TestSuite/TestCases/IO/Makefile
   test/TestSuite/TestCases/IO/Port/Makefile
   test/TestSuite/TestCases/IO/Socket/Makefile
   test/TestSuite/TestCases/IO/Stats/Makefile
   test/TestSuite/TestCases/Simulator/Makefile
   test/TestSuite/TestCases/Socket/Makefile
   test/TestSuite/TestCases/Thread/Makefile
   test/TestSuite/TestCases/Util/Makefile
   test/SerialPort/Makefile
   test/Socket/Makefile
   test/Sync/Makefile
   test/Thread/Makefile
   mtree/VPR.include.dist
   mtree/VPR.install.dist
   mtree/VPR.test.dist
   VARS.pl
   ],
   [chmod a+x vpr-config])

AC_OUTPUT
