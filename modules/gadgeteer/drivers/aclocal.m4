# aclocal.m4 generated automatically by aclocal 1.6.3 -*- Autoconf -*-

# Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002
# Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
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
dnl     DPP_GEN_RECONFIG      - Generate a script called 'reconfig' that will
dnl                             reconfigure a build directory from scratch.
dnl ===========================================================================

dnl $Id$

dnl Set the version of Doozer++.
define(DPP_DPPVERSION, 1.5.1)

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
AC_DEFUN(DPP_PREREQ,
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
AC_DEFUN(DPP_VERSION_CHECK,
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
AC_DEFUN(DPP_VERSION_CHECK_MSG,
[
   AC_CACHE_CHECK([whether $1 version is >= $3], [$4],
      [DPP_VERSION_CHECK([$2], [$3],
         [ $4="$2"
           ifelse([$5], , :, [$5])
         ],
         [ $4='no'
           AC_MSG_RESULT([$2])
           ifelse([$6], , :, [$6])
         ])])
])

dnl ---------------------------------------------------------------------------
dnl Initialize Doozer++.
dnl
dnl Usage:
dnl     DPP_INIT
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_INIT,
[
   topdir=`pwd`
])

dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl DPP_ACLOCAL_INCLUDE(macrodir)
AC_DEFUN(DPP_ACLOCAL_INCLUDE,
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
AC_DEFUN(DPP_SUBST,
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
AC_DEFUN(DPP_GET_PROJECTDIR,
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
AC_DEFUN(DPP_GEN_RECONFIG,
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

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Determine the canonical system type and define a ton of variables for
dnl system-dependent settings.  The platforms supported in this file are AIX,
dnl Cygnus-Win32, FreeBSD, HP-UX, IRIX, Linux, and Solaris.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_SYSTEM_SETUP - Based on the given detected host and CPU, set up
dnl                        the system-specific configuration.
dnl
dnl Variables defined:
dnl     ABI_LIST         - The list of supported ABI/ISA combinations.
dnl     OS_TYPE          - The type of the target platform (UNIX or Win32).
dnl     PLATFORM         - The vendor name of the target platform.
dnl     OS_REL_STR       - The full, unmodified string giving the operating
dnl                        system version number.
dnl     OS_REL_NUM       - The numerical value of the operating system
dnl                        version number.
dnl     OS_REL_NUM_MAJOR - The major component of the OS version number.
dnl     OS_REL_NUM_MINOR - The minor component of the OS version number.
dnl
dnl Possible preprocssor symbols defined:
dnl     IRIXREL      - Defined to the string "IRIX5" or "IRIX6" based on the
dnl                    determined version of IRIX.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Based on the given detected host and CPU, set up the system-specific
dnl configuration.
dnl
dnl Usage:
dnl     DPP_SYSTEM_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SYSTEM_SETUP,
[
   dnl We use AC_CANONICAL SYSTEM so that we can find out information about
   dnl the build, arget and host platforms rather than only the host.
   AC_REQUIRE([AC_CANONICAL_SYSTEM])

   dnl Operating system release information.  This may contain anything and
   dnl should be treated as a string.
   OS_REL_STR=`uname -r`

   dnl Operating system version number only (any miscellaneous text is
   dnl stripped).  This will only contain a number and can be treated as a
   dnl float-point value.
   changequote(<<, >>)
   OS_REL_NUM=`uname -r | sed -e 's/^[^0-9]*\([0-9][0-9]*\.[0-9][0-9]*\)[^0-9]*$/\1/'`
   changequote([, ])

   dnl Major and minor version numbers from $OS_REL_NUM separated for use
   dnl with the C preprocessor.  Using cut(1) is kind of a hack, but at least
   dnl it's a clean one.
   OS_REL_NUM_MAJOR=`echo "$OS_REL_NUM" | cut -f1 -d'.'`
   OS_REL_NUM_MINOR=`echo "$OS_REL_NUM" | cut -f1 -d' ' | cut -f2 -d'.'`

   dnl General type of operating system (e.g., UNIX, Win32) and actual
   dnl platform name.
   OS_TYPE='UNIX'
   PLATFORM=''

   dnl List of supported ABIs on a given platform.
   ABI_LIST=''

   dnl The OS for which this is being configured.  The operating systems are
   dnl listed alphabetically.
   case $target_os in
      dnl IBM RS/6000 running AIX.
      aix*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         PLATFORM='AIX'
         ;;
      dnl i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
      cygwin)
         dnl If no ABI has been set yet, default to WIN32 with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('WIN32')
         fi

         ABI_LIST='WIN32'
         OS_TYPE='Win32'

         dpp_sys_name=`uname -s`

         dnl Determine the value for $PLATFORM based on what uname(1) tells
         dnl us.  We define our own values because Cygnus' uname(1) returns
         dnl somewhat annoying names.
         case "$dpp_sys_name" in
            dnl Windows NT 4.0, 5.0 (aka Windows 2000), 5.1 (Windows XP), and
            dnl beyond (I hope).
            *NT-*)
               PLATFORM='WinNT'
               ;;
            dnl Windows 95 and 98.
            *WIN_9*-4.*)
               PLATFORM='Win9x'
               ;;
            dnl Windows ME.
            *WIN_ME-4.*)
               PLATFORM='WinMe'
               ;;
            dnl Unknown version of Windows that may not yet be supported.
            *)
               AC_MSG_WARN(Unknown Windows type $dpp_sys_name!)
               ;;
         esac
         ;;
      dnl A machine running Darwin (aka, Mac OS X).
      darwin*)
         dnl If no ABI has been set yet, default to Mach-O with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('Mach-O', $target_cpu)
         fi

         PLATFORM='Darwin'
         ;;
      dnl A machine running FreeBSD.
      freebsd*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            *)
               ABI_LIST="ELF_$target_cpu"
               ;;
         esac

         PLATFORM='FreeBSD'
         ;;
      dnl HP PA-RISC machine running HP-UX 10.20.
      hpux10.20)
         dnl If no ABI has been set yet, default to HP PA-RISC.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('HP', 'PA-RISC')
         fi

         ABI_LIST='HP HP64'
         PLATFORM='HP'
         ;;
      dnl HP PA-RISC machine running HP-UX 11.x.
      hpux11*)
         dnl If no ABI has been set yet, default to HP PA-RISC.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('HP', 'PA-RISC')
         fi

         ABI_LIST='HP HP64'
         PLATFORM='HP'
         ;;
      dnl A machine running IRIX 6.x.
      irix6*)
         dnl If no ABI has been set yet, default to N32_M3.
         if test "x$ABI" = "x" -a "x$USE_GCC" != "xyes"; then
            DPP_ABI_CFG('N32', 'mips3', '32', '-n32 -mips3')
         fi

         PLATFORM='IRIX'

         if test $OS_REL_NUM_MAJOR -lt 6 ; then
            dpp_IRIXREL='IRIX5'
         else
            dpp_IRIXREL='IRIX6'
         fi

         AC_DEFINE_UNQUOTED(IRIXREL, "$dpp_IRIXREL",
            [define to \"IRIX5\" or \"IRIX6\" depending on IRIX version])
         ;;
      dnl A machine running Linux.
      linux*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            *)
               ABI_LIST="ELF_$target_cpu"
               ;;
         esac

         PLATFORM='Linux'
         ;;
      dnl A machine running NetBSD.
      netbsd*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            *)
               ABI_LIST="ELF_$target_cpu"
               ;;
         esac

         PLATFORM='NetBSD'
         ;;
      dnl A machine running OpenBSD.
      openbsd*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            *)
               ABI_LIST="ELF_$target_cpu"
               ;;
         esac

         PLATFORM='OpenBSD'
         ;;
      dnl DEC Alpha running Digital UNIX 4.0.
      osf4.0)
         dnl If no ABI has been set yet, default to ALPHA (whatever that
         dnl means).
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ALPHA')
         fi

         ABI_LIST='ALPHA'
         PLATFORM='alpha-osf'
         ;;
      dnl A machine running Solaris (aka SunOS).
      solaris*)
         dnl If no ABI has been set yet, default to ELF with whatever the
         dnl CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG('ELF', $target_cpu)
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            *)
               ABI_LIST="ELF_$target_cpu"
               ;;
         esac

         PLATFORM='Solaris'
         ;;
      dnl Unkown operating system (to config.guess).
      unknown)
         dnl If no ABI has been set yet, default to the OS (which will be
         dnl 'unknown') with whatever the CPU architecture is.
         if test "x$ABI" = "x" ; then
            DPP_ABI_CFG($target_os, $target_cpu)
         fi  

         PLATFORM='UNKNOWN'
         ;;
   esac

   if test "x$PLATFORM" = "x" ; then
      AC_MSG_WARN(Unknown target OS!)
      PLATFORM='UNKNOWN'
      OS_TYPE='UNKNOWN'
   fi

   dpp_platform="$PLATFORM"
   dpp_os_type="$OS_TYPE"

   AC_SUBST(ABI_LIST)
   AC_SUBST(OS_TYPE)
   AC_SUBST(PLATFORM)
])

dnl ---------------------------------------------------------------------------
dnl This macro is deprecated and does nothing.  This definition is retained
dnl for backwards compatibility with previous Doozer++ 1.x releases.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SYSTEM_SUBST,
[
   AC_OBSOLETE([$0], [; it will be removed in Doozer++ 2.0])
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide functionality for application binary interface (ABI) and
dnl instruction set architecture (ISA) configurations.  The code here performs
dnl substitution for the variables $ABI, $ISA and $LIBBITUF (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_ABI_CFG   - Set the values for $ABI, $ISA, $LIBBITSUF and
dnl                     ${_EXTRA_FLAGS} respectively.  These variables are
dnl                     described next.
dnl     DPP_ABI_SETUP - Add the --with-abi option and configure the ABI/ISA
dnl                     settings based on the value given.
dnl
dnl Command-line options added:
dnl     --with-abi - Choose the ABI/ISA setting for compiling.  This can take
dnl                  one of the following values (stored in $DPP_ABI_TYPE):
dnl
dnl                      N32_M3 (N32/mips3)
dnl                      N32_M4 (N32/mips4)
dnl                      64_M3 (64/mips3)
dnl                      64_M4 (64/mips4)
dnl                      ELF_i386 (ELF/i386)
dnl                      WIN32_i386 (WIN32/i386)
dnl                      COFF_ALPHA (COFF/alpha)
dnl                      ELF_ALPHA (ELF/alpha)
dnl                      HP (pa-risc)
dnl
dnl Variables defined:
dnl     ABI          - The ABI chosen.  It will have one of the following
dnl                    values: 64, N32, ELF, WIN32
dnl     ISA          - The ISA chosen.  It will have one of the following
dnl                    values: mips3, mips4, i386
dnl     LIBBITSUF    - The suffix for some library directories (which are
dnl                    typically named in $LDFLAGS).  If it has a value, it
dnl                    will be 32 or 64 which only has meaning on IRIX.
dnl     _EXTRA_FLAGS - Any extra compiler flags needed for compiling with the
dnl                    named ABI/ISA setting.  This will be appended to
dnl                    $CFLAGS and $CXXFLAGS.
dnl     DPP_ABI_TYPE - The argument given to --with-abi.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Define a macro DPP_ABI_CFG for setting up the configuration parameters
dnl for a given ABI.
dnl
dnl Usage:
dnl     DPP_ABI_CFG([ ABI [, ISA [, library-suffix [, extra-flags]]]])
dnl
dnl Arguments:
dnl     ABI            - The application binary interface to use.  This
dnl                      argument is optional.
dnl     ISA            - The instruction set architecture for the target CPU.
dnl                      This argument is optional.
dnl     library-suffix - The bit-width suffix for the library directory
dnl                      tied to the ABI.  This is only meaningful on IRIX.
dnl                      This argument is optional.
dnl     extra-flags    - Extra compiler flags related to the ABI and ISA.
dnl                      This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_ABI_CFG, [ ABI=$1 ISA=$2 LIBBITSUF=$3 _EXTRA_FLAGS=$4 ; ])

dnl ---------------------------------------------------------------------------
dnl Adds the --with-abi option and configure the ABI/ISA settings based on the
dnl value given.  Substitution on the variables $ABI, $ISA and $LIBBITSUF is
dnl also performed.
dnl 
dnl Usage:
dnl     DPP_ABI_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_ABI_SETUP,
[
   AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

   dnl -----------------------------------------------------------------------
   dnl Define the binary format.  Possible values are the following:
   dnl     N32_M3        - On IRIX, use N32 mips3 binaries
   dnl     N32_M4        - On IRIX, use N32 mips4 binaries
   dnl     64_M3         - On IRIX, use 64-bit mips3 binaries
   dnl     64_M4         - On IRIX, use 64-bit mips4 binaries
   dnl     ELF_i386      - On an i386 OS (e.g., Linux/i386), use ELF binaries
   dnl     WIN32_i386    - On an i386 Win32 OS, use Win32 binaries
   dnl     COFF_ALPHA    - On an Alpha, use COFF binaries
   dnl     ELF_ALPHA     - On an Alpha, use ELF binaries
   dnl     HP            - On HP-UX, use PA-RISC binaries
   dnl     HP64          - On HP-UX, use 64-bit PA-RISC binaries
   dnl     DARWIN_PPC    - On Darwin, use PowerPC binaries
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(abi,
               [  --with-abi=<N32_M3|N32_M4|64_M3|64_M4|ELF_i386|WIN32_i386|COFF_ALPHA|ELF_ALPHA|HP|HP64|DARWIN_PPC>
                          Define the Application Binary
                          Interface to use],
               DPP_ABI_TYPE="$withval")

   dnl Set default values for these before checking $DPP_ABI_TYPE.
   dnl Currently, these are for non-IRIX platforms.
   ABI=''
   LIBBITSUF=''

   dnl Based on the value of $abi_type, set $ABI for later use and the
   dnl Makefile substitution variables $LIBBITSUF and $ISA.
   case "x$DPP_ABI_TYPE" in
      x64_M3)
         DPP_ABI_CFG('64', 'mips3', '64', '-64 -mips3')
         ;;
      x64_M4)
         DPP_ABI_CFG('64', 'mips4', '64', '-64 -mips4')
         ;;
      xN32_M3)
         DPP_ABI_CFG('N32', 'mips3', '32', '-n32 -mips3')
         ;;
      xN32_M4)
         DPP_ABI_CFG('N32', 'mips4', '32', '-n32 -mips4')
         ;;
      xELF_i386)
         DPP_ABI_CFG('ELF', 'i386')
         ;;
      xWIN32_i386)
         DPP_ABI_CFG('WIN32')
         ;;
      xCOFF_ALPHA)
         DPP_ABI_CFG('COFF', 'alpha')
         ;;
      xELF_ALPHA)
         DPP_ABI_CFG('ELF', 'alpha')
         ;;
      xHP)
         DPP_ABI_CFG('HP', 'pa-risc')
         ;;
      xHP64)
         DPP_ABI_CFG('HP64', 'pa-risc', '/pa20_64', '+DD64')
         ;;
      xDARWIN_PPC)
         DPP_ABI_CFG('Mach-O', 'powerpc')
         ;;
   esac

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables $ABI, $ISA and $LIBBITSUF.
   dnl -----------------------------------------------------------------------
   AC_SUBST(ABI)
   AC_SUBST(ISA)
   AC_SUBST(LIBBITSUF)
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Define macros useful when working in a Win32/DOS-based environment.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WIN32_SETUP - Define path conversion (DOS to UNIX and UNIX to DOS)
dnl                       subroutines.
dnl     DPP_PROG_CYGCL  - Find the cygcl Perl script wrapper around the
dnl                       Microsoft Visual C++ command-line compiler CL.EXE.
dnl     DPP_PROG_MSVCCC - Find the msvccc shell script wrapper around the
dnl                       Microsoft Visual C++ command-line compiler CL.EXE.
dnl 
dnl Subroutines defined:
dnl     dospath  - Convert a UNIX-style path into a DOS-style path.
dnl     unixpath - Convert a DOS-style path into a UNIX-style path.
dnl
dnl Variables defined:
dnl     DPP_USING_CYGCL  - Set to either 'yes' or 'no' depending on whether
dnl                        cygcl will be used as the compiler.
dnl     DPP_USING_MSVCCC - Set to either 'yes' or 'no' depending on whether
dnl                        msvccc will be used as the compiler.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Define path conversion (DOS to UNIX and UNIX to DOS) subroutines.
dnl
dnl Usage:
dnl     DPP_WIN32_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_WIN32_SETUP,
[
   AC_PATH_PROG([CYGPATH], [cygpath], [no])

   dnl UNIX path translator.  This converts a UNIX-style path to a DOS/Win32
   dnl path.  With this routine, we can safely use UNIX paths for all Win32
   dnl variables while the configure script runs.  Then, immediately before
   dnl Makefile substitution is done, we can translate all the paths to
   dnl DOS-style for use in the Makefiles.
   dospath ( ) {
      dnl The first expression changes a "//C" drive name to "C:".  It has a
      dnl pair of surrounding []'s because those symbols have special meaning
      dnl to m4(1).  Without them, the first pair of []'s in the regular
      dnl expression will be lost.  Similarly, the second expression chagnes
      dnl a "/cygdrive/C" drive name to "C:".
      dnl The third expression ensures that a path beginning with / is
      dnl returned as being a directory on the home drive.  The UNIX utilities
      dnl treat $HOMEDRIVE as the root directory and will give, for example,
      dnl /tmp instead of //D/tmp if D: is the home drive.  The expression
      dnl simply prepends "$HOMEDRIVE" (after evaluation) to the path.
      dpp_drive_exp=['s/^\/\/\([[:alpha:]]\)/\1:/']
      dpp_drive_exp2=['s/^\/cygdrive\/\([[:alpha:]]\)/\1:/']
      dpp_add_home_exp=["s/^\//$HOMEDRIVE\//"]

      dnl If we have a value in $CYGPATH, use it to convert the path from
      dnl UNIX to Win32.
      if test "x$CYGPATH" != "xno" ; then
         dnl The -p option tells that we are given a simple path.
         if test "x${1}" = "x-p" ; then
            dpp_retval=`$CYGPATH --windows ${2} | sed -e ['s/\\\\/\//g']`
         else
            dnl With the newer Cygwin code, we can use $CYGPATH to get the
            dnl Win32 version of /.
            dpp_cygroot=`$CYGPATH --windows / | sed -e ['s/\\\\/\//g']`
            dpp_cygroot_exp=["s|^\/|$dpp_cygroot\/|"]
            dpp_retval=`echo ${1} | sed -e "$dpp_drive_exp" -e "$dpp_drive_exp2" -e "$dpp_cygroot_exp" -e "$dpp_add_home_exp"`
         fi
      dnl Otherwise, use only sed(1) which is a little more clumsy.
      else
         dnl Ignore the -p option in this case.
         if test "x${1}" = "x-p" ; then
            dpp_unix_path="${2}"
         else
            dpp_unix_path="${1}"
         fi

         dpp_retval=`echo $dpp_unix_path | sed -e "$dpp_drive_exp" -e "$dpp_drive_exp2" -e "$dpp_add_home_exp"`
      fi

      echo "$dpp_retval"
   }

   dnl DOS path translator.  This converts a DOS-style path to a UNIX path.
   unixpath ( ) {
      dnl All expressions hers are enclosed in []'s because those symbols
      dnl have special meaning to m4(1).  This is the same situation as
      dnl above.

       dnl Ignore the -p option in this case.
      if test "x${1}" = "x-p" ; then
         dpp_dos_path="${2}"
      else
         dpp_dos_path="${1}"
      fi

      dnl If we have a value in $CYGPATH, use it to convert the path from
      dnl Win32 to UNIX.
      dnl The -p option tells that we are given a simple path.
      if test "x$CYGPATH" != "xno" -a "x${1}" != "x-p" ; then
         dpp_retval=`$CYGPATH --unix $dpp_dos_path`
      dnl Otherwise, use sed(1) which is a little more clumsy.
      else
         dnl This changes \'s to /'s and escapes any whitespace in the path.
         dpp_slash_exp=['s/\\/\//g']
         dpp_ws_esc_exp=['s/ /\\ /g']
         dpp_unix_root=`echo "$dpp_dos_path" | sed -e "$dpp_slash_exp" -e "$dpp_ws_esc_exp"`

         dnl This changes the leading "C:" to "//C" (for the appropriate
         dnl drive letter).
         dpp_colon_rem_exp=['s/\([[:alpha:]]\):/\/\/\1/g']
         dpp_colon_rem_root=`echo "$dpp_unix_root" | sed -e "$dpp_colon_rem_exp"`

         dnl This changes any ";" into ":"
         dpp_semicolon_exp=['s/\;/:/g']
         dpp_retval=`echo "$dpp_colon_rem_root" | sed -e "$dpp_semicolon_exp"`
      fi

      echo "$dpp_retval"
   }
])

dnl ---------------------------------------------------------------------------
dnl Find the cygcl Perl script wrapper around the Microsoft Visual C++
dnl command-line compiler CL.EXE.  This works similarly to a normal
dnl AC_CHECK_PROG call, but it looks for a specific program (cygcl) and will
dnl perform an optional action if that program is not found.  The variable
dnl $DPP_USING_CYGCL will be set to 'yes' or 'no' depending on the results of
dnl the lookup of cygcl in the path.
dnl
dnl Note:
dnl     This macro must be called before any compiler checks are performed.
dnl
dnl Usage:
dnl     DPP_PROG_CYGCL([path [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     path                - The path to use when trying to find cygcl.
dnl                           This argument is optional.
dnl     action-if-found     - Any action(s) to take if cygcl is found.  This
dnl                           argument is optional.
dnl     action-if-not-found - Any action(s) to take if cygcl is not found.
dnl                           This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CYGCL,
[
   AC_CHECK_PROG([dpp_cygcl], [cygcl], [cygcl], [no], $1)

   if test "x$dpp_cygcl" = "xno" ; then
      DPP_USING_CYGCL='no'
      ifelse([$3], , :, [$3])
   else
      DPP_USING_CYGCL='yes'
      CXX='cygcl'
      CC="$CXX"

      ifelse([$2], , :, [$2])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Find the msvccc shell script wrapper around the Microsoft Visual C++
dnl command-line compiler CL.EXE.  This works similarly to a normal
dnl AC_CHECK_PROG call, but it looks for a specific program (msvccc) and will
dnl perform an optional action if that program is not found.  The variable
dnl $DPP_USING_MSVCCC will be set to 'yes' or 'no' depending on the results of
dnl the lookup of msvccc in the path.
dnl
dnl Note:
dnl     This macro must be called before any compiler checks are performed.
dnl
dnl Usage:
dnl     DPP_PROG_MSVCCC([path [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     path                - The path to use when trying to find msvccc.
dnl                           This argument is optional.
dnl     action-if-found     - Any action(s) to take if msvccc is found.  This
dnl                           argument is optional.
dnl     action-if-not-found - Any action(s) to take if msvccc is not found.
dnl                           This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_MSVCCC,
[
   AC_CHECK_PROG([dpp_msvccc], [msvccc], [msvccc], [no], $1)

   if test "x$dpp_msvccc" = "xno" ; then
      DPP_USING_MSVCCC='no'
      ifelse([$3], , :, [$3])
   else
      AC_MSG_WARN([*** msvccc has many bugs--consider using cygcl from Doozer++ instead ***])

      DPP_USING_MSVCCC='yes'
      CXX='msvccc'
      CC="$CXX"

      ifelse([$2], , :, [$2])
   fi
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl C++ macros.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_STD_CPP          - Add a command line option to enable or disable
dnl                            the use of standard C++ by the C++ compiler.
dnl     DPP_CXX_TEMPLATES    - Test the C++ compiler's template-handling
dnl                            capabilities.
dnl     DPP_CXX_NAMESPACE    - Test the C++ compiler's namespace-handling
dnl                            capabilities.
dnl     DPP_CXX_HAVE_STD     - Test if the C++ compiler supports the std
dnl                            namespace and the standard C++ library.
dnl     DPP_CXX_INLINE       - Test the C++ compiler's inlining capabilities.
dnl     DPP_CXX_RTTI         - Test the C++ compiler's RTTI capabilities.
dnl     DPP_CXX_STATIC_CAST  - Test the C++ compiler's support for
dnl                            static_cast<>.
dnl     DPP_CXX_DYNAMIC_CAST - Test the C++ compiler's support for
dnl                            dynamic_cast<>.
dnl     DPP_CXX_CONST_CAST   - Test the C++ compiler's support for
dnl                            const_cast<>.
dnl     DPP_CXX_EXCEPTIONS   - Test if the C++ compiler supports exceptions.
dnl
dnl Command-line options added:
dnl     --enable-std
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Add a command line option to enable or disable the use of standard C++ by
dnl the C++ compiler.  This includes use of the std namespace.
dnl
dnl Usage:
dnl     DPP_STD_CPP([default-std-setting])
dnl
dnl Arguments:
dnl     default-std-setting - The default value, "yes" or "no, for the use of
dnl                           standard C++ by the compiler.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_STD_CPP,
[
   AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

   AC_ARG_ENABLE(std,
                 [  --enable-std            Force the use of standard C++   [default=$1]],
                 dpp_std="$enableval", dpp_std=$1)
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's template-handling capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_TEMPLATES([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with templates.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_TEMPLATES,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ template support],
       [dpp_cv_CXX_handles_templates],
       [AC_TRY_COMPILE([template<class T> class Test {
public:
Test() {;}
};
template<class T> void f (const Test<T>&){;}],
                      [
Test<double> d;
Test<int> i;
f(d);
f(i);
return 0; ],
                      [ dpp_cv_CXX_handles_templates='yes'
                        rm -rf ./ii_files ./SunWS_cache ],
                      [dpp_cv_CXX_handles_templates='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_templates" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_TEMPLATES, ,
                [define if the C++ compiler supports basic templates])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for default parameters with templates.
dnl
dnl Usage:
dnl     DPP_CXX_DEFAULT_TEMPLATE_PARAMETERS([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not handle default parameters for templates.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_DEFAULT_TEMPLATE_PARAMETERS,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])
   AC_REQUIRE([DPP_CXX_TEMPLATES])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports default template parameters],
      [dpp_cv_CXX_default_template_parameters],
      [AC_TRY_COMPILE([
template<class T = double, int N = 10> class Test {
public:
    int func (void) { return 0; }
};
],
      [Test<float> obj; return obj.func();],
      [ dpp_cv_CXX_default_template_parameters='yes'
        rm -rf ./ii_files ./SunWS_cache ],
      dpp_cv_CXX_default_template_parameters='no')])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_default_template_parameters" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE([HAVE_DEFAULT_TEMPLATE_PARAMETERS], ,
         [define if the C++ compiler supports default template parameters])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's namespace-handling capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_NAMESPACE([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with namespaces.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_NAMESPACE,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ namespace support],
      [dpp_cv_CXX_handles_namespace],
      [AC_TRY_COMPILE([ namespace dpp_test { void func (void) {;} }; ],
                     [ dpp_test::func(); ],
                     [ dpp_cv_CXX_handles_namespace='yes'
                       rm -rf ./ii_files ./SunWS_cache ],
                     [dpp_cv_CXX_handles_namespace='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_namespace" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_NAMESPACE, ,
                [define if the C++ compiler supports namespaces])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the C++ compiler supports the std namespace and the standard C++
dnl library.
dnl
dnl Usage:
dnl     DPP_CXX_HAVE_STD([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler does
dnl                             not support the standard library.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_HAVE_STD,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])
   AC_REQUIRE([DPP_CXX_NAMESPACE])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ standard library support], [dpp_cv_CXX_have_std],
      [AC_TRY_COMPILE(
[
#include <iostream>
#include <map>
], ,
          [ dpp_cv_CXX_have_std='yes'
             rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_have_std='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_have_std" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_STD, ,
                [define if the C++ compiler supports the standard library])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's inlining capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_INLINE([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with the use of the inline modifier.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_INLINE,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ inline support],
      [dpp_cv_CXX_handles_inline],
      [AC_TRY_COMPILE([ inline void func (void) {;}], [ func(); ],
                     [ dpp_cv_CXX_handles_inline='yes'
                       rm -rf ./ii_files ./SunWS_cache ],
                     [dpp_cv_CXX_handles_inline='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_inline" = "xno" ; then
      ifelse([$1], , :, [$1])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's RTTI capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_RTTI([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler does
dnl                             not implement RTTI.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_RTTI,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ RTTI support], [dpp_cv_CXX_handles_rtti],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual int f (void) { return 0; }
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual int f (void) { return 1; }
}; ],
          [
Derived d;
Base* ptr = &d;
return (typeid(*ptr) == typeid(Derived)); ],
          [ dpp_cv_CXX_handles_rtti='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_handles_rtti='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_rtti" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_RTTI, , [define if the C++ compiler supports RTTI])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for static_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support static_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_STATIC_CAST,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports static_cast<>],
      [dpp_cv_CXX_static_cast],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual void f (void) = 0;
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual void f (void) {;}
};

int testFunc (Derived& obj)
{
   return 0;
}],
           [
Derived d;
Base& b = d;
Derived& d2 = static_cast<Derived&>(b);
return testFunc(d2); ],
           [ dpp_cv_CXX_static_cast='yes'
             rm -rf ./ii_files ./SunWS_cache ],
           [dpp_cv_CXX_static_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_static_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_STATIC_CAST, ,
                [define if the C++ compiler supports static_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for dynamic_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support dynamic_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_DYNAMIC_CAST,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports dynamic_cast<>],
      [dpp_cv_CXX_dynamic_cast],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual void f (void) = 0;
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual void f (void) {;}
};],
          [
Derived d;
Base& b = d;
return (dynamic_cast<Derived*>(&b) ? 0 : 1); ],
          [ dpp_cv_CXX_dynamic_cast='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_dynamic_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_dynamic_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_DYNAMIC_CAST, ,
                [define if the C++ compiler supports dynamic_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for const_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support const_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_CONST_CAST,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports const_cast<>],
      [dpp_cv_CXX_const_cast],
      [AC_TRY_COMPILE( , [
int x = 0;
const int& y = x;
int& z = const_cast<int&>(y);
return z;],
          [ dpp_cv_CXX_const_cast='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_const_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_const_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_CONST_CAST, ,
                [define if the C++ compiler supports const_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the C++ compiler supports exceptions.
dnl
dnl Usage:
dnl     DPP_CXX_EXCEPTIONS([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support exceptions.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_EXCEPTIONS,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ exception support], [dpp_cv_CXX_has_exceptions],
      [AC_TRY_COMPILE( , [try { throw 1; } catch (int i) { return i; }],
         [dpp_cv_CXX_has_exceptions='yes'], [dpp_cv_CXX_has_exceptions='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_has_exceptions" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_EXCEPTIONS, ,
                [define if the C++ compiler supports exceptions])
   fi
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Determine which C and C++ compilers to use for compiling.  The
dnl capabilities of the C++ compiler are tested here as well to ensure that
dnl the proper language support is available.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_COMPILER_FLAG       - Check if the given compiler accepts a given
dnl                               flag.
dnl     DPP_SETUP_COMPILER      - Setup software development environment for
dnl                               building C and C++ code.
dnl     DPP_PROG_CC             - Check for the existence and usability of a C
dnl                               compiler.
dnl     DPP_PROG_CC_NOSTDINC    - Check if the C compiler supports clearing
dnl                               the default include path.
dnl     DPP_PROG_CC_ANSI        - Check if the C compiler supports -ansi.
dnl     DPP_PROG_CC_PIPE        - Check if the C compiler supports -pipe.
dnl     DPP_PROG_CC_PROF_P      - Check if the C compiler supports -p.
dnl     DPP_PROG_CC_PROF_PG     - Check if the C compiler supports -p.
dnl     DPP_GET_EXT             - Get the extensions for object files and
dnl                               executables.
dnl     DPP_PROG_CXX            - Check for the existence and usability of a
dnl                               C++ compiler.
dnl     DPP_PROG_CXX_NOSTDINCXX - Check if the C++ compiler supports clearing
dnl                               the default C++ include path.
dnl     DPP_PROG_CXX_ANSI       - Check if the C++ compiler supports -ansi.
dnl     DPP_PROG_CXX_PIPE       - Check if the C++ compiler supports -pipe.
dnl     DPP_PROG_CXX_PROF_P     - Check if the C++ compiler supports -p.
dnl     DPP_PROG_CXX_PROF_PG    - Check if the C++ compiler supports -pg.
dnl
dnl Variables defined:
dnl     AR                   - The archiver program (ar(1) for example).
dnl     ARFLAGS              - Basic flags passed to $AR.
dnl     AR_NAME_FLAG         - The flag passed to $AR to specify the name of
dnl                            the archive being generated.
dnl     CC                   - The C compiler.
dnl     CC_NOSTDINC_FLAGS    - The flag(s) that will clear the default include
dnl                            path for the C compiler (if any).
dnl     CC_ANSI_FLAGS        - The flag(s) to require ANSI C compliance.
dnl     CC_PIPE_FLAGS        - The flag(s) to use pipes instead of temporary
dnl                            files when compiling.
dnl     CXX_NOSTDINCXX_FLAGS - The flag(s) that will clear the default include
dnl                            path for the C++ compiler (if any).
dnl     CFLAGS               - All options passed to the C compiler.
dnl     CFLAGS_DYNLIB        - Position-independent code generation flag for
dnl                            the C compiler.  Substituted automatically.
dnl     CPP                  - The C compiler's preprocessor.
dnl     CXX                  - The C++ compiler.
dnl     CXX_ANSI_FLAGS       - The flag(s) to require ANSI C++ compliance.
dnl     CXX_PIPE_FLAGS       - The flag(s) to use pipes instead of temporary
dnl                            files when compiling.
dnl     CXXFLAGS             - All options passed to the C++ compiler.
dnl     CXXFLAGS_DYNLIB      - Position-independent code generation flag for
dnl                            the C++ compiler.  Substituted automatically.
dnl     CXXCPP               - The C++ compiler's preprocessor.
dnl     C_DLL                - The command to generate a DLL (shared library)
dnl                            from C object files.
dnl     CXX_DLL              - The command to generate a DLL (shared library)
dnl                            from C++ object files.
dnl     C_PLUGIN             - The command to generate a C plug-in (code loaded
dnl                            at runtime, usually with dlopen(3) or equivalent
dnl                            system call).
dnl     CXX_PLUGIN           - The command to generate a C++ plug-in (code
dnl                            loaded at runtime, usually with dlopen(3) or
dnl                            equivalent system call).
dnl     DEP_GEN_FLAG         - Compiler flag for generating source file
dnl                            dependencies.
dnl     DEPEND_FLAGS         - Flags for building dependencies (typically with
dnl                            makedepend(1)).
dnl     DEPEND_EXTRAS        - Miscellaneous flags that the dependency
dnl                            generator may or may not recognize.  If it does
dnl                            not recognize them, they will be ignored.
dnl     DYLIB_NAME_FLAG      - Flag used for naming a compiled dynamic library.
dnl     EXE_NAME_FLAG        - Flag used for naming a compiled executable file.
dnl     INCLUDES             - Extensions to the compiler's default include
dnl                            path.
dnl     LD                   - The linker program (ld(1) for example).
dnl     LIBS                 - Libraries needed for linking.
dnl     LDOPTS               - Basic options for $LD.
dnl     LDOPTS_DBG           - Options for $LD when making a library with
dnl                            debugging symbols.
dnl     LDOPTS_OPT           - Options for $LD when making an optimized
dnl                            library.
dnl     LDOPTS_PROF          - Options for $LD when making a profiled library.
dnl     OBJ_NAME_FLAG        - Flag used for naming a compiled object file.
dnl     OBJ_BUILD_FLAG       - Flag passed to compiler to build a compiled
dnl                            object file.
dnl
dnl     DBG_FLAGS            - Complier flags for adding debugging symbols.
dnl     OPT_FLAGS            - Complier flags for compiling optimized code.
dnl
dnl     LDFLAGS_STATIC       - Linker flags that invoke static linking.
dnl     LDFLAGS_DYNAMIC      - Linker flags that invoke dynamic linking.
dnl
dnl     LIB_PREFIX           - Prefix for library names.
dnl     STATICLIB_EXT        - Filename extension for static library names.
dnl     DYNAMICLIB_EXT       - Filename extension for dynamic library names.
dnl
dnl     DSOREGFILE
dnl     DSOVERSION
dnl     DSOVERSIONOPTS
dnl
dnl Possible preprocssor symbols defined:
dnl     WIN32
dnl     _MBCS
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Check if the given compiler accepts a given flag.  This can be used for
dnl all compiler flags except -c.
dnl
dnl Usage:
dnl     DPP_COMPILER_FLAG(compiler, flag, cache-var)
dnl
dnl Arguments:
dnl     compiler  - The compiler to use for the check.
dnl     flag      - The flag to check.
dnl     cache-var - The cache variable used to store the result of the test.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_COMPILER_FLAG,
[
   AC_CACHE_CHECK([whether $1 accepts $2], $3,
[echo 'void f(){;}' > conftest.$ac_ext
if test -z "`$1 $2 -c conftest.$ac_ext 2>&1`"; then
  $3='yes'
else
  $3='no'
fi
rm -f conftest*
])
])

dnl ---------------------------------------------------------------------------
dnl Based on the given detected host and CPU, set up the system-specific
dnl configuration for commonly used software developer tools.
dnl
dnl Usage:
dnl     DPP_SETUP_COMPILER
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SETUP_COMPILER,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl Archive library command (generally ar(1)) and associated flags.
   dnl $AR_NAME_FLAG is an extra (optional) argument that allows $AR to
   dnl name its output file.
   AR=''
   ARFLAGS=''
   AR_NAME_FLAG=''

   dnl Flag for the compiler to generate dependencies
   DEP_GEN_FLAG='-M'

   dnl Shared library command (usually ld(1)) and associated flags.
   LD=''
   LDOPTS=''
   LDOPTS_DBG=''
   LDOPTS_OPT=''
   LDOPTS_PROF=''
   C_DLL=''
   CXX_DLL=''
   C_PLUGIN=''
   CXX_PLUGIN=''

   dnl Flag used for naming a compiled object file.
   OBJ_NAME_FLAG='-o $[@]'
   OBJ_BUILD_FLAG='-c'
   EXE_NAME_FLAG='-o $[@]'
   DYLIB_NAME_FLAG="$EXE_NAME_FLAG"

   dnl Library naming conventions (prefix, extension for static libraries,
   dnl extension for dynamic libraries respectively).
   LIB_PREFIX='lib'
   STATICLIB_EXT='a'
   DYNAMICLIB_EXT=''

   dnl Compiler object file debugging flags and optimization flags
   dnl respectively.
   DBG_FLAGS=''
   OPT_FLAGS=''

   dnl Initialize these here just to be safe.  Subsequent code will fill in
   dnl the details.
   C_WARNS_LEVEL_0=''
   C_WARNS_LEVEL_1=''
   C_WARNS_LEVEL_2=''
   C_WARNS_LEVEL_3=''
   C_WARNS_LEVEL_4=''
   C_WARNS_LEVEL_5=''
   CXX_WARNS_LEVEL_0=''
   CXX_WARNS_LEVEL_1=''
   CXX_WARNS_LEVEL_2=''
   CXX_WARNS_LEVEL_3=''
   CXX_WARNS_LEVEL_4=''
   CXX_WARNS_LEVEL_5=''

   dnl IRIX junk.
   DSOREGFILE=''
   DSOVERSION=''
   DSOVERSIONOPTS=''

   USE_MAKEDEPEND='N'

   dpp_opt_level=${OPT_LEVEL-2}

   dnl The OS for which this is being configured.  The operating systems are
   dnl listed alphabetically.
   case $target_os in
      dnl IBM RS/6000 running AIX.
      aix*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
      cygwin)
         if test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)
         else
            AR='link /lib'
            ARFLAGS="$ARFLAGS /nologo"
            AR_NAME_FLAG='/out:'
            CFLAGS="$CFLAGS /nologo"
            CXXFLAGS="$CXXFLAGS /nologo"
            DBG_FLAGS='/Od /GZ /Z7'
            DEP_GEN_FLAG='/FD'
            OPT_FLAGS="/Ogityb$dpp_opt_level /GB"
            LD='link /dll'
            LDOPTS="$LDOPTS /nologo"
            LDOPTS_DBG='/DEBUG'
            LDOPTS_OPT='/RELEASE'
            C_DLL='link /dll'
            CXX_DLL='link /dll'
            C_PLUGIN='link /dll'
            CXX_PLUGIN='link /dll'
            OBJ_NAME_FLAG='/Fo$[@]'
            OBJ_BUILD_FLAG='/c'
            EXE_NAME_FLAG='/OUT:$[@]'
            DYLIB_NAME_FLAG="$EXE_NAME_FLAG"

            USE_MAKEDEPEND='Y'
         fi

         DEPEND_EXTRAS="-D_WIN32 -D_M_IX86 $DEPEND_EXTRAS"
         LIB_PREFIX=''
         STATICLIB_EXT='lib'
         DYNAMICLIB_EXT='dll'

         AC_DEFINE(WIN32, , [define if compiling on a Win32 platform])
         AC_DEFINE(_MBCS, , [define if compiling on a Win32 platform])
         ;;
      dnl A machine running OSX
      darwin*)
         AR='ar'
         ARFLAGS='-ruv'
         LD='$(CXX) -dynamiclib'
         LDOPTS=''
         C_DLL='$(CC) -dynamiclib'
         CXX_DLL='$(CXX) -dynamiclib'
         C_PLUGIN='$(CC) -bundle'
         CXX_PLUGIN='$(CXX) -bundle'
         OPT_FLAGS="-O$dpp_opt_level"
         DBG_FLAGS='-g'
         DYNAMICLIB_EXT='dylib'
         CFLAGS="-fno-common $CFLAGS"
         CXXFLAGS="-fno-common $CXXFLAGS"
         ;;
      dnl A machine running FreeBSD.
      freebsd*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl HP PA-RISC machine running HP-UX 10.20.
      hpux10.20)
         DPP_SETUP_GCC($dpp_opt_level)

         if test "x$USE_GCC" != "xyes" ; then
            CFLAGS="-Aa $CFLAGS"
            CXXFLAGS="+p $CXXFLAGS"
         fi

         LD='ld'
         C_DLL='ld'
         CXX_DLL='ld'
         C_PLUGIN='ld'
         CXX_PLUGIN='ld'
         LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
         LDOPTS='-b'
         OPT_FLAGS='+inline_level 3'
         DYNAMICLIB_EXT='sl'
         ;;
      dnl HP PA-RISC machine running HP-UX 11.x.
      hpux11*)
         DPP_SETUP_GCC($dpp_opt_level)

         if test "x$USE_GCC" != "xyes" ; then
            CFLAGS="-Aa $CFLAGS"
            CXXFLAGS="-AA $CXXFLAGS"
            DEP_GEN_FLAG='-E +m'
            LD='$(CXX)'
            C_DLL='$(CC)'
            CXX_DLL='$(CXX)'
            C_PLUGIN='$(CC)'
            CXX_PLUGIN='$(CXX)'
            LDOPTS='-b'
         else
            LD='ld'
            C_DLL='ld'
            CXX_DLL='ld'
            C_PLUGIN='ld'
            CXX_PLUGIN='ld'
         fi

         LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
         OPT_FLAGS=''
         DYNAMICLIB_EXT='sl'
         ;;
      dnl A machine running IRIX 6.x.
      irix6*)
         if test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)

            dpp_cc_defs_file='/etc/compiler.defaults'

            if test -r "$dpp_cc_defs_file" ; then
               dpp_cc_defs=`cat $dpp_cc_defs_file`
               dpp_abi_val=`echo $dpp_cc_defs | sed -e 's/.*abi=\([[a-z0-9]]\{2,3\}\).*/\1/'`
               dpp_isa_val=`echo $dpp_cc_defs | sed -e 's/.*isa=\(mips[[0-9]]\).*/\1/'`
 
               dpp_abi_isa_name=''

               if test "x$dpp_abi_val" = "xn32" ; then
                  dpp_abi_isa_name='N32_'
                  dpp_abi_val='N32'
                  dpp_bit_suffix='32'
               elif test "x$dpp_abi_val" = "x64" ; then
                  dpp_abi_isa_name='64_'
                  dpp_bit_suffix='64'
               else
                  AC_MSG_WARN([*** Unknown ABI type $dpp_abi_val ***])
               fi

               if test "x$dpp_isa_val" = "xmips3" ; then
                  dpp_abi_isa_name="${dpp_abi_isa_name}M3"
               elif test "x$dpp_isa_val" = "xmips4" ; then
                  dpp_abi_isa_name="${dpp_abi_isa_name}M4"
               else
                  AC_MSG_WARN([*** Unknown ISA type $dpp_isa_val ***])
               fi

               DPP_ABI_CFG([$dpp_abi_val], [$dpp_isa_val], [$dpp_bit_suffix])
               ABI_LIST="$dpp_abi_isa_name"
            else
               AC_MSG_WARN([*** Defaulting ABI list to N32_M3 ***])
               ABI_LIST='N32_M3'
            fi
         else
            STDFLAGS=''

            if test "x$dpp_std" = "xyes" ; then
               STDFLAGS='-LANG:std'
            fi

            dnl Set compiler flags and the location of $DSOREGFILE
            dnl depending on $ABI.
            if test "x$ABI" = "x64" ; then
               DSOREGFILE='/usr/lib64/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -OPT:Olimit=0"
            elif test "x$ABI" = "xN32" ; then
               DSOREGFILE='/usr/lib32/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -OPT:Olimit=0"
            elif test "x$ABI" = "xO32" ; then
               DSOREGFILE='/usr/lib/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -Olimit 0"
            fi

            AR='$(CXX)'
            ARFLAGS="$STDFLAGS -J6 -ar -WR,-v -o"

            CXXFLAGS="$CXXFLAGS $STDFLAGS"

            DBG_FLAGS="-g"
dnl            _LD_REGOPTS='-check_registry $(DSOREGFILE)'
            LDOPTS="$LDOPTS $STDFLAGS -all ${_LD_REGOPTS} -elf -rdata_shared"
            LDFLAGS_STATIC='-B static'
            LDFLAGS_DYNAMIC='-B dynamic'


            DSOVERSION='sgi4.0'
            DSOVERSIONOPTS='-set_version $(DSOVERSION)'

            ABI_LIST='64_M4 64_M3 N32_M4 N32_M3'
         fi

         LD='$(CXX) -shared'
         C_DLL='$(CC) -shared'
         CXX_DLL='$(CXX) -shared'
         C_PLUGIN='$(CC) -shared'
         CXX_PLUGIN='$(CXX) -shared'
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running Linux.
      linux*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running NetBSD.
      netbsd*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running OpenBSD.
      openbsd*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl DEC Alpha running Digital UNIX 4.0.
      osf4.0)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running Solaris (aka SunOS).
      solaris*)
         dnl GCC.
         if test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)
         dnl Sun WorkShop.
         else
            AR='ar'
            ARFLAGS='-ruv'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DBG_FLAGS='-g'
            DEP_GEN_FLAG='-xM'
         fi

         LD='$(CXX) -G'
         C_DLL='$(CC) -G'
         CXX_DLL='$(CXX) -G'
         C_PLUGIN='$(CC) -G'
         CXX_PLUGIN='$(CXX) -G'
         DYNAMICLIB_EXT='so'
         ;;
   esac

   AC_SUBST(AR)
   AC_SUBST(ARFLAGS)
   AC_SUBST(AR_NAME_FLAG)
   AC_SUBST(CFLAGS)
   AC_SUBST(CXXFLAGS)
   AC_SUBST(C_DLL)
   AC_SUBST(CXX_DLL)
   AC_SUBST(C_PLUGIN)
   AC_SUBST(CXX_PLUGIN)
   AC_SUBST(DEP_GEN_FLAG)
   AC_SUBST(DEPEND_FLAGS)
   AC_SUBST(DEPEND_EXTRAS)
   AC_SUBST(INCLUDES)
   AC_SUBST(LD)
   AC_SUBST(LIBS)
   AC_SUBST(LDOPTS)
   AC_SUBST(LDOPTS_DBG)
   AC_SUBST(LDOPTS_OPT)
   AC_SUBST(LDOPTS_PROF)
   AC_SUBST(OBJ_NAME_FLAG)
   AC_SUBST(OBJ_BUILD_FLAG)
   AC_SUBST(EXE_NAME_FLAG)
   AC_SUBST(DYLIB_NAME_FLAG)

   AC_SUBST(DBG_FLAGS)
   AC_SUBST(OPT_FLAGS)

   AC_SUBST(LDFLAGS_STATIC)
   AC_SUBST(LDFLAGS_DYNAMIC)

   AC_SUBST(LIB_PREFIX)
   AC_SUBST(STATICLIB_EXT)
   AC_SUBST(DYNAMICLIB_EXT)

   AC_SUBST(C_WARNS_LEVEL_0)
   AC_SUBST(C_WARNS_LEVEL_1)
   AC_SUBST(C_WARNS_LEVEL_2)
   AC_SUBST(C_WARNS_LEVEL_3)
   AC_SUBST(C_WARNS_LEVEL_4)
   AC_SUBST(C_WARNS_LEVEL_5)
   AC_SUBST(CXX_WARNS_LEVEL_0)
   AC_SUBST(CXX_WARNS_LEVEL_1)
   AC_SUBST(CXX_WARNS_LEVEL_2)
   AC_SUBST(CXX_WARNS_LEVEL_3)
   AC_SUBST(CXX_WARNS_LEVEL_4)
   AC_SUBST(CXX_WARNS_LEVEL_5)

   AC_SUBST(DSOREGFILE)
   AC_SUBST(DSOVERSION)
   AC_SUBST(DSOVERSIONOPTS)

   AC_SUBST(USE_MAKEDEPEND)
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of a C compiler (or the given C
dnl compiler if one is specified) and how to run its preprocessor.  A
dnl platform-specific hint for the C compiler can be given by setting a value
dnl for the variable $CC_<PLATFORM> (e.g., $CC_Linux).  A value for the
dnl C-compiler argument overrides any hint given, however.  A list of
dnl fallbacks can be provided by setting a value for the variable
dnl $CC_FALLBACKS.  This must be in the format of an sh list.
dnl
dnl Usage:
dnl     DPP_PROG_CC([C-compiler [, extra-flags [, action-if-not-found [, path ]]]])
dnl
dnl Arguments:
dnl     C-compiler          - Force the check to look for this C compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C compiler cannot be
dnl                           found or does not work.  This is optional.
dnl     path                - Extra path information for finding the C
dnl                           compiler.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC,
[
   AC_REQUIRE([DPP_SETUP_COMPILER])

   dpp_save_CFLAGS="$CFLAGS"
   CFLAGS="$CFLAGS $2 ${_EXTRA_FLAGS}"

   dnl Get the platform-specific compiler hint value.
   dpp_cc_var="CC_$dpp_platform"
   dpp_cc_val=`eval echo '$''{'$dpp_cc_var'}'`

   if test "x$dpp_cc_val" != "x" -a "x$CC" = "x" ; then
      CC="$dpp_cc_val"
   fi

   if test "x$1" != "x" -o "x$CC" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_CC_found_val=$1
         AC_CHECK_PROG([dpp_CC_check_val], $1, $1, , $4)
      else
         dpp_CC_found_val="$CC"
         AC_CHECK_PROG([dpp_CC_check_val], [$CC], [$CC], , $4)
      fi

      if test "x$dpp_CC_check_val" = "x$dpp_CC_found_val" ; then
         CC="$dpp_CC_found_val"
      else
         dpp_CC_found='no'

         dnl If the caller provided a list of fallbacks in the $CC_FALLBACKS
         dnl variable, loop over them and try to use another C compiler.
         if test "x$CC_FALLBACKS" != "x" ; then
            for dpp_cc in $CC_FALLBACKS ; do
               AC_CHECK_PROG([dpp_CC_check_val], [$dpp_cc], [$dpp_cc], , $4)
               if test "x$dpp_CC_check_val" = "x$dpp_cc" ; then
                  CC="$dpp_cc"
                  dpp_CC_found='yes'
                  break
               fi
            done
         fi

         dnl No viable C compiler was found.
         if test "x$dpp_CC_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_PROG_CC
   AC_PROG_CPP

   CFLAGS="$dpp_save_CFLAGS"

   dnl If GCC will be the C compiler, -fPIC is the position-independent code
   dnl generation option.
   if test "x$GCC" = "xyes" ; then
      CFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_cc_pic_var="CC_PIC_$dpp_platform"
      CFLAGS_DYNLIB=`eval echo '$''{'$dpp_cc_pic_var'}'`
   fi

   CFLAGS_DYNLIB="$CFLAGS_DYNLIB -DPIC"

   AC_SUBST(CFLAGS_DYNLIB)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports clearing the default include path.  If
dnl -nostdinc is not supporte, try -I- as a fallback.  Depending on the
dnl results of the tests, $CC_NOSTDINC_FLAGS will be set to '-nostdinc',
dnl '-I-', or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_NOSTDINC
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC_NOSTDINC,
[
   DPP_COMPILER_FLAG([${CC-cc}], [-nostdinc], [dpp_cv_prog_cc_nostdinc])

   dnl If the compiler supports -nostdinc, use it to clear the include path.
   if test "x$dpp_cv_prog_cc_nostdinc" = "xyes" ; then
      dpp_cc_nostdinc='yes'
      CC_NOSTDINC_FLAGS='-nostdinc'
   dnl Otherwise, check if -I- will do the job.
   else
      dpp_cc_nostdinc='no'

      DPP_COMPILER_FLAG([${CC-cc}], [-I-], [dpp_cv_prog_cc_inc_clear])

      if test "x$dpp_cv_prog_cc_nostdinc" = "xyes" ; then
         dpp_cc_inc_clear='yes'
         CC_NOSTDINC_FLAGS='-I-'
      else
         CC_NOSTDINC_FLAGS=''
         dpp_cc_inc_clear='no'
      fi
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -ansi.  Based on the results of the test,
dnl $CC_ANSI_FLAGS will be set to '-ansi' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_ANSI
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC_ANSI,
[
   DPP_COMPILER_FLAG([${CC-cc}], [-ansi], [dpp_cv_prog_cc_ansi])

   if test "x$dpp_cv_prog_cc_ansi" = "xyes" ; then
      dpp_cc_ansi='yes'
      CC_ANSI_FLAGS='-ansi'
   else
      dpp_cc_ansi='no'
      CC_ANSI_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -pipe.  Based on the results of the test,
dnl $CC_PIPE_FLAGS will be set to '-pipe' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PIPE
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC_PIPE,
[
   DPP_COMPILER_FLAG([${CC-cc}], [-pipe], [dpp_cv_prog_cc_pipe])

   if test "x$dpp_cv_prog_cc_pipe" = "xyes" ; then
      dpp_cc_pipe='yes'
      CC_PIPE_FLAGS='-pipe'
   else
      dpp_cc_pipe='no'
      CC_PIPE_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -p.  Based on the results of the test,
dnl $CC_PROF_FLAGS will be set to '-p' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PROF_P
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC_PROF_P,
[
   DPP_COMPILER_FLAG([${CC-cc}], [-p], [dpp_cv_prog_cc_prof_p])

   if test "x$dpp_cv_prog_cc_prof_p" = "xyes" ; then
      dpp_cc_prof_p='yes'
      CC_PROF_FLAGS='-p'
   else
      dpp_cc_prof_p='no'
      CC_PROF_FLAGS=''
   fi

   AC_SUBST(CC_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -pg.  Based on the results of the test,
dnl $CC_PROF_FLAGS will be set to '-pg' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PROF_PG
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC_PROF_PG,
[
   DPP_COMPILER_FLAG([${CC-cc}], [-pg], [dpp_cv_prog_cc_prof_pg])

   if test "x$dpp_cv_prog_cc_prof_pg" = "xyes" ; then
      dpp_cc_prof_pg='yes'
      CC_PROF_FLAGS='-pg'
   else
      dpp_cc_prof_pg='no'
      CC_PROF_FLAGS=''
   fi

   AC_SUBST(CC_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Get the extensions for object files and executables.  The extension
dnl strings are stored in $OBJEXT and $EXEEXT.
dnl
dnl Usage:
dnl     DPP_GET_EXT
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_GET_EXT,
[
   AC_REQUIRE([DPP_PROG_CC])

   dnl Determine the suffix for object file names.
   AC_OBJEXT

   dnl Determine the suffix for executable file names.
   AC_EXEEXT
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of a C++ compiler (or the given C++
dnl compiler if one is specified) and how to run its preprocessor.  A
dnl platform-specific hint for the C++ compiler can be given by setting a
dnl value for the variable $CXX_<PLATFORM> (e.g., $CXX_Linux).  A value for
dnl the C++-compiler argument overrides any hint given, however.  A list of
dnl fallbacks can be provided by setting a value for the variable
dnl $CXX_FALLBACKS.  This must be in the format of an sh list.
dnl
dnl Usage:
dnl     DPP_PROG_CXX([C++-compiler [, extra-flags [, action-if-not-found [, path]]]])
dnl
dnl Arguments:
dnl     C++-compiler        - Force the check to look for this C++ compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C++ compiler cannot be
dnl                           found or does not work.  This is optional.
dnl     path                - Extra path information for finding the C++
dnl                           compiler.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX,
[
   AC_REQUIRE([DPP_SETUP_COMPILER])

   dpp_save_CXXFLAGS="$CXXFLAGS"
   CXXFLAGS="$CXXFLAGS $2 ${_EXTRA_FLAGS}"

   dnl Get the platform-specific compiler hint value.
   dpp_cxx_var="CXX_$dpp_platform"
   dpp_cxx_val=`eval echo '$''{'$dpp_cxx_var'}'`

   if test "x$dpp_cxx_val" != "x" -a "x$CXX" = "x" ; then
      CXX="$dpp_cxx_val"
   fi

   if test "x$1" != "x" -o "x$CXX" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_CXX_found_val=$1
         AC_CHECK_PROG([dpp_CXX_check_val], $1, $1, , $4)
      else
         dpp_CXX_found_val="$CXX"
         AC_CHECK_PROG([dpp_CXX_check_val], [$CXX], [$CXX], , $4)
      fi

      if test "x$dpp_CXX_check_val" = "x$dpp_CXX_found_val" ; then
         CXX="$dpp_CXX_found_val"
      else
         dpp_CXX_found='no'

         dnl If the caller provided a list of fallbacks in the
         dnl $CXX_FALLBACKS variable, loop over them and try to use another
         dnl C++ compiler.
         if test "x$CXX_FALLBACKS" != "x" ; then
            for dpp_cxx in $CXX_FALLBACKS ; do
               AC_CHECK_PROG([dpp_CXX_check_val], [$dpp_cxx], [$dpp_cxx], , $4)
               if test "x$dpp_CXX_check_val" = "x$dpp_cxx" ; then
                  CXX="$dpp_cxx"
                  dpp_CXX_found='yes'
                  break
               fi
            done
         fi

         dnl No viable C++ compiler was found.
         if test "x$dpp_CXX_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_PROG_CXX
   AC_PROG_CXXCPP

   CXXFLAGS="$dpp_save_CXXFLAGS"

   dnl If GCC's g++ will be the C++ compiler, -fPIC is the
   dnl position-independent code generation option.
   if test "x$GXX" = "xyes" ; then
      CXXFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_cxx_pic_var="CXX_PIC_$dpp_platform"
      CXXFLAGS_DYNLIB=`eval echo '$''{'$dpp_cxx_pic_var'}'`
   fi

   CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -DPIC"

   AC_SUBST(CXXFLAGS_DYNLIB)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports clearing the default C++ include path.
dnl Depending olnt he results of the test, $CXX_NOSTDINCXX_FLAGS will be set
dnl to '-nostdinc++' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_NOSTDINCXX
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX_NOSTDINCXX,
[
   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS
   DPP_COMPILER_FLAG([${CXX-c++}], [-nostdinc++], [dpp_cv_prog_cxx_nostdinc])
   DPP_LANG_RESTORE

   if test "x$dpp_cv_prog_cxx_nostdincxx" = "xyes" ; then
      dpp_cxx_nostdincxx='yes'
      CXX_NOSTDINCXX_FLAGS='-nostdinc++'
   else
      dpp_cxx_nostdincxx='no'
      CXX_NOSTDINCXX_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -ansi.  Based on the results of the
dnl test, $CXX_ANSI_FLAGS will be set to '-ansi' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_ANSI
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX_ANSI,
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-ansi], [dpp_cv_prog_cxx_ansi])

   if test "x$dpp_cv_prog_cxx_ansi" = "xyes" ; then
      dpp_cxx_ansi='yes'
      CXX_ANSI_FLAGS='-ansi'
   else
      dpp_cxx_ansi='no'
      CXX_ANSI_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -pipe.  Based on the results of the
dnl test, $CXX_PIPE_FLAGS will be set to '-pipe' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PIPE
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX_PIPE,
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-pipe], [dpp_cv_prog_cxx_pipe])

   if test "x$dpp_cv_prog_cxx_pipe" = "xyes" ; then
      dpp_cxx_pipe='yes'
      CXX_PIPE_FLAGS='-pipe'
   else
      dpp_cxx_pipe='no'
      CXX_PIPE_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -p.  Based on the results of the test,
dnl $CXX_PROF_FLAGS will be set to '-p' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PROF_P
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX_PROF_P,
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-p], [dpp_cv_prog_cxx_prof_p])

   if test "x$dpp_cv_prog_cxx_prof_p" = "xyes" ; then
      dpp_cxx_prof_p='yes'
      CXX_PROF_FLAGS='-p'
   else
      dpp_cxx_prof_p='no'
      CXX_PROF_FLAGS=''
   fi

   AC_SUBST(CXX_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -pg.  Based on the results of the test,
dnl $CXX_PROF_FLAGS will be set to '-pg' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PROF_PG
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX_PROF_PG,
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-pg], [dpp_cv_prog_cxx_prof_pg])

   if test "x$dpp_cv_prog_cxx_prof_pg" = "xyes" ; then
      dpp_cxx_prof_pg='yes'
      CXX_PROF_FLAGS='-pg'
   else
      dpp_cxx_prof_pg='no'
      CXX_PROF_FLAGS=''
   fi

   AC_SUBST(CXX_PROF_FLAGS)
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Perform tests related to the use of GCC as the compiler.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WITH_GCC         - Adds a command-line argument telling whether or
dnl                            not to force the use of GCC as the compiler
dnl                            suite.
dnl     DPP_SETUP_GCC        - Setup software development options that are
dnl                            tied to GCC.
dnl     DPP_GPLUSPLUS_VER    - Verify that the version of g++ available is at
dnl                            least the given version.
dnl     DPP_GPLUSPLUS_MODERN - Verify that the version of g++ is at least
dnl                            2.91 (EGCS 1.1.2).
dnl
dnl Command-line options added:
dnl     --with-gcc 
dnl
dnl Variables defined:
dnl     USE_GCC - This is set to "yes" if GCC is to be used.  If not, it will
dnl               be "no" or the empty string.  This is set by the
dnl               DPP_WITH_GCC macro.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Force the use of GCC as the compiler suite.
dnl
dnl Usage:
dnl     DPP_WITH_GCC(default-GCC-use)
dnl
dnl Arguments:
dnl     default-GCC-use - "yes" to default to using GCC or "no" to default to
dnl                       not using GCC.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_WITH_GCC,
[
   dnl Force the use of GCC as the compiler.
   AC_ARG_WITH(gcc,
               [  --with-gcc              Force the use of GCC as the compiler],
               USE_GCC="$withval", USE_GCC=$1)
   AC_SUBST(USE_GCC)
])

dnl ---------------------------------------------------------------------------
dnl Setup software development options that are tied to GCC.
dnl
dnl Usage:
dnl     DPP_SETUP_GCC
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SETUP_GCC,
[
   AR='ar'
   ARFLAGS='-ruv'
   LD='$(CXX) -shared'
   LDOPTS=''
   C_DLL='$(CC) -shared'
   CXX_DLL='$(CXX) -shared'
   C_PLUGIN='$(CC) -shared'
   CXX_PLUGIN='$(CXX) -shared'
   OPT_FLAGS="-O$1"
   DBG_FLAGS='-g'
   LDFLAGS_STATIC='-static'
   LDFLAGS_DYNAMIC=''

   # These are based on the FreeBSD 5.x build system settings.
   C_WARNS_LEVEL_0='-w'
   C_WARNS_LEVEL_1='-Wsystem-headers'
   C_WARNS_LEVEL_2="$C_WARNS_LEVEL_1 -Wall -Wno-format-y2k"
   C_WARNS_LEVEL_3="$C_WARNS_LEVEL_2 -W -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith"
   C_WARNS_LEVEL_4="$C_WARNS_LEVEL_3 -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wshadow -Wcast-align"
   C_WARNS_LEVEL_5="$C_WARNS_LEVEL_4"

   CXX_WARNS_LEVEL_0='-w'
])

dnl ---------------------------------------------------------------------------
dnl Verify that g++ is at least the given version.
dnl
dnl Usage:
dnl     DPP_GPLUSPLUS_VER(minimum-version [, action-if-not-found])
dnl
dnl Arguments:
dnl     minimum-version     - The minimum allowable version number.
dnl     action-if-not-found - The action to take if the version is less than
dnl                           that specified.  Optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_GPLUSPLUS_VER,
[
   AC_REQUIRE([DPP_PROG_CXX])

   dpp_gcc_ver=`$CXX -dumpversion` 
   DPP_VERSION_CHECK_MSG([$CXX], [$dpp_gcc_ver], [$1],
                         [dpp_cv_CXX_is_egcs], , [$2])
])

dnl ---------------------------------------------------------------------------
dnl Verify that g++ is at least 2.91 (aka, EGCS 1.1.2) since it implements the
dnl more modern C++ features such as templates.
dnl
dnl Usage:
dnl     DPP_GPLUSPLUS_MODERN([action-if-not-found])
dnl
dnl     action-if-not-found - The action to take if the version is less than
dnl                           that specified.  Optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_GPLUSPLUS_MODERN,
[
   DPP_GPLUSPLUS_VER([2.91],
      [AC_CHECK_PROG([CC], [egcc], [egcc], [$1])
       AC_CHECK_PROG([CXX], [eg++], [eg++], [$1])])
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl This file provides simple macros for language-specific state changes in a
dnl configure script.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_LANG_SAVE      - Save the current language state.
dnl     DPP_LANG_RESTORE   - Restore the previously saved language state.
dnl     DPP_LANG_C         - Set the current language to C.
dnl     DPP_LANG_CPLUSPLUS - Set the current language to C++.
dnl     DPP_LANG_JAVA      - Set the current language to Java.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Save the current language state.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LANG_SAVE, [ AC_LANG_SAVE ])

dnl ---------------------------------------------------------------------------
dnl Restore the previous language state.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LANG_RESTORE, [ AC_LANG_RESTORE ])

dnl ---------------------------------------------------------------------------
dnl Set the current language to C.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LANG_C, [ AC_LANG_C ])

dnl ---------------------------------------------------------------------------
dnl Set the current language to C++.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LANG_CPLUSPLUS,
[
   AC_LANG_CPLUSPLUS
   ac_ext='cpp'
])

dnl ---------------------------------------------------------------------------
dnl Set the current language to Java.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LANG_JAVA,
[
])

dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
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
dnl ************* <auto-copyright.pl END do not edit this line> *************

AC_DEFUN(VJ_STRIP_JAR_PATH,
[
   dnl Helper function that removes any path information leading up to a JAR
   dnl file name (i.e, a file that ends in .jar).
   strip_jar_path ( ) {
      jar_list=`echo $[1] | sed -e "s/$JCPS/ /g"`

      changequote(<<, >>)
      for jar in $jar_list ; do
         no_path_jar=`echo $jar | sed -e 's/^.*[/\]\([A-Za-z0-9_][A-Za-z0-9_.-]*\.jar\)/\1/'`
         no_path_jar_list="$no_path_jar_list $no_path_jar"
      done
      changequote([, ])

      dnl Strip off any leading spaces that would have been added in the above
      dnl loop.
      retval=`echo "$no_path_jar_list" | sed -e 's/^  *//'`
      echo "$retval"
   }

   $1=`strip_jar_path $2`
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_PROG_CC_PROF_FLAG([prefer-gnu])
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_PROG_CC_PROF_FLAG,
[
    if test "x$GCC" = "xyes" ; then
        if test "x$1" = "xyes" ; then
            DPP_PROG_CC_PROF_PG

            if test "x$CC_PROF_FLAGS" = "x" ; then
                DPP_PROG_CC_PROF_P
            fi
        else
            DPP_PROG_CC_PROF_P

            if test "x$CC_PROF_FLAGS" = "x" ; then
                DPP_PROG_CC_PROF_PG
            fi
        fi

        if test "x$CC_PROF_FLAGS" = "x" ; then
            BUILD_PROF_C='N'
        else
            BUILD_PROF_C='Y'
        fi
    else
        BUILD_PROF_C='N'
    fi

    AC_SUBST(BUILD_PROF_C)
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_PROG_CXX_PROF_FLAG([prefer-gnu])
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_PROG_CXX_PROF_FLAG,
[
    vrj_prefer_gnu_prof=$1

    if test "x$GXX" = "xyes" ; then
        if test "x$vrj_prefer_gnu_prof" = "xyes" ; then
            DPP_PROG_CXX_PROF_PG

            if test "x$CXX_PROF_FLAGS" = "x" ; then
                DPP_PROG_CXX_PROF_P
            fi
        else
            DPP_PROG_CXX_PROF_P

            if test "x$CXX_PROF_FLAGS" = "x" ; then
                DPP_PROG_CXX_PROF_PG
            fi
        fi

        if test "x$CXX_PROF_FLAGS" = "x" ; then
            BUILD_PROF_CXX='N'
        else
            BUILD_PROF_CXX='Y'
            LDOPTS_PROF="$LDOPTS_PROF $CXX_PROF_FLAGS"
        fi
    else
        BUILD_PROF_CXX='N'
    fi

    AC_SUBST(BUILD_PROF_CXX)
])

AC_DEFUN(VJ_COMPILER_SETUP,
[
    AC_REQUIRE([DPP_WITH_GCC])
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    # Override the C compiler that configure would otherwise find itself.
    AC_ARG_WITH(cc, [  --with-cc=<PATH>        Alternate C compiler ],
                _alt_cc="$withval", _alt_cc='none')

    # Override the C++ compiler that configure would otherwise find itself.
    AC_ARG_WITH(cxx, [  --with-cxx=<PATH>       Alternate C++ compiler ],
                _alt_cxx="$withval", _alt_cxx='none')

    # -------------------------------------------------------------------------
    # Checks for programs.
    # -------------------------------------------------------------------------
    CC_ERR=''
    CXX_ERR=''

    _vjsave_CFLAGS="$CFLAGS"
    _vjsave_CXXFLAGS="$CXXFLAGS"

    # If no alternate C compiler was specified, check to see if it is
    # necessary to force the use of a specific compiler on a given platform.
    if test "x${_alt_cc}" = "xnone" ; then
        # On IRIX, we can use MIPSpro or GCC.  We default to MIPSpro.
        if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
            CC_IRIX='cc'
            CC_PIC_IRIX='-KPIC'
        # On Solaris/SPARC, we can use Sun WorkShop or GCC.  We default to
        # WorkShop.
        elif test "x$PLATFORM" = "xSolaris" -a "x$USE_GCC" != "xyes" \
               -a "x$ISA" = "xsparc"
        then
            CC_Solaris='cc'
            CC_PIC_Solaris='-KPIC'
        fi
    # Otherwise, force the use of the alternate C compiler.
    else
        CC="${_alt_cc}"
    fi

    # If no alternate C++ compiler was specified, check to see if it is
    # necessary to force the use of a specific compiler on a given platform.
    if test "x${_alt_cxx}" = "xnone" ; then
        CXX_HP='aCC'

        # On HP-UX, we have to use the aCC C++ compiler.
        if test "$PLATFORM" = "HP" ; then
            CXX_ERR='*** The library requires the HP-UX aCC C++ compiler ***'
        # On Windows, we have to use the Microsoft Visual C++ compiler CL.EXE.
        elif test "$OS_TYPE" = "Win32" -a "x$USE_GCC" != "xyes" ; then
            # Prefer the use of cygcl over msvccc.
            DPP_PROG_CYGCL( , , [DPP_PROG_MSVCCC])
            CXX_ERR='*** The library requires the MS Visual C++ compiler ***'
        # On IRIX, we can use MIPSpro or GCC.  We default to MIPSpro.
        elif test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
            CXX_IRIX='CC'
            CXX_PIC_IRIX='-KPIC'
        # On Soalris/SPARC, we can use WorkShop or GCC.  We default to
        # WorkShop.
        elif test "x$PLATFORM" = "xSolaris" -a "x$USE_GCC" != "xyes" \
               -a "x$ISA" = "xsparc"
        then
            CXX_Solaris='CC'
            CXX_PIC_Solaris='-KPIC'
        fi
    # Otherwise, force the use of the alternate C++ compiler.
    else
        CXX="${_alt_cxx}"
    fi

    DPP_PROG_CC([$CC], , [AC_MSG_ERROR($CC_ERR)])
    DPP_PROG_CXX([$CXX], , [AC_MSG_ERROR($CXX_ERR)])
    DPP_GET_EXT

    # GCC is our compiler (g++ will compile the C++ code).
    if test "x$GXX" = "xyes" ; then
        # If we are using GCC as the compiler, we need to be using version
        # 3.0 or newer.
        DPP_GPLUSPLUS_VER(3.0,
                          [AC_MSG_ERROR([*** VR Juggler requires a modern G++ ***])])
    fi

    # Test to see if the C and C++ compilers support the -pipe (or equivalent)
    # argument.  If one or both of them do, then it is added to $CFLAGS and
    # $CXXFLAGS respectively.
    DPP_PROG_CC_PIPE
    DPP_PROG_CXX_PIPE
    CFLAGS="$CFLAGS $CC_PIPE_FLAGS"
    CXXFLAGS="$CXXFLAGS $CXX_PIPE_FLAGS"

    # The following sets up the warning levels for the compilers we support.
    # The levels we want for C++ are as follows (in increasing order of
    # noisiness):
    #
    #    0: Supress all warnings
    #    1: Basic warnings such as unused/unreferenced variables,
    #       uninitialziezd variables, initializer list disordering
    #    2: Overloaded virtual functions, sign promotion, non-virtual
    #       destructor
    #    3: Private constructor(s) and destructor, inability to inline
    #    4: Shadowing of methods, variables, etc.; unreachable code; ...
    #    5: Recommendations from Scott Meyers, cast issues, etc.
    if test "x$GXX" = "xyes" ; then
        # Doozer++ gives us reasonable defaults for C code.

        CXX_WARNS_LEVEL_0="-w"
        CXX_WARNS_LEVEL_1="-Wall"
        CXX_WARNS_LEVEL_2="$CXX_WARNS_LEVEL_1 -W -Woverloaded-virtual -Wsign-promo -Wnon-virtual-dtor"
        CXX_WARNS_LEVEL_3="$CXX_WARNS_LEVEL_2 -Winline -Wctor-dtor-privacy"
        CXX_WARNS_LEVEL_4="$CXX_WARNS_LEVEL_3 -Wshadow -Wunreachable-code"
        CXX_WARNS_LEVEL_5="$CXX_WARNS_LEVEL_4 -Weffc++ -Wold-style-cast"
    # MIPSpro is our compiler.  The warning levels go in reverse here be we
    # disable fewer warnings at higher levels.
    elif test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
        C_WARNS_LEVEL_0='-woff all'
        C_WARNS_LEVEL_1=''
        C_WARNS_LEVEL_2=''
        C_WARNS_LEVEL_3=''
        C_WARNS_LEVEL_4=''
        C_WARNS_LEVEL_5=''

        # The warnings we care about:
        #    1174: Unreferenced variable
        #    1183: Unsigned int compared to 0
        #    1552: Unused variable
        #    1681: Virtual function overload
        #    3303: Meaningless return type qualifier
        CXX_WARNS_LEVEL_0='-woff all'
        CXX_WARNS_LEVEL_1='-woff 1183,1681,3303'
        CXX_WARNS_LEVEL_2='-woff 1183,3303'
        CXX_WARNS_LEVEL_3='-woff 3303'
        CXX_WARNS_LEVEL_4=''
        CXX_WARNS_LEVEL_5=''
    # Visual C++ is our compiler.
    elif test "$OS_TYPE" = "Win32" -a "x$USE_GCC" != "xyes" ; then
        C_WARNS_LEVEL_0='/w'
        C_WARNS_LEVEL_1='/W1'
        C_WARNS_LEVEL_2='/W2'
        C_WARNS_LEVEL_3='/W3'
        C_WARNS_LEVEL_4='/W4'
        C_WARNS_LEVEL_5='/Wall'

        CXX_WARNS_LEVEL_0='/w'
        CXX_WARNS_LEVEL_1='/W1'
        CXX_WARNS_LEVEL_2='/W2'
        CXX_WARNS_LEVEL_3='/W3'
        CXX_WARNS_LEVEL_4='/W4'
        CXX_WARNS_LEVEL_5='/Wall'
    fi

    if test "x$GCC" = "xyes" ; then
        OPT_FLAGS="$OPT_FLAGS -fno-strict-aliasing"
    fi

    # Ensure that the C++ compiler we've found is capable of compiling the
    # newer C++ features that we need.
    DPP_CXX_TEMPLATES([AC_MSG_ERROR([*** The library requires C++ template support ***])])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will remove all duplicate strings
dnl from arg-list and assign the result to variable.  The given argument list
dnl is searched from beginning to end.  The first instance of a given argument
dnl is saved, and any subsequent duplicates are removed.  This macro is
dnl suitable for use with compiler arguments where the precendence of arguments
dnl is usually based on the first instance.
dnl
dnl Usage:
dnl     VJ_STRIP_DUPLICATE_ARGS(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                stripped list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of arguments from which duplicates will be removd.
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_STRIP_DUPLICATE_ARGS],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0;
   for ( i = 1; i <= NF; i++ )
   {
      # Here, valid_list acts like a string-based dictionary of all the
      # command-line arguments passwed to awk(1).  These come in as $1, $2,
      # etc.  That is the reason for the $i expressions.
      # On the other hand, arg_list acts as an ordered list and will contain
      # only those arguments not previously added.
      if ( ! valid_list[$i] )
      {
         valid_list[$i] = 1;
         arg_list[j] = $i;
         j++;
      }
   }

   # Now, build result as a space-separated string of everything in arg_list.
   result = arg_list[0]
   for ( i = 1; i < j; i++ )
      result = sprintf("%s %s", result, arg_list[i]);

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will remove all duplicate strings
dnl from arg-list and assign the result to variable.  The given argument list
dnl is searched from the end to the beginning.  The first instance of a given
dnl argument is saved, and any subsequent duplicates are removed.  This macro
dnl is suitable for use with linker flags where flags that appear at the end
dnl usually satisfy dependencies of preceding flags.
dnl
dnl Usage:
dnl     VJ_STRIP_DUPLICATE_ARGS_REVERSE(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                stripped list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of arguments from which duplicates will be removd.
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_STRIP_DUPLICATE_ARGS_REVERSE],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0;
   for ( i = NF; i >= 1; i-- )
   {
      # Here, valid_list acts like a string-based dictionary of all the
      # command-line arguments passwed to awk(1).  These come in as $1, $2,
      # etc.  That is the reason for the $i expressions.
      # On the other hand, arg_list acts as an ordered list and will contain
      # only those arguments not previously added.
      if ( ! valid_list[$i] )
      {
         valid_list[$i] = 1;
         arg_list[j] = $i;
         j++;
      }
   }

   # Now, build result as a space-separated string of everything in arg_list.
   result = arg_list[j - 1]
   for ( i = j - 2; i >= 0; i-- )
      result = sprintf("%s %s", result, arg_list[i]);

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will "clean up" linker arguments.
dnl The basic process is simple: move all path arguments (-L, /libpath:) to
dnl the beginning and move all library arguments (-l, *.lib, -framework <X>,
dnl -pthread[s]) to the end.  Order is preserved, but duplicate arguments are
dnl removed.
dnl
dnl Usage:
dnl     VJ_CLEAN_LINKRE_ARGS(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                clean list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of linker arguments that will be "cleansed."
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_CLEAN_LINKER_ARGS],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0
   k = 0

   # Go through the list in forward order and extract the linker path
   # arguments.  We only want those that are not duplicated.
   for ( i = 1; i <= NF; i++ )
   {
      if ( $i ~ /^-L/ || $i ~ /[-\/]libpath:/ )
      {
         if ( ! valid_path_list[$i] )
         {
            valid_path_list[$i] = 1
            path_list[k] = $i
            k++
         }
      }
   }

   # Go through the list in reverse order and extract the library
   # arguments.  We only want those that are not duplicated.
   for ( i = NF; i >= 1; i-- )
   {
      if ( $i ~ /^-l/ || $i ~ /\.[Ll][Ii][Bb]$/ || $i ~ /^-pthread/ )
      {
         if ( ! valid_list[$i] )
         {
            valid_list[$i] = 1
            arg_list[j] = $i
            j++
         }
      }
      # Ignore linker path arguments.  Those are already stored in path_list
      # from above.
      else if ( $i !~ /^-L/ && $i !~ /[-\/]libpath:/ )
      {
         arg_list[j] = $i
         j++
      }
   }

   # Put the path arguments at the beginning of result.
   if ( k > 0 )
   {
      result = path_list[0]
      for ( i = 1; i < k; i++ )
         result = sprintf("%s %s", result, path_list[i])
   }

   # Now, build result as a space-separated string of everything in arg_list.
   if ( j > 0 )
   {
      result = sprintf("%s %s", result, arg_list[j - 1])
      for ( i = j - 2; i >= 0; i-- )
         result = sprintf("%s %s", result, arg_list[i])
   }

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_MTREE_GEN(file-prefix, path, platform [, ISA])
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_MTREE_LIB_GEN,
[
    mtree_path=`echo $2 | sed -e 's|/| |g'`

    # Ensure that the release directory exists since configure would not
    # create it otherwise.
    for dir in $mtree_path ; do
        full_path="${full_path}${dir}/"
        if test ! -d "$full_path" ; then
            mkdir "$full_path"
        fi
    done

    # Create the VJ.lib.dist file in release/mtree.  This is generated from
    # scratch since it cannot be generalized into a template using our
    # library directory scheme.  The only time this file needs to be generated
    # is when configure is run, so it is not set up to be one of the commands
    # that config.status can execute.
    _lib_file="$2/$1.lib.dist"
    _set_line="/set type=dir mode=$DIR_PERMS uname=$OWNER_NAME gname=$GROUP_NAME"

    # Ensure that the file exists and has zero length.  Then write out the
    # /set line which is going to be there no matter what.
    echo "creating ${_lib_file}"
    echo >${_lib_file}
    echo "${_set_line}" >>${_lib_file}
    echo "" >>${_lib_file}

    # On IRIX, it's easy to compile for different ABI/ISA combinations, so we
    # just make cases for all -n32/-64/-mips3/-mips4 settings.
    if test "x$3" = "xIRIX" ; then
        cat >>${_lib_file} <<END_IRIX_DIST
.
    lib32
        mips3
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
    ..
    lib64
        mips3
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
    ..
..
END_IRIX_DIST
    # If a value for $4 has been given (which is the case for some platforms),
    # create a subdirectory in lib for that value.
    elif test "x$4" != "x" ; then
        cat >>${_lib_file} <<END_ISA_DIST
.
    lib
        $4
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
    ..
..
END_ISA_DIST
    # In all other cases, just make a simple directory tree for debugging and
    # optimized libraries.
    else
        cat >>${_lib_file} <<END_DIST
.
    lib
        debug
        ..
        opt
        ..
        profiled
        ..
    ..
..
END_DIST
    fi
])

dnl ---------------------------------------------------------------------------
dnl Variables set in Makefiles found in the application trees.  All variables
dnl listed are for use only in the Makefile.in's found in that tree unless
dnl otherwise noted.  They are used in the following way:
dnl
dnl     * APP_EXTRA_CFLAGS: Extra flags for the compiler.
dnl
dnl Usage
dnl     VJ_APP_COMPILER(C-compiler, C-flags, C++-compiler, C++-flags, debug-flags, optimization-flags, base-dir, defs, includes, extra-flags)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_APP_COMPILER,
[
    if test "$OS_TYPE" = "UNIX" ; then
        APP_CC="$1"
        APP_CXX="$3"
    elif test "$OS_TYPE" = "Win32" ; then
        if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
        then
            APP_CC='cl'
            APP_CXX='cl'
        else
            APP_CC="$1"
            APP_CXX="$3"
        fi
    fi

    APP_CFLAGS="$2"
    APP_CXXFLAGS="$4"
    APP_DEBUG_CFLAGS="$5 -D_DEBUG"
    APP_OPTIM_CFLAGS="$6 -D_OPT"
    APP_DEFS="$8"
    APP_INCLUDES=ifelse([$7], , "$9", "-I\$($7)/include $9")
    APP_EXTRA_CFLAGS="$10"

    AC_SUBST(APP_CC)
    AC_SUBST(APP_CFLAGS)
    AC_SUBST(APP_CXX)
    AC_SUBST(APP_CXXFLAGS)
    AC_SUBST(APP_DEBUG_CFLAGS)
    AC_SUBST(APP_OPTIM_CFLAGS)
    AC_SUBST(APP_DEFS)
    AC_SUBST(APP_INCLUDES)
    AC_SUBST(APP_EXTRA_CFLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Variables set in Makefiles found in the application trees.  All variables
dnl listed are for use only in the Makefile.in's found in that tree unless
dnl otherwise noted.  They are used in the following way:
dnl
dnl     * APP_LINK: The command that will be used to link applications.
dnl     * APP_LINK_FLAGS: Flags passed to the linker.
dnl     * APP_DEBUG_LDFLAGS: Debugging linker flags.
dnl     * APP_OPTIM_LDFLAGS: Optimizing linker flags.
dnl     * PROJ_BASE_DIR_ENV: The name of the environment variable that deifnes
dnl       the base installation directory.
dnl     * BASIC_LIBS: The platform-indepenedent names of the basic libraries
dnl       that must be linked (for example, "GL X11 m").
dnl     * APP_EXTRA_LIBS: A basic set of extra libraries and linker flags
dnl       needed for compiling.  These are put after those listed in the
dnl       previous variables in the compile line.
dnl
dnl Usage
dnl     VJ_APP_LINKER(linker, link-flags, debug-flags, optimization-flags, base-dir-name, basic-libs, extra-libs)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_APP_LINKER,
[
    AC_REQUIRE([DPP_PROG_LINKER])

    if test "x$OS_TYPE" = "xUNIX" ; then
        APP_LINK="$1"
        APP_LINK_FLAGS="$2"

        for lib in $6 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS -l$lib"
        done

        APP_EXTRA_LIBS="$7"

        dnl IRIX without GNU linker.
        if test "x$PLATFORM" = "xIRIX" -a "x$GNU_LD" != "xyes" ; then
            APP_LINKALL_ON='-all'
            APP_LINKALL_OFF=''

            APP_BASIC_LIBS_BEGIN="-B static \$(LINKALL_ON) -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END='$(LINKALL_OFF) -B dynamic'
            APP_BASIC_LIBS_BEGIN_INST="-B dynamic -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($5)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-B dynamic'
            APP_EXTRA_LIBS_END=''
        dnl UNIX-based system (but not Darwin) using GNU linker.
        elif test "x$GNU_LD" = "xyes" -a "x$PLATFORM" != "xDarwin" ; then
            if test "x$PLATFORM" = "xIRIX" ; then
                APP_LINKALL_ON='-Wl,-all'
                APP_LINKALL_OFF=''
            else
                APP_LINKALL_ON='-Wl,--whole-archive'
                APP_LINKALL_OFF='-Wl,--no-whole-archive'
            fi

            APP_BASIC_LIBS_BEGIN="-Wl,-Bstatic \$(LINKALL_ON) -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END="\$(LINKALL_OFF) -Wl,-Bdynamic"
            APP_BASIC_LIBS_BEGIN_INST="-Wl,-Bdynamic -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($5)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-Wl,-Bdynamic'
            APP_EXTRA_LIBS_END=''
        else
            # For now, we are disabling static linking for the sample
            # applications when compiled in a developer's build tree.  This
            # is only the case on non-IRIX platforms and non-GNU linkers.
            APP_BASIC_LIBS_BEGIN="-L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"
        fi
    elif test "$OS_TYPE" = "Win32" ; then
        APP_LINKALL_ON=''
        APP_LINKALL_OFF=''

        if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
        then
            APP_LINK='link'
        else
            APP_lINK="$1"
        fi

        APP_LINK_FLAGS="$2"

        for lib in $6 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS ${lib}.lib"
        done

        APP_BASIC_LIBS_BEGIN="-libpath:\$($5)/lib/debug"
        APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"

        APP_EXTRA_LIBS="$7"
    fi

    APP_DEBUG_LFLAGS="$3"
    APP_OPTIM_LFLAGS="$4"

    AC_SUBST(APP_LINK)
    AC_SUBST(APP_DEBUG_LFLAGS)
    AC_SUBST(APP_OPTIM_LFLAGS)
    AC_SUBST(APP_EXTRA_LFLAGS)
    AC_SUBST(APP_LINK_FLAGS)
    AC_SUBST(APP_LINKALL_ON)
    AC_SUBST(APP_LINKALL_OFF)
    AC_SUBST(APP_BASIC_LIBS_BEGIN)
    AC_SUBST(APP_BASIC_LIBS_END)
    AC_SUBST(APP_BASIC_LIBS_BEGIN_INST)
    AC_SUBST(APP_BASIC_LIBS_END_INST)
    AC_SUBST(APP_BASIC_LIBS)
    AC_SUBST(APP_BASIC_EXT_LIBS)
    AC_SUBST(APP_EXTRA_LIBS_BEGIN)
    AC_SUBST(APP_EXTRA_LIBS_END)
    AC_SUBST(APP_EXTRA_LIBS_BEGIN_INST)
    AC_SUBST(APP_EXTRA_LIBS_END_INST)
    AC_SUBST(APP_EXTRA_LIBS)
])

dnl ---------------------------------------------------------------------------
dnl Set up variables for substituation in a makefile that will be used in
dnl conjunction with Doozer (not Doozer++).
dnl
dnl Usage:
dnl    VJ_APP_LINKER_DOOZER(linker, link-flags, debug-flags, optimization-flags, basic librar(y|ies), extra-libraries)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_APP_LINKER_DOOZER,
[
   AC_REQUIRE([DPP_PROG_LINKER])

   APP_BASIC_LIBS_BEGIN='$(STATIC_ON) $(LINKALL_ON)'
   APP_BASIC_LIBS=$5
   APP_BASIC_LIBS_END='$(LINKALL_OFF) $(STATIC_OFF)'

   dnl Values used for installation. 
   APP_BASIC_LIBS_BEGIN_INST=''
   APP_BASIC_LIBS_END_INST=''

   APP_EXTRA_LIBS=$6

   if test "x$OS_TYPE" = "xUNIX" ; then
      APP_LINK=$1
      APP_LINK_FLAGS=$2

      dnl IRIX without GNU linker.
      if test "x$PLATFORM" = "xIRIX" -a "x$GNU_LD" != "xyes" ; then
         APP_LINKALL_ON='-all'
         APP_LINKALL_OFF=''

         APP_LINK_STATIC_ON='-B static'
         APP_LINK_STATIC_OFF='-B dynamic'

         APP_EXTRA_LIBS_BEGIN='-B dynamic'
         APP_EXTRA_LIBS_END=''
      dnl UNIX-based system (but not Darwin) using GNU linker.
      elif test "x$GNU_LD" = "xyes" -a "x$PLATFORM" != "xDarwin" ; then
         if test "x$PLATFORM" = "xIRIX" ; then
            APP_LINKALL_ON='-Wl,-all'
            APP_LINKALL_OFF=''
         else
            APP_LINKALL_ON='-Wl,--whole-archive'
            APP_LINKALL_OFF='-Wl,--no-whole-archive'
         fi

         APP_LINK_STATIC_ON='-Wl,-Bstatic'
         APP_LINK_STATIC_OFF='-Wl,-Bdynamic'

         APP_EXTRA_LIBS_BEGIN='-Wl,-Bdynamic'
         APP_EXTRA_LIBS_END=''
      fi
   elif test "$OS_TYPE" = "Win32" ; then
      APP_LINKALL_ON=''
      APP_LINKALL_OFF=''

      if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
      then
         APP_LINK='link'
      else
         APP_lINK=$1
      fi

      APP_LINK_FLAGS=$2
   fi

   APP_DEBUG_LFLAGS=$3
   APP_OPTIM_LFLAGS=$4

   AC_SUBST(APP_LINK)
   AC_SUBST(APP_DEBUG_LFLAGS)
   AC_SUBST(APP_OPTIM_LFLAGS)
   AC_SUBST(APP_EXTRA_LFLAGS)
   AC_SUBST(APP_LINK_FLAGS)
   AC_SUBST(APP_LINKALL_ON)
   AC_SUBST(APP_LINKALL_OFF)
   AC_SUBST(APP_LINK_STATIC_ON)
   AC_SUBST(APP_LINK_STATIC_OFF)
   AC_SUBST(APP_BASIC_LIBS_BEGIN)
   AC_SUBST(APP_BASIC_LIBS_END)
   AC_SUBST(APP_BASIC_LIBS_BEGIN_INST)
   AC_SUBST(APP_BASIC_LIBS_END_INST)
   AC_SUBST(APP_BASIC_LIBS)
   AC_SUBST(APP_BASIC_EXT_LIBS)
   AC_SUBST(APP_EXTRA_LIBS_BEGIN)
   AC_SUBST(APP_EXTRA_LIBS_END)
   AC_SUBST(APP_EXTRA_LIBS_BEGIN_INST)
   AC_SUBST(APP_EXTRA_LIBS_END_INST)
   AC_SUBST(APP_EXTRA_LIBS)
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_VERSION_GROK(version-file)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_VERSION_GROK,
[
    version_file="$srcdir/$1"

    MAJOR_VERSION=0
    MINOR_VERSION=0
    MICRO_VERSION=0

    if test -r "$version_file" ; then
        ver_str=`head -1 "$version_file"`
        ver_num=`echo "$ver_str" | sed -e 's/^\(.*\) @.*@$/\1/'`
        ver_str_sep=`echo "$ver_num" | sed -e 's/\./ /g'`
        MAJOR_VERSION=`echo "$ver_str_sep" | sed -e 's/^\(..*\) ..* ..*$/\1/'`
        MINOR_VERSION=`echo "$ver_str_sep" | sed -e 's/^..* \(..*\) ..*$/\1/'`
        MICRO_VERSION=`echo "$ver_str_sep" | sed -e 's/^..* ..* \(..*\)-..*$/\1/'`
    fi

    AC_SUBST(MAJOR_VERSION)
    AC_SUBST(MINOR_VERSION)
    AC_SUBST(MICRO_VERSION)
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Determine which C and C++ compilers to use for compiling.  The
dnl capabilities of the C++ compiler are tested here as well to ensure that
dnl the proper language support is available.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PROG_LINKER      - Finds the tool used for linking object files.
dnl     DPP_LINKER_IS_GNU_LD - Test if the given ld(1) command is GNU ld.
dnl
dnl Variables defined:
dnl     LD                   - The full path to the linker (as determined by
dnl                            DPP_PROG_LINKER).
dnl     GNU_LD               - This will be set to "yes" if the linker is
dnl                            GNU ld (as determined by DPP_LINKER_IS_GNU_LD).
dnl                            It will be set to "no" otherwise.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Finds the platform-specific tool used for linking object files.  The
dnl checks performed by this macro are very simple, and it should only be
dnl used if you know for sure that both of the following are true: on Win32,
dnl the command LINK.EXE will be the linker, and on UNIX, the command ld will
dnl be the linker.  At a later time, the usage of this macro may change should
dnl better checks be developed.
dnl
dnl Usage:
dnl     DPP_PROG_LINKER([path [, action-if-found [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     path                - The path to use to find the linker.  This is
dnl                           optional.
dnl     action-if-found     - The action(s) to take if platform-specific
dnl                           linker is found.   This argument is optional.
dnl     action-if-not-found - The action(s) to take if platform-specific
dnl                           linker is not found.   This argument is
dnl                           optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_LINKER,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([AC_PROG_CC])

   dpp_linker_prog='ld'

   if test "x$GCC" = "xyes" ; then
      AC_MSG_CHECKING([for linker used by $CC])
      dpp_linker_prog=`($CC -print-prog-name=ld) 2>&5`
   fi

   case $dpp_linker_prog in
      dnl Accept absolute paths.
      [[\\/]* | [A-Za-z]:[\\/]*)]
         [re_direlt='/[^/][^/]*/\.\./']
         dnl Canonicalize the path of the linker.
         dpp_linker_prog=`echo $dpp_linker_prog | sed 's%\\\\%/%g'`
         while echo $dpp_linker_prog | grep "$re_direlt" > /dev/null 2>&1; do
            dpp_linker_prog=`echo $dpp_linker_prog | sed "s%$re_direlt%/%"`
         done
         ;;
      "")
         dnl If it fails, then pretend we aren't using GCC.
         dpp_linker_prog='ld'
         ;;
      *)
         GNU_LD='unknown'
         ;;
   esac

   test -z "$LD" && LD="$dpp_linker_prog"

   dnl Print the result of the above check for GCC's linker.
   if test "x$GCC" = "xyes" ; then
      AC_MSG_RESULT($dpp_linker_prog)
   fi

   case $dpp_linker_prog in
      [[\\/]* | [A-Za-z]:[\\/]*)]
         LINKER="$dpp_linker_prog"
         ;;
      *)
         if test "x$OS_TYPE" = "xWin32" ; then
            AC_PATH_PROG(LINKER, link, no, [$1])
         else
            AC_PATH_PROG(LINKER, $dpp_linker_prog, no, [$1])
         fi
         ;;
   esac

   dnl Failure.
   if test "x$LINKER" = "xno" ; then
      ifelse([$3], , :, [$3])
      true
   dnl Success.
   else
      ifelse([$2], , :, [$2])
      true
   fi

   DPP_LINKER_IS_GNU_LD
])

dnl ---------------------------------------------------------------------------
dnl Test if the given $LD command (linker-cmd) is GNU ld.
dnl
dnl Usage:
dnl     DPP_LINKER_IS_GNU_LD([action-if-gnu-ld [, action-if-not-gnu-ld]])
dnl
dnl Arguments:
dnl     action-if-gnu-ld     - The action(s) to take if the given command is
dnl                            a GNU ld executable.  This argument is
dnl                            optional.
dnl     action-if-not-gnu-ld - The action(s) to take if the given command is
dnl                            not a GNU ld executable.  This argument is
dnl                            optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_LINKER_IS_GNU_LD,
[
   AC_MSG_CHECKING([whether $LINKER is GNU ld])
   dpp_linker_ver_cmd='$LINKER --version | grep GNU 1>&AC_FD_CC'

   if (eval "$dpp_linker_ver_cmd") 2>&AC_FD_CC ; then
      AC_MSG_RESULT(yes)
      GNU_LD="yes"
      ifelse([$1], , :, [$1])
   else
      AC_MSG_RESULT(no)
      GNU_LD="no"
      ifelse([$2], , :, [$2])
   fi
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
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

dnl $Id$

AC_DEFUN(DPP_EXTRACT_PERL_VER,
[
   perl_ver_line=`$1 -v | grep -i 'this is perl'`
   changequote(<<, >>)
   PERL_VER_NUM=`echo $perl_ver_line | sed -e 's/^[^0-9]*\([1-9][^ ]*\) .*$/\1/'`
   split_ver_num=`echo $PERL_VER_NUM | sed -e 's/\./ /g' -e 's/_/ /g'`
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
AC_DEFUN(DPP_PERL_VER,
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

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Perform checks for various programs that are helpful for a complete build
dnl system.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_BASIC_PROGS      - Test for basic programs need by most, if not
dnl                            all, build systems.
dnl     DPP_MAKE_IS_GNU_MAKE - Test if the given make(1) command (make-cmd) is
dnl                            GNU make.
dnl     DPP_HAVE_GNU_MAKE    - Test if the host system has at least the given
dnl                            version of GNU make.
dnl
dnl Variables defined:
dnl     GMAKE                - The GNU make executable.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Test for basic programs need by most, if not all, build systems.
dnl
dnl Usage:
dnl     DPP_BASIC_PROGS
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_BASIC_PROGS,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   AC_PROG_MAKE_SET

   dnl While ranlib(1) is not part of the base IRIX installation, the VRAC
   dnl SGIs have a ranlib(1) installed that does very bad things to ar(1)
   dnl archives.  We also do not want to use the Cygwin ranlib(1) with the
   dnl Visual C++ LINK.EXE command.
   if test "x$dpp_platform" = "xIRIX" -o "x$1" = "xIRIX" -o	\
           "x$dpp_os_type" = "xWin32" -o "x$2" = "xWin32"
   then
      RANLIB=':'
   else
      AC_PROG_RANLIB
   fi

   AC_PROG_LN_S

   if test "x$USE_MAKEDEPEND" = "xY" ; then
      AC_CHECK_PROG([MAKEDEPEND], [makedepend], [makedepend])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the given make(1) command (make-cmd) is GNU make.
dnl
dnl Usage:
dnl     DPP_MAKE_IS_GNU_MAKE(make-cmd [, action-if-gnu-make [, action-if-not-gnu-make]])
dnl
dnl Arguments:
dnl     make-cmd               - The make(1) command to test.
dnl     action-if-gnu-make     - The action(s) to take if the given command is
dnl                              a GNU make executable.  This argument is
dnl                              optional.
dnl     action-if-not-gnu-make - The action(s) to take if the given command is
dnl                              not a GNU make executable.  This argument is
dnl                              optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_MAKE_IS_GNU_MAKE,
[
   AC_MSG_CHECKING([whether $1 is GNU make])
   dpp_gmake_ver_cmd='$1 --version | grep GNU 1>&AC_FD_CC'

   if (eval "$dpp_gmake_ver_cmd") 2>&AC_FD_CC ; then
      AC_MSG_RESULT(yes)
      ifelse([$2], , :, [$2])
   else
      AC_MSG_RESULT(no)
      ifelse([$3], , :, [$3])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the host system has at least the given version of GNU make.
dnl
dnl Usage:
dnl     DPP_HAVE_GNU_MAKE(version [, action-if-found [, action-if-not-found [, path]]])
dnl
dnl Arguments:
dnl     version             - The minimum required version of GNU make.
dnl     action-if-found     - The action(s) to take if a suitable version of
dnl                           GNU make is found.  This argument is optional.
dnl     action-if-not-found - The action(s) to take if a suitable version of
dnl                           GNU make is not found.  This argument is
dnl                           optional.
dnl     path                - Extra directories where GNU make might be found.
dnl                           This must be a colon-separated list of
dnl                           UNIX-style directories. This argument is
dnl                           optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_GNU_MAKE,
[
   dpp_GMAKE=''

   if test "x$GMAKE" = "x" ; then
      AC_CHECK_PROG([dpp_make_is_gmake], [make], [make], [no], $4)

      if test "x$dpp_make_is_gmake" != "xno" ; then
         DPP_MAKE_IS_GNU_MAKE([$dpp_make_is_gmake],
            [GMAKE="$dpp_make_is_gmake"],
            [ AC_CHECK_PROG([dpp_gmake_is_gmake], [gmake], [gmake], [no], $4)
              DPP_MAKE_IS_GNU_MAKE([$dpp_gmake_is_gmake],
                                   [GMAKE="$dpp_gmake_is_gmake"], $3) ])
      else
         AC_CHECK_PROG([dpp_gmake_is_gmake], [gmake], [gmake], [no], $4)

         if test "x$dpp_gmake_is_gmake" != "xno" ; then
            DPP_MAKE_IS_GNU_MAKE([$dpp_gmake_is_gmake],
                                 [GMAKE="$dpp_gmake_is_gmake"], $3)
         else
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   dnl The sed(1) expressions below will work with GNU Make 3.7x and 3.80.
   dnl The basic idea is to extract the "GNU Make x.yz" string and then pick
   dnl out the "x.yz" part.
   changequote(<<, >>)
   dpp_gmake_ver_exp1='s/^\(.*\),.*$/\1/'
   dpp_gmake_ver_exp2='s/^GNU Make[^0-9]*\(.*\)/\1/'
   dpp_gmake_ver=`$GMAKE --version | grep 'GNU Make' | sed -e "$dpp_gmake_ver_exp1" -e "$dpp_gmake_ver_exp2"`
   changequote([, ])

   DPP_VERSION_CHECK_MSG([GNU make], [$dpp_gmake_ver], $1,
                         [dpp_cv_gmake_version_okay], $2, $3)

   AC_SUBST(GMAKE)
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Define command-line options and variables useful in performing an
dnl installation.  Specifically, this code helps with setting up permissions
dnl and ownership of installed files.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_INSTALLER - Set up the installer configuration. 
dnl
dnl Command-line options added:
dnl     --with-install-owner - Set the owner of everything installed.
dnl     --with-install-group - Set the group of everything installed.
dnl     --with-file-perms    - Set the permissions for normal files.
dnl     --with-exec-perms    - Set the permissions for executable files.
dnl     --with-dir-perms     - Set the permissions for directories.
dnl
dnl Variables defined:
dnl     OWNER_NAME - The owner of everything installed.
dnl     GROUP_NAME - The group of everything installed.
dnl     FILE_PERMS - The UNIX-style octal permissions of normal files.
dnl     EXEC_PERMS - The UNIX-style octal permissions of executable files.
dnl     DIR_PERMS  - The UNIX-style octal permissions of directories.
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Set up the installer configuration.  This adds command line options for
dnl setting the installed group name and the permissions on various file types
dnl (which only works if the target file systems supports file permissions).
dnl The owner name is determined by the user running the macro.  Substitution
dnl is performed on the variables $OWNER_NAME, $GROUP_NAME, $FILE_PERMS,
dnl $EXEC_PERMS and $DIR_PERMS.
dnl
dnl Usage:
dnl     DPP_INSTALLER([group-name [, file-perms [, exec-perms [, dir-perms]]]])
dnl
dnl Arguments:
dnl     group-name - The name of the group owner of everything installed.
dnl     file-perms - The UNIX-style permissions of installed normal files.
dnl     exec-perms - The UNIX-style permissions of installed executable files
dnl                  and other files with the execute bit set.
dnl     dir-perms  - The UNIX-style permissions of directories created when
dnl                  installing.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_INSTALLER,
[
   dnl -----------------------------------------------------------------------
   dnl Name the user who will own installed files.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(install-owner,
               [  --with-install-owner=<USER NAME>
                          Installed file owner name],
               dpp_install_owner="$withval")

   dnl -----------------------------------------------------------------------
   dnl Name the group used for setting ownership on installed files.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(install-group,
               [  --with-install-group=<GROUP NAME>
                          Installed file group name       [default=$1]],
               dpp_install_group="$withval", dpp_install_group=$1)

   dnl -----------------------------------------------------------------------
   dnl Provide permissions for installed (normal) files.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(file-perms,
               [  --with-file-perms=<FILE PERMISSIONS>
                          Installed file permissions      [default=$2]],
               dpp_file_perms="$withval", dpp_file_perms=$2)

   dnl -----------------------------------------------------------------------
   dnl Provide permissions for installed executable files.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(exec-perms,
               [  --with-exec-perms=<EXEC PERMISSIONS>
                          Installed executable permisions [default=$3]],
               dpp_exec_perms="$withval", dpp_exec_perms=$3)

   dnl -----------------------------------------------------------------------
   dnl Provide permissions for directories in the installation tree.
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(dir-perms,
               [  --with-dir-perms=<DIRECTORY PERMISSIONS>
                          Installed directory permisions  [default=$4]],
               dpp_dir_perms="$withval", dpp_dir_perms=$4)

   FILE_PERMS="${dpp_file_perms-0644}"
   EXEC_PERMS="${dpp_exec_perms-0755}"
   DIR_PERMS="${dpp_dir_perms-0755}"

   dpp_save_PATH="$PATH"
   PATH="/usr/xpg4/bin:$PATH"

   dnl Set $OWNER_NAME here in case the check for the user-specified owner
   dnl failed.  This provides a simple fallback in that case.
   OWNER_NAME=`id -un`

   dnl Make sure that the named install owner is actually available and can
   dnl be set by this user.  This is done by creating a file (conftest.c) and
   dnl setting the owner name to be what is in ${_install_owner}.  Any output
   dnl from chown(1) is redirected to chown_out.  If chown_out has non-zero
   dnl length, chown(1) gave error output, and thus the name is not
   dnl avaialble.
   if test "x$dpp_install_owner" != "x" ; then
      AC_MSG_CHECKING([for user $dpp_install_owner])
      touch conftest.c
      chown $dpp_install_owner conftest.c >chown_out 2>&1
      if test -s chown_out ; then
         AC_MSG_RESULT([no])
      else
         AC_MSG_RESULT([yes])
         OWNER_NAME="$dpp_install_owner"
      fi

      rm -f conftest.c chown_out
   fi

   dnl Set $GROUP_NAME here in case the check for the user-specified group
   dnl failed.  This provides a simple fallback in that case.
   GROUP_NAME=`id -gn`

   dnl Make sure that the named install group is actually available and can
   dnl be set by this user.  This is done by creating a file (conftest.c) and
   dnl setting the group name to be what is in ${_install_group}.  Any output
   dnl from chgrp(1) is redirected to chgrp_out.  If chgrp_out has non-zero
   dnl length, chgrp(1) gave error output, and thus the group is not
   dnl avaialble.
   if test "x$dpp_install_group" != "x" ; then
      AC_MSG_CHECKING([for $dpp_install_group group])
      touch conftest.c
      chgrp $dpp_install_group conftest.c >chgrp_out 2>&1
      if test -s chgrp_out ; then
         AC_MSG_RESULT([no])
      else
         AC_MSG_RESULT([yes])
         GROUP_NAME="$dpp_install_group"
      fi

      rm -f conftest.c chgrp_out
   fi

   PATH="$dpp_save_PATH"

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables defined here.
   dnl -----------------------------------------------------------------------
   AC_SUBST(OWNER_NAME)
   AC_SUBST(GROUP_NAME)
   AC_SUBST(FILE_PERMS)
   AC_SUBST(EXEC_PERMS)
   AC_SUBST(DIR_PERMS)
])

dnl ---------------------------------------------------------------------------
dnl Find a tool compatible with BSD install(1) and set its value in the
dnl $INSTALL variable.  This may be the true BSD install(1) utility, a
dnl work-alike, the common Autoconf install-sh script, or the Doozer++
dnl bsd-install.pl script.  The use of bsd-install.pl is preferred over all
dnl others because it is fully compatible with BSD install(1) but has a few
dnl extended features.  To disable the use of bsd-install.pl, pass [no] as the
dnl macro argument.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_INSTALL,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   USE_DPP_BSD_INSTALL='no'

   dnl Allow the calling code to prevent the use of the Doozer++ BSD
   dnl install(1) work-alike.
   if test "x$1" != "xno" ; then
      dnl The Doozer++ BSD install(1) script requires Perl 5.004 or newer.
      if test "x$PERL" != "xno" -o "x$PERL" != "x" ; then
         if test $PERL_MAJOR_VER -ge 5 -a $PERL_MINOR_VER -ge 4 ; then
            INSTALL='\$(PERL) \$(DPP_SCRIPTDIR)/bsd-install.pl'
            USE_DPP_BSD_INSTALL='yes'
         fi
      fi
   fi

   if test "x$USE_DPP_BSD_INSTALL" = "xno" ; then
      dnl If we are on a Win32 system, use $ac_install_sh for the install(1)
      dnl program.  This prevents problems with paths if an install program is
      dnl found elsewhere on the system.
      if test "x$dpp_os_type" = "xWin32" ; then
         dnl XXX: This may not be a safe value to use since it is internal to
         dnl the generated configure script.
         INSTALL="$ac_install_sh"
      dnl Otherwise, use the safe AC_PROG_INSTALL macro.
      else
         AC_PROG_INSTALL
      fi
   fi
   
   AC_SUBST(USE_DPP_BSD_INSTALL)
])

dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
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
dnl ************* <auto-copyright.pl END do not edit this line> *************

dnl ---------------------------------------------------------------------------
dnl GADGETEER_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for Gadgeteer and then define the following variables:
dnl     GADGET_CXXFLAGS
dnl     GADGET_CXXFLAGS_MIN
dnl     GADGET_INCLUDES
dnl     GADGET_INCLUDES_MIN
dnl     GADGET_LIBS_CC
dnl     GADGET_LIBS_LD
dnl     GADGET_PROF_LIBS_CC
dnl     GADGET_PROF_LIBS_LD
dnl     GADGET_LIBS_STATIC_CC
dnl     GADGET_LIBS_STATIC_LD
dnl     GADGET_PROF_LIBS_STATIC_CC
dnl     GADGET_PROF_LIBS_STATIC_LD
dnl     GADGET_EXTRA_LIBS_CC
dnl     GADGET_EXTRA_LIBS_LD
dnl     GADGET_USE_X11
dnl ---------------------------------------------------------------------------
AC_DEFUN(GADGETEER_PATH,
[
    dnl Get the cflags and libraries from the gadgeteer-config script
    AC_ARG_WITH(gadget-prefix,
                [  --with-gadget-prefix=<PATH>
                          Prefix where Gadgeteer is
                          installed (optional)            [No default]],
                gadget_config_prefix="$withval", gadget_config_prefix="")
    AC_ARG_WITH(gadget-exec-prefix,
                [  --with-gadget-exec-prefix=<PATH>
                          Exec prefix where Gadgeteer is
                          installed (optional)            [No default]],
                gadget_config_exec_prefix="$withval",
                gadget_config_exec_prefix="")
dnl    AC_ARG_ENABLE(gadgettest,
dnl                  [  --disable-gadgettest    Do not try to compile and run a
dnl                          test Gadgeteer program], , enable_gadgettest=yes)

    if test "x$gadget_config_exec_prefix" != "x" ; then
        gadget_config_args="$gadget_config_args --exec-prefix=$gadget_config_exec_prefix"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$gadget_config_exec_prefix/bin/gadgeteer-config"
        fi
    fi

    if test "x$gadget_config_prefix" != "x" ; then
        gadget_config_args="$gadget_config_args --prefix=$gadget_config_prefix"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$gadget_config_prefix/bin/gadgeteer-config"
        fi
    fi

    if test "x$GADGET_BASE_DIR" != "x" ; then
        gadget_config_args="$gadget_config_args --prefix=$GADGET_BASE_DIR"

        if test x${GADGETEER_CONFIG+set} != xset ; then
            GADGETEER_CONFIG="$GADGET_BASE_DIR/bin/gadgeteer-config"
        fi
    fi

    AC_PATH_PROG(GADGETEER_CONFIG, gadgeteer-config, no)
    min_gadget_version=ifelse([$1], , 0.0.1, $1)

    dnl Do a sanity check to ensure that $GADGETEER_CONFIG actually works.
    if ! (eval $GADGETEER_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        GADGETEER_CONFIG='no'
    fi

    no_gadgeteer=''
    if test "x$GADGETEER_CONFIG" = "xno" ; then
        no_gadgeteer=yes
    else
        GADGET_CXXFLAGS=`$GADGETEER_CONFIG $gadget_config_args --cxxflags $ABI`
        GADGET_INCLUDES=`$GADGETEER_CONFIG $gadget_config_args --includes`
        GADGET_LIBS_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI`"
        GADGET_LIBS_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI`"
        GADGET_PROF_LIBS_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --profiled`"
        GADGET_PROF_LIBS_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --profiled`"
        GADGET_LIBS_STATIC_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static`"
        GADGET_LIBS_STATIC_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static`"
        GADGET_PROF_LIBS_STATIC_CC="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --profiled`"
        GADGET_PROF_LIBS_STATIC_LD="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --profiled`"
        GADGET_EXTRA_LIBS_CC=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI`
        GADGET_EXTRA_LIBS_LD=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --linker`
        GADGET_VERSION=`$GADGETEER_CONFIG --version`

        GADGET_CXXFLAGS_MIN=`$GADGETEER_CONFIG $gadget_config_args --cxxflags $ABI --min`
        GADGET_INCLUDES_MIN=`$GADGETEER_CONFIG $gadget_config_args --includes --min`
        GADGET_LIBS_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --min`"
        GADGET_LIBS_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --min`"
        GADGET_PROF_LIBS_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --min --profiled`"
        GADGET_PROF_LIBS_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --min --profiled`"
        GADGET_LIBS_STATIC_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --min`"
        GADGET_LIBS_STATIC_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --min`"
        GADGET_PROF_LIBS_STATIC_CC_MIN="`$GADGETEER_CONFIG $gadget_config_args --libs $ABI --static --profiled --min`"
        GADGET_PROF_LIBS_STATIC_LD_MIN="`$GADGETEER_CONFIG $gadget_config_args --linker --libs $ABI --static --profiled --min`"
        GADGET_EXTRA_LIBS_CC_MIN=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --min`
        GADGET_EXTRA_LIBS_LD_MIN=`$GADGETEER_CONFIG $gadget_config_args --extra-libs $ABI --min --linker`
        GADGET_USE_X11=`$GADGETEER_CONFIG $gadget_config_args --use-x11`

        AC_MSG_CHECKING([whether Gadgeteer version is >= $min_gadget_version])
        AC_MSG_RESULT([$GADGET_VERSION])
        DPP_VERSION_CHECK([$GADGET_VERSION], [$min_gadget_version], $2, $3)
    fi

    if test "x$no_gadgeteer" != x ; then
        if test "$GADGETEER_CONFIG" = "no" ; then
            echo "*** The gadgeteer-config script installed by Gadgeteer could not be found"
            echo "*** If Gadgeteer was installed in PREFIX, make sure PREFIX/bin is in"
            echo "*** your path, or set the GADGETEER_CONFIG environment variable to the"
            echo "*** full path to gadgeteer-config."
        fi
        GADGET_CXXFLAGS=""
        GADGET_INCLUDES=""
        GADGET_LIBS_CC=""
        GADGET_LIBS_LD=""
        GADGET_PROF_LIBS_CC=""
        GADGET_PROF_LIBS_LD=""
        GADGET_LIBS_STATIC_CC=""
        GADGET_LIBS_STATIC_LD=""
        GADGET_PROF_LIBS_STATIC_CC=""
        GADGET_PROF_LIBS_STATIC_LD=""
        GADGET_EXTRA_LIBS_CC=""
        GADGET_EXTRA_LIBS_LD=""
        GADGET_VERSION="-1"

        GADGET_CXXFLAGS_MIN=""
        GADGET_INCLUDES_MIN=""
        GADGET_LIBS_CC_MIN=""
        GADGET_LIBS_LD_MIN=""
        GADGET_PROF_LIBS_CC_MIN=""
        GADGET_PROF_LIBS_LD_MIN=""
        GADGET_LIBS_STATIC_CC_MIN=""
        GADGET_LIBS_STATIC_LD_MIN=""
        GADGET_PROF_LIBS_STATIC_CC_MIN=""
        GADGET_PROF_LIBS_STATIC_LD_MIN=""
        GADGET_EXTRA_LIBS_CC_MIN=""
        GADGET_EXTRA_LIBS_LD_MIN=""
        GADGET_USE_X11=''

        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(GADGET_CXXFLAGS)
    AC_SUBST(GADGET_INCLUDES)
    AC_SUBST(GADGET_LIBS_CC)
    AC_SUBST(GADGET_LIBS_LD)
    AC_SUBST(GADGET_PROF_LIBS_CC)
    AC_SUBST(GADGET_PROF_LIBS_LD)
    AC_SUBST(GADGET_LIBS_STATIC_CC)
    AC_SUBST(GADGET_LIBS_STATIC_LD)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_CC)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_LD)
    AC_SUBST(GADGET_EXTRA_LIBS_CC)
    AC_SUBST(GADGET_EXTRA_LIBS_LD)
    AC_SUBST(GADGET_VERSION)

    AC_SUBST(GADGET_CXXFLAGS_MIN)
    AC_SUBST(GADGET_INCLUDES_MIN)
    AC_SUBST(GADGET_LIBS_CC_MIN)
    AC_SUBST(GADGET_LIBS_LD_MIN)
    AC_SUBST(GADGET_PROF_LIBS_CC_MIN)
    AC_SUBST(GADGET_PROF_LIBS_LD_MIN)
    AC_SUBST(GADGET_LIBS_STATIC_CC_MIN)
    AC_SUBST(GADGET_LIBS_STATIC_LD_MIN)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_CC_MIN)
    AC_SUBST(GADGET_PROF_LIBS_STATIC_LD_MIN)
    AC_SUBST(GADGET_EXTRA_LIBS_CC_MIN)
    AC_SUBST(GADGET_EXTRA_LIBS_LD_MIN)

    AC_SUBST(GADGET_USE_X11)
])

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
AC_DEFUN(_JCCL_PATH_SETUP,
[
    dnl Get the cflags and libraries from the jccl-config script
    AC_ARG_WITH(jccl-prefix,
                [  --with-jccl-prefix=<PATH>
                          Prefix where JCCL is installed
                          (optional)                      [No default]],
                jccl_config_prefix="$withval", jccl_config_prefix="")
    AC_ARG_WITH(jccl-exec-prefix,
                [  --with-jccl-exec-prefix=<PATH>
                          Exec prefix where JCCL is
                          installed (optional)            [No default]],
                jccl_config_exec_prefix="$withval",
                jccl_config_exec_prefix="")
dnl    AC_ARG_ENABLE(jccltest,
dnl                  [  --disable-jccltest     Do not try to compile and run a
dnl                          test JCCL program], , enable_jccltest=yes)

    if test "x$jccl_config_exec_prefix" != "x" ; then
        jccl_config_args="$jccl_config_args --exec-prefix=$jccl_config_exec_prefix"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$jccl_config_exec_prefix/bin/jccl-config"
        fi
    fi

    if test "x$jccl_config_prefix" != "x" ; then
        jccl_config_args="$jccl_config_args --prefix=$jccl_config_prefix"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$jccl_config_prefix/bin/jccl-config"
        fi
    fi

    if test "x$JCCL_BASE_DIR" != "x" ; then
        jccl_config_args="$jccl_config_args --prefix=$JCCL_BASE_DIR"

        if test x${JCCL_CONFIG+set} != xset ; then
            JCCL_CONFIG="$JCCL_BASE_DIR/bin/jccl-config"
        fi
    fi

    AC_PATH_PROG(JCCL_CONFIG, jccl-config, no)

    dnl Do a sanity check to ensure that $JCCL_CONFIG actually works.
    if ! (eval $JCCL_CONFIG --cxxflags >/dev/null 2>&1) 2>&1 ; then
        JCCL_CONFIG='no'
        echo "*** The jccl-config script installed by JCCL could not be found"
        echo "*** If JCCL was installed in PREFIX, make sure PREFIX/bin is in"
        echo "*** your path, or set the JCCL_CONFIG environment variable to the"
        echo "*** full path to jccl-config."
    fi
])

dnl ---------------------------------------------------------------------------
dnl _JCCL_VERSION_CHECK(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl NOTE: This should not be called by external code.
dnl ---------------------------------------------------------------------------
AC_DEFUN(_JCCL_VERSION_CHECK,
[
   AC_REQUIRE([_JCCL_PATH_SETUP])

   if test "x$JCCL_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      JCCL_VERSION=`$JCCL_CONFIG --version`

      min_jccl_version=ifelse([$1], , 0.0.1, $1)
      AC_MSG_CHECKING([whether JCCL version is >= $min_jccl_version])
      AC_MSG_RESULT([$JCCL_VERSION])
      DPP_VERSION_CHECK([$JCCL_VERSION], [$min_jccl_version], $2, $3)
   fi
])

dnl ---------------------------------------------------------------------------
dnl JCCL_PATH_CXX([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for JCCL C++ API and then defines the following variables:
dnl     JCCL_CXXFLAGS
dnl     JCCL_CXXFLAGS_MIN
dnl     JCCL_INCLUDES
dnl     JCCL_INCLUDES_MIN
dnl     JCCL_LIBS_LD
dnl     JCCL_LIBS_LD_MIN
dnl     JCCL_LIBS_STATIC_LD
dnl     JCCL_LIBS_STATIC_LD_MIN
dnl     JCCL_LIBS_CC
dnl     JCCL_LIBS_CC_MIN
dnl     JCCL_LIBS_STATIC_CC
dnl     JCCL_LIBS_STATIC_CC_MIN
dnl     JCCL_PROF_LIBS_LD
dnl     JCCL_PROF_LIBS_LD_MIN
dnl     JCCL_PROF_LIBS_STATIC_LD
dnl     JCCL_PROF_LIBS_STATIC_LD_MIN
dnl     JCCL_PROF_LIBS_CC
dnl     JCCL_PROF_LIBS_CC_MIN
dnl     JCCL_PROF_LIBS_STATIC_CC
dnl     JCCL_PROF_LIBS_STATIC_CC_MIN
dnl ---------------------------------------------------------------------------
AC_DEFUN(JCCL_PATH_CXX,
[
   AC_REQUIRE([_JCCL_PATH_SETUP])

   JCCL_CXXFLAGS=""
   JCCL_CXXFLAGS_MIN=""
   JCCL_INCLUDES=""
   JCCL_INCLUDES_MIN=""
   JCCL_LIBS_LD=""
   JCCL_LIBS_LD_MIN=""
   JCCL_LIBS_STATIC_LD=""
   JCCL_LIBS_STATIC_LD_MIN=""
   JCCL_LIBS_CC=""
   JCCL_LIBS_CC_MIN=""
   JCCL_LIBS_STATIC_CC=""
   JCCL_LIBS_STATIC_CC_MIN=""
   JCCL_PROF_LIBS_LD=""
   JCCL_PROF_LIBS_LD_MIN=""
   JCCL_PROF_LIBS_STATIC_LD=""
   JCCL_PROF_LIBS_STATIC_LD_MIN=""
   JCCL_PROF_LIBS_CC=""
   JCCL_PROF_LIBS_CC_MIN=""
   JCCL_PROF_LIBS_STATIC_CC=""
   JCCL_PROF_LIBS_STATIC_CC_MIN=""

   if test "x$JCCL_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      AC_MSG_CHECKING([whether JCCL C++ API is available])
      has_cxx=`$JCCL_CONFIG --has-cxx`

      if test "x$has_cxx" = "xY" ; then
         AC_MSG_RESULT([yes])
         if test "x$JCCL_VERSION" = "x" ; then
            _JCCL_VERSION_CHECK($1, [jccl_version_okay='yes'],
                                 [jccl_version_okay='no'
                                  $3])
         fi

         if test "x$jccl_version_okay" = "xyes" ; then
            JCCL_CXXFLAGS=`$JCCL_CONFIG $jccl_config_args --cxxflags $ABI`
            JCCL_INCLUDES=`$JCCL_CONFIG $jccl_config_args --includes`
            JCCL_EXTRA_LIBS_CC=`$JCCL_CONFIG $jccl_config_args --extra-libs $ABI`
            JCCL_EXTRA_LIBS_LD=`$JCCL_CONFIG $jccl_config_args --extra-libs $ABI --linker`
            JCCL_LIBS_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI` $JCCL_EXTRA_LIBS_CC"
            JCCL_LIBS_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker` $JCCL_EXTRA_LIBS_LD"
            JCCL_PROF_LIBS_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI --profiled` $JCCL_EXTRA_LIBS_CC"
            JCCL_PROF_LIBS_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --profiled` $JCCL_EXTRA_LIBS_LD"
            JCCL_LIBS_STATIC_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI --static` $JCCL_EXTRA_LIBS_CC"
            JCCL_LIBS_STATIC_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --static` $JCCL_EXTRA_LIBS_LD"
            JCCL_PROF_LIBS_STATIC_CC="`$JCCL_CONFIG $jccl_config_args --libs $ABI --static --profiled` $JCCL_EXTRA_LIBS_CC"
            JCCL_PROF_LIBS_STATIC_LD="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --static --profiled` $JCCL_EXTRA_LIBS_LD"

            JCCL_CXXFLAGS_MIN=`$JCCL_CONFIG $jccl_config_args --cxxflags $ABI --min`
            JCCL_INCLUDES_MIN=`$JCCL_CONFIG $jccl_config_args --includes --min`
            JCCL_EXTRA_LIBS_CC_MIN=`$JCCL_CONFIG $jccl_config_args --extra-libs $ABI --min`
            JCCL_EXTRA_LIBS_LD_MIN=`$JCCL_CONFIG $jccl_config_args --extra-libs $ABI --linker --min`
            JCCL_LIBS_CC_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --min` $JCCL_EXTRA_LIBS_CC_MIN"
            JCCL_LIBS_LD_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --min` $JCCL_EXTRA_LIBS_LD_MIN"
            JCCL_LIBS_LD_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --min` $JCCL_EXTRA_LIBS_LD_MIN"
            JCCL_PROF_LIBS_CC_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --min --profiled` $JCCL_EXTRA_LIBS_CC_MIN"
            JCCL_PROF_LIBS_LD_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --min --profiled` $JCCL_EXTRA_LIBS_LD_MIN"
            JCCL_LIBS_STATIC_CC_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --static --min` $JCCL_EXTRA_LIBS_CC_MIN"
            JCCL_LIBS_STATIC_LD_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --static --min` $JCCL_EXTRA_LIBS_LD_MIN"
            JCCL_PROF_LIBS_STATIC_CC_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --static --min --profiled` $JCCL_EXTRA_LIBS_CC_MIN"
            JCCL_PROF_LIBS_STATIC_LD_MIN="`$JCCL_CONFIG $jccl_config_args --libs $ABI --linker --static --min --profiled` $JCCL_EXTRA_LIBS_LD_MIN"

            ifelse([$2], , :, [$2])
         fi
      else
         AC_MSG_RESULT([no])
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST([JCCL_CXXFLAGS])
   AC_SUBST([JCCL_LIBS_CC])
   AC_SUBST([JCCL_LIBS_LD])
   AC_SUBST([JCCL_PROF_LIBS_CC])
   AC_SUBST([JCCL_PROF_LIBS_LD])
   AC_SUBST([JCCL_LIBS_STATIC_LD])
   AC_SUBST([JCCL_LIBS_STATIC_CC])
   AC_SUBST([JCCL_PROF_LIBS_STATIC_LD])
   AC_SUBST([JCCL_PROF_LIBS_STATIC_CC])

   AC_SUBST([JCCL_CXXFLAGS_MIN])
   AC_SUBST([JCCL_INCLUDES_MIN])
   AC_SUBST([JCCL_LIBS_CC_MIN])
   AC_SUBST([JCCL_LIBS_LD_MIN])
   AC_SUBST([JCCL_PROF_LIBS_CC_MIN])
   AC_SUBST([JCCL_PROF_LIBS_LD_MIN])
   AC_SUBST([JCCL_LIBS_STATIC_CC_MIN])
   AC_SUBST([JCCL_LIBS_STATIC_LD_MIN])
   AC_SUBST([JCCL_PROF_LIBS_STATIC_CC_MIN])
   AC_SUBST([JCCL_PROF_LIBS_STATIC_LD_MIN])
])

dnl ---------------------------------------------------------------------------
dnl JCCL_PATH_JAVA([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for JCCL Java API and then defines the following variables:
dnl     JCCL_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(JCCL_PATH_JAVA,
[
   AC_REQUIRE([_JCCL_PATH_SETUP])

   JCCL_JARS=''

   if test "x$JCCL_CONFIG" = "xno" ; then
      ifelse([$3], , :, [$3])
   else
      AC_MSG_CHECKING([whether JCCL Java API is available])
      has_java=`$JCCL_CONFIG --has-java`

      if test "x$has_java" = "xY" ; then
         AC_MSG_RESULT([yes])
         if test "x$JCCL_VERSION" = "x" ; then
            _JCCL_VERSION_CHECK($1, [jccl_version_okay='yes'],
                                 [jccl_version_okay='no'
                                  $3])
         fi

         if test "x$jccl_version_okay" = "xyes" ; then
            JCCL_JARS="`$JCCL_CONFIG $jccl_config_args --jars`"

            ifelse([$2], , :, [$2])
         fi
      else
         AC_MSG_RESULT([no])
         ifelse([$3], , :, [$3])
      fi
   fi

   AC_SUBST([JCCL_JARS])
])

dnl ---------------------------------------------------------------------------
dnl JCCL_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Tests for JCCL C++ and Java APIs and then defines the following
dnl variables:
dnl     JCCL_CXXFLAGS
dnl     JCCL_CXXFLAGS_MIN
dnl     JCCL_LIBS_LD
dnl     JCCL_LIBS_LD_MIN
dnl     JCCL_LIBS_STATIC_LD
dnl     JCCL_LIBS_STATIC_LD_MIN
dnl     JCCL_LIBS_CC
dnl     JCCL_LIBS_CC_MIN
dnl     JCCL_LIBS_STATIC_CC
dnl     JCCL_LIBS_STATIC_CC_MIN
dnl     JCCL_PROF_LIBS_LD
dnl     JCCL_PROF_LIBS_LD_MIN
dnl     JCCL_PROF_LIBS_STATIC_LD
dnl     JCCL_PROF_LIBS_STATIC_LD_MIN
dnl     JCCL_PROF_LIBS_CC
dnl     JCCL_PROF_LIBS_CC_MIN
dnl     JCCL_PROF_LIBS_STATIC_CC
dnl     JCCL_PROF_LIBS_STATIC_CC_MIN
dnl     JCCL_JARS
dnl ---------------------------------------------------------------------------
AC_DEFUN(JCCL_PATH,
[
   JCCL_PATH_CXX($1, [jccl_have_cxx='yes'], $3, $4)

   if test "x$jccl_have_cxx" = "xyes" ; then
      JCCL_PATH_JAVA($1, $2, $3, $4)
   fi
])

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2003 by Iowa State University
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
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide command-line options and checks for Java utilities in the
dnl configuration process.  This does the substitution of the variables
dnl defined here (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PATH_JAVA - Find the path to the Java installation.
dnl     DPP_CHECK_JNI - Verify the installation of the libraries needed for
dnl                     compiling JNI code.
dnl
dnl     DPP_CHECK_JAVA_METHOD        - Determine if the named class contains
dnl                                    the non-static method.
dnl     DPP_CHECK_JAVA_STATIC_METHOD - Determine if the named class contains
dnl                                    the static method.
dnl
dnl Command-line options added:
dnl     --with-jdkhome - Define the root directory for the Java installation.
dnl
dnl Variables defined:
dnl     JDK_HOME - The JDK installation directory.
dnl     JAVA     - The path up to and including the java(1) command.
dnl     JAVAC    - The path up to and including the javac(1) Java compiler.
dnl     JAVADOC  - The path up to and including the javadoc(1) utility.
dnl     JAVAH    - The path up to and including the javah(1) utility.
dnl     JAVAP    - The path up to and including the javap(1) utility.
dnl     JAR      - The path up to and including the jar(1) utility.
dnl     RMIC     - The RMI stub and skeleton compiler.
dnl     JNI_INC  - The include paths necessary for JNI.
dnl     JNI_LIB  - The library which needs to be statically linked for JNI.
dnl     JCPS     - Java classpath separator character (: on UNIX, ; on Win32).
dnl ===========================================================================

dnl $Id$

dnl ---------------------------------------------------------------------------
dnl Find the path to the Java installation.  Substition is performed on the
dnl variables $JDK_HOME, $JAVAC and $JAR.  If a Java installation is not found,
dnl set $JDK_HOME to 'none' to indicate error.  The search for a Java
dnl installation is performed in the following order:
dnl
dnl     1. Search path given on the command line (--with-jdkhome)
dnl     2. Search in the path(s) given as arguments to this macro
dnl     3. Search in $JDK_HOME (if it is set)
dnl     4. Search in the user's path
dnl     5. Search in a preset list of common locations
dnl
dnl Usage:
dnl     DPP_PATH_JAVA([path [, action-if-found [, action-if-not-found ]]]) 
dnl
dnl Arguments:
dnl     path - A colon-separated list of directories giving possible
dnl            locations of the Java installation.  These must be directories
dnl            where javac can be found.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PATH_JAVA,
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_WIN32_SETUP])

   GIVEN_JDK_LIST=''

   dnl -----------------------------------------------------------------------
   dnl If an argument was given, assign it to $GIVEN_JDK_LIST and append a
   dnl ':' so that we can prepend it to the search path given to AC_PATH_PROG
   dnl below.
   dnl -----------------------------------------------------------------------
   if test "x$1" != "x" ; then
      GIVEN_JDK_LIST="$1:"
   fi

   dnl -----------------------------------------------------------------------
   dnl Define the root directory for the Java installation.
   dnl The default is /usr/java or what the user has defined in his or her
   dnl $JDK_HOME environment variable.  ${_JDK_HOME_ALT_LIST} must be a
   dnl colon-separated list of bin directories that could contain javac(1)
   dnl and the other JDK utilities.
   dnl -----------------------------------------------------------------------
   if test "x$JAVA_HOME" != "x" ; then
      _JDK_HOME_ALT_LIST="/usr/local/java/bin"
   else
      _JDK_HOME_ALT_LIST="${JAVA_HOME}/bin:/usr/local/java/bin"
   fi

   AC_ARG_WITH(jdkhome,
               [  --with-jdkhome=<PATH>   JDK installation directory      [default=\$JDK_HOME
                                                           then /usr/java]],
               jdkhome="$withval")

   if test "x$jdkhome" != "x" ; then
      cmdline_path="$jdkhome/bin:"
      JDK_HOME="$jdkhome"
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      JDK_HOME=`unixpath -p "$JDK_HOME"`
      JCPS=';'
   else
      JCPS=':'
   fi

   JAVA=''
   JAVAC=''
   JAVADOC=''
   JAVAH=''
   JAVAP=''
   JAR=''

   dnl -----------------------------------------------------------------------
   dnl Check for the JDK (specifically try to find javac(1) in $JDK_HOME or
   dnl in ${_JDK_HOME_ALT_LIST}).  The order we want here is that a path to
   dnl Java specified on the command line supercedes everything else.  This
   dnl is followed by arguments passed to this macro; then the $JDK_HOME
   dnl environment variable; then the user's path; and finally the simple
   dnl list of possible alternates.
   dnl -----------------------------------------------------------------------
   dpp_temp_javac_search_path=${cmdline_path}${GIVEN_JDK_LIST}${JDK_HOME}/bin:${PATH}:${_JDK_HOME_ALT_LIST}
   AC_PATH_PROG([JAVAC], [javac], [no], [${dpp_temp_javac_search_path}])

   dnl -----------------------------------------------------------------------
   dnl If $JAVAC has a value other than "no", extract the value of $JDK_HOME
   dnl from it.  Also use the path to $JAVAC as the basis for the path to
   dnl jar, javah, and rmic.
   dnl -----------------------------------------------------------------------
   if test "x$JAVAC" != "xno" ; then
      dpp_temp_JDK_HOME=`echo $JAVAC | sed -e 's|/bin/javac$||'`

      if test "x$JDK_HOME" != "x$dpp_temp_JDK_HOME" ; then
         if test ! -x "$JDK_HOME/bin/javac" ; then
            JDK_HOME="$dpp_temp_JDK_HOME"
         fi
      fi

      if test "x$OS_TYPE" = "xWin32" ; then
         JDK_HOME=`dospath -p "$JDK_HOME"`
         JAVAC=`dospath -p "$JAVAC"`
      fi

      JAVA=`echo $JAVAC | sed -e 's|javac$||'`java
      JAVADOC=`echo $JAVAC | sed -e 's|javac$||'`javadoc
      JAR=`echo $JAVAC | sed -e 's|javac$||'`jar
      RMIC=`echo $JAVAC | sed -e 's|javac$||'`rmic

      ifelse([$2], , :, [$2])
   else
      JDK_HOME='none'
      ifelse([$3], , :, [$3])
   fi

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables.
   dnl -----------------------------------------------------------------------
   AC_SUBST(JCPS)
   AC_SUBST(JDK_HOME)
   AC_SUBST(JAVA)
   AC_SUBST(JAVAC)
   AC_SUBST(JAVADOC)
   AC_SUBST(JAR)
   AC_SUBST(RMIC)
])

dnl ---------------------------------------------------------------------------
dnl Verify that the available JDK is at least the required version.
dnl
dnl Usage:
dnl     DPP_JDK_VERSION(jdk-home, version, [, action-if-found [, action-if-not-found ]])
dnl
dnl Arguments:
dnl     jdk-home            - JDK installation directory.
dnl     version             - The minimum required JDK version.
dnl     action-if-found     - The action to take if the JNI libraries are
dnl                           found.
dnl     action-if-not-found - The action to take if the JNI libraries are not
dnl                           found.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_JDK_VERSION,
[
   AC_REQUIRE([DPP_PATH_JAVA])

   if test -x "$1/bin/java" ; then
      dpp_jdk_ver_line=`$1/bin/java -version 2>&1 | grep 'java version'`

      changequote(<<, >>)
      dpp_jdk_ver=`echo $dpp_jdk_ver_line | sed -e 's/^.*"[^0-9]*\([0-9][.0-9]*\).*"$/\1/'`
      changequote([, ])

      DPP_VERSION_CHECK_MSG([JDK], [$dpp_jdk_ver], $2,
                            [dpp_cv_JDK_version_okay], $3, $4)
   fi
])

dnl ---------------------------------------------------------------------------
dnl Verify the installation of the libraries needed for compiling JNI code.
dnl
dnl Usage:
dnl     DPP_CHECK_JNI([jdk-home [, action-if-found [, action-if-not-found [, extra-libraries ]]]])
dnl
dnl Arguments:
dnl     jdk-home            - JDK installation directory.
dnl     action-if-found     - The action to take if the JNI libraries are
dnl                           found.
dnl     action-if-not-found - The action to take if the JNI libraries are not
dnl                           found.
dnl     extra-libraries     - Any extra libraries or library paths needed for
dnl                           the JNI code.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CHECK_JNI,
[
   AC_ARG_WITH(jni-inc, [  --with-jni-inc=<val>    JNI include path(s)],
               JNI_INC="$withval")

   AC_ARG_WITH(jni-lib, [  --with-jni-lib=<val>    JNI libraries and path(s)],
               JNI_LIB="$withval")

   AC_ARG_WITH(java-threads,
               [  --with-java-threads=<green|native>
                          Java threading type             [default=native]],
               dpp_java_threads="$withval", dpp_java_threads='native')

   dnl To help avoid typos, check the first letter of the value given for
   dnl $dpp_java_threads and assign the full string based on the result.
   case $dpp_java_threads in
      g*)
         dpp_java_threads='green'
         ;;
      n*)
         dpp_java_threads='native'
         ;;
      *)
         AC_MSG_ERROR([*** Unknown Java thread type $dpp_java_thread ***])
         ;;
   esac

   dnl Sanity check for Win32.
   if test "x$OS_TYPE" = "xWin32" -a "x$dpp_java_threads" = "xgreen" ; then
      AC_MSG_WARN([*** Win32 JVM only supports native threads ***])
      dpp_java_threads='native'
   fi

   dnl If no value was given for the first argument, set $JDK_HOME to the
   dnl default value of /usr/java unless it already has a value.  Otherwise,
   dnl set $JDK_HOME to the value given in the first argument.
   ifelse([$1], , : ${JDK_HOME=/usr/java}, JDK_HOME=$1)

   dnl -----------------------------------------------------------------------
   dnl SJJ added code
   dnl -----------------------------------------------------------------------
   : ${JNI_INC_PATH=$JDK_HOME/include}

   case "$PLATFORM" in
      AIX)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/aix}
         ;;
      Darwin)
         : ${JNI_INC=-F/System/Library/Frameworks/JavaVM.framework}
         : ${JNI_LIB=-framework JavaVM}
         ;;
      FreeBSD)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/freebsd}
         : ${JNI_LIB=-L$JDK_HOME/jre/lib/i386/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/i386/classic -L$JDK_HOME/jre/lib/i386 -L$JDK_HOME/jre/lib/i386/client -ljvm}
         ;;
      HP)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/hp-ux}
         : ${JNI_LIB=-L$JDK_HOME/lib/PA_RISC/${dpp_java_threads}_threads -L$JDK_HOME/lib/PA_RISC/classic -L$JDK_HOME/lib/PA_RISC -L$JDK_HOME/lib/PA_RISC/client -ljvm}
         ;;
      IRIX)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/irix}
         : ${JNI_LIB=-L$JDK_HOME/lib32/sgi/${dpp_java_threads}_threads -L$JDK_HOME/lib32/sgi/classic -L$JDK_HOME/lib32/sgi -L$JDK_HOME/lib32/sgi/client -ljvm}
         ;;
      Linux)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/linux}
         : ${JNI_LIB=-L$JDK_HOME/jre/lib/i386/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/i386/classic -L$JDK_HOME/jre/lib/i386 -L$JDK_HOME/jre/lib/i386/client -ljvm -lhpi}
         ;;
      alpha-osf)
         ;;
      Solaris)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/solaris}
         : ${JNI_LIB=-L$JDK_HOME/jre/lib/i386/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/i386 -ljvm}
         ;;
      Win*)
         : ${JNI_INC=-I`dospath -p "$JNI_INC_PATH"` -I`dospath -p "$JNI_INC_PATH"/win32`}
         : ${JNI_LIB=`dospath -p "$JDK_HOME/lib/jvm.lib"`}
         ;;
      UNKNOWN)
         ;;
   esac

   dnl Ensure that $JAVAC has a value before it is used.
   : ${JAVAC=$JDK_HOME/bin/javac}

   JAVAH=`echo $JAVAC | sed -e 's|javac$||'`javah
   JAVAP=`echo $JAVAC | sed -e 's|javac$||'`javap

   dnl Now that the paths are all defined, verify that the function
   dnl JNI_CreateJavaVM() is avaialble.  This is used to ensure that the
   dnl above settings are correct for the host platform.
   DPP_LANG_SAVE
   DPP_LANG_C

   dpp_save_CPPFLAGS="$CPPFLAGS"
   dpp_save_LIBS="$LIBS"
   CPPFLAGS="$CPPFLAGS $JNI_INC"

   AC_CHECK_LIB([jvm], [JNI_CreateJavaVM],
      [AC_CHECK_HEADER([jni.h], [dpp_jni_libs='yes'], [dpp_jni_libs='no'])],
      [dpp_jni_libs='no'], [$4 $JNI_LIB])

   CPPFLAGS="$dpp_save_CPPFLAGS"
   LIBS="$dpp_save_LIBS"

   DPP_LANG_RESTORE

   if test "x$dpp_jni_libs" = "xyes" ; then
      ifelse([$2], , :, [$2])
   else
      ifelse([$3], , :, [$3])
   fi

   AC_SUBST(JAVAH)
   AC_SUBST(JAVAP)
   AC_SUBST(JNI_INC)
   AC_SUBST(JNI_LIB)
])

dnl ---------------------------------------------------------------------------
dnl Determine if the named class contains the given static method.  The method
dnl can take arguments which must be declared using Java syntax.
dnl
dnl Usage:
dnl     DPP_CHECK_JAVA_STATIC_METHOD(class, method [, arg-decls, [, action-if-found [, action-if-not-found [, classpath]]]])
dnl
dnl Arguments:
dnl     class               - The class that should contain the static method.
dnl     method              - The method being looked up.  It must be passed
dnl                           using the full calling syntax except for the
dnl                           class name.  For example, pass 'staticMethod()'
dnl                           for the method staticMethod() that takes no
dnl                           arguments.
dnl     arg-decls           - Declarations for the method's arguments.  This
dnl                           can be any valid Java code.
dnl     action-if-found     - The action(s) to take if the class does contain
dnl                           the method.
dnl     action-if-not-found - The action(s) to take if the class does not
dnl                           contain the method.
dnl     classpath           - The classpath to use for looking up the class.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CHECK_JAVA_STATIC_METHOD,
[
   DPP_LANG_SAVE
   DPP_LANG_JAVA

   dnl The following is basically an expansion of the AC_CACHE_CHECK macro.
   dnl This was necessary because the variable name depends on the class and
   dnl the method being checked.

   dnl Construct the variable name.  All non-alphanumeric characters are
   dnl converted to _'s.  
   dnl XXX: This long sed command sucks, but I can't seem to get changequote()
   dnl to do what I need...
   dpp_cache_var=`echo dpp_cv_'$1.$2' | sed -e 's/\./_/g' -e 's/(/_/g' -e 's/)/_/g' -e 's/ /_/g'`

   AC_MSG_CHECKING([for $2 in $1])

   dnl Check cached value (based on AC_CACHE_VAL).
   if eval "test \"`echo '$''{'$dpp_cache_var'+set}'`\" = set" ; then
      echo $ac_n "(cached) $ac_c" 1>&AC_FD_MSG
   dnl The value was not cached.
   else
      dpp_class_name='ConfTest'

      dnl Create the .java file that will be compiled.
      cat > $dpp_class_name.java <<STATIC_EOF
public class $dpp_class_name
{
   public static void main (String args[])
   {
      $3;
      $1.$2;
   }
};
STATIC_EOF

      if test "x$6" != "x" ; then
         dpp_classpath="-classpath $6"
      fi 

      dpp_java_compile='${JAVAC-javac} $dpp_classpath $dpp_class_name.java 1>&AC_FD_CC'

      dnl Try compiling the Java test file.  If it succeeded, set the
      dnl variable to 'yes'.
      if (eval $dpp_java_compile) 2>&AC_FD_CC; then
         eval "$dpp_cache_var=yes"
      dnl If the test file failed to compile, set the variable to 'no'.
      else
         eval "$dpp_cache_var=no"
         echo "configure: failed program was:" >&AC_FD_CC
         cat $dpp_class_name.java >&AC_FD_CC
      fi

      rm -f ${dpp_class_name}.*
   fi

   dpp_test_result=`eval echo '$''{'$dpp_cache_var'}'`
   AC_MSG_RESULT([$dpp_test_result])

   DPP_LANG_RESTORE

   if test "x$dpp_test_result" = "xyes" ; then
      ifelse([$4], , :, [$4])
   else
      ifelse([$5], , :, [$5])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Determine if the named class contains the given non-static method.  The
dnl method can take arguments which must be declared using Java syntax.
dnl
dnl Usage:
dnl     DPP_CHECK_JAVA_METHOD(class, method [, arg-decls, [, action-if-found [, action-if-not-found [, classpath]]]])
dnl
dnl Arguments:
dnl     class               - The class that should contain the non-static
dnl                           method.
dnl     method              - The method being looked up.  It must be passed
dnl                           using the full calling syntax except for an
dnl                           object name.  For example, pass 'myMethod()'
dnl                           for the method myMethod() that takes no
dnl                           arguments.
dnl     arg-decls           - Declarations for the method's arguments.  This
dnl                           can be any valid Java code.
dnl     action-if-found     - The action(s) to take if the class does contain
dnl                           the method.
dnl     action-if-not-found - The action(s) to take if the class does not
dnl                           contain the method.
dnl     classpath           - The classpath to use for looking up the class.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CHECK_JAVA_METHOD,
[
   DPP_LANG_SAVE
   DPP_LANG_JAVA

   dnl The following is basically an expansion of the AC_CACHE_CHECK macro.
   dnl This was necessary because the variable name depends on the class and
   dnl the method being checked.

   dnl Construct the variable name.  All non-alphanumeric characters are
   dnl converted to _'s.
   dnl XXX: This long sed command sucks, but I can't seem to get changequote()
   dnl to do what I need...
   dpp_cache_var=`echo dpp_cv_'$1.$2' | sed -e 's/\./_/g' -e 's/(/_/g' -e 's/)/_/g' -e 's/ /_/g'`

   AC_MSG_CHECKING([for $2 in $1])

   dnl Check cached value (based on AC_CACHE_VAL).
   if eval "test \"`echo '$''{'$dpp_cache_var'+set}'`\" = set" ; then
      echo $ac_n "(cached) $ac_c" 1>&AC_FD_MSG
   dnl The value was not cached.
   else
      dpp_class_name='ConfTest'

       dnl Create the .java file that will be compiled.
       cat > $dpp_class_name.java <<NONSTATIC_EOF
public class $dpp_class_name
{
   public static void main (String args[])
   {
      $3;
      $1 obj = new $1();
      obj.$2;
   }
};
NONSTATIC_EOF

      if test "x$6" != "x" ; then
         dpp_classpath="-classpath $6"
      fi 

      dpp_java_compile='${JAVAC-javac} $dpp_classpath $dpp_class_name.java 1>&AC_FD_CC'

      dnl Try compiling the Java test file.  If it succeeded, set the
      dnl variable to 'yes'.
      if (eval $dpp_java_compile) 2>&AC_FD_CC; then
         eval "$dpp_cache_var=yes"
      dnl If the test file failed to compile, set the variable to 'no'.
      else
         eval "$dpp_cache_var=no"
         echo "configure: failed program was:" >&AC_FD_CC
         cat $dpp_class_name.java >&AC_FD_CC
      fi

      rm -f ${dpp_class_name}.*
   fi

   dpp_test_result=`eval echo '$''{'$dpp_cache_var'}'`
   AC_MSG_RESULT([$dpp_test_result])

   DPP_LANG_RESTORE

   if test "x$dpp_test_result" = "xyes" ; then
      ifelse([$4], , :, [$4])
   else
      ifelse([$5], , :, [$5])
   fi
])

