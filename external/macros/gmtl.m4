dnl ************************************************************** ggt-head beg
dnl
dnl GGT: Generic Graphics Toolkit
dnl
dnl Original Authors:
dnl   Allen Bierbaum
dnl
dnl -----------------------------------------------------------------
dnl File:          gmtl.m4,v
dnl Date modified: 2006/11/09 21:40:01
dnl Version:       1.16.2.2
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
dnl    action-if-found     - The action to take if a GMTL implementation is
dnl                          found.  This argument is optional.
dnl    action-if-not-found - The action to take if a GMTL implementation is
dnl                          not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([GMTL_PATH],
[
   AC_PATH_PROG(FLAGPOLL, flagpoll, no)
   if test "x$FLAGPOLL" = "xno" ; then
      AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

      if pkg-config --atleast-pkgconfig-version 0.7 ; then
         QUERY_TOOL=$PKG_CONFIG
      else
         no_gmtl='yes'
         PKG_CONFIG='no'
      fi
   else
      QUERY_TOOL=$FLAGPOLL
      req_fp_major='0'
      req_fp_minor='8'
      req_fp_micro='1'
      req_fp_version="$req_fp_major.$req_fp_minor.$req_fp_micro"

      AC_MSG_CHECKING([whether flagpoll version is >= $req_fp_version])

      flagpoll_version=`$FLAGPOLL --version`
      fp_major=`echo $flagpoll_version | \
                  sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
      fp_minor=`echo $flagpoll_version | \
                  sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
      fp_micro=`echo $flagpoll_version | \
                  sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

      ok='no'
      if test "$fp_major" -gt "$req_major"; then
        ok='yes'
      else 
        if test "$fp_major" -eq "$req_fp_major"; then
            if test "$fp_minor" -gt "$req_fp_minor"; then
               ok='yes'
            elif test "$fp_minor" -ge "$req_fp_minor"; then
               if test "$fp_micro" -ge "$req_fp_micro"; then
                  ok='yes'
               fi
            fi
        fi
      fi

      AC_MSG_RESULT([$ok ($fp_major.$fp_minor.$fp_micro)])

      if test "x$ok" != "xyes" ; then
         AC_MSG_WARN([*** Flagpoll version is too old; version $req_fp_versio or better required. ***])
         no_gmtl='yes'
         FLAGPOLL='no'
      fi
   fi

   min_gmtl_version=ifelse([$1], ,0.4.11,$1)
   AC_MSG_CHECKING([for GMTL version >= $min_gmtl_version])

   GMTL_VERSION=`$QUERY_TOOL gmtl --modversion`
   if $QUERY_TOOL gmtl --atleast-version=$min_gmtl_version >/dev/null 2>&1
   then
      :
   else
      no_gmtl='yes'
   fi

   if test "x$no_gmtl" = "xyes" ; then
      AC_MSG_RESULT([no ($GMTL_VERSION)])

      ifelse([$3], , :, [$3])
   else
      AC_MSG_RESULT([yes ($GMTL_VERSION)])

      GMTL_CXXFLAGS=`$QUERY_TOOL gmtl --cflags`
      GMTL_INCLUDES=`$QUERY_TOOL gmtl --cflags-only-I`

      ifelse([$2], , :, [$2])
   fi

   AC_SUBST(GMTL_CXXFLAGS)
   AC_SUBST(GMTL_INCLUDES)
   AC_SUBST(GMTL_VERSION)
])
