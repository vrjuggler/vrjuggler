dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl NOTE: This should not be called by external code.
dnl ---------------------------------------------------------------------------
AC_DEFUN([_TWEEK_PATH_SETUP],
[
    dnl Get the cflags and libraries from flagpoll
    AC_ARG_WITH(tweek-cxx-meta-file,
                [  --with-tweek-cxx-meta-file=<PATH>
                          Flagpoll metadata file for
                          Tweek C++ API (optional)        [No default]],
                [tweek_cxx_meta_file="$withval"], [tweek_cxx_meta_file=""])
    AC_ARG_WITH(tweek-java-meta-file,
                [  --with-tweek-java-meta-file=<PATH>
                          Flagpoll metadata file for
                          Tweek Java API (optional)       [No default]],
                [tweek_java_meta_file="$withval"], [tweek_java_meta_file=""])

    dnl See if the user specified where to find the Tweek C++ API meta file.
    dnl If not, take a guess.
    if test "x$tweek_cxx_meta_file" != "x" ; then
        tweek_cxx_meta_dir=`dirname $tweek_cxx_meta_file`
        tweek_cxx_flagpoll_args="--extra-paths=$tweek_cxx_meta_dir tweek --from-file=$tweek_cxx_meta_file"
    elif test -f "$instlinks/share/flagpoll/tweek.fpc" ; then
        tweek_cxx_meta_dir="$instlinks/share/flagpoll"
        tweek_cxx_flagpoll_args="--extra-paths=$tweek_cxx_meta_dir tweek --from-file=$tweek_cxx_meta_dir/tweek.fpc"
    else
        tweek_cxx_flagpoll_args="tweek"
    fi

    dnl See if the user specified where to find the Tweek Java API meta file.
    dnl If not, take a guess.
    if test "x$tweek_java_meta_file" != "x" ; then
        tweek_java_meta_dir=`dirname $tweek_java_meta_file`
        tweek_java_flagpoll_args="--extra-paths=$tweek_java_meta_dir tweek-java --from-file=$tweek_java_meta_file"
    elif test -f "$instlinks/share/flagpoll/tweek.fpc" ; then
        tweek_java_meta_dir="$instlinks/share/flagpoll"
        tweek_java_flagpoll_args="--extra-paths=$tweek_java_meta_dir tweek-java --from-file=$tweek_java_meta_dir/tweek-java.fpc"
    else
        tweek_java_flagpoll_args="tweek-java"
    fi

    dnl See if the user specified where to find the Tweek Python API meta
    dnl file. If not, take a guess.
    if test "x$tweek_python_meta_file" != "x" ; then
        tweek_python_meta_dir=`dirname $tweek_python_meta_file`
        tweek_python_flagpoll_args="--extra-paths=$tweek_python_meta_dir tweek-python --from-file=$tweek_python_meta_file"
    elif test -f "$instlinks/share/flagpoll/tweek.fpc" ; then
        tweek_python_meta_dir="$instlinks/share/flagpoll"
        tweek_python_flagpoll_args="--extra-paths=$tweek_python_meta_dir tweek-python --from-file=$tweek_python_meta_dir/tweek-python.fpc"
    else
        tweek_python_flagpoll_args="tweek-python"
    fi

    AM_PATH_FLAGPOLL([0.9.1], ,
                     [AC_MSG_ERROR(*** Flagpoll required for Tweek flags ***)])

    dnl Do a sanity check to ensure that $FLAGPOLL actually works.
    if ! (eval $FLAGPOLL --help >/dev/null 2>&1) 2>&1 ; then
        FLAGPOLL='no'
        echo "*** Flagpoll is required to build Tweek."
        echo "*** Please check that the PATH variable is set to "
        echo "*** include the proper path to flagpoll."
    fi
])

dnl ---------------------------------------------------------------------------
dnl _TWEEK_VERSION_CHECK(minimum-version, [action-if-found [, action-if-not-found]])
dnl NOTE: This should not be called by external code.
dnl ---------------------------------------------------------------------------
AC_DEFUN([_TWEEK_VERSION_CHECK],
[
   DPP_PREREQ([2.0.1])

   if test "x$tweek_cxx_flagpoll_args" = "x" ; then
      _TWEEK_PATH_SETUP()
   fi

   if test "x$FLAGPOLL" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      TWEEK_VERSION=`$FLAGPOLL $tweek_cxx_flagpoll_args --modversion`

      min_tweek_version=ifelse([$1], ,0.0.1,[$1])
      DPP_VERSION_CHECK_MSG_NO_CACHE([Tweek], [$TWEEK_VERSION],
                                     [$min_tweek_version], [$2], [$3])
   fi
])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH_CXX([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Tests for Tweek C++ API and then defines the following variables:
dnl     TWEEK_CXXFLAGS
dnl     TWEEK_LIBS
dnl     TWEEK_LIBS_STATIC
dnl     TWEEK_PROF_LIBS
dnl     TWEEK_PROF_LIBS_STATIC
dnl     TWEEK_CXX_IDL
dnl     TWEEK_CXX_IDL_OPTS
dnl     TWEEK_CXX_IDL_GENDIR_OPT
dnl ---------------------------------------------------------------------------
AC_DEFUN([TWEEK_PATH_CXX],
[
   if test "x$tweek_cxx_flagpoll_args" = "x" ; then
      _TWEEK_PATH_SETUP()
   fi

   TWEEK_CXXFLAGS=""
   TWEEK_LIBS=""
   TWEEK_LIBS_STATIC=""
   TWEEK_PROF_LIBS=""
   TWEEK_PROF_LIBS_STATIC=""
   TWEEK_CXX_IDL=""
   TWEEK_CXX_IDL_OPTS=""
   TWEEK_CXX_IDL_GENDIR_OPT=""
   TWEEK_CXX_IDL_INCFLAG=""

   if test "x$FLAGPOLL" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      if ! (eval $FLAGPOLL $tweek_cxx_flagpoll_args --modversion >/dev/null 2>&1)
      then
         AC_MSG_WARN([*** Flagpoll has no valid Tweek C++ configuration ***])
         ifelse([$3], , :, [$3])
      else
         AC_MSG_CHECKING([whether Tweek C++ API is available])
         has_cxx=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-cxx-api-available`

         if test "x$has_cxx" = "xY" ; then
            AC_MSG_RESULT([yes])
            if test "x$TWEEK_VERSION" = "x" ; then
               _TWEEK_VERSION_CHECK($1, [tweek_version_okay='yes'],
                                    [tweek_version_okay='no'
                                     $3])
            fi

            if test "x$tweek_version_okay" = "xyes" ; then
               TWEEK_CXXFLAGS=`$FLAGPOLL $tweek_cxx_flagpoll_args --cflags`
               TWEEK_LIBS=`$FLAGPOLL $tweek_cxx_flagpoll_args --libs`
               TWEEK_EXTRA_LIBS=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-extra-libs`
               TWEEK_PROF_LIBS=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-profiled_libs`
               TWEEK_LIBS_STATIC=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-static-libs`
               TWEEK_PROF_LIBS_STATIC_LD=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-profiled-static-libs`
               TWEEK_DATA_SUBDIR=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-tweek-data-subdir`

               TWEEK_CXX_IDL=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-cxx-idl`
               TWEEK_CXX_IDL_OPTS=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-cxx-idlflags`
               TWEEK_CXX_IDL_GENDIR_OPT=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-cxx-idlgendir`
               TWEEK_CXX_IDL_INCFLAG=`$FLAGPOLL $tweek_cxx_flagpoll_args --get-cxx-idlincflag`

               ifelse([$2], , :, [$2])
            fi
         else
            AC_MSG_RESULT([no])
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_SUBST([TWEEK_CXXFLAGS])
   AC_SUBST([TWEEK_LIBS])
   AC_SUBST([TWEEK_PROF_LIBS])
   AC_SUBST([TWEEK_LIBS_STATIC])
   AC_SUBST([TWEEK_PROF_LIBS_STATIC])

   AC_SUBST([TWEEK_CXX_IDL])
   AC_SUBST([TWEEK_CXX_IDL_OPTS])
   AC_SUBST([TWEEK_CXX_IDL_GENDIR_OPT])
   AC_SUBST([TWEEK_CXX_IDL_INCFLAG])

])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH_JAVA([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Tests for Tweek Java API and then defines the following variables:
dnl     TWEEK_JAVA_IDL
dnl     TWEEK_JAVA_IDL_OPTS
dnl     TWEEK_JAVA_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL_INCFLAG
dnl     TWEEK_EXT_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN([TWEEK_PATH_JAVA],
[
   if test "x$tweek_java_flagpoll_args" = "x" ; then
      _TWEEK_PATH_SETUP()
   fi

   TWEEK_JAVA_IDL=''
   TWEEK_JAVA_IDL_OPTS=''
   TWEEK_JAVA_IDL_GENDIR_OPT=''
   TWEEK_JAVA_IDL_INCFLAG=''
   TWEEK_JARS=''
   TWEEK_EXT_JARS=''

   if test "x$FLAGPOLL" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      if ! (eval $FLAGPOLL $tweek_java_flagpoll_args --modversion >/dev/null 2>&1)
      then
         AC_MSG_WARN([*** Flagpoll has no valid Tweek Java configuration ***])
         ifelse([$3], , :, [$3])
      else
         AC_MSG_CHECKING([whether Tweek Java API is available])
         has_java=`$FLAGPOLL $tweek_java_flagpoll_args --get-java-api-available`

         if test "x$has_java" = "xY" ; then
            AC_MSG_RESULT([yes])
            if test "x$TWEEK_VERSION" = "x" ; then
               _TWEEK_VERSION_CHECK($1, [tweek_version_okay='yes'],
                                    [tweek_version_okay='no'
                                     $3])
            fi

            if test "x$tweek_version_okay" = "xyes" ; then
               TWEEK_JAVA_IDL=`$FLAGPOLL $tweek_java_flagpoll_args --get-java-idl`
               TWEEK_JAVA_IDL_OPTS=`$FLAGPOLL $tweek_java_flagpoll_args --get-java-idlflags`
               TWEEK_JAVA_IDL_GENDIR_OPT=`$FLAGPOLL $tweek_java_flagpoll_args --get-java-idlgendir`
               TWEEK_JAVA_IDL_INCFLAG=`$FLAGPOLL $tweek_java_flagpoll_args --get-java-idlincflag`
               TWEEK_JARS=`$FLAGPOLL $tweek_java_flagpoll_args --get-jars`
               TWEEK_EXT_JARS=`$FLAGPOLL $tweek_java_flagpoll_args --get-ext-jars`
               TWEEK_DATA_SUBDIR=`$FLAGPOLL $tweek_java_flagpoll_args --get-tweek-data-subdir`

               ifelse([$2], , :, [$2])
            fi
         else
            AC_MSG_RESULT([no])
            ifelse([$3], , :, [$3])
         fi
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
dnl TWEEK_PATH_PYTHON([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Tests for Tweek Python API and then defines the following variables:
dnl     TWEEK_PYTHON_IDL
dnl     TWEEK_PYTHON_IDL_OPTS
dnl     TWEEK_PYTHON_IDL_GENDIR_OPT
dnl     TWEEK_PYTHON_IDL_INCFLAG
dnl ---------------------------------------------------------------------------
AC_DEFUN([TWEEK_PATH_PYTHON],
[
   if test "x$tweek_python_flagpoll_args" = "x" ; then
      _TWEEK_PATH_SETUP()
   fi

   TWEEK_PYTHON_IDL=''
   TWEEK_PYTHON_IDL_OPTS=''
   TWEEK_PYTHON_IDL_GENDIR_OPT=''
   TWEEK_PYTHON_IDL_INCFLAG=''

   if test "x$FLAGPOLL" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      AC_MSG_CHECKING([whether Tweek Python API is available])
      has_java=`$FLAGPOLL $tweek_python_flagpoll_args --get-python-idl-available`

      if test "x$has_java" = "xY" ; then
         AC_MSG_RESULT([yes])
         if test "x$TWEEK_VERSION" = "x" ; then
            _TWEEK_VERSION_CHECK($1, [tweek_version_okay='yes'],
                                 [tweek_version_okay='no'
                                  $3])
         fi

         if test "x$tweek_version_okay" = "xyes" ; then
            TWEEK_PYTHON_IDL=`$FLAGPOLL $tweek_python_flagpoll_args --get-python-idl`
            TWEEK_PYTHON_IDL_OPTS=`$FLAGPOLL $tweek_python_flagpoll_args ---get-python-idlflags`
            TWEEK_PYTHON_IDL_GENDIR_OPT=`$FLAGPOLL $tweek_python_flagpoll_args --get-python-idlgendir`
            TWEEK_PYTHON_IDL_INCFLAG=`$FLAGPOLL $tweek_python_flagpoll_args --get-python-idlincflag`
            TWEEK_DATA_SUBDIR=`$FLAGPOLL $tweek_python_flagpoll_args --get-tweek-data-subdir`

            ifelse([$2], , :, [$2])
         fi
      else
         AC_MSG_RESULT([no])
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST([TWEEK_PYTHON_IDL])
   AC_SUBST([TWEEK_PYTHON_IDL_OPTS])
   AC_SUBST([TWEEK_PYTHON_IDL_GENDIR_OPT])
   AC_SUBST([TWEEK_PYTHON_IDL_INCFLAG])
])

dnl ---------------------------------------------------------------------------
dnl TWEEK_PATH([minimum-version, [action-if-found [, action-if-not-found]]])
dnl
dnl Tests for Tweek C++, Java, and Python APIs and then defines the following
dnl variables:
dnl     TWEEK_CXXFLAGS
dnl     TWEEK_LIBS_LD
dnl     TWEEK_LIBS_STATIC_LD
dnl     TWEEK_LIBS_CC
dnl     TWEEK_LIBS_STATIC_CC
dnl     TWEEK_CXX_IDL
dnl     TWEEK_CXX_IDL_OPTS
dnl     TWEEK_CXX_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL
dnl     TWEEK_JAVA_IDL_OPTS
dnl     TWEEK_JAVA_IDL_GENDIR_OPT
dnl     TWEEK_JAVA_IDL_INCFLAG
dnl     TWEEK_JARS
dnl     TWEEK_EXT_JARS
dnl     TWEEK_PYTHON_IDL
dnl     TWEEK_PYTHON_IDL_OPTS
dnl     TWEEK_PYTHON_IDL_GENDIR_OPT
dnl     TWEEK_PYTHON_IDL_INCFLAG
dnl ---------------------------------------------------------------------------
AC_DEFUN([TWEEK_PATH],
[
   TWEEK_PATH_CXX($1, [tweek_have_cxx='yes'], $3)

   if test "x$tweek_have_cxx" = "xyes" ; then
      TWEEK_PATH_JAVA($1, $2, $3)
      TWEEK_PATH_PYTHON($1, $2, $3)
   fi
])
