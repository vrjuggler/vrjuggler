dnl ===========================================================================
dnl Determine which C and C++ compilers to use for compiling.  The
dnl capabilities of the C++ compiler are tested here as well to ensure that
dnl the proper language support is available.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_STD_CPP       - Add a command line option to enable or disable the
dnl                         use of standard C++ by the C++ compiler.
dnl     DPP_PROG_CC       - Check for the existence and usability of a C
dnl                         compiler.
dnl     DPP_PROG_CXX      - Check for the existence and usability of a C++
dnl                         compiler.
dnl     DPP_CXX_TEMPLATES - Test the C++ compiler's template-handling
dnl                         capabilities.
dnl
dnl Command-line options added:
dnl     --enable-std
dnl
dnl Variables defined:
dnl
dnl     CC     - The C compiler.
dnl     CPP    - The C compiler's preprocessor.
dnl     CXX    - The C++ compiler.
dnl     CXXCPP - The C++ compiler's preprocessor.
dnl ===========================================================================

dnl compiler.m4,v 1.3 2001/01/07 16:02:02 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Add a command line option to enable or disable the use of standard C++ by
dnl the C++ compiler.  This includes use of the std namespace.
dnl
dnl Usage:
dnl     DPP_STD_CPP([default-std-setting])
dnl
dnl Arguments:
dnl     default-std-setting - The default value, "yes" or "no, for the use of
dnl                           standard C++ by the compiler.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_STD_CPP,
[
    AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

    AC_ARG_ENABLE(std,
                  [  --enable-std            Force the use of standard C++   [default=$1]],
                  dpp_std="$enableval", dpp_std=$1)
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of a C compiler (or the given C
dnl compiler if one is specified) and how to run its preprocessor.
dnl
dnl Usage:
dnl     DPP_PROG_CC([C-compiler [, extra-flags [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     C-compiler          - Force the check to look for this C compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C compiler cannot be
dnl                           found or does not work.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CC,
[
    dpp_save_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $2 ${_EXTRA_FLAGS}"

    if test "x$1" != "x" ; then
        AC_CHECK_PROG(CC, $1, $1, $3)
    fi

    AC_PROG_CC
    AC_PROG_CPP

    CFLAGS="$dpp_save_CFLAGS"
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of a C++ compiler (or the given C++
dnl compiler if one is specified) and how to run its preprocessor.
dnl
dnl Usage:
dnl     DPP_PROG_CXX([C++-compiler [, extra-flags [, action-if-not-found]]])
dnl
dnl Arguments:
dnl     C++-compiler        - Force the check to look for this C++ compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C++ compiler cannot be
dnl                           found or does not work.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PROG_CXX,
[
    dpp_save_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS $2 ${_EXTRA_FLAGS}"

    if test "x$1" != "x" ; then
        AC_CHECK_PROG(CXX, $1, $1, $3)
    fi

    AC_PROG_CXX
    AC_PROG_CXXCPP

    CXXFLAGS="$dpp_save_CXXFLAGS"
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's template-handling capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_TEMPLATES([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with modern templates.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CXX_TEMPLATES,
[
    AC_REQUIRE([DPP_PROG_CXX])
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl Ensure that the C++ compiler we've found is capable of compiling the
    dnl newer C++ features that we need.
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    dpp_save_CXXFLAGS="$CXXFLAGS"

    dnl When compiling on Windows, we need to force the compiler to treat the
    dnl source file as a .cpp file even though it will end in .C.  The lack
    dnl of case-sensitive file names strikes again!
    if test "x$dpp_os_type" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
        CXXFLAGS="$CXXFLAGS -TP"
    fi

    AC_CACHE_CHECK(whether the C++ compiler handles templates properly,
        dpp_cv_CXX_handles_templates,
        AC_TRY_COMPILE([#include <map> ],
                       [ std::map<const char*, int> test_map; ],
                       [ dpp_cv_CXX_handles_templates='yes' ;
                         rm -rf ./ii_files ],
                       [ dpp_cv_CXX_handles_templates='no' ; $1 ]))

    CXXFLAGS="$dpp_save_CXXFLAGS"

    AC_LANG_RESTORE
])
