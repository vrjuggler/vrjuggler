dnl ===========================================================================
dnl Perform checks for various programs that are helpful for a complete build
dnl system.  Currently, it checks for a BSD-compatible install(1), for
dnl ranlib(1) (except on IRIX) and for a working 'ln -s' command.  It also
dnl determines whether make(1) sets $(MAKE).
dnl ===========================================================================

dnl basic_progs.m4,v 1.3 2001/01/10 00:09:27 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Test for basic programs need by most, if not all, build systems.
dnl
dnl Usage:
dnl     DPP_BASIC_PROGS
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_BASIC_PROGS,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    dnl If we are on a Win32 system, use $ac_install_sh for the install(1)
    dnl program.  This prevents problems with paths if an install program is
    dnl found elsewhere on the system.
    if test "x$dpp_os_type" = "xWin32" -o "x$2" = "xWin32" ; then
        dnl XXX: This may not be a safe value to use since it is internal to
        dnl the generated configure script.
        INSTALL="$ac_install_sh"
    dnl Otherwise, use the safe AC_PROG_INSTALL macro.
    else
        AC_PROG_INSTALL
    fi

    AC_PROG_MAKE_SET

    dnl While ranlib(1) is not part of the base IRIX installation, the VRAC
    dnl SGIs have a ranlib(1) installed that does very bad things to ar(1)
    dnl archives.  We also do not want to use the Cygwin ranlib(1) with the
    dnl Visual C++ LINK.EXE command.
    if test "x$dpp_platform" = "xIRIX" -o "x$1" = "xIRIX" -o	\
            "x$dpp_os_type" = "xWin32 -o "x$2" = "xWin32" ; then
        RANLIB=':'
    else
        AC_PROG_RANLIB
    fi

    AC_PROG_LN_S
])
