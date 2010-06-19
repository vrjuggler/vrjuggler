dnl Doozer++ is (C) Copyright 2000-2009 by Iowa State University
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
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.

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

dnl ---------------------------------------------------------------------------
dnl Test for basic programs need by most, if not all, build systems.
dnl
dnl Usage:
dnl     DPP_BASIC_PROGS
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_BASIC_PROGS],
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
AC_DEFUN([DPP_MAKE_IS_GNU_MAKE],
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
dnl     DPP_HAVE_GNU_MAKE(required-version [, action-if-found [, action-if-not-found [, path]]])
dnl
dnl Arguments:
dnl     required-version    - The minimum required version of GNU make.
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
AC_DEFUN([DPP_HAVE_GNU_MAKE],
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

   dnl The sed(1) expressions below will work with GNU Make 3.7x and 3.8x.
   dnl The basic idea is to extract the "GNU Make x.yz" string and then pick
   dnl out the "x.yz" part.
   dnl dpp_gmake_ver_exp2 handles cases such as "3.82alpha12" by taking out
   dnl the "alpha12" part.
   changequote(<<, >>)
   dpp_gmake_ver_exp1='s/^\(.*\),.*$/\1/'
   dpp_gmake_ver_exp2='s/^\(.*[0-9]\)[a-bA-B].*$/\1/'
   dpp_gmake_ver_exp3='s/^GNU Make[^0-9]*\(.*\)/\1/'
   dpp_gmake_ver=`$GMAKE --version | grep 'GNU Make' | sed -e "$dpp_gmake_ver_exp1" -e "$dpp_gmake_ver_exp2" -e "$dpp_gmake_ver_exp3"`
   changequote([, ])

   DPP_VERSION_CHECK_MSG_NO_CACHE([GNU make], [$dpp_gmake_ver], [$1], [$2],
                                  [$3])

   AC_SUBST(GMAKE)
])
