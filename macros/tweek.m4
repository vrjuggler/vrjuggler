dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl VR Juggler is (C) Copyright 1998-2003 by Iowa State University
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Christopher Just,
dnl   Patrick Hartling, Kevin Meinert,
dnl   Carolina Cruz-Neira, Albert Baker
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Library General Public
dnl License as published by the Free Software Foundation; either
dnl version 2 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public
dnl License along with this library; if not, write to the
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl NOTE: This should not be called by external code.
dnl ---------------------------------------------------------------------------
AC_DEFUN(_TWEEK_PATH_SETUP,
[
    dnl Get the cflags and libraries from the tweek-config script
    AC_ARG_WITH(tweek-prefix,
                [  --with-tweek-prefix=<PATH>
                          Prefix where Tweek is installed
                          (optional)                      [No default]],
                tweek_config_prefix="$withval", tweek_config_prefix="")
    AC_ARG_WITH(tweek-exec-prefix,
                [  --with-tweek-exec-prefix=<PATH>
                          Exec prefix where Tweek is
                          installed (optional)            [No default]],
                tweek_config_exec_prefix="$withval",
                tweek_config_exec_prefix="")
dnl    AC_ARG_ENABLE(tweektest,
dnl                  [  --disable-tweektest     Do not try to compile and run a
dnl                          test Tweek program], , enable_tweektest=yes)

    if test "x$tweek_config_exec_prefix" != "x" ; then
        tweek_config_args="$tweek_config_args --exec-prefix=$tweek_config_exec_prefix"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$tweek_config_exec_prefix/bin/tweek-config"
        fi
    fi

    if test "x$tweek_config_prefix" != "x" ; then
        tweek_config_args="$tweek_config_args --prefix=$tweek_config_prefix"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$tweek_config_prefix/bin/tweek-config"
        fi
    fi

    if test "x$TWEEK_BASE_DIR" != "x" ; then
        tweek_config_args="$tweek_config_args --prefix=$TWEEK_BASE_DIR"

        if test x${TWEEK_CONFIG+set} != xset ; then
            TWEEK_CONFIG="$TWEEK_BASE_DIR/bin/tweek-config"
        fi
    fi

    AC_PATH_PROG(TWEEK_CONFIG, tweek-config, no)

    dnl Do a sanity check to ensure that $TWEEK_CONFIG actually works.
    if ! (eval $TWEEK_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        TWEEK_CONFIG='no'
        echo "*** The tweek-config script installed by Tweek could not be found"
        echo "*** If Tweek was installed in PREFIX, make sure PREFIX/bin is in"
        echo "*** your path, or set the TWEEK_CONFIG environment variable to the"
        echo "*** full path to tweek-config."
    fi
])

dnl ---------------------------------------------------------------------------
dnl _TWEEK_VERSION_CHECK(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl NOTE: This should not be called by external code.
dnl ---------------------------------------------------------------------------
AC_DEFUN(_TWEEK_VERSION_CHECK,
[
   AC_REQUIRE([_TWEEK_PATH_SETUP])

   if test "x$TWEEK_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      TWEEK_VERSION=`$TWEEK_CONFIG --version`

      min_tweek_version=ifelse([$1], , 0.0.1, $1)
      AC_MSG_CHECKING([whether Tweek version is >= $min_tweek_version])
      AC_MSG_RESULT([$TWEEK_VERSION])
      DPP_VERSION_CHECK([$TWEEK_VERSION], [$min_tweek_version], $2, $3)
   fi
])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH_CXX([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for Tweek C++ API and then defines the following variables:
dnl     TWEEK_CXXFLAGS
dnl     TWEEK_CXXFLAGS_MIN
dnl     TWEEK_INCLUDES
dnl     TWEEK_INCLUDES_MIN
dnl     TWEEK_LIBS_LD
dnl     TWEEK_LIBS_LD_MIN
dnl     TWEEK_LIBS_STATIC_LD
dnl     TWEEK_LIBS_STATIC_LD_MIN
dnl     TWEEK_LIBS_CC
dnl     TWEEK_LIBS_CC_MIN
dnl     TWEEK_LIBS_STATIC_CC
dnl     TWEEK_LIBS_STATIC_CC_MIN
dnl     TWEEK_PROF_LIBS_LD
dnl     TWEEK_PROF_LIBS_LD_MIN
dnl     TWEEK_PROF_LIBS_STATIC_LD
dnl     TWEEK_PROF_LIBS_STATIC_LD_MIN
dnl     TWEEK_PROF_LIBS_CC
dnl     TWEEK_PROF_LIBS_CC_MIN
dnl     TWEEK_PROF_LIBS_STATIC_CC
dnl     TWEEK_PROF_LIBS_STATIC_CC_MIN
dnl     TWEEK_CXX_IDL
dnl     TWEEK_CXX_IDL_OPTS
dnl     TWEEK_CXX_IDL_GENDIR_OPT
dnl ---------------------------------------------------------------------------
AC_DEFUN(TWEEK_PATH_CXX,
[
   AC_REQUIRE([_TWEEK_PATH_SETUP])

   TWEEK_CXXFLAGS=""
   TWEEK_CXXFLAGS_MIN=""
   TWEEK_INCLUDES=""
   TWEEK_INCLUDES_MIN=""
   TWEEK_LIBS_LD=""
   TWEEK_LIBS_LD_MIN=""
   TWEEK_LIBS_STATIC_LD=""
   TWEEK_LIBS_STATIC_LD_MIN=""
   TWEEK_LIBS_CC=""
   TWEEK_LIBS_CC_MIN=""
   TWEEK_LIBS_STATIC_CC=""
   TWEEK_LIBS_STATIC_CC_MIN=""
   TWEEK_PROF_LIBS_LD=""
   TWEEK_PROF_LIBS_LD_MIN=""
   TWEEK_PROF_LIBS_STATIC_LD=""
   TWEEK_PROF_LIBS_STATIC_LD_MIN=""
   TWEEK_PROF_LIBS_CC=""
   TWEEK_PROF_LIBS_CC_MIN=""
   TWEEK_PROF_LIBS_STATIC_CC=""
   TWEEK_PROF_LIBS_STATIC_CC_MIN=""
   TWEEK_CXX_IDL=""
   TWEEK_CXX_IDL_OPTS=""
   TWEEK_CXX_IDL_GENDIR_OPT=""
   TWEEK_CXX_IDL_INCFLAG=""

   if test "x$TWEEK_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      AC_MSG_CHECKING([whether Tweek C++ API is available])
      has_cxx=`$TWEEK_CONFIG --too-much-pressure`

      if test "x$has_cxx" = "xY" ; then
         AC_MSG_RESULT([yes])
         if test "x$TWEEK_VERSION" = "x" ; then
            _TWEEK_VERSION_CHECK($1, [tweek_version_okay='yes'],
                                 [tweek_version_okay='no'
                                  $3])
         fi

         if test "x$tweek_version_okay" = "xyes" ; then
            TWEEK_CXXFLAGS=`$TWEEK_CONFIG $tweek_config_args --cxxflags $ABI`
            TWEEK_INCLUDES=`$TWEEK_CONFIG $tweek_config_args --includes`
            TWEEK_EXTRA_LIBS_CC=`$TWEEK_CONFIG $tweek_config_args --extra-libs $ABI`
            TWEEK_EXTRA_LIBS_LD=`$TWEEK_CONFIG $tweek_config_args --extra-libs $ABI --linker`
            TWEEK_LIBS_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI` $TWEEK_EXTRA_LIBS_CC"
            TWEEK_LIBS_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker` $TWEEK_EXTRA_LIBS_LD"
            TWEEK_PROF_LIBS_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --profiled` $TWEEK_EXTRA_LIBS_CC"
            TWEEK_PROF_LIBS_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --profiled` $TWEEK_EXTRA_LIBS_LD"
            TWEEK_LIBS_STATIC_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --static` $TWEEK_EXTRA_LIBS_CC"
            TWEEK_LIBS_STATIC_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --static` $TWEEK_EXTRA_LIBS_LD"
            TWEEK_PROF_LIBS_STATIC_CC="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --static --profiled` $TWEEK_EXTRA_LIBS_CC"
            TWEEK_PROF_LIBS_STATIC_LD="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --static --profiled` $TWEEK_EXTRA_LIBS_LD"

            TWEEK_CXX_IDL="`$TWEEK_CONFIG $tweek_config_args --idl cxx`"
            TWEEK_CXX_IDL_OPTS="`$TWEEK_CONFIG $tweek_config_args --idlflags cxx`"
            TWEEK_CXX_IDL_GENDIR_OPT="`$TWEEK_CONFIG $tweek_config_args --idlgendir cxx`"
            TWEEK_CXX_IDL_INCFLAG="`$TWEEK_CONFIG $tweek_config_args --idlincflag cxx`"

            TWEEK_CXXFLAGS_MIN=`$TWEEK_CONFIG $tweek_config_args --cxxflags $ABI --min`
            TWEEK_INCLUDES_MIN=`$TWEEK_CONFIG $tweek_config_args --includes --min`
            TWEEK_EXTRA_LIBS_CC_MIN=`$TWEEK_CONFIG $tweek_config_args --extra-libs $ABI --min`
            TWEEK_EXTRA_LIBS_LD_MIN=`$TWEEK_CONFIG $tweek_config_args --extra-libs $ABI --linker --min`
            TWEEK_LIBS_CC_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --min`"
            TWEEK_LIBS_LD_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --min`"
            TWEEK_PROF_LIBS_CC_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --min --profiled`"
            TWEEK_PROF_LIBS_LD_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --min --profiled`"
            TWEEK_LIBS_STATIC_CC_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --static --min`"
            TWEEK_LIBS_STATIC_LD_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --static --min`"
            TWEEK_PROF_LIBS_STATIC_CC_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --static --min --profiled`"
            TWEEK_PROF_LIBS_STATIC_LD_MIN="`$TWEEK_CONFIG $tweek_config_args --libs $ABI --linker --static --min --profiled`"

            ifelse([$2], , :, [$2])
         fi
      else
         AC_MSG_RESULT([no])
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST([TWEEK_CXXFLAGS])
   AC_SUBST([TWEEK_LIBS_CC])
   AC_SUBST([TWEEK_LIBS_LD])
   AC_SUBST([TWEEK_PROF_LIBS_CC])
   AC_SUBST([TWEEK_PROF_LIBS_LD])
   AC_SUBST([TWEEK_LIBS_STATIC_LD])
   AC_SUBST([TWEEK_LIBS_STATIC_CC])
   AC_SUBST([TWEEK_PROF_LIBS_STATIC_LD])
   AC_SUBST([TWEEK_PROF_LIBS_STATIC_CC])

   AC_SUBST([TWEEK_CXX_IDL])
   AC_SUBST([TWEEK_CXX_IDL_OPTS])
   AC_SUBST([TWEEK_CXX_IDL_GENDIR_OPT])
   AC_SUBST([TWEEK_CXX_IDL_INCFLAG])

   AC_SUBST([TWEEK_CXXFLAGS_MIN])
   AC_SUBST([TWEEK_INCLUDES_MIN])
   AC_SUBST([TWEEK_LIBS_CC_MIN])
   AC_SUBST([TWEEK_LIBS_LD_MIN])
   AC_SUBST([TWEEK_PROF_LIBS_CC_MIN])
   AC_SUBST([TWEEK_PROF_LIBS_LD_MIN])
   AC_SUBST([TWEEK_LIBS_STATIC_CC_MIN])
   AC_SUBST([TWEEK_LIBS_STATIC_LD_MIN])
   AC_SUBST([TWEEK_PROF_LIBS_STATIC_CC_MIN])
   AC_SUBST([TWEEK_PROF_LIBS_STATIC_LD_MIN])
])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH_JAVA([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for Tweek Java API and then defines the following variables:
dnl     TWEEK_JAVA_IDL
dnl     TWEEK_JAVA_IDL_OPTS
dnl     TWEEK_JAVA_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL_INCFLAG
dnl     TWEEK_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(TWEEK_PATH_JAVA,
[
   AC_REQUIRE([_TWEEK_PATH_SETUP])

   TWEEK_JAVA_IDL=''
   TWEEK_JAVA_IDL_OPTS=''
   TWEEK_JAVA_IDL_GENDIR_OPT=''
   TWEEK_JAVA_IDL_INCFLAG=''
   TWEEK_JARS=''
   TWEEK_EXT_JARS=''

   if test "x$TWEEK_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      AC_MSG_CHECKING([whether Tweek Java API is available])
      has_java=`$TWEEK_CONFIG --is-jittery`

      if test "x$has_java" = "xY" ; then
         AC_MSG_RESULT([yes])
         if test "x$TWEEK_VERSION" = "x" ; then
            _TWEEK_VERSION_CHECK($1, [tweek_version_okay='yes'],
                                 [tweek_version_okay='no'
                                  $3])
         fi

         if test "x$tweek_version_okay" = "xyes" ; then
            TWEEK_JAVA_IDL="`$TWEEK_CONFIG $tweek_config_args --idl java`"
            TWEEK_JAVA_IDL_OPTS="`$TWEEK_CONFIG $tweek_config_args --idlflags java`"
            TWEEK_JAVA_IDL_GENDIR_OPT="`$TWEEK_CONFIG $tweek_config_args --idlgendir java`"
            TWEEK_JAVA_IDL_INCFLAG="`$TWEEK_CONFIG $tweek_config_args --idlincflag java`"
            TWEEK_JARS="`$TWEEK_CONFIG $tweek_config_args --jars`"
            TWEEK_EXT_JARS="`$TWEEK_CONFIG $tweek_config_args --ext-jars`"

            ifelse([$2], , :, [$2])
         fi
      else
         AC_MSG_RESULT([no])
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST([TWEEK_JAVA_IDL])
   AC_SUBST([TWEEK_JAVA_IDL_OPTS])
   AC_SUBST([TWEEK_JAVA_IDL_GENDIR_OPT])
   AC_SUBST([TWEEK_JAVA_IDL_INCFLAG])
   AC_SUBST([TWEEK_JARS])
   AC_SUBST([TWEEK_EXT_JARS])
])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for Tweek C++ and Java APIs and then defines the following
dnl variables:
dnl     TWEEK_CXXFLAGS
dnl     TWEEK_CXXFLAGS_MIN
dnl     TWEEK_LIBS_LD
dnl     TWEEK_LIBS_LD_MIN
dnl     TWEEK_LIBS_STATIC_LD
dnl     TWEEK_LIBS_STATIC_LD_MIN
dnl     TWEEK_LIBS_CC
dnl     TWEEK_LIBS_CC_MIN
dnl     TWEEK_LIBS_STATIC_CC
dnl     TWEEK_LIBS_STATIC_CC_MIN
dnl     TWEEK_CXX_IDL
dnl     TWEEK_CXX_IDL_OPTS
dnl     TWEEK_CXX_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL
dnl     TWEEK_JAVA_IDL_OPTS
dnl     TWEEK_JAVA_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL_INCFLAG
dnl     TWEEK_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(TWEEK_PATH,
[
   TWEEK_PATH_CXX($1, [tweek_have_cxx='yes'], $3, $4)

   if test "x$tweek_have_cxx" = "xyes" ; then
      TWEEK_PATH_JAVA($1, $2, $3, $4)
   fi
])
