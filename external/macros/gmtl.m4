dnl ************************************************************** ggt-head beg
dnl
dnl GGT: Generic Graphics Toolkit
dnl
dnl Original Authors:
dnl   Allen Bierbaum
dnl
dnl -----------------------------------------------------------------
dnl File:          gmtl.m4,v
dnl Date modified: 2006/04/13 21:49:39
dnl Version:       1.16
dnl -----------------------------------------------------------------
dnl
dnl ************************************************************** ggt-head end

dnl *************************************************************** ggt-cpr beg
dnl GGT: The Generic Graphics Toolkit
dnl Copyright (C) 2001,2002 Allen Bierbaum 
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
dnl
dnl *************************************************************** ggt-cpr end

dnl ---------------------------------------------------------------------------
dnl Test for GMTL and then define GMTL_CXXFLAGS, GMTL_INCLUDES, and
dnl GMTL_VERSION.
dnl
dnl Usage:
dnl    GMTL_PATH(version , gmtl-root [, include-path [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl    version             - The minimum required version.
dnl    gmtl-root           - The default directory where the GMTL installation
dnl                          is rooted.  This directory should contain an
dnl                          include/gmtl directory with the GMTL headers.
dnl                          The value given is used as the default value of
dnl                          the --with-gmtl-prefix command-line argument.
dnl    gmtl-inc            - The directory where the GMTL headers can be
dnl                          found.  This is used to override the use of
dnl                          <gmtl-root>/include as the default path to
dnl                          those headers.  This argument is optional.
dnl    action-if-found     - The action to take if a GMTL implementation is
dnl                          found.  This argument is optional.
dnl    action-if-not-found - The action to take if a GMTL implementation is
dnl                          not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([GMTL_PATH],
[
   dnl Get the cflags and libraries from the gmtl-config script
   AC_ARG_WITH(gmtl,
               [  --with-gmtl=<PATH>      Directory where GMTL is
                          installed                       [default=$2]],
               [GMTL_ROOT="$withval"], [GMTL_ROOT=$2])
   AC_ARG_WITH(gmtl-includes,
               [  --with-gmtl-includes=<DIR>
                          GMTL header file directory      [No default]],
               [gmtl_inc_dir="$withval"],
               ifelse([$3], , [gmtl_inc_dir='no'], [gmtl_inc_dir=$3]))
   AC_ARG_WITH(gmtl-exec-prefix,
               [  --with-gmtl-exec-prefix=<PATH>
                          Exec prefix where GMTL is
                          installed (optional)            [No default]],
               gmtl_config_exec_prefix="$withval", gmtl_config_exec_prefix="")

   if test "x$gmtl_config_exec_prefix" != "x" ; then
      gmtl_config_args="$gmtl_config_args --exec-prefix=$gmtl_config_exec_prefix"

      if test x${GMTL_CONFIG+set} != xset ; then
         GMTL_CONFIG="$gmtl_config_exec_prefix/bin/gmtl-config"
      fi
   fi

   if test "x$GMTL_ROOT" != "x" ; then
      gmtl_config_args="$gmtl_config_args --prefix=$GMTL_ROOT"

      if test x${GMTL_CONFIG+set} != xset ; then
         GMTL_CONFIG="$GMTL_ROOT/bin/gmtl-config"
      fi
   fi

   if test "x$GMTL_BASE_DIR" != "x" ; then
      gmtl_config_args="$gmtl_config_args --prefix=$GMTL_BASE_DIR"

      if test x${GMTL_CONFIG+set} != xset ; then
         GMTL_CONFIG="$GMTL_BASE_DIR/bin/gmtl-config"
      fi
   fi

   AC_PATH_PROG(GMTL_CONFIG, gmtl-config, no)
   min_gmtl_version=ifelse([$1], ,0.0.1,$1)

   dnl Do a sanity check to ensure that $GMTL_CONFIG actually works.
   if ! (eval $GMTL_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
      GMTL_CONFIG='no'
   fi

   dnl If $GMTL_ROOT has no value, use $GMTL_CONFIG to get it.
   if test "x$GMTL_ROOT" = "x" ; then
      GMTL_ROOT=`$GMTL_CONFIG $gmtl_config_args --prefix`
   fi

   GMTL_CXXFLAGS=''

   no_gmtl='no'
   if test "x$GMTL_CONFIG" = "xno" ; then
      no_gmtl='yes'
   else
      GMTL_CXXFLAGS=`$GMTL_CONFIG $gmtl_config_args --cxxflags`
   fi

   ggt_save_CPPFLAGS="$CPPFLAGS"
   CPPFLAGS="$GMTL_CXXFLAGS $CPPFLAGS"

   dnl Add the user-specified GMTL installation directory to the preprocessor
   dnl arguments.  Ensure that /usr/include is not included multiple times if
   dnl $GMTL_ROOT is "/usr".  The goal here is to let the user specify either
   dnl the GMTL root directory, the GMTL include directory, or both.
   if test "x$GMTL_ROOT" != "x/usr" -o "x$gmtl_inc_dir" != "xno" ; then
      dnl No GMTL include directory, so append "/include" to $GMTL_ROOT.
      if test "x$gmtl_inc_dir" = "xno" ; then
         ggt_gmtl_incdir="$GMTL_ROOT/include"
      dnl We have a GMTL include directory, so we'll use it and hope for
      dnl the best.
      else
         ggt_gmtl_incdir="$gmtl_inc_dir"
      fi

      CPPFLAGS="$CPPFLAGS -I$ggt_gmtl_incdir"
   else
      ggt_gmtl_incdir="$GMTL_ROOT/include"
   fi

   AC_LANG_SAVE
   AC_LANG_CPLUSPLUS
   AC_CHECK_HEADER([gmtl/Version.h], [ggt_have_gmtl='yes'], [$5])
   AC_LANG_RESTORE

   dnl Restore all the variables now that we are done testing.
   CPPFLAGS="$ggt_save_CPPFLAGS"

   dnl Do the version number comparison.
   if test "x$ggt_have_gmtl" = "xyes" ; then
      dnl This expression passed to grep(1) is not great.  It could stand to
      dnl test for one or more whitespace characters instead of just one for
      dnl book-ending GMTL_VERSION_*.
      gmtl_patch=`grep 'define GMTL_VERSION_PATCH ' $ggt_gmtl_incdir/gmtl/Version.h | awk '{ print $[3] }' -`
      gmtl_minor=`grep 'define GMTL_VERSION_MINOR ' $ggt_gmtl_incdir/gmtl/Version.h | awk '{ print $[3] }' -`
      gmtl_major=`grep 'define GMTL_VERSION_MAJOR ' $ggt_gmtl_incdir/gmtl/Version.h | awk '{ print $[3] }' -`
      gmtl_version="$gmtl_major.$gmtl_minor.$gmtl_patch"

      AC_MSG_CHECKING([whether GMTL version is >= $1])
      AC_MSG_RESULT([$gmtl_version])
      DPP_VERSION_CHECK([$gmtl_version], [$1], ,
                        [ggt_have_gmtl='no'
                        $5])
   fi

   if test "x$ggt_have_gmtl" = "xyes" ; then
      ifelse([$4], , :, [$4])
   fi

   dnl If GMTL API files were found, define this extra stuff that may be
   dnl helpful in some Makefiles.
   if test "x$ggt_have_gmtl" = "xyes" ; then
      if test "x$ggt_gmtl_incdir" != "x" ; then
         GMTL_INCLUDES="-I$ggt_gmtl_incdir"
      fi

      GMTL='yes'
   fi

   dnl XXX: Get this working again.
dnl   if test "x$no_gmtl" = "xyes" ; then
dnl      if test "$GMTL_CONFIG" = "no" ; then
dnl         echo "*** The gmtl-config script installed by GMTL could not be found"
dnl         echo "*** If GMTL was installed in PREFIX, make sure"
dnl         echo "*** PREFIX/include/gmtl exists." 
dnl      fi
dnl      GMTL_VERSION='-1'
dnl      ifelse([$4], , :, [$4])
dnl   fi

   AC_SUBST(GMTL_CXXFLAGS)
   AC_SUBST(GMTL_INCLUDES)
   AC_SUBST(GMTL_VERSION)
])
