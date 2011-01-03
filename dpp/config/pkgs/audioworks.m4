dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

dnl ===========================================================================
dnl Find the target host's AudioWorks installation if one exists.
dnl NOTE: only works on UNIX type systems... (does AW exist on Win32?)
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_AUDIOWORKS - Determine if the target system has AudioWorks
dnl                           installed.
dnl
dnl Command-line options added:
dnl     --with-audioworks - Give the root directory of an AudioWorks
dnl                         installation.
dnl
dnl Variables defined:
dnl     AUDIOWORKS           - Does the system have AudioWorks?
dnl     AUDIOWORKS_ROOT      - The AudioWorks installation directory.
dnl     LIBAUDIOWORKS        - The list of libraries to link for AudioWorks
dnl                            appliations.
dnl     AUDIOWORKS__INCLUDES - Extra include path for the AudioWorks header
dnl                            directory.
dnl     AUDIOWORKS__LDFLAGS  - Extra linker flags for the AudioWorks library
dnl                            directory.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has AudioWorks installed.  This
dnl adds command-line arguments --with-audioworks.
dnl
dnl Usage:
dnl     DPP_HAVE_AUDIOWORKS(audioworks [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     audioworks-root     - The default directory where the AudioWorks
dnl                           installation is rooted.  This directory should
dnl                           contain an include/PSI directory with the PSI
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the PSI libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-audioworks command-line argument.
dnl     action-if-found     - The action to take if an AudioWorks installation
dnl                           is found.  This argument is optional.
dnl     action-if-not-found - The action to take if an AudioWorks installation
dnl                           is not found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_AUDIOWORKS],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl initialize returned data...
   AUDIOWORKS='no'
   LIBAUDIOWORKS=''
   AUDIOWORKS_INCLUDES=''
   AUDIOWORKS_LDFLAGS=''
   dpp_have_audioworks='no'

   dnl Define the root directory for the AudioWorks installation.
   AC_ARG_WITH(audioworks,
               [  --with-audioworks=<PATH> AudioWorks installation directory   [default=$1]],
               AUDIOWORKS_ROOT="$withval", AUDIOWORKS_ROOT=$1)

   if test "x$AUDIOWORKS_ROOT" != "xno" ; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CXXFLAGS="$CXXFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_INCLUDES="$INCLUDES"
      dpp_save_LDFLAGS="$LDFLAGS"

      dnl Add the user-specified AudioWorks installation directory to these
      dnl paths.  Ensure that /usr/lib(32|64) is not included multiple times if
      dnl if $AUDIOWORKS_ROOT is "/usr".
      if test "x$AUDIOWORKS_ROOT" != "x/usr" ; then
         LDFLAGS="-L$AUDIOWORKS_ROOT/lib$LIBBITSUF $LDFLAGS"
      fi

      CPPFLAGS="$CPPFLAGS -I$AUDIOWORKS_ROOT/include/PSI"
      CXXFLAGS="$CXXFLAGS $ABI_FLAGS"

      dnl This is necessary because AC_CHECK_LIB() adds -laudioworks to
      dnl $LIBS.  We want to do that ourselves later.
      dpp_save_LIBS="$LIBS"

      DPP_LANG_SAVE
      DPP_LANG_CPLUSPLUS

      AC_CHECK_LIB([aw], [awInitSys],
         [AC_CHECK_HEADER([aw.h], [dpp_have_audioworks='yes'],
            [dpp_have_audioworks='no'])],
         [dpp_have_audioworks='no'],
         [-lawhwi -lpsi -laudiofile -lm])

      DPP_LANG_RESTORE

      dnl This is necessary because AC_CHECK_LIB() adds -laudioworks to
      dnl $LIBS.  We want to do that ourselves later.
      LIBS="$dpp_save_LIBS"

      if test "x$dpp_have_audioworks" = "xyes" ; then
         ifelse([$2], , :, [$2])
      else
         ifelse([$3], , :, [$3])
      fi

      dnl If AudioWorks API files were found, define this extra stuff that may be
      dnl helpful in some Makefiles.
      if test "x$dpp_have_audioworks" = "xyes" ; then
         LIBAUDIOWORKS="-law -lawhwi -lpsi -laudiofile -lm"
         AUDIOWORKS_INCLUDES="-I$AUDIOWORKS_ROOT/include/PSI"
         AUDIOWORKS_LDFLAGS="-L$AUDIOWORKS_ROOT/lib$LIBBITSUF/PSI"
         AUDIOWORKS='yes'
      fi

      dnl Restore all the variables now that we are done testing.
      CXXFLAGS="$dpp_save_CXXFLAGS"
      CPPFLAGS="$dpp_save_CPPFLAGS"
      INCLUDES="$dpp_save_INCLUDES"
      LDFLAGS="$dpp_save_LDFLAGS"
   fi

   dnl export all of the output vars for use by makefiles and configure script
   AC_SUBST(AUDIOWORKS)
   AC_SUBST(AUDIOWORKS_ROOT)
   AC_SUBST(LIBAUDIOWORKS)
   AC_SUBST(AUDIOWORKS_INCLUDES)
   AC_SUBST(AUDIOWORKS_LDFLAGS)
])
