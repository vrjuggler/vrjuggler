dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
dnl File:          general.m4,v
dnl Date modified: 2006/06/26 02:01:39
dnl Version:       1.85
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl General macros for use within Doozer++ and by some users constructing
dnl moderately complex tests.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PREREQ            - Complain and exit if the Doozer++ version is
dnl                             less than the given version number.
dnl     DPP_INIT              - Initialize Doozer++.
dnl     DPP_SUBST             - Finish up with Doozer++ by substituting
dnl                             variables defined by DPP_INIT.
dnl     DPP_VERSION_CHECK     - Compare two version numbers.
dnl     DPP_VERSION_CHECK_MSG - Wrap a call to DPP_VERSION_CHECK inside a call
dnl                             to AC_CACHE_CHECK. 
dnl     DPP_VERSION_CHECK_MSG_NO_CACHE
dnl                           -  Wrap a call to DPP_VERSION_CHECK with calls
dnl                              to AC_MSG_CHECKING and AC_MSG_RESULT.
dnl     DPP_GEN_RECONFIG      - Generate a script called 'reconfig' that will
dnl                             reconfigure a build directory from scratch.
dnl ===========================================================================

dnl general.m4,v 1.85 2006/06/26 02:01:39 patrickh Exp

dnl Set the version of Doozer++.
define(DPP_DPPVERSION, 2.1.2)

dnl ---------------------------------------------------------------------------
dnl Change the dots in NUMBER into commas.
dnl
dnl Usage:
dnl     DPP_PREREQ_SPLIT(NUMBER)
dnl
dnl Arguments:
dnl     NUMBER - A version number of the form major.minor.patch
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_SPLIT, [translit($1, ., [, ])])

dnl ---------------------------------------------------------------------------
dnl Default the ternary version number to 0 (e.g., 1, 7 -> 1, 7, 0).
dnl
dnl Usage:
dnl     DPP_PREREQ_CANON(MAJOR, MINOR [,TERNARY])
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_CANON, [$1, $2, ifelse([$3], , 0, [$3])])

dnl ---------------------------------------------------------------------------
dnl Complain and exit if version number 1 is less than version number 2.
dnl PRINTABLE2 is the printable version of version number 2.
dnl
dnl Usage:
dnl     DPP_PREREQ_COMPARE(MAJOR1, MINOR1, TERNARY1, MAJOR2, MINOR2, TERNARY2,
dnl                        PRINTABLE2)
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_COMPARE,
[ifelse(builtin([eval],
[$3 + $2 * 1000 + $1 * 1000000 < $6 + $5 * 1000 + $4 * 1000000]), 1,
[errprint(dnl
FATAL ERROR: Doozer++ version $7 or higher is required for this script
)m4exit(3)])])

dnl ---------------------------------------------------------------------------
dnl Complain and exit if the Doozer++ version is less than VERSION.
dnl
dnl Usage:
dnl     DPP_PREREQ(VERSION)
dnl
dnl Arguments:
dnl     VERSION - The Doozer++ version number of the form major.minor[.patch]
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PREREQ],
[DPP_PREREQ_COMPARE(DPP_PREREQ_CANON(DPP_PREREQ_SPLIT(DPP_DPPVERSION)),
DPP_PREREQ_CANON(DPP_PREREQ_SPLIT([$1])), [$1])])

dnl ---------------------------------------------------------------------------
dnl Compare two version numbers separated into their components.
dnl
dnl Usage:
dnl     DPP_VERSION_COMPARE(known-major, known-minor, known-patch,
dnl                         required-major, required-minor, required-patch
dnl                         [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     known-major         - The known major number value to be compared.
dnl     known-minor         - The known minor number value to be compared.
dnl     known-patch         - The known patch number value to be compared.
dnl     required-major      - The minimum required major number.
dnl     required-minor      - The minimum required minor number.
dnl     required-patch      - The minimum required patch number.
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
define(DPP_VERSION_COMPARE,
[
   if test `expr $3 + $2 \* 1000 + $1 \* 1000000 \>= $6 + $5 \* 1000 + $4 \* 1000000` = 1
   then
      $7
      true
   else
      $8
      true
   fi
])

dnl ---------------------------------------------------------------------------
dnl Compare two version numbers.  The first is the version number in question,
dnl and the second is the minimum required value.
dnl
dnl Usage:
dnl     DPP_VERSION_CHECK(known-version, required-version
dnl                       [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     known-version       - The known version number to be tested.  It must
dnl                           be of the form major.minor[.patch]
dnl     required-version    - The minimum required version number.  It must
dnl                           be of the form major.minor[.patch]
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_VERSION_CHECK],
[
   split_known_ver=`echo $1 | sed -e 's/\./ /g'`
   split_req_ver=`echo $2 | sed -e 's/\./ /g'`

   known_major=`echo $split_known_ver | awk '{ print $[1] }' -`
   known_minor=`echo $split_known_ver | awk '{ print $[2] }' -`
   known_patch=`echo $split_known_ver | awk '{ print $[3] }' -`

   if test "x$known_patch" = "x" ; then
      known_patch=0
   fi

   req_major=`echo $split_req_ver | awk '{ print $[1] }' -`
   req_minor=`echo $split_req_ver | awk '{ print $[2] }' -`
   req_patch=`echo $split_req_ver | awk '{ print $[3] }' -`

   if test "x$req_patch" = "x" ; then
      req_patch=0
   fi

   DPP_VERSION_COMPARE([$known_major], [$known_minor], [$known_patch],
                       [$req_major], [$req_minor], [$req_patch],
                       [$3], [$4])
])

dnl ---------------------------------------------------------------------------
dnl Wrap a call to DPP_VERSION_CHECK inside a call to AC_CACHE_CHECK.  This is
dnl a helper macro to simplify user-level code down to the bare minimum.
dnl
dnl NOTE: This uses AC_CACHE_CHECK to speed up the version check process, but
dnl       caching version numbers is not always a good idea.  Either avoid
dnl       using an dnl Autoconf cache file (recommended since Autoconf 2.50)
dnl       or use DPP_VERSION_CHECK directly.
dnl
dnl Usage:
dnl     DPP_VERSION_CHECK_MSG(pkg-name, known-version, required-version, cache_var [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     pkg-name            - The name of the package whose version number
dnl                           will be tested.
dnl     known-version       - The known version number to be tested.  It must
dnl                           be of the form major.minor[.patch]
dnl     required-version    - The minimum required version number.  It must
dnl                           be of the form major.minor[.patch]
dnl     cache-var           - A variable to be used for caching the comparison
dnl                           results.
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_VERSION_CHECK_MSG],
[
   AC_CACHE_CHECK([whether $1 version is >= $3], [$4],
                  [DPP_VERSION_CHECK([$2], [$3], [$4="$2"], [$4='no'])])

   dnl Successful version comparison.  Note that $4 is a variable name, not
   dnl the result of evaluating a variable.  As such, we have to prepend a '$'
   dnl to its name here in order to get its value.
   if test "x$$4" != "xno" ; then
      ifelse([$5], , :, [$5])
      true
   dnl Failed version comparison.
   else
      ifelse([$6], , :, [$6])
      true
   fi
])

dnl ---------------------------------------------------------------------------
dnl Wrap a call to DPP_VERSION_CHECK with calls to AC_MSG_CHECKING and
dnl AC_MSG_RESULT.  This is a helper macro to simplify user-level code down to
dnl the bare minimum.  No caching is performed.
dnl
dnl Since: 2.0.1
dnl
dnl Usage:
dnl     DPP_VERSION_CHECK_MSG_NO_CACHE(pkg-name, known-version, required-version, [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     pkg-name            - The name of the package whose version number
dnl                           will be tested.
dnl     known-version       - The known version number to be tested.  It must
dnl                           be of the form major.minor[.patch]
dnl     required-version    - The minimum required version number.  It must
dnl                           be of the form major.minor[.patch]
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_VERSION_CHECK_MSG_NO_CACHE],
[
   AC_MSG_CHECKING([whether $1 version is >= $3])
   DPP_VERSION_CHECK([$2], [$3],
                     [AC_MSG_RESULT([yes (version $2)])
                      $4],
                     [AC_MSG_RESULT([no (version $2)])
                      $5])
])

dnl ---------------------------------------------------------------------------
dnl Initialize Doozer++.
dnl
dnl Usage:
dnl     DPP_INIT
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_INIT],
[
   topdir=`pwd`
])

dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl DPP_ACLOCAL_INCLUDE(macrodir)
AC_DEFUN([DPP_ACLOCAL_INCLUDE],
[
   test -n "$ACLOCAL_FLAGS" && ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

   for k in $1 ; do ACLOCAL="$ACLOCAL -I $k" ; done
])

dnl ---------------------------------------------------------------------------
dnl Do final substitutions for variables defined in DPP_INIT.
dnl
dnl Usage:
dnl     DPP_SUBST
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_SUBST],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_WIN32_SETUP])

   if test "x$dpp_os_type" = "xWin32"; then
      topdir=`dospath -p "$topdir"`
   fi

   AC_SUBST(topdir)
])

dnl ---------------------------------------------------------------------------
dnl Get the absolute path to your project directory
dnl (directory where you run configure)
dnl
dnl Sets:
dnl   $projectdir  --- within configure.in
dnl   @projectdir@ --- within external files processed by AC_OUTPUT 
dnl
dnl Usage:
dnl     DPP_GET_PROJECTDIR
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_GET_PROJECTDIR],
[
   AC_REQUIRE([DPP_INIT])
   AC_REQUIRE([DPP_SUBST])

   dnl $srcdir is the root directory of the juggler source tree reletive to 
   dnl where configure is being run.  To get an absolute path for 
   dnl $projectdir, we cd there and save the value of running pwd.  
   dnl Then return to the directory where configure is being run ($topdir).
   dnl 
   dnl NOTE: look for a less hacky way to do this!  srcdir is only valid to 
   dnl       go off of in the configure script, not within individual subdirs.
   dnl
   cd "$srcdir"
   projectdir=`pwd`
   cd "$topdir"
   
   AC_SUBST(projectdir)
])

dnl ---------------------------------------------------------------------------
dnl Generate a script called 'reconfig'.  This script cleans out all the files
dnl generated by configure in a build directory.  It then runs configure again
dnl using the same arguments given to it when the script was generated.
dnl
dnl Usage:
dnl     DPP_GEN_RECONFIG
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_GEN_RECONFIG],
[
   dnl Create a script for handy reconfiguration of a build tree.  It removes
   dnl configure's cache and status files and then runs configure with the
   dnl same arguments.
   : ${RECONFIG=./reconfig}

   echo "creating $RECONFIG"
   rm -f $RECONFIG

[
   cat > $RECONFIG <<RECONFIG_SCRIPT
rm -f config.status config.cache config.log
${0} $ac_configure_args $*
RECONFIG_SCRIPT
]

   chmod +x $RECONFIG
])
