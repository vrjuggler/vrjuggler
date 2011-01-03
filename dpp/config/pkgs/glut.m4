dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

dnl ===========================================================================
dnl Find the target host's GLUT installation if one exists.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_GLUT - Determine if the target system has GLUT.
dnl
dnl Command-line options added:
dnl     --with-glut   - Give the root directory of the GLUT installation.
dnl
dnl Variables defined:
dnl     GLUT_ROOT     - The GLUT installation directory.
dnl     LIBGLUT       - The list of libraries to link for GLUT appliations.
dnl     GLUT_INCLUDES - Extra include path for the GLUT header directory.
dnl     GLUT_LDFLAGS  - Extra linker flags for the GLUT library directory.
dnl     GLUT_LIBDIR   - The directory containing the GLUT library. On Mac OS X,
dnl                     this is the directory containing the GLUT framework
dnl                     directory.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Determine if the target system has GLUT installed.  This adds the
dnl command-line argument --with-glut.
dnl
dnl Usage:
dnl     DPP_HAVE_GLUT(glut-root [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     glut-root           - The default directory where the GLUT
dnl                           installation is rooted.  This directory should
dnl                           contain an include/GL directory with the GLUT
dnl                           headers and a lib (with appropriate bit suffix)
dnl                           directory with the GLUT libraries.  The value
dnl                           given is used as the default value of the
dnl                           --with-glut command-line argument.
dnl     action-if-found     - The action to take if a GLUT installation is
dnl                           found.  This argument is optional.
dnl     action-if-not-found - The action to take if a GLUT installation is not
dnl                           found.  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_HAVE_GLUT],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_HAVE_OPENGL])

   dnl Define the root directory for the GLUT installation.
   AC_ARG_WITH(glut,
               [  --with-glut=<PATH>  GLUT installation directory     [default=$1]],
               GLUT_ROOT="$withval", GLUT_ROOT=$1)

   dpp_have_glut='no'
   LIBGLUT=''

   if test "x$GLUT_ROOT" != "xno" ; then
      dnl Save these values in case they need to be restored later.
      dpp_save_CFLAGS="$CFLAGS"
      dpp_save_CPPFLAGS="$CPPFLAGS"
      dpp_save_LDFLAGS="$LDFLAGS"

      dnl Add the user-specified GLUT installation directory to these
      dnl paths.  Ensure that /usr/include and /usr/lib are not included
      dnl multiple times if $GLUT_ROOT is "/usr".
      if test "x$GLUT_ROOT" != "x/usr" ; then
         CPPFLAGS="$CPPFLAGS -I$GLUT_ROOT/include"
         LDFLAGS="-L$GLUT_ROOT/lib$LIBBITSUF $LDFLAGS"
      fi

      CFLAGS="$CFLAGS $ABI_FLAGS"

      if test "x$dpp_os_type" = "xWin32" ; then
         AC_MSG_WARN([Assuming that GLUT is available in a standard location])
         dpp_have_glut='yes'
         ifelse([$2], , :, [$2])
      elif test "x$PLATFORM" = "xDarwin" ; then
         DPP_LANG_SAVE
         DPP_LANG_C

         CPPFLAGS="-F$GLUT_ROOT $CPPFLAGS"
         LDFLAGS="-F$GLUT_ROOT $LDFLAGS"
         LIBS="$LIBS -framework GLUT"

         AC_CACHE_CHECK([for glutMainLoop in GLUT framework],
                        [dpp_cv_glutMainLoop_glut_lib],
                        [AC_TRY_LINK([#include <GLUT/glut.h>],
                                     [glutMainLoop();],
                                     [dpp_cv_glutMainLoop_lib='yes'],
                                     [dpp_cv_glutMainLoop_lib='no'])])

         DPP_LANG_RESTORE

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         dnl Success.
         if test "x$dpp_cv_glutMainLoop_lib" = "xyes" ; then
            dpp_have_glut='yes'
            GLUT_LIBDIR="$GLUT_ROOT"
            ifelse([$2], , :, [$2])
         dnl Failure.
         else
            dpp_have_glut='no'
            ifelse([$3], , :, [$3])
         fi
      else
         dpp_save_LIBS="$LIBS"
         LIBS="$LIBS -lglut $LIBOPENGL -lm"

         if test "lib$LIBBITSUF" != "lib" ; then
            libdirs="lib$LIBBITSUF lib"
         else
            libdirs="lib"
         fi

         DPP_LANG_SAVE
         DPP_LANG_C

         for l in $libdirs ; do
            cur_glut_libdir="$GLUT_ROOT/$l"
            LDFLAGS="-L$cur_glut_libdir $dpp_save_LDFLAGS"

            AC_MSG_CHECKING([for glutMainLoop in -lglut in $cur_glut_libdir])
            AC_TRY_LINK([#include <GL/glut.h>], [glutMainLoop();],
                        [dpp_have_glut='yes'], [dpp_have_glut='no'])
            AC_MSG_RESULT([$dpp_have_glut])

            LIBS="$dpp_save_LIBS"

            if test "x$dpp_have_glut" = "xyes" ; then
               GLUT_LIBDIR="$cur_glut_libdir"
               break
            fi
         done

         DPP_LANG_RESTORE

         dnl Restore all the variables now that we are done testing.
         CFLAGS="$dpp_save_CFLAGS"
         CPPFLAGS="$dpp_save_CPPFLAGS"
         LDFLAGS="$dpp_save_LDFLAGS"
         LIBS="$dpp_save_LIBS"

         if test "x$dpp_cv_glutMainLoop_available" = "xyes" ; then
            dpp_have_glut='yes'
            ifelse([$2], , :, [$2])
         else
            ifelse([$3], , :, [$3])
         fi
      fi

      dnl If GLUT API files were found, define this extra stuff that may be
      dnl helpful in some Makefiles.
      if test "x$dpp_have_glut" = "xyes" ; then
         if test "x$OS_TYPE" = "xUNIX" ;  then
            LIBGLUT="-lglut"
         else
            LIBGLUT='glut.lib'
         fi

         if test "x$GLUT_ROOT" != "x/usr" ; then
            GLUT_INCLUDES="-I$GLUT_ROOT/include"
            GLUT_LDFLAGS="-L$GLUT_ROOT/lib\$(LIBBITSUF)"
         fi
      fi
   fi

   AC_SUBST(GLUT_ROOT)
   AC_SUBST(GLUT_INCLUDES)
   AC_SUBST(GLUT_LDFLAGS)
   AC_SUBST(GLUT_LIBDIR)
])
