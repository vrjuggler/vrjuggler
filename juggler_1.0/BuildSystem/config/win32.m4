dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Define macros useful when working in a Win32/DOS-based environment.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WIN32_SETUP - Define path conversion (DOS to UNIX and UNIX to DOS)
dnl                       subroutines.
dnl 
dnl Subroutines defined:
dnl     unix2dos - Convert a UNIX-style path into a DOS-style path.
dnl     dos2unix - Convert a DOS-style path into a UNIX-style path.
dnl ===========================================================================

dnl win32.m4,v 1.8 2001/02/16 22:05:24 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Define path conversion (DOS to UNIX and UNIX to DOS) subroutines.
dnl
dnl Usage:
dnl     DPP_WIN32_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_WIN32_SETUP,
[
    AC_PATH_PROG(CYGPATH, cygpath, no)

    dnl UNIX path translator.  This converts a UNIX-style path to a DOS/Win32
    dnl path.  With this routine, we can safely use UNIX paths for all Win32
    dnl variables while the configure script runs.  Then, immediately before
    dnl Makefile substitution is done, we can translate all the paths to
    dnl DOS-style for use in the Makefiles.
    unix2dos ( ) {
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
    dos2unix ( ) {
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
            dpp_colon_rem_exp=['s/^\([[:alpha:]]\):/\/\/\1/']
            dpp_retval=`echo "$dpp_unix_root" | sed -e "$dpp_colon_rem_exp"`
        fi

        echo "$dpp_retval"
    }
])
