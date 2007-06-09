dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2007 by Iowa State University
dnl
dnl Original Author:
dnl   Patrick Hartling
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
dnl File:          ace.m4,v
dnl Date modified: 2006/12/31 16:55:00
dnl Version:       1.18
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide command-line functions and checks for including the ACE library
dnl in the configuration process.  This does the substitution of the variable
dnl defined here (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_ACE - Determine if the ACE is on the system.
dnl     DPP_ACE_VER  - Make sure that the installed ACE version is at least
dnl                    the given version number.
dnl
dnl Command-line options added:
dnl     --with-ace          - Define the path to the ACE installation.
dnl     --with-ace-includes - Define the path to the ACE headers.
dnl     --with-ace-libs     - Define the path to the ACE libraries.
dnl     --with-acever       - Define the ACE version number.
dnl
dnl Variables defined:
dnl     ACE_ROOT     - The root of the ACE installation.
dnl     ACE_INCDIR   - The path to the ACE header files.
dnl     ACE_LIBDIR   - The path to the ACE library files.
dnl     ACE_INCLUDES - The compiler argument to add the ACE include path.
dnl     ACE_LDFLAGS  - The compiler argument to add the ACE library path and
dnl                    libraries.
dnl ===========================================================================

dnl	ace.m4,v 1.18 2006/12/31 16:55:00 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     DPP_HAVE_ACE(required-version [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     ace-root            - Default ACE root directory.
dnl     version             - Minimum required ACE version.
dnl     action-if-found     - Action to take if the ACE is found.
dnl     action-if-not-found - Action to take if the ACE could not be found or
dnl                           if the ACE version requirement is not met.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_ACE],
[
   AC_ARG_WITH([ace],
               [  --with-ace=<PATH>       ACE installation directory
                             [default=$1 or \$ACE_ROOT if defined]],
               ACE_ROOT="$withval", ACE_ROOT="$1")

   dnl Define the directory where the ACE header files can be found.
   AC_ARG_WITH(ace-includes,
               [  --with-ace-includes=<PATH>
                         Path to ACE header file directory
                                             [default=\$ACE_ROOT/include]],
               ACE_INCDIR="$withval", ACE_INCDIR="$ACE_ROOT/include")

   dnl Define the directory where the ACE libraries can be found.
   AC_ARG_WITH(ace-libs,
               [  --with-ace-libs=<PATH>  Path to ACE libraries           [default=\$ACE_ROOT/lib]],
               ACE_LIBDIR="$withval", ACE_LIBDIR="$ACE_ROOT/lib")

   dpp_have_ace='no'

   if test "x$ACE_ROOT" != "xno" ; then
      dnl Check the ACE version if a version number was given.
      if test "x$1" != "x" ; then
         DPP_ACE_VER([$ACE_INCDIR $ACE_ROOT], $2, $3, $4)
      else
         AC_MSG_ERROR([*** No ACE version given! ***])
      fi

      dpp_save_CXXFLAGS="$CXXFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"
      dpp_save_LIBS="$LIBS"

      if test "x$ACE_ROOT" != "x/usr" ; then
         if test "x$ACE_INCDIR" != "x$dpp_ace_header_dir" ; then
            ACE_INCLUDES="-I$dpp_ace_header_dir -I$ACE_INCDIR"
         else
            ACE_INCLUDES="-I$ACE_INCDIR"
         fi

         LDFLAGS="-L$ACE_LIBDIR $LDFLAGS"
      fi

      CPPFLAGS="$CPPFLAGS $ACE_INCLUDES"
      CXXFLAGS="$CXXFLAGS $ACE_INCLUDES $ABI_FLAGS"

      if test "x$dpp_os_type" = "xWin32" ; then
         CPPFLAGS="$CPPFLAGS -DACE_HAS_DLL=0 /GX /MT"
         CXXFLAGS="$CXXFLAGS -DACE_HAS_DLL=0 /GX /MT"
      fi

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      LIBS="$LIBS -lACE"

      dnl If the ACE library was found, add the API object files to the files
      dnl to be compiled and enable the Performer API compile-time option.
      dnl This ace_main_i crap is for Windows.
      AC_CACHE_CHECK([for ACE_OS::perror in -lACE],
         [dpp_cv_ACE_OS__perror_in_ACE],
         [AC_TRY_LINK([#include <ace/OS.h>
int ace_main_i(int argc, char** const argv)
{
   return 0;
}],
                     [ACE_OS::perror("test");],
                     [ dpp_cv_ACE_OS__perror_in_ACE='yes'
                       rm -rf ./ii_files ],
                     [dpp_cv_ACE_OS__perror_in_ACE='no'])])

      if test "x$dpp_cv_ACE_OS__perror_in_ACE" = "xyes" ; then
         AC_CHECK_HEADER([ace/OS.h],
            [ dpp_have_ace='yes' ACE_LDFLAGS="-L$ACE_LIBDIR -lace" ], $4)

         if test "x$dpp_have_ace" = "xyes" ; then
            ifelse([$3], , :, [$3])
         fi
      else
         AC_MSG_WARN([*** Cannot find ACE library ***])
         ifelse([$4], , :, [$4])
      fi

      DPP_LANG_RESTORE

      CXXFLAGS="$dpp_save_CXXFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      LDFLAGS="$dpp_save_LDFLAGS"
      LIBS="$dpp_save_LIBS"

   fi
   dnl -----------------------------------------------------------------------
   dnl Do the substition step for $ACE_ROOT.
   dnl -----------------------------------------------------------------------
   AC_SUBST(ACE_ROOT)
])

dnl ---------------------------------------------------------------------------
dnl Make sure that the installed ACE is at least the given version.
dnl
dnl Usage:
dnl     DPP_ACE_VER(ace-root, version [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     ace-include-dirs    - Space-separated list of directories that might
dnl                           contain the ACE heder files.
dnl     required-version    - Minimum required version.
dnl     action-if-found     - Action to take if the version requirement is
dnl                           met.
dnl     action-if-not-found - Action to take if the version requirement is not
dnl                           met.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ACE_VER],
[
   dnl -----------------------------------------------------------------------
   dnl Define the version number of the ACE installation.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(acever,
               [  --with-acever=<VER>     Minimum ACE version             [default=$2]],
               ACE_VER="$withval", ACE_VER=$2)

   dpp_save_CPPFLAGS="$CPPFLAGS"
   dpp_ace_header_dir=''

   for dir in $1 ; do
      CPPFLAGS="$CPPFLAGS -I$dir"

      dpp_safe=`echo "$dir/ace/Version.h" | sed 'y%.:/+-%___p_%'`

      AC_MSG_CHECKING([for ace/Version.h in $dir])
      AC_CACHE_VAL([dpp_cv_header_$dpp_safe],
         [AC_TRY_CPP([#include <ace/Version.h>],
                     eval "dpp_cv_header_$dpp_safe=yes",
                     eval "dpp_cv_header_$dpp_safe=no")])dnl

      if eval "test \"`echo '$dpp_cv_header_'$dpp_safe`\" = yes"; then
         AC_MSG_RESULT(yes)
         dpp_ace_header_dir="$dir"
         break
      else
         AC_MSG_RESULT(no)
         CPPFLAGS="$dpp_save_CPPFLAGS"
      fi
   done

   CPPFLAGS="$dpp_save_CPPFLAGS"

   if test "x$dpp_ace_header_dir" != "x" ; then
      ifelse([$3], , :, [$3])
   else
      ifelse([$4], , :, [$4])
   fi

   dnl Determine the ACE version being used.
   dpp_ver_num_exp=['s/.*\([0-9][0-9]*\)$/\1/']
   dpp_ace_ver_major=`grep ACE_MAJOR_VERSION $dpp_ace_header_dir/ace/Version.h | sed -e $dpp_ver_num_exp` ;
   dpp_ace_ver_minor=`grep ACE_MINOR_VERSION $dpp_ace_header_dir/ace/Version.h | sed -e $dpp_ver_num_exp` ;
   dpp_ace_ver_patch=`grep ACE_BETA_VERSION $dpp_ace_header_dir/ace/Version.h | sed -e $dpp_ver_num_exp` ;

   dpp_ace_ver="${dpp_ace_ver_major}.${dpp_ace_ver_minor}.${dpp_ace_ver_patch}"

   DPP_VERSION_CHECK_MSG_NO_CACHE([ACE], [$dpp_ace_ver], [$2], [$3], [$4])

   ACE_VER="$dpp_ace_ver_major"
])
