dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
dnl
dnl Original Author:
dnl   Patrick Hartling
dnl ---------------------------------------------------------------------------
dnl VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
dnl File:          perl.m4,v
dnl Date modified: 2005/01/08 22:44:39
dnl Version:       1.12
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Perl-related macros.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PERL_VER - Find a version of Perl that is at least the given
dnl                    version number.
dnl
dnl Command-line options added:
dnl     --with-perl
dnl
dnl Variables defined:
dnl     PERL - The full path to the Perl binary meeting the requirements.
dnl ===========================================================================

dnl perl.m4,v 1.12 2005/01/08 22:44:39 patrickh Exp

AC_DEFUN([DPP_EXTRACT_PERL_VER],
[
   perl_ver_line=`$1 -v | grep -i 'this is perl'`
   changequote(<<, >>)
   PERL_VER_NUM=`echo $perl_ver_line | sed -e 's/^[^0-9]*\([1-9][^ ]*\) .*$/\1/'`
   split_ver_num=`echo $PERL_VER_NUM | sed -e 's/[\._-]/ /g'`
   PERL_MAJOR_VER=`echo $split_ver_num | awk '{ print $<<1>> }' -`
   PERL_MINOR_VER=`echo $split_ver_num | awk '{ print $<<2>> }' - | sed -e 's/^0*\([^0][1-9]*\)/\1/'`
   PERL_PATCH_VER=`echo $split_ver_num | awk '{ print $<<3>> }' - | sed -e 's/^0*\([^0][1-9]*\)/\1/'`
   changequote([, ])

   if test "x$PERL_PATCH_VER" = "x" ; then
      PERL_PATCH_VER=0
   fi
])

dnl ---------------------------------------------------------------------------
dnl Find a version of Perl that is at least the given version number.
dnl Substitution is performed on the variable $PERL.
dnl
dnl Usage:
dnl     DPP_PERL_VER(version, alternate-path)
dnl
dnl Arguments:
dnl     version        - Minimum required Perl version.
dnl     alternate-path - Alternate path(s) to check for the Perl installation.
dnl                      For more than one path value, the path list should be
dnl                      space-separated.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PERL_VER],
[
   dnl Specify a directory containing a working Perl 5.004 (or newer) binary.
   dnl There is no default since standard locations are always available.
   AC_ARG_WITH(perl,
               [  --with-perl=<PATH>      Directory containing Perl $1
                          or newer binary                 [No default]],
               dpp_user_perl_path="$withval")

   changequote(<<, >>)
   split_req_ver=`echo $1 | sed -e 's/\./ /g' -e 's/_/ /g'`
   req_perl_major=`echo $split_req_ver | awk '{ print $<<1>> }' -`
   req_perl_minor=`echo $split_req_ver | awk '{ print $<<2>> }' - | sed -e 's/^0*\([^0][1-9]*\)/\1/'`
   req_perl_patch=`echo $split_req_ver | awk '{ print $<<3>> }' - | sed -e 's/^0*\([^0][1-9]*\)/\1/'`
   changequote([, ])

   if test "x$req_perl_patch" = "x" ; then
      req_perl_patch=0
   fi

   chkPerl ( ) {
      if test -x "${1}" ; then
         DPP_EXTRACT_PERL_VER(${1})
         DPP_VERSION_COMPARE($PERL_MAJOR_VER, $PERL_MINOR_VER, $PERL_PATCH_VER,
                             $req_perl_major, $req_perl_minor, $req_perl_patch,
                             retval=0, retval=1)
      else
         retval=1
      fi

      echo $retval
   }

   dpp_perl_path='$2 /usr/local/bin /usr/bin C:/Perl/bin'

   dnl If $dpp_user_perl_path has a value, prepend that on $dpp_perl_path so
   dnl that it will be checked before the standard places.
   if test "x$dpp_user_perl_path" != "x" ; then
      dpp_perl_path="$dpp_user_perl_path $dpp_perl_path"
   fi

   AC_CACHE_CHECK([for Perl version >= $1], [dpp_cv_usable_perl],
      [ dpp_cv_usable_perl='no' ;
        for dpp_dir in $dpp_perl_path ; do
           if eval "test \"`chkPerl $dpp_dir/perl`\" -eq 0" ; then
              dpp_cv_usable_perl="$dpp_dir/perl"
              break
           fi
        done
      ])

   PERL="$dpp_cv_usable_perl"

   if test "x$PERL" = "xno" ; then
      ifelse([$4], , :, [$4])
   else
      DPP_EXTRACT_PERL_VER($PERL)
      ifelse([$3], , :, [$3])
   fi

   AC_SUBST(PERL)
])
