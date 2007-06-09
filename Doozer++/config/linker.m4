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
dnl File:          linker.m4,v
dnl Date modified: 2006/12/31 16:54:59
dnl Version:       1.11
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

dnl linker.m4,v 1.11 2006/12/31 16:54:59 patrickh Exp

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
AC_DEFUN([DPP_PROG_LINKER],
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
AC_DEFUN([DPP_LINKER_IS_GNU_LD],
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
