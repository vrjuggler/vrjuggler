dnl ===========================================================================
dnl Perform tests related to the use of GCC as the compiler.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WITH_GCC      - Adds a command-line argument telling whether or
dnl                         not to force the use of GCC as the compiler suite.
dnl     DPP_GPLUSPLUS_VER - Verify that the version of g++ available is at
dnl                         least the given version.
dnl
dnl Command-line options added:
dnl     --with-gcc 
dnl
dnl Variables defined:
dnl     USE_GCC - This is set to "yes" if GCC is to be used.  If not, it will
dnl               be "no" or the empty string.  This is set by the
dnl               DPP_WITH_GCC macro.
dnl ===========================================================================

dnl gcc.m4,v 1.2 2001/01/07 16:02:02 patrick Exp

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

    AC_CACHE_CHECK(if $CXX version is >= $1,
        dpp_cv_CXX_is_egcs,
        [ dpp_gcc_ver=`$CXX -dumpversion` 
          DPP_VERSION_CHECK($dpp_gcc_ver, $1, dpp_cv_CXX_is_egcs='yes',
                            dpp_cv_CXX_is_egcs='no')
        ])

    if test "x$dpp_cv_CXX_is_egcs" = "xno" ; then
        $2
        true
    fi
])
