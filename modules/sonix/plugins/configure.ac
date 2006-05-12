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
# Base configure.in for Sonix plug-ins.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([Sonix Plug-Ins], [1.1], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2006 by Iowa State University])
AC_CONFIG_SRCDIR([Makefile.in])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../../share/config])
DPP_PREREQ([2.0.2])
DPP_INIT

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #

DPP_STD_CPP(yes)

# does the user want OpenAL?
AC_ARG_ENABLE(openal,
              [  --enable-openal         Compile in OpenAL support       [default=yes]],
              [OPENAL_ENABLED="$enableval"], [OPENAL_ENABLED='yes'])

# does the user want Subsynth?
AC_ARG_ENABLE(subsynth,
              [  --enable-subsynth       Compile in Subsynth support     [default=yes]],
              [SUBSYNTH_ENABLED="$enableval"], [SUBSYNTH_ENABLED='yes'])

# does the user want AudioWorks?
AC_ARG_ENABLE(audioworks,
              [  --enable-audioworks     Compile in AudioWorks support   [default=yes]],
              [AUDIOWORKS_ENABLED="$enableval"], [AUDIOWORKS_ENABLED='yes'])

# does the user want Audiere?
AC_ARG_ENABLE(audiere,
              [  --enable-audiere        Compile in Audiere support      [default=yes]],
              [AUDIERE_ENABLED="$enableval"], [AUDIERE_ENABLED='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Allow forcing the use of GCC as the compiler.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
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
DPP_WIN32_SETUP

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $PLUGIN_ROOT_ABS, we cd there and save the value of running pwd.  Then
# return to the directory where configure is being run ($topdir).
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
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** Sonix plug-ins require C++ namespace support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** Sonix plug-ins require C++ static_cast<> ***])])
DPP_CXX_DYNAMIC_CAST([AC_MSG_ERROR([*** Sonix plug-ins require C++ dynamic_cast<> ***])])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER([5.004], , , [AC_MSG_ERROR([*** Perl is required ***])])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
DPP_PROG_LINKER

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
VPR_PATH([0.32.0], [VPR_SUBSYSTEM=`$VPR_CONFIG --subsystem`],
         [AC_MSG_ERROR([*** VPR required for Sonix plug-ins ***])])

SNX_PATH([0.5.0], [BUILD_PLUGINS='Y'],
         [AC_MSG_WARN([*** Sonix required for Sonix plug-ins ***])
          BUILD_PLUGINS='N'
         ])

if test "x$BUILD_PLUGINS" = "xY" ; then
   # Test for Audiere.
   HAVE_AUDIERE='N'
   if test "x$AUDIERE_ENABLED" = "xyes" ; then
      if test "x$VPR_SUBSYSTEM" != "xSPROC" ; then
         DPP_HAVE_AUDIERE([/usr/local], [HAVE_AUDIERE='Y'],
            [AC_MSG_WARN([*** Audiere plug-in cannot be built. ***])])
      else
         AC_MSG_WARN([*** Audiere connot be used with a VPR installation that ues SPROC threads. ***])
      fi
   fi

   # Test for AudioWorks.
   HAVE_AUDIOWORKS='N'
   if test "x$AUDIOWORKS_ENABLED" = "xyes" ; then
      if test "x$VPR_SUBSYSTEM" = "xSPROC"; then
         DPP_HAVE_AUDIOWORKS([/usr], [HAVE_AUDIOWORKS='Y'],
            [AC_MSG_WARN([*** AudioWorks plug-in cannot be built. ***])])
      else
         AC_MSG_WARN([*** AudioWorks cannot be used with a VPR installation that does not use SPROC threads. ***])
      fi
   fi

   # Test for OpenAL.
   HAVE_OPENAL='N'
   if test "x$OPENAL_ENABLED" = "xyes" ; then
      if test "x$VPR_SUBSYSTEM" != "xSPROC"; then
         snx_save_LIBS="$LIBS"
         LIBS="$LIBS $DYN_LOAD_LIB"

         DPP_HAVE_OPENAL([/usr/local], [HAVE_OPENAL='Y'],
            [AC_MSG_WARN([*** OpenAL plug-in cannot be built. ***])])

         LIBS="$snx_save_LIBS"

         if test "x$HAVE_OPENAL" = "xY" ; then
            DPP_PTHREAD_FINISH

            snxsave_CPPFLAGS="$CPPFLAGS"
            CPPFLAGS="$CPPFLAGS $AL_INCLUDES $ALUT_INCLUDES"
            AC_CHECK_HEADERS(AL/al.h AL/alut.h)
            CPPFLAGS="$snxsave_CPPFLAGS"

            if test "x$LIBALUT" != "x" ; then
               snxsave_CXXFLAGS="$CXXFLAGS"
               snxsave_CPPFLAGS="$CPPFLAGS"
               snxsave_LDFLAGS="$LDFLAGS"
               snxsave_LIBS="$LIBS"

               CFLAGS="$CFLAGS $ABI_FLAGS"
               CPPFLAGS="$CPPFLAGS $AL_INCLUDES"
               LDFLAGS="$LDFLAGS $AL_LDFLAGS"
               LIBS="$LIBS $LIBALUT"

               DPP_LANG_SAVE
               DPP_LANG_C

               AC_CACHE_CHECK([for alutInitWithoutContext() in $LIBALUT],
                  [ac_cv_has_alutInitWithoutContext],
                  [AC_TRY_LINK([#include <AL/alut.h> ],
                     [alutInitWithoutContext(0, 0);],
                     [ac_cv_has_alutInitWithoutContext='yes'],
                     [ac_cv_has_alutInitWithoutContext='no'])])

               AC_CACHE_CHECK([for aluGetError() in $LIBALUT],
                  [ac_cv_has_alutGetError],
                  [AC_TRY_LINK([#include <AL/alut.h> ],
                     [alutGetError();],
                     [ac_cv_has_alutGetError='yes'],
                     [ac_cv_has_alutGetError='no'])])

               AC_CACHE_CHECK([for alutLoadMemoryFromFile() in $LIBALUT],
                  [ac_cv_has_alutLoadMemoryFromFile],
                  [AC_TRY_LINK([#include <AL/alut.h> ],
                     [alutLoadMemoryFromFile("test", 0, 0, 0);],
                     [ac_cv_has_alutLoadMemoryFromFile='yes'],
                     [ac_cv_has_alutLoadMemoryFromFile='no'])])

               DPP_LANG_RESTORE

               CFLAGS="$snxsave_CFLAGS"
               CPPFLAGS="$snxsave_CPPFLAGS"
               LDFLAGS="$snxsave_LDFLAGS"
               LIBS="$snxsave_LIBS"

               if test "x$ac_cv_has_alutInitWithoutContext" = "xyes" ; then
                  AC_DEFINE(HAVE_ALUT_INIT_WITHOUT_CONTEXT, ,
                            [Define if alutInitWithoutContext() is available])
               fi

               if test "x$ac_cv_has_alutGetError" = "xyes" ; then
                  AC_DEFINE(HAVE_ALUT_GET_ERROR, ,
                            [Define if alutGetError() is available])
               fi

               if test "x$ac_cv_has_alutLoadMemoryFromFile" = "xyes" ; then
                  AC_DEFINE(HAVE_ALUT_LOAD_MEMORY_FROM_FILE, ,
                            [Define if alutLoadMemoryFromFile() is available])
               fi
            fi
         fi
      else
         AC_MSG_WARN([*** OpenAL cannot be used with a VPR installation that uses SPROC threads. ***])
      fi
   fi

   # Test for Subsynth.
   HAVE_SUBSYNTH='N'
   if test "x$SUBSYNTH_ENABLED" = "xyes" ; then
      DPP_HAVE_SUBSYNTH([/usr], [HAVE_SUBSYNTH='Y'],
         [AC_MSG_WARN([*** Subsynth plug-in cannot be built. ***])])
   fi
fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VPR_CXXFLAGS_MIN $SNX_CXXFLAGS_MIN"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$VPR_INCLUDES_MIN $SNX_INCLUDES_MIN"

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" = "xno" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
#    CXXFLAGS="$CXXFLAGS -woff 3322,1388 -w2"
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
    CFLAGS="$CFLAGS /W3 $vc_cflags"
    CXXFLAGS="$CXXFLAGS /W3 $vc_cxxflags"
    BASE_APP_LINK_FLAGS="$BASE_APP_LINK_FLAGS /nologo /incremental:no"
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

# $DSO_PLUGIN_DEPS must list all the libraries required to link a Sonix
# plug-in.
if test "x$OS_TYPE" = "xWin32" ; then
    DSO_PLUGIN_DEPS="$LDFLAGS $SNX_LIBS_LD_MIN $SNX_EXTRA_LIBS_LD_MIN"
    DSO_PLUGIN_PROF_DEPS="$DSO_PLUGIN_DEPS"
    jccl_extra_libs="$LDFLAGS $LIBS"
else
    DSO_PLUGIN_DEPS="$LDFLAGS $SNX_LIBS_CC $LIBS $SNX_EXTRA_LIBS_CC"
    DSO_PLUGIN_PROF_DEPS="$LDFLAGS $SNX_PROF_LIBS_CC $LIBS $SNX_EXTRA_LIBS_CC"
fi

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(PLUGIN_ROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)

AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(DSO_PLUGIN_DEPS)
AC_SUBST(DSO_PLUGIN_PROF_DEPS)

AC_SUBST(HAVE_AUDIERE)
AC_SUBST(HAVE_AUDIOWORKS)
AC_SUBST(HAVE_OPENAL)
AC_SUBST(HAVE_SUBSYNTH)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------
AC_CONFIG_FILES([
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   plugin.defs.mk
   Audiere/Makefile
   AudioWorks/Makefile
   OpenAL/Makefile
   Subsynth/Makefile
   ])

AC_OUTPUT
