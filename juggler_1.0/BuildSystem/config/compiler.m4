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
dnl     CC              - The C compiler.
dnl     CPP             - The C compiler's preprocessor.
dnl     CXX             - The C++ compiler.
dnl     CXXCPP          - The C++ compiler's preprocessor.
dnl     CFLAGS_DYNLIB   - Position-independent code generation flag for the
dnl                       C compiler.  Substituted automatically.
dnl     CXXFLAGS_DYNLIB - Position-independent code generation flag for the
dnl                       C++ compiler.  Substituted automatically.
dnl ===========================================================================

dnl compiler.m4,v 1.9 2001/02/20 17:33:17 patrick Exp

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
dnl compiler if one is specified) and how to run its preprocessor.  A
dnl platform-specific hint for the C compiler can be given by setting a value
dnl for the variable $CC_<PLATFORM> (e.g., $CC_Linux).  A value for the
dnl C-compiler argument overrides any hint given, however.  A list of
dnl fallbacks can be provided by setting a value for the variable
dnl $CC_FALLBACKS.  This must be in the format of an sh list.
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

    dnl Get the platform-specific compiler hint value.
    dpp_cc_var="CC_$dpp_platform"
    dpp_cc_val=`eval echo '$''{'$dpp_cc_var'}'`

    if test "x$dpp_cc_val" != "x" -a "x$CC" = "x" ; then
        CC="$dpp_cc_val"
    fi

    if test "x$1" != "x" -o "x$CC" != "x" ; then
        if test "x$1" != "x" ; then
            dpp_CC_found_val=$1
            AC_CHECK_PROG(dpp_CC_check_val, $1, $1)
        else
            dpp_CC_found_val="$CC"
            AC_CHECK_PROG(dpp_CC_check_val, $CC, $CC)
        fi

        if test "x$dpp_CC_check_val" = "x$dpp_CC_found_val" ; then
            CC="$dpp_CC_found_val"
        else
            dpp_CC_found='no'

            dnl If the caller provided a list of fallbacks in the $CC_FALLBACKS
            dnl variable, loop over them and try to use another C compiler.
            if test "x$CC_FALLBACKS" != "x" ; then
                for dpp_cc in $CC_FALLBACKS ; do
                    AC_CHECK_PROG(dpp_CC_check_val, $dpp_cc, $dpp_cc)
                    if test "x$dpp_CC_check_val" = "x$dpp_cc" ; then
                        CC="$dpp_cc"
                        dpp_CC_found='yes'
                        break
                    fi
                done
            fi

            dnl No viable C compiler was found.
            if test "x$dpp_CC_found" = "xno" ; then
                $3
                true
            fi
        fi
    fi

    AC_PROG_CC
    AC_PROG_CPP

    CFLAGS="$dpp_save_CFLAGS"

    dnl If GCC will be the C compiler, -fPIC is the position-independent code
    dnl generation option.
    if test "x$GCC" = "xyes" ; then
        CFLAGS_DYNLIB='-fPIC'
    dnl Otherwise, get the platform-specific compiler PIC option hint value.
    else
        dpp_cc_pic_var="CC_PIC_$dpp_platform"
        CFLAGS_DYNLIB=`eval echo '$''{'$dpp_cc_pic_var'}'`
    fi

    AC_SUBST(CFLAGS_DYNLIB)
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of a C++ compiler (or the given C++
dnl compiler if one is specified) and how to run its preprocessor.  A
dnl platform-specific hint for the C++ compiler can be given by setting a
dnl value for the variable $CXX_<PLATFORM> (e.g., $CXX_Linux).  A value for
dnl the C++-compiler argument overrides any hint given, however.  A list of
dnl fallbacks can be provided by setting a value for the variable
dnl $CXX_FALLBACKS.  This must be in the format of an sh list.
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

    dnl Get the platform-specific compiler hint value.
    dpp_cxx_var="CXX_$dpp_platform"
    dpp_cxx_val=`eval echo '$''{'$dpp_cxx_var'}'`

    if test "x$dpp_cxx_val" != "x" -a "x$CXX" = "x" ; then
        CXX="$dpp_cxx_val"
    fi

    if test "x$1" != "x" -o "x$CXX" != "x" ; then
        if test "x$1" != "x" ; then
            dpp_CXX_found_val=$1
            AC_CHECK_PROG(dpp_CXX_check_val, $1, $1)
        else
            dpp_CXX_found_val="$CXX"
            AC_CHECK_PROG(dpp_CXX_check_val, $CXX, $CXX)
        fi

        if test "x$dpp_CXX_check_val" = "x$dpp_CXX_found_val" ; then
            CXX="$dpp_CXX_found_val"
        else
            dpp_CXX_found='no'

            dnl If the caller provided a list of fallbacks in the
            dnl $CXX_FALLBACKS variable, loop over them and try to use another
            dnl C++ compiler.
            if test "x$CXX_FALLBACKS" != "x" ; then
                for dpp_cxx in $CXX_FALLBACKS ; do
                    AC_CHECK_PROG(dpp_CXX_check_val, $dpp_cxx, $dpp_cxx)
                    if test "x$dpp_CXX_check_val" = "x$dpp_cxx" ; then
                        CXX="$dpp_cxx"
                        dpp_CXX_found='yes'
                        break
                    fi
                done
            fi

            dnl No viable C++ compiler was found.
            if test "x$dpp_CXX_found" = "xno" ; then
                $3
                true
            fi
        fi
    fi

    AC_PROG_CXX
    AC_PROG_CXXCPP

    CXXFLAGS="$dpp_save_CXXFLAGS"

    dnl If GCC's g++ will be the C++ compiler, -fPIC is the
    dnl position-independent code generation option.
    if test "x$GXX" = "xyes" ; then
        CXXFLAGS_DYNLIB='-fPIC'
    dnl Otherwise, get the platform-specific compiler PIC option hint value.
    else
        dpp_cxx_pic_var="CXX_PIC_$dpp_platform"
        CXXFLAGS_DYNLIB=`eval echo '$''{'$dpp_cxx_pic_var'}'`
    fi

    AC_SUBST(CXXFLAGS_DYNLIB)
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
