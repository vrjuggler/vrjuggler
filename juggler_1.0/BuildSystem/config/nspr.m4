dnl ===========================================================================
dnl Provide command-line functions and checks for including the NSPR library
dnl in the configuration process.  This does the substitution of the variable
dnl defined here (described below).
dnl
dnl If the NSPR was compiled to use POSIX threads, the macros
dnl DPP_CC_PTHREAD_ARG() and DPP_GET_PTHREAD_LIB() must have been executed
dnl before any macro here is called.  See pthreads.m4 for more information.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_HAVE_NSPR - Determine if the NSPR is on the system.
dnl     DPP_NSPR_VER  - Make sure that the installed NSPR version is at least
dnl                     the given version number.
dnl
dnl Command-line options added:
dnl     --with-nspr     - Define the path to the NSPR installation.
dnl     --with-nsprver  - Define the NSPR version number.
dnl
dnl Variables defined:
dnl     NSPR_ROOT       - The root of the NSPR installation.
dnl     NSPR_LIB_STATIC - Full path to the static NSPR base library.
dnl     PLC_LIB_STATIC  - Full path to the static NSPR PLC library.
dnl     PLDS_LIB_STATIC - Full path to the static NSPR PLDS library.
dnl ===========================================================================

dnl nspr.m4,v 1.1.1.1 2000/12/08 06:16:37 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Verify that the NSPR library and headers are where they are expected to
dnl be.
dnl
dnl Usage:
dnl     DPP_HAVE_NSPR(version [, pthreads-enabled [, action-if-not-found]])
dnl
dnl Arguments:
dnl     version             - Minimum required NSPR version.
dnl     pthreads-enabled    - Set to "yes" if the NSPR was compiled to use
dnl                           POSIX threads.
dnl     action-if-not-found - Action to take if the NSPR could not be found or
dnl                           if the NSPR version requirement is not met.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_HAVE_NSPR,
[
    if test "x$2" = "x" ; then
        AC_REQUIRE([DPP_ENABLE_PTHREADS])
        true
    fi

    if test "x$PTHREADS_ENABLED" = "xyes" -o "x$2" = "xyes" ; then
        AC_REQUIRE([DPP_CC_PTHREAD_ARG])
        true
    fi

    dnl -----------------------------------------------------------------------
    dnl Define the root directory for the NSPR API installation.
    dnl The default is /usr/local.
    dnl -----------------------------------------------------------------------
    AC_ARG_WITH(nspr,
                [  --with-nspr=<PATH>      NSPR installation directory     [default=/usr/local]],
                NSPR_ROOT="$withval", NSPR_ROOT='/usr/local')

    AC_LANG_SAVE
    AC_LANG_C

    dpp_save_CPPFLAGS="$CPPFLAGS"
    dpp_save_LDFLAGS="$LDFLAGS"

    CPPFLAGS="$CPPFLAGS -I$NSPR_ROOT/include"
    LDFLAGS="-L$NSPR_ROOT/lib $LDFLAGS"

    if test "x$2" = "xyes"; then
        if test "x$CC_ACCEPTS_PTHREAD" = "xyes" ; then
            LDFLAGS="$LDFLAGS -pthread"
        else
            dpp_pthread_lib="$PTHREAD_LIB"
        fi
    else
        dpp_pthread_lib=''
    fi

    dnl Check the NSPR version if a version number was given.
    if test "x$1" != "x" ; then
        DPP_NSPR_VER($NSPR_ROOT, $1, $3)
    else
        AC_MSG_ERROR(*** No NSPR version given! ***)
    fi

    AC_CHECK_LIB(nspr$NSPR_VER, PR_CreateThread, AC_CHECK_HEADER(nspr.h, , $3),
                 $3, $dpp_pthread_lib)

    CPPFLAGS="$dpp_save_CPPFLAGS"
    LDFLAGS="$dpp_save_LDFLAGS"

    AC_LANG_RESTORE

    NSPR_LIB_STATIC="\$(NSPR_ROOT)/lib/libnspr$NSPR_VER.a"
    PLC_LIB_STATIC="\$(NSPR_ROOT)/lib/libplc$NSPR_VER.a"
    PLDS_LIB_STATIC="\$(NSPR_ROOT)/lib/libplds$NSPR_VER.a"

    dnl -----------------------------------------------------------------------
    dnl Do the substition step for $NSPR_ROOT.
    dnl -----------------------------------------------------------------------
    AC_SUBST(NSPR_ROOT)
    AC_SUBST(NSPR_LIB_STATIC)
    AC_SUBST(PLC_LIB_STATIC)
    AC_SUBST(PLDS_LIB_STATIC)
])

dnl ---------------------------------------------------------------------------
dnl Make sure that the installed NSPR is at least the given version.
dnl
dnl Usage:
dnl     DPP_NSPR_VER(nspr-root, version [, action-if-not-found])
dnl
dnl Arguments:
dnl     nspr-root           - Root directory of the NSPR installation.
dnl     version             - Minimum required version.
dnl     action-if-not-found - Action to take if the version requirement is not
dnl                           met.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_NSPR_VER,
[
    AC_REQUIRE([DPP_HAVE_NSPR])

    dnl -----------------------------------------------------------------------
    dnl Define the version number of the NSPR installation.  This is the number
    dnl at the end of the library names (e.g., libnspr4.so).
    dnl -----------------------------------------------------------------------
    AC_ARG_WITH(nsprver,
                [  --with-nsprver=<VER>    Minimum NSPR version  [default=$2]],
                NSPR_VER="$withval", NSPR_VER='$2')

    dpp_save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS -I$1/include"

    AC_CHECK_HEADER(prinit.h, , $3)

    CPPFLAGS="$dpp_save_CPPFLAGS"

    dnl Determine the NSPR version being used.
    dpp_ver_num_exp=['s/.*\([0-9][0-9]*\)$/\1/']
    dpp_nspr_ver_major=`grep PR_VMAJOR $1/include/prinit.h | sed -e $dpp_ver_num_exp` ;
    dpp_nspr_ver_minor=`grep PR_VMINOR $1/include/prinit.h | sed -e $dpp_ver_num_exp` ;
    dpp_nspr_ver_patch=`grep PR_VPATCH $1/include/prinit.h | sed -e $dpp_ver_num_exp` ;

    dpp_nspr_ver="${dpp_nspr_ver_major}.${dpp_nspr_ver_minor}.${dpp_nspr_ver_patch}"

    AC_CACHE_CHECK(NSPR version is >= $2, dpp_cv_nspr_version_okay,
        DPP_VERSION_CHECK($dpp_nspr_ver, $2,
          dpp_cv_nspr_version_okay='yes',
          dpp_cv_nspr_version_okay='no'))

    if test "x$dpp_cv_nspr_version_okay" = "xno" ; then
        $3
        true
    fi

    NSPR_VER="$dpp_nspr_ver_major"
])
