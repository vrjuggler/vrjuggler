dnl ===========================================================================
dnl Provide a command-line option to enable a check for the POSIX threads
dnl library and the associated check.  This is useful because it
dnl is often installed using a different name from platform to platform.  This
dnl code also checks to see if -pthread is a valid compiler option and stores
dnl the result in a cache variable that can be used later.  Several
dnl preprocessor definitions are made depending on the results of the checks.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_ENABLE_PTHREADS - State that POSIX threads are needed for
dnl                           compiling.
dnl     DPP_CC_PTHREAD_ARG  - Determine if the C compiler accepts the -pthread
dnl                           command-line argument.
dnl     DPP_GET_PTHREAD_LIB - Determine the name of the pthread library needed
dnl                           for linking.
dnl     DPP_GET_PTHREAD_VER - Check for the pthread draft revision implemented
dnl                           on the host system.
dnl     DPP_PTHREAD_FINISH  - Complete the pthread configuration process.
dnl
dnl Command-line options added:
dnl     --enable-pthreads  - State that POSIX threads are needed for
dnl                          compiling.
dnl
dnl Variables defined:
dnl     PTHREAD_LIB        - The name of the POSIX threads library (named as
dnl                          -l<pthread lib>).
dnl     CC_ACCEPTS_PTHREAD - A cache variable stating whether or not (i.e.,
dnl                          'yes' or 'no) the compilers accept -pthread as a
dnl                          valid argument.
dnl     EXTRA_LDFLAGS      - Additional flags to pass to $LD when linking.
dnl
dnl Possible preprocessor definitions:
dnl     _PTHREADS_DRAFT_10 - The POSIX thread implementation is Draft 10.
dnl     _PTHREADS_DRAFT_4  - The POSIX thread implementation is Draft 4.
dnl ===========================================================================

dnl pthreads.m4,v 1.3 2000/12/15 23:56:17 patrick Exp

dnl ---------------------------------------------------------------------------
dnl State that POSIX threads are needed for compiling.
dnl
dnl Usage:
dnl     DPP_ENABLE_PTHREADS([default-pthread-enableval])
dnl
dnl Arguments:
dnl     default-pthread-enableval - The default value for enabling or
dnl                                 disabling pthreads.  This should be either
dnl                                 "yes" or "no".  This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_ENABLE_PTHREADS,
[
    AC_ARG_ENABLE(pthreads,
                  [  --enable-pthreads       Use of pthreads are needed      [default=$1]],
                  PTHREADS_ENABLED="$enableval", PTHREADS_ENABLED='$1')
])

dnl ---------------------------------------------------------------------------
dnl Determine if the C compiler (and by assumption, the C++ compiler) accepts
dnl the -pthread command-line argument.  Based on the result of the check, the
dnl variable $CC_ACCEPTS_PTHREAD is set to "yes" or "no".
dnl
dnl Usage:
dnl     DPP_CC_PTHREAD_ARG
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CC_PTHREAD_ARG,
[
    AC_REQUIRE([DPP_PROG_CC])
    AC_BEFORE([$0], [DPP_GET_PTHREAD_LIB])
    AC_BEFORE([$0], [DPP_PTHREAD_FINISH])

    AC_LANG_SAVE
    AC_LANG_C

    dnl Determine if -pthread is a valid compiler option.  If it is, set
    dnl $dpp_cv_CC_accepts_pthread to 'yes'; otherwise, set it to 'no'.
    AC_CACHE_CHECK(whether $CC accepts -pthread,
        dpp_cv_CC_accepts_pthread,
        [ echo 'int main(){}' > conftest.c
          if test -z "`${CC} -pthread -o conftest conftest.c 2>&1`" ; then
              dpp_cv_CC_accepts_pthread='yes'
          else
              dpp_cv_CC_accepts_pthread='no'
          fi
          rm -f conftest*
        ])

    CC_ACCEPTS_PTHREAD="$dpp_cv_CC_accepts_pthread"

    AC_LANG_RESTORE
])

dnl ---------------------------------------------------------------------------
dnl Determine the name of the pthread library needed for linking.  The default
dnl names to check are pthread, pthreads, cma, and c_r in that order.  Based
dnl on the result of the check, the variable $PTHREAD_LIB is set to the 
dnl linker option with the name of the library (-l<name>).
dnl
dnl Usage:
dnl     DPP_GET_PTHREAD_LIB([lib1 lib2 ...])
dnl
dnl Arguments:
dnl     lib1 lib2 ... - A space-separated list of possible alternative names
dnl                     for the pthreads library.  The names passed should be
dnl                     that used with the linker such as -l<name>.  In other
dnl                     words, only <name> should be passed.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_GET_PTHREAD_LIB,
[
    AC_REQUIRE([DPP_CC_PTHREAD_ARG])
    AC_BEFORE([$0], [DPP_GET_PTHREAD_VER])

    dpp_pthread_lib=''

    dnl If the compiler accepts -pthread, don't bother trying to find the
    dnl pthreads library.
    if test "x$CC_ACCEPTS_PTHREAD" = "xno" ; then
        AC_LANG_SAVE
        AC_LANG_C

        dnl On HP-UX 10.20, the pthreads library is in the cma library.  On
        dnl other systems, the pthreads stuff is in libc_r.
        dpp_pthread_lib_names="$1 pthread pthreads cma c_r"

        for lib in $dpp_pthread_lib_names ; do
            AC_CHECK_LIB($lib, pthread_create,
                AC_CHECK_HEADER(pthread.h,
                    [ dpp_pthread_lib="-l${lib}" ; break ],
                    AC_MSG_WARN(*** POSIX threads will not be used (header file not found) ***)))
        done

        AC_LANG_RESTORE
    fi

    PTHREAD_LIB="$dpp_pthread_lib"
])

dnl ---------------------------------------------------------------------------
dnl Check for the pthread draft revision implemented on the host system.  This
dnl is done by determining if pthread_kill(3) is in the library.  If it is,
dnl Draft 10 is in use.  Otherwise, assume it is Draft 4.  Based on the
dnl results of the test, either _PTHREADS_DRAFT_10 or _PTHREADS_DRAFT_4 is
dnl defined as a preprocessor symbol.
dnl
dnl Usage:
dnl     DPP_GET_PTHREAD_VER(platform [, symbol])
dnl
dnl Arguments:
dnl     platform - The name of the target platform.
dnl     symbol   - The preprocessor symbol used for _POSIX_C_SOURCE to define
dnl                based on the result of the tests.  For example, if MY_SYM
dnl                were passed, it would be #define'd to the value appropriate
dnl                for the pthreads version found, and _POSIX_C_SOURCE should
dnl                then be #define'd to MY_SYM.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_GET_PTHREAD_VER,
[
    AC_REQUIRE([DPP_GET_PTHREAD_LIB])

    AC_LANG_SAVE
    AC_LANG_C

    dpp_save_CFLAGS="$CFLAGS"

    if test "x$CC_ACCEPTS_PTHREAD" = "xyes" ; then
        CFLAGS="$CFLAGS -pthread"
    fi

    dpp_save_LIBS="$LIBS"
    LIBS="$LIBS $PTHREAD_LIB"

    AC_CACHE_CHECK(POSIX threads draft revision,
        dpp_cv_pthread_version,
        AC_TRY_LINK([#include <pthread.h> ],
            [ pthread_t t; pthread_kill(t, 1); ],
            dpp_cv_pthread_version='Draft 10',
            dpp_cv_pthread_version='Draft 4'))

    CFLAGS="$dpp_save_CFLAGS"
    LIBS="$dpp_save_LIBS"

    if test "x$dpp_cv_pthread_version" = "xDraft 10" ; then
        AC_DEFINE(_PTHREADS_DRAFT_10,)
    else
        AC_DEFINE(_PTHREADS_DRAFT_4,)
    fi

    if test "x$2" != "x" ; then
        dnl Define the appropriate value for _POSIX_C_SOURCE on the current
        dnl platform.
        if test "$1" = "IRIX" -o "$1" = "Linux" -o "$1" = "Solaris" ; then
            AC_DEFINE($2, 199506L)
        else
            AC_DEFINE($2, 2)
        fi
    fi

    AC_LANG_RESTORE
])

dnl ---------------------------------------------------------------------------
dnl Complete the pthread configuration process.  If the C compiler accepts the
dnl -pthread argument, "-pthread" is appended to the variable $EXTRA_LDFLAGS.
dnl Substitution is performed on $EXTRA_LDFLAGS.
dnl
dnl Usage:
dnl     DPP_PTHREAD_FINISH
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PTHREAD_FINISH,
[
    AC_REQUIRE([DPP_CC_PTHREAD_ARG])

    if test "x$CC_ACCEPTS_PTHREAD" = "xyes" ; then
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -pthread"
    fi

    AC_SUBST(EXTRA_LDFLAGS)
])
