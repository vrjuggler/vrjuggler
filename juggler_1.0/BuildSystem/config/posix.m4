dnl ===========================================================================
dnl Check for POSIX types.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_CHECK_TYPE_POSIX - Check for POSIX types such as int32_t and
dnl                            caddr_t.
dnl ===========================================================================

dnl posix.m4,v 1.2 2000/12/29 04:17:20 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Check for POSIX types such as int32_t and caddr_t.
dnl
dnl Usage:
dnl     DPP_CHECK_TYPE_POSIX
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_CHECK_TYPE_POSIX,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    if test "x$dpp_os_type" = "xWin32" ; then
        AC_CHECK_TYPE(u_int, UINT)
        AC_CHECK_TYPE(int32_t, __int32)
        AC_CHECK_TYPE(int64_t, __int64)
    else
        AC_CHECK_TYPE(u_int, unsigned int)
    fi

    AC_CHECK_TYPE(caddr_t, char*)
])
