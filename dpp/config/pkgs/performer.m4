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
dnl Find the target host's IRIS/OpenGL Performer installation if one exists.
dnl This supports Performer 2.2 through 3.0.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_ERFORMER - Determine if the target system has IRIS/OpenGL
dnl                         Performer installed.
dnl
dnl Command-line options added:
dnl     --with-performer - Give the root directory of the Perfromer
dnl                        installation.
dnl     --with-pfver     - Give the Performer version installed on the target
dnl                        system.
dnl
dnl Variables defined:
dnl     PFROOT                - The Performer installation directory.
dnl     PF_VER                - The Performer version being used.
dnl     LIBERFORMER           - The list of libraries to link for Performer
dnl                             appliations (safe for use with the msvccc
dnl                             shell script).
dnl     PF_INCLUDES           - Extra include path for the Performer header
dnl                             directory.
dnl     PF_LDFLAGS            - Extra linker flags for the Performer library
dnl                             directory.
dnl     LIBPERFORMER_MSVCCC   - The list of libraries to link for Performer
dnl                             applications using the msvccc shell script.
dnl     PF_LDFLAGS_MSVCCC     - Extra linker flags for the Performer library
dnl                             directory for use with the msvccc shell
dnl                             script.
dnl     LIBPERFORMER_LINK_EXE - The list of libraries to link for Performer
dnl                             applications using Microsoft Visual C++
dnl                             LINK.EXE.
dnl     PF_LDFLAGS_LINK_EXE   - Extra linker flags for the Performer library
dnl                             directory for use with Microsoft Visual C++j
dnl                             LINK.EXE.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has IRIS/OpenGL Performer installed.  This
dnl adds command-line arguments --with-performer and --with-pfver.
dnl
dnl Usage:
dnl     DPP_HAVE_PERFORMER(pfroot [, version, [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     pfroot              - The default directory where the Performer
dnl                           installation is rooted.  This directory should
dnl                           contain an include/Performer directory with the
dnl                           Performer headers and a lib (with appropriate
dnl                           bit suffix) directory with the Performer
dnl                           libraries.  The value given is used as the
dnl                           default value of the --with-performer
dnl                           command-line argument.
dnl     version             - Specify which version of Performer will be used.
dnl                           The value given is used as the default value of
dnl                           the --with-pfver command-line argument.  This
dnl                           argument is optional and if not specified, it
dnl                           assumes version 2.4.
dnl     action-if-found     - The action to take if Performer is found.  This
dnl                           argument is optional.
dnl     action-if-not-found - The action to take if Performer is not found.
dnl                           This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_PERFORMER],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl Define the root directory for the Performer installation.
   AC_ARG_WITH(performer,
               [  --with-performer=<PATH> Performer installation
                          directory                       [default=$1]],
               PFROOT="$withval", PFROOT=$1)

   dnl Define the Performer version installed..
   AC_ARG_WITH(pfver,
               [  --with-pfver=<VER>      Performer version to use        [default=$2]],
               PF_VER="$withval", PF_VER=$2)

   dpp_have_performer='no'

   if test "x$PFROOT" != "xno" ; then
      dpp_save_CFLAGS="$CFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"
      dpp_save_LIBS="$LIBS"

      dnl Ensure that /usr/include and /usr/lib are not included multiple times
      dnl if $PFROOT is "/usr".
      if test "x$PFROOT" != "x/usr" ; then
         CPPFLAGS="-I$PFROOT/include $CPPFLAGS"
         LDFLAGS="-L$PFROOT/lib$LIBBITSUF $LDFLAGS"

         if test "x$OS_TYPE" = "xWin32" ; then
            CPPFLAGS="$CPPFLAGS -I$PFROOT/include/Performer"
         fi
      fi

      CFLAGS="$CFLAGS $ABI_FLABS"

      dnl Default to OpenGL Performer 2.4 if nothing was given for the default
      dnl argument.
      if test "x$PF_VER" = "x" ; then
         PF_VER='2.4'
      fi

      dnl We are using Performer 2.[23] as defined by the second argument or by
      dnl the --with-pfver command-line argument.
      if test "x$2" = "x2.2" -o "x$2" = "x2.3" -o \
              "x$PF_VER" = "x2.2" -o "x$PF_VER" = "x2.3"
      then
         pf_ext='_ogl'
      dnl We are using Performer 2.4 or newer.
      else
         pf_ext=''
      fi

      dnl On Linux, we have to link all the Performer libraries AND libdl to
      dnl get the check for pfInit() in libpf to compile.
      if test "x$dpp_platform" = "xLinux" ; then
         LIBS="$LIBS -lpf$pf_ext -lpfdu$pf_ext -lpfui -lpfutil$pf_ext -ldl"
      fi

      dnl Argh!
      if test "x$OS_TYPE" = "xWin32" ; then
         PF_LIB_NAME_PREFIX='lib'
      fi

      dpp_pf_lib="${PF_LIB_NAME_PREFIX}pf"
      dpp_pfdu_lib="${PF_LIB_NAME_PREFIX}pfdu"
      dpp_pfui_lib="${PF_LIB_NAME_PREFIX}pfui"
      dpp_pfutil_lib="${PF_LIB_NAME_PREFIX}pfutil"

      dnl If the Performer library was found, add the API object files to the
      dnl files to be compiled and enable the Performer API compile-time
      dnl option.
      AC_CHECK_LIB([$dpp_pf_lib], [pfInit],
         [AC_CHECK_HEADER([Performer/pf.h], [dpp_have_performer='yes'], $4)],
         $4)

      if test "x$dpp_have_performer" = "xyes" ; then
         ifelse([$3], , :, [$3])
      fi

      LIBS="$dpp_save_LIBS"

      dnl Restore $LIBS to its original value (it was modified before the
      dnl previous check block).
      if test "x$dpp_have_performer" = "xyes" ; then
         if test "x$OS_TYPE" = "xWin32" ; then
            LIBPERFORMER="-llibpf -llibpfdu-util -llibpfui"
            LIBPERFORMER_MSVCCC="$LIBPERFORMER"
            LIBPERFORMER_LINK_EXE="libpf.lib libpfdu-util.lib libpfui.lib"
         else
            LIBPERFORMER="-lpf$pf_ext -lpfdu$pf_ext -lpfui -lpfutil$pf_ext"
         fi

         case $dpp_platform in
            dnl Under IRIX users will need -limage to load .flt files
            dnl referencing .sgi images.
            IRIX)
               LIBPERFORMER="$LIBPERFORMER -limage"
               ;;
            dnl Append -ldl to $LIBPERFORMER on Linux.
            Linux)
               LIBPERFORMER="$LIBPERFORMER -ldl"
               ;;
         esac

         if test "x$PFROOT" != "x/usr" ; then
            PF_INCLUDES="-I$PFROOT/include"
            PF_LDFLAGS="-L$PFROOT/lib\$(LIBBITSUF)"

            if test "x$OS_TYPE" = "xWin32" ; then
               PF_INCLUDES="-I\"$PFROOT/include\" -I\"$PFROOT/include/Performer\""
               PF_LDFLAGS_MSVCCC="$PF_LDFLAGS"
               PF_LDFLAGS_LINK_EXE="/libpath:\"$PFROOT/lib\""
            fi
         fi
      fi

      dnl Restore all the variables now that we are done testing.
      CFLAGS="$dpp_save_CFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      LDFLAGS="$dpp_save_LDFLAGS"
   fi

   AC_SUBST(PFROOT)
])
