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
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Base configure.in for VR Juggler Portable Runtime.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([VR Juggler Portable Runtime], [2.2],
        [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([vpr/vprDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2010 by Iowa State University])
AC_CONFIG_SRCDIR([vpr/vprConfig.h])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.2.2])
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

# Define which threading model to use.  Possible values are "POSIX" or "NSPR".
AC_ARG_ENABLE([subsystem],
              [  --enable-subsystem=<POSIX|NSPR>
                          Define OS abstraction subsystem],
              [SUBSYSTEM="$enableval"], [SUBSYSTEM='none'])

AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

AC_ARG_ENABLE([simulator],
              [  --enable-simulator      Compile simulated sockets layer ],
              [SIMULATOR="$enableval"], [SIMULATOR='no'])

# Rudimentary spell checking based on the first letter in the $SUBSYSTEM name.
case $SUBSYSTEM in
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

AC_ARG_WITH([boost-system-lib],
            [  --with-boost-system-lib=<LIB NAME>
                          The name of the Boost.System
                          (1.35 and newer) library
                          suitable for the -l compiler
                          option                          [default=Auto-detect]],
            [given_boost_system_libname="$withval"],
            [given_boost_system_libname=''])

AC_ARG_WITH([boost-fs-lib],
            [  --with-boost-fs-lib=<LIB NAME>
                          The name of the Boost.Filesystem
                          library suitable for the -l
                          compiler option                 [default=Auto-detect]],
            [given_boost_fs_libname="$withval"], [given_boost_fs_libname=''])

AC_ARG_WITH([boost-signals-lib],
            [  --with-boost-signals-lib=<LIB NAME>
                          The name of the Boost.Signals
                          library suitable for the -l
                          compiler option                 [default=Auto-detect]],
            [given_boost_signals_libname="$withval"], [given_boost_signals_libname=''])

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
    # A machine running DragonFlyBSD.
    dragonfly*)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_DragonFly, , [Define if building on DragonFly.])
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

        VPR_FORCE_SUBSYSTEM([POSIX])
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
    # A machine running Solaris (aka SunOS) 10.  Currently only i386 is known
    # to work.
    solaris2.10)
        VPR_FORCE_SUBSYSTEM([POSIX])
        AC_DEFINE(VPR_OS_Solaris, , [Define if building on Solaris/SunOS.])
        ;;
    # A machine running Solaris (aka SunOS) older than version 10 (2.10/5.10).
    # Currently only i386 is known to work.
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
if test "x$NSPR_THREADS_ENABLED" = "xno" ; then
    DPP_CC_PTHREAD_ARG

    if test "x$CC_ACCEPTS_PTHREAD" = "xno" ; then
        DPP_CC_PTHREADS_ARG
    fi
fi

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_HAVE_PERL5( , , [AC_MSG_ERROR([*** Perl is required ***])])

AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree],
              [\$(PERL5) \$(scriptdir)/mtree.pl])

DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

DPP_HAVE_CPPUNIT([1.9.10])

# Shared memory and synchronization primitives.
SEMAPHORE_SYS=''		# POSIX, NSPR
MUTEX_SYS=''			# POSIX, NSPR
SHMEM_SYS=''			# POSIX, NSPR
THREAD_SYS=''			# POSIX, NSPR

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

    DPP_LANG_SAVE
    DPP_LANG_C

    vprsave_CFLAGS="$CFLAGS"
    vprsave_LIBS="$LIBS"

    CFLAGS="$CFLAGS $PTHREAD_ARG"
    LIBS="$LIBS $PTHREAD_LIB"

    # If the code below compiles, links, and returns 0, then we can use
    # unnamed POSIX semaphores.
    AC_CACHE_CHECK([for unnamed POSIX semaphore support],
                   [ac_cv_has_unnamed_posix_semaphores],
                   [AC_RUN_IFELSE([
#include <semaphore.h>

int main()
{
   sem_t sema;
   return sem_init(&sema, 0, 1);
}],
                        [ac_cv_has_unnamed_posix_semaphores='yes'],
                        [ac_cv_has_unnamed_posix_semaphores='no'])]
                  )

    if test "x$ac_cv_has_unnamed_posix_semaphores" = "xyes" ; then
        AC_DEFINE(VPR_HAVE_UNNAMED_POSIX_SEMAPHORE, ,
                  [Define if unnamed POSIX semaphores are available.])
    fi

    CFLAGS="$vprsave_CFLAGS"
    LIBS="$vprsave_LIBS"

    DPP_LANG_RESTORE

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

    AC_CACHE_CHECK([for TCP_CORK], [ac_cv_has_tcp_cork],
        [AC_TRY_COMPILE([
#include <sys/types.h>
#include <netinet/tcp.h>
],
                        [int cork = TCP_CORK; ],
                        [ac_cv_has_tcp_cork='yes'],
                        [ac_cv_has_tcp_cork='no'])])

    AH_TEMPLATE([HAVE_CORKABLE_TCP],
                [Define if TCP_CORK or TCP_NOPUSH is defined in netint/tcp.h])

    # If TCP_CORK is not defined, check for TCP_NOPUSH.
    if test "x$ac_cv_has_tcp_cork" = "xno" ; then
       AC_CACHE_CHECK([for TCP_NOPUSH], [ac_cv_has_tcp_nopush],
           [AC_TRY_COMPILE([
#include <sys/types.h>
#include <netinet/tcp.h>
],
                           [int np = TCP_NOPUSH; ],
                           [ac_cv_has_tcp_nopush='yes'],
                           [ac_cv_has_tcp_nopush='no'])])

       # TCP_NOPUSH is defined.
       if test "x$ac_cv_has_tcp_nopush" = "xyes" ; then
           # Darwin has a buggy nopush implementation
           if test "x$PLATFORM" != "xDarwin" ; then
               AC_DEFINE(HAVE_CORKABLE_TCP)
           fi
       fi
    # TCP_CORK is defined.
    else
        AC_DEFINE(HAVE_CORKABLE_TCP)
    fi


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

DPP_HAVE_BOOST([1.31.0], [/usr], , ,
               [AC_MSG_ERROR([*** Boost is required for VPR ***])])
BOOST_VERSION_DOT="$dpp_boost_version"

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

VJ_GET_BOOST_LIB_SUFFIX_LIST([$dpp_boost_major], [$dpp_boost_minor],
                             [$BOOST_LIB_VERSION], [boost_suffixes])

if test "x$given_boost_system_libname" != "x" ; then
   boost_system_name="$given_boost_system_libname"
fi

if test "x$given_boost_fs_libname" != "x" ; then
   boost_fs_name="$given_boost_fs_libname"
fi

if test "x$given_boost_signals_libname" != "x" ; then
   boost_signals_name="$given_boost_signals_libname"
fi

# Ensure that a compiled version of the Boost.Filesystem library is available.
vprsave_CXXFLAGS="$CXXFLAGS"
vprsave_CPPFLAGS="$CPPFLAGS"
vprsave_LIBS="$LIBS"
vprsave_LDFLAGS="$LDFLAGS"

CXXFLAGS="$CXXFLAGS $ABI_FLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_INCLUDES"

if test "lib$LIBBITSUF" != "lib" ; then
   libdirs="lib$LIBBITSUF lib"
else
   libdirs="lib"
fi

DPP_LANG_SAVE
DPP_LANG_CPLUSPLUS

testBoostSystem ( )
{
   libname="$1"
   orig_LIBS="$2"

   # NOTE: We do not need $ABI_FLAGS in $LDFLAGS since $CXXFLAGS and $LDFLAGS
   # are used together by AC_TRY_LINK().
   LIBS="-l$libname $orig_LIBS"

   AC_MSG_CHECKING([for boost::system::error_code::clear() in $libname in $cur_boost_libdir])
   AC_TRY_LINK([#include <boost/system/error_code.hpp>],
               [boost::system::error_code ec; ec.clear();],
               [vpr_have_boost_system='yes'], [vpr_have_boost_system='no'])
   rm -rf ./ii_files
   AC_MSG_RESULT([$vpr_have_boost_system])

   LIBS="$orig_LIBS"
}

for l in $libdirs ; do
   cur_boost_libdir="$BOOST_ROOT/$l"
   LDFLAGS="-L$cur_boost_libdir $vprsave_LDFLAGS"

   if test "x$boost_system_name" != "x" ; then
      testBoostSystem $boost_system_name $vprsave_LIBS
   else
      # The extra empty string is to allow for no suffix to be used on the
      # boost_system library name.
      for s in $boost_suffixes "" ; do
         cur_boost_system_name="boost_system$s"

         testBoostSystem $cur_boost_system_name $vprsave_LIBS

         if test "$vpr_have_boost_system" = "yes" ; then
            boost_system_name="$cur_boost_system_name"
            break
         fi
      done
   fi

   if test "$vpr_have_boost_system" = "yes" ; then
      BOOST_LIBDIR="$cur_boost_libdir"
      break
   fi
done

if test "x$vpr_have_boost_system" = "xyes" ; then
   BOOST_SYSTEM_LINK="-l$boost_system_name"
fi

testBoostFilesystem ( )
{
   libname="$1"
   orig_LIBS="$2"

   # NOTE: We do not need $ABI_FLAGS in $LDFLAGS since $CXXFLAGS and $LDFLAGS
   # are used together by AC_TRY_LINK().
   LIBS="-l$libname $BOOST_SYSTEM_LINK $orig_LIBS"

   AC_MSG_CHECKING([for boost::filesystem::exists() in $libname in $cur_boost_libdir])
   AC_TRY_LINK([#include <boost/filesystem/operations.hpp>],
               [boost::filesystem::path p("dir"); boost::filesystem::exists(p);],
               [vpr_have_boost_fs='yes'], [vpr_have_boost_fs='no'])
   rm -rf ./ii_files
   AC_MSG_RESULT([$vpr_have_boost_fs])

   LIBS="$orig_LIBS"
}

for l in $libdirs ; do
   cur_boost_libdir="$BOOST_ROOT/$l"
   LDFLAGS="-L$cur_boost_libdir $vprsave_LDFLAGS"

   if test "x$boost_fs_name" != "x" ; then
      testBoostFilesystem $boost_fs_name $vprsave_LIBS
   else
      # The extra empty string is to allow for no suffix to be used on the
      # boost_filesystem library name.
      for s in $boost_suffixes "" ; do
         cur_boost_fs_name="boost_filesystem$s"

         testBoostFilesystem $cur_boost_fs_name $vprsave_LIBS

         if test "$vpr_have_boost_fs" = "yes" ; then
            boost_fs_name="$cur_boost_fs_name"
            break
         fi
      done
   fi

   if test "$vpr_have_boost_fs" = "yes" ; then
      BOOST_LIBDIR="$cur_boost_libdir"
      break
   fi
done

if test "x$vpr_have_boost_fs" != "xyes" ; then
   AC_MSG_ERROR([*** Boost.Filesystem required by VPR ***])
else
   BOOST_FS_LINK="-l$boost_fs_name"
fi

testBoostSignals ( )
{
   libname="$1"
   orig_LIBS="$2"

   # NOTE: We do not need $ABI_FLAGS in $LDFLAGS since $CXXFLAGS and $LDFLAGS
   # are used together by AC_TRY_LINK().
   LIBS="-l$libname $orig_LIBS"

   AC_MSG_CHECKING([for boost::signal::operator()() in $libname])
   AC_TRY_LINK([#include <boost/signal.hpp>],
               [boost::signal<void ()> s; s();],
               [vpr_have_boost_signals='yes'], [vpr_have_boost_signals='no'])
   rm -rf ./ii_files
   AC_MSG_RESULT([$vpr_have_boost_signals])

   LIBS="$orig_LIBS"
}

LDFLAGS="-L$BOOST_LIBDIR $vprsave_LDFLAGS"

if test "x$boost_signals_name" != "x" ; then
   testBoostSignals $boost_signals_name $vprsave_LIBS
else
   # The extra empty string is to allow for no suffix to be used on the
   # boost_signals library name.
   for s in $boost_suffixes "" ; do
      cur_boost_signals_name="boost_signals$s"

      testBoostSignals $cur_boost_signals_name $vprsave_LIBS

      if test "$vpr_have_boost_signals" = "yes" ; then
         boost_signals_name="$cur_boost_signals_name"
         break
      fi
   done
fi

if test "x$vpr_have_boost_signals" != "xyes" ; then
   AC_MSG_ERROR([*** Boost.Signals required by VPR ***])
else
   BOOST_SIGNALS_LINK="-l$boost_signals_name"
fi

DPP_LANG_RESTORE

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

AM_PATH_FLAGPOLL([0.9.4], ,
   [AC_MSG_ERROR([*** Flagpoll (http://code.google.com/p/flagpoll/) is required to build Juggler ***])])

if ! (eval $FLAGPOLL cppdom --modversion >/dev/null 2>&1) ; then
   AC_MSG_ERROR([*** Flagpoll does not have a valid CppDOM configuration ***])
fi

CPPDOM_CXXFLAGS=`$FLAGPOLL cppdom --cflags`
CPPDOM_LDFLAGS=`$FLAGPOLL cppdom --libs-only-L`
CPPDOM_LIBS=`$FLAGPOLL cppdom --libs-only-l`
CPPDOM_VERSION=`$FLAGPOLL cppdom --modversion`

# Verify that we have a new enough version of CppDOM.
min_cppdom_version='0.6.5'

AC_MSG_CHECKING([whether CppDOM version is >= $min_cppdom_version])
DPP_VERSION_CHECK([$CPPDOM_VERSION], [$min_cppdom_version],
   [AC_MSG_RESULT([yes (version $CPPDOM_VERSION)])],
   [AC_MSG_RESULT([no (version $CPPDOM_VERSION)])
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
VJ_VERSION_GROK(VERSION)

# Set the form of the version number used for versioned directory and file
# naming. Unstable builds use all three version numbers; stable builds use
# only the major and minor version numbers.
if test "x$ENABLE_VERSIONING" = "xyes" ; then
   VERSION_DOT="$MAJOR_VERSION.$MINOR_VERSION"
   VERSION_UNDERSCORE="${MAJOR_VERSION}_${MINOR_VERSION}"

   HEADER_VERSION="vpr-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="vpr-$VERSION_DOT"

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Its use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   DATA_VERSION="vpr"
fi

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
    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    vpr_gcc_minor=`echo $dpp_gcc_ver | sed -e 's/^[0-9]*\.\([0-9]*\).*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CFLAGS_DYNLIB="$CFLAGS_DYNLIB -fexport-coalesced"
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
    fi

    # Do not use -Wno-long-double with GCC 4.2.
    if test $vpr_gcc_minor -lt 2 ; then
       CFLAGS="$CFLAGS -Wno-long-double"
       CXXFLAGS="$CXXFLAGS -Wno-long-double"

       vpr_cxxflags="$vpr_cxxflags -Wno-long-double"
    fi

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
APP_EXTRA_LIBS="$LDFLAGS $CPPDOM_LDFLAGS $CPPDOM_LIBS $BOOST_LDFLAGS $BOOST_SIGNALS_LINK $BOOST_FS_LINK $BOOST_SYSTEM_LINK $PTHREAD_ARG $LIBS $NSPR_LDFLAGS $NSPR_LIB $PLC_LIB $PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"

vpr_lib_name="vpr$LIBRARY_VERSION"
vpr_prof_lib_name="vpr_p$LIBRARY_VERSION"

VJ_APP_COMPILER($CC, $CFLAGS, $CXX, $CXXFLAGS, $DBG_FLAGS, $OPT_FLAGS,
                VPR_BASE_DIR, $DEFS, $INCLUDES, $ABI_FLAGS, $HEADER_VERSION)
VJ_APP_LINKER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG], [$LDOPTS_OPT],
              [VPR_BASE_DIR], [$vpr_lib_name], [$APP_EXTRA_LIBS])

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
    DYLIB_DEPS="$CPPDOM_LDFLAGS $CPPDOM_LIBS $BOOST_LDFLAGS_LINK_EXE $boost_signals_name.lib $boost_fs_name.lib $NSPR_LDFLAGS_LINK_EXE $NSPR_LIB_LINK_EXE $PLC_LIB_LINK_EXE ws2_32.lib"
else
    DYLIB_DEPS="$APP_EXTRA_LIBS"
    VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS], [DYLIB_DEP_FILES])
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
VPR_SHARE_DIR="$data_subdir/$DATA_VERSION"

subsystem_cxxflags="$subsystem_includes"
vpr_extra_cxxflags=''
vpr_extra_include_dirs=''
vpr_ldflags="-L\${prefix}/lib\${LIBBITSUF}"
vpr_extra_ldflags="$EXTRA_LDFLAGS"
vpr_libs="-l$vpr_lib_name"
vpr_prof_libs="-l$vpr_prof_lib_name"

# Information needed to generate vpr-config.
case $SUBSYSTEM in
    POSIX)
        subsystem_libs="$PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"
        # This is for the MIPSpro Compilers 7.3.
        #BOOST_INCLUDES="$BOOST_INCLUDES -I$BOOST_INCLUDE_DIR/boost/compatibility/cpp_c_headers"
        ;;
    NSPR)
        vpr_extra_deps='nspr >= 4.0'

        subsystem_libs="$NSPR_LIB $PLC_LIB $PTHREAD_LIB $SEM_LIB $SOCKET_LIB $NS_LIB $UUID_LIB $DYN_LOAD_LIB"
        # This is for the MIPSpro Compilers 7.3.
        #BOOST_INCLUDES="$BOOST_INCLUDES -I$BOOST_INCLUDE_DIR/boost/compatibility/cpp_c_headers"
        ;;
esac

case $SUBSYSTEM in
   POSIX|NSPR)
      if test "x$PLATFORM" != "xDarwin" ; then
         if test "x$GNU_LD" = "xyes" ; then
            static_begin="-Wl,-Bstatic"
            static_end="-Wl,-Bdynamic"
         else
            static_begin="-B static"
            static_end="-B dynamic"
         fi
      fi
      ;;
esac

includedir="$includedir/$HEADER_VERSION"

DPP_SUBST

AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_VPRROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(VPRROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(VPR_SHARE_DIR)

AC_SUBST(RM_LN)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(DATA_VERSION)

AC_SUBST(CPPDOM_VERSION)

AC_SUBST(SUBSYSTEM)
AC_SUBST(SIMULATOR)
AC_SUBST(SEMAPHORE_SYS)
AC_SUBST(SHMEM_SYS)
AC_SUBST(MUTEX_SYS)
AC_SUBST(THREAD_SYS)
AC_SUBST(HANDLE_ABS)
AC_SUBST(SERIAL_PORT_ABS)
AC_SUBST(SOCKET_ABS)

AC_SUBST(CPPDOM_CXXFLAGS)
AC_SUBST(CPPDOM_LIBS)
AC_SUBST(CPPDOM_LDFLAGS)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS)
AC_SUBST(DYLIB_DEP_FILES)

AC_SUBST(UUID_DEFS)
AC_SUBST(USE_LEACH_UUID)

AC_SUBST(ABI_FLAGS)

AC_SUBST(PLATFORM_SDK)
AC_SUBST(target_cpu)
AC_SUBST(ABI_LIST)

AC_SUBST(BOOST_VERSION_DOT)
AC_SUBST(BOOST_LIBDIR)
AC_SUBST(BOOST_SYSTEM_LINK)
AC_SUBST(BOOST_FS_LINK)
AC_SUBST(BOOST_SIGNALS_LINK)
AC_SUBST(subsystem_cxxflags)
AC_SUBST(subsystem_libs)
AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(vpr_cxxflags)
AC_SUBST(vpr_ldflags)
AC_SUBST(vpr_libs)
AC_SUBST(vpr_prof_libs)
AC_SUBST(vpr_extra_cxxflags)
AC_SUBST(vpr_extra_include_dirs)
AC_SUBST(vpr_extra_ldflags)
AC_SUBST(vpr_extra_deps)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN(VPR, mtree, $PLATFORM, $ISA)

AC_CONFIG_FILES([
   boost.fpc
   boost_system.fpc
   boost_filesystem.fpc
   boost_signals.fpc
   nspr.fpc
   vpr.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   vpr/Makefile
   vpr/md/Makefile
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
   ])


AC_OUTPUT
