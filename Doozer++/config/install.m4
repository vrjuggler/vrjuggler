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
dnl File:          install.m4,v
dnl Date modified: 2005/01/08 22:44:39
dnl Version:       1.13
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

dnl install.m4,v 1.13 2005/01/08 22:44:39 patrickh Exp

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
AC_DEFUN([DPP_INSTALLER],
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
AC_DEFUN([DPP_PROG_INSTALL],
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
