dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

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
dnl Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl Boston, MA 02110-1301, USA.

dnl ===========================================================================
dnl Determine the canonical system type and define a ton of variables for
dnl system-dependent settings.  The platforms supported in this file are AIX,
dnl Cygnus-Win32, FreeBSD, NetBSD, OpenBSD, DragonFly, HP-UX, IRIX, Linux, and
dnl Solaris.
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

dnl ---------------------------------------------------------------------------
dnl Based on the given detected host and CPU, set up the system-specific
dnl configuration.
dnl
dnl Usage:
dnl     DPP_SYSTEM_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_SYSTEM_SETUP],
[
   dnl We use AC_CANONICAL TARGET so that we can find out information about
   dnl the build, target, and host platforms rather than only the host.
   AC_REQUIRE([AC_CANONICAL_TARGET])

   dnl Operating system release information.  This may contain anything and
   dnl should be treated as a string.
   OS_REL_STR=`uname -r`

   dnl Operating system version number only (any miscellaneous text is
   dnl stripped).  This will only contain a number and can be treated as a
   dnl float-point value.
   changequote(<<, >>)
   OS_REL_NUM=`uname -r | sed -e 's/^[^0-9]*\([0-9][0-9]*\.[0-9][0-9]*\)[ .a-zA-Z_0-9-]*$/\1/'`
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
            if test $OS_REL_NUM_MAJOR -ge 10 ; then
               DPP_ABI_CFG('Mach-O', 'x86_64')
            else
               DPP_ABI_CFG('Mach-O', $target_cpu)
            fi
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
      dnl A machine running DragonFly.
      dragonfly*)
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

         PLATFORM='DragonFly'
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
            if test -r '/etc/lsb-release' ; then
               dpp_using_debian=1
            else
               dpp_using_debian=0
            fi

            case $target_cpu in
               x86_64)
                  dpp_libbitsuf='64'
                  if test $dpp_using_debian -eq 1 ; then
                     dpp_libbitsuf=''
                  fi
                  ;;
               *)
                  dpp_libbitsuf=''
                  ;;

            esac

            DPP_ABI_CFG('ELF', "$target_cpu", "$dpp_libbitsuf")
         fi

         case $target_cpu in
            *86)
               ABI_LIST='ELF_i386'
               ;;
            x86_64)
               ABI_LIST='ELF_x86_64'
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
