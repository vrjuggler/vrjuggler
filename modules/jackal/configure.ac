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
# Base configure.in for JCCL.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([JCCL], [1.4], [vrjuggler-devel@lists.sourceforge.net])
AC_CONFIG_HEADERS([common/jccl/jcclDefines.h])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2010 by Iowa State University])
AC_CONFIG_SRCDIR([common/jccl/jcclConfig.h])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../share/config])
DPP_PREREQ([2.1.2])
DPP_INIT

# Generate the reconfig script first so that it's easy to run configure again
# if it fails later on.
DPP_GEN_RECONFIG

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $VJROOT_ABS, we cd there and save the value of running pwd.  Then return to
# the directory where configure is being run ($topdir).
cd "$srcdir"
JCCLROOT_ABS=`pwd`

cd "$JCCLROOT_ABS/../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   JUGGLERROOT_ABS=`cygpath -w "$UNIX_JUGGLERROOT_ABS" | sed -e 's/\\\\/\\//g'`
else
   JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"
fi

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------
DPP_STD_CPP(yes)

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is partially
# usable on Windows.
DPP_WITH_GCC(no)

# Define the binary format.
DPP_ABI_SETUP

# Specify where JDOM is located
#AC_ARG_WITH(jdom, [  --with-jdom=<PATH>      Path to JDOM JAR file(s) ],
#            JDOM_JAR="$withval", JDOM_JAR='none')

# Specify where JUnit is located
AC_ARG_WITH(junit,
            [  --with-junit=<PATH>     Path to JUnit JAR file(s) ],
            JUNIT_JAR="$withval", JUNIT_JAR='none')

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
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP
VJ_PROG_CC_PROF_FLAG(yes)
VJ_PROG_CXX_PROF_FLAG(yes)

# Ensure that the C++ compiler we've found is capable of compiling the newer
# newer C++ features that we need.
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** The library requires C++ namespace support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_INLINE([AC_MSG_ERROR([*** The library requires C++ inline support ***])])
DPP_CXX_RTTI([AC_MSG_ERROR([*** The library requires C++ RTTI support ***])])
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** The library requires C++ static_cast<> ***])])
DPP_CXX_DYNAMIC_CAST([AC_MSG_ERROR([*** The library requires C++ dynamic_cast<> ***])])

# Check for the JDK.
DPP_PATH_JAVA

# If there is still no value in $JAVAC, then warn that the GUI will not be
# built.
if test "x$JAVAC" = "xno" ; then
    AC_MSG_WARN([*** Editor JavaBeans require javac -- they will not be built ***])
    BUILD_JAVA='N'
else
    BUILD_JAVA='Y'
#    # Check for JDOM.
#    DPP_CHECK_JAVA_STATIC_METHOD([org.jdom.Namespace],
#       [getNamespace(uri)], [String uri = new String("test")],
#       [BUILD_JAVA='Y'],
#       [ AC_MSG_WARN([*** JDOM required for editor JavaBeans ***])
#         BUILD_JAVA='N' ],
#       [$JDOM_JAR])
fi

# If we're building the editor JavaBeans, we should build the test cases too.
if test "x$BUILD_JAVA" = "xY" ; then
    # Check for the JUnit library
    DPP_CHECK_JAVA_STATIC_METHOD([junit.framework.Assert],
        [assertTrue(true)], ,
        [BUILD_JAVA_TEST_SUITE='Y'],
        [ BUILD_JAVA_TEST_SUITE='N' ;
          AC_MSG_WARN(*** JUnit required for Java test suite ***)
        ],
        [$JUNIT_JAR])

    ANT="JAVA_HOME=\"$JDK_HOME\" \"$UNIX_JUGGLERROOT_ABS/external/ant/bin/ant\" --noconfig"
fi

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER([5.004], , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG([MTREE_CMD], [mtree], [mtree], [\$(PERL) \$(scriptdir)/mtree.pl])
DPP_BASIC_PROGS([$PLATFORM], [$OS_TYPE])
DPP_PROG_INSTALL
VJ_LINKER_SETUP

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------
MIN_VPR_VERSION='2.1.4'
VPR_PATH([$MIN_VPR_VERSION], BUILD_CXX='Y', 
         [AC_MSG_WARN([*** VPR required for JCCL ***])
          BUILD_CXX='N'])

TWEEK_PATH_JAVA([0.19.2], ,
                [AC_MSG_WARN([*** Tweek Java required for editor JavaBeans ***])
                 BUILD_JAVA='N'
                ])

if test "x$BUILD_CXX" = "xY" ; then
   #VPR_SUBSYSTEM=`$VPR_CONFIG --subsystem`
   DPP_HAVE_CPPUNIT([1.9.10])
fi

# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
AC_HEADER_STDC

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

   HEADER_VERSION="jccl-$VERSION_DOT"
   CLOSE_HEADER_VERSION='..'
   LIBRARY_VERSION="-$VERSION_UNDERSCORE"
   DATA_VERSION="jccl-$VERSION_DOT"
   PLUGIN_VERSION="jccl-$VERSION_DOT"

   AC_DEFINE([JCCL_USE_VERSIONING], ,
             [Define if full package versioning capabilities are enabled])
   AC_DEFINE_UNQUOTED([JCCL_VERSION_DOT], ["$VERSION_DOT"],
                      [Jackal version string])
# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION and $PLUGIN_VERSION. Their use is different
# than the others. The important thing to note is that the directory name is
# unversioned in this case.
else
   DATA_VERSION="jccl"
   PLUGIN_VERSION="jccl"
fi

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $VPR_CXXFLAGS"
BASE_APP_LINK_FLAGS="$STDFLAGS $BASE_APP_LINK_FLAGS"
DEPEND_FLAGS="$VPR_CXXFLAGS"
jccl_cxxflags="$STD_FLAGS"

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
    jccl_cxxflags="$vc_cxxflags $jccl_cxxflags"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Define the base path to the source directory using $(JCCLROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_JCCLROOT_ABS="$JCCLROOT_ABS"

# $APP_EXTRA_LIBS must list all the libraries need to link a JCCL-based
# application.
# $jccl_extra_libs only lists the external (non-Juggler) libraries needed by
# JCCL.
if test "x$OS_TYPE" = "xWin32" ; then
    APP_EXTRA_LIBS="$LDFLAGS $VPR_LIBS $LIBS"
    APP_EXTRA_PROF_LIBS="$APP_EXTRA_LIBS"
    jccl_extra_libs="$LDFLAGS $LIBS"
else
    APP_EXTRA_LIBS="$LDFLAGS $VPR_LIBS $LIBS"
    APP_EXTRA_PROF_LIBS="$LDFLAGS $VPR_PROF_LIBS $LIBS"
    jccl_extra_libs="$LDFLAGS $LIBS"
fi

jccl_lib_name="jccl$LIBRARY_VERSION"
jccl_prof_lib_name="jccl_p$LIBRARY_VERSION"

# Variables set in Makefiles found in the application trees.  All variables
# listed are for use only in the Makefile.in's found in that tree unless
# otherwise noted.  They are used in the following way:
#
#     * APP_VJ_LIBS_UTILS: The VR Juggler general utilities library.

# Put together the basic information needed to compile JCCL applications.
VJ_APP_COMPILER([$CC], [$CFLAGS], [$CXX], [$CXXFLAGS], [$DBG_FLAGS],
                [$OPT_FLAGS], [JCCL_BASE_DIR], [$DEFS], [$INCLUDES],
                [${_EXTRA_FLAGS}], [$HEADER_VERSION])
VJ_APP_LINKER([$CXX], [$BASE_APP_LINK_FLAGS], [$LDOPTS_DBG], [$LDOPTS_OPT],
              [JCCL_BASE_DIR], [$jccl_lib_name], [$APP_EXTRA_LIBS])

APP_BASE_DIR='$(topdir)/instlinks'
APP_BASE_DIR_INST='$(JCCL_BASE_DIR)'

DYLIB_DEPS_JCCL="$APP_EXTRA_LIBS"
DYLIB_PROF_DEPS_JCCL="$APP_EXTRA_PROF_LIBS"

VJ_BUILD_LIB_FILE_LIST([$DYLIB_DEPS_JCCL], [DYLIB_DEP_FILES_JCCL])
VJ_BUILD_LIB_FILE_LIST([$DYLIB_PROF_DEPS_JCCL], [DYLIB_PROF_DEP_FILES_JCCL])

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    JCCLROOT_ABS=`dospath -p "$JCCLROOT_ABS"`

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
    JDK_HOME=`dospath -p "$JDK_HOME"`
#    APP_EXTRA_LIBS=`dospath "$APP_EXTRA_LIBS"`
else
    RM_LN='rm -f'
    JCCLROOT_ABS="$JCCLROOT_ABS"
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
JCCL_SHARE_DIR="$data_subdir/$DATA_VERSION"

# Information needed to generate jccl.fpc.
if test "x$GNU_LD" = "xyes" ; then
   static_begin="-Wl,-Bstatic"
   static_end="-Wl,-Bdynamic"
else
   static_begin="-B static"
   static_end="-B dynamic"
fi

jccl_ldflags="-L\$prefix/lib\$LIBBITSUF"
jccl_libs="-l$jccl_lib_name"
jccl_prof_libs="-l$jccl_prof_lib_name"
jccl_extra_cxxflags=''

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
    jccl32_flags='-n32'
    jccl64_flags='-64'
elif test "x$GCC" = "xyes" ; then
    jccl32_flags='-m32'
    jccl64_flags='-m64'
fi

jccl_extra_ldflags="$EXTRA_LDFLAGS"

jars='TableLayout.jar'
beans='jccl_config.jar jccl_editors.jar jccl_rtrc.jar'

# XXX: For now, we won't include $JDOM_JAR in the JAR files output by
# jccl-config.  This is okay (for now) because Tweek depends on JDOM, and we
# depend on Tweek.  The real problem is with the sed expression below that
# has jccl_ in it.  I don't want to mess with it for fear of breaking something
# else.
#if test "x$JDOM_JAR" != "xnone" ; then
#   VJ_STRIP_JAR_PATH(strip_jdom, [$JDOM_JAR])
#   jars="$jars $strip_jdom"
#fi

changequote(<<, >>)
jccl_java_api_jars=`echo "$jars" | sed -e "s|\([A-Za-z]*\.jar\)|\\\${prefix}/$JCCL_SHARE_DIR/java/\1|g" -e "s/ /$JCPS/g"`
jccl_java_api_beans=`echo "$beans" | sed -e "s|\(jccl_[A-Za-z]*\.jar\)|\\\${prefix}/$JCCL_SHARE_DIR/beans/\1|g" -e "s/ /$JCPS/g"`
jccl_java_api_jars="${jccl_java_api_jars}${JCPS}${jccl_java_api_beans}"
changequote([, ])

includedir="$includedir/$HEADER_VERSION"
jccl_header_subdir="$HEADER_VERSION"
jccl_data_subdir="$DATA_VERSION"
jccl_plugin_subdir="$PLUGIN_VERSION"

DPP_SUBST

AC_SUBST(data_subdir)
AC_SUBST(topdir)
AC_SUBST(UNIX_JCCLROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(JCCLROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(JCCL_SHARE_DIR)

AC_SUBST(VERSION_DOT)
AC_SUBST(VERSION_UNDERSCORE)
AC_SUBST(HEADER_VERSION)
AC_SUBST(CLOSE_HEADER_VERSION)
AC_SUBST(LIBRARY_VERSION)
AC_SUBST(DATA_VERSION)
AC_SUBST(PLUGIN_VERSION)

AC_SUBST(MIN_VPR_VERSION)

AC_SUBST(BUILD_CXX)

AC_SUBST(ANT)
AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(BUILD_JAVA)

AC_SUBST(BUILD_JAVA_TEST_SUITE)
AC_SUBST(JUNIT_JAR)

AC_SUBST(APP_BASE_DIR)
AC_SUBST(APP_BASE_DIR_INST)

AC_SUBST(DYLIB_DEPS_JCCL)
AC_SUBST(DYLIB_DEP_FILES_JCCL)
AC_SUBST(DYLIB_PROF_DEPS_JCCL)
AC_SUBST(DYLIB_PROF_DEP_FILES_JCCL)

AC_SUBST(APP_BASIC_LIBS)
AC_SUBST(APP_EXTRA_LIBS)

AC_SUBST(static_begin)
AC_SUBST(static_end)
AC_SUBST(jccl_cxxflags)
AC_SUBST(jccl_ldflags)
AC_SUBST(jccl_libs)
AC_SUBST(jccl_prof_libs)
AC_SUBST(jccl_extra_cxxflags)
AC_SUBST(jccl_extra_ldflags)
AC_SUBST(jccl_extra_libs)
AC_SUBST(jccl32_flags)
AC_SUBST(jccl64_flags)
AC_SUBST(jccl_java_api_jars)
AC_SUBST(jccl_header_subdir)
AC_SUBST(jccl_data_subdir)
AC_SUBST(jccl_plugin_subdir)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

VJ_MTREE_LIB_GEN([JCCL], [mtree], [$PLATFORM], [$ISA])

AC_CONFIG_FILES([
   jccl.fpc
   Makefile
   Makefile.inc
   common.defs.mk
   make.defs.mk
   common/Makefile
   common/Makefile.inc
   common/jccl/Makefile
   common/jccl/Plugins/Makefile
   common/jccl/Util/Makefile
   config/build.xml
   config/Makefile
   config/Makefile.inc
   config/jccl/Makefile
   config/jccl/Config/Makefile
   editors/build.xml
   editors/Makefile
   editors/Makefile.inc
   rtrc/Makefile
   rtrc/jccl/Makefile
   rtrc/jccl/RTRC/Makefile
   data/Makefile
   test/Makefile
   test/TestSuite/Makefile
   test/TestSuite/test.defs.mk
   test/TestSuite/TestCases/Makefile
   test/TestSuite/java/Makefile
   mtree/JCCL.data.dist
   mtree/JCCL.include.dist
   mtree/JCCL.install.dist
   mtree/JCCL.java.dist
   mtree/JCCL.test.dist
   mtree/JCCL.tools.dist
   VARS.pl
   ]
   )

AC_OUTPUT
