dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
dnl
dnl VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ************* <auto-copyright.pl END do not edit this line> *************

AC_DEFUN(VJ_COMPILER_SETUP,
[
    AC_REQUIRE([DPP_WITH_GCC])
    AC_REQUIRE([DPP_ENABLE_NSPR_THREADS])
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    # Override the C compiler that configure would otherwise find itself.
    AC_ARG_WITH(cc, [  --with-cc=<PATH>        Alternate C compiler ],
                _alt_cc="$withval", _alt_cc='none')

    # Override the C++ compiler that configure would otherwise find itself.
    AC_ARG_WITH(cxx, [  --with-cxx=<PATH>       Alternate C++ compiler ],
                _alt_cxx="$withval", _alt_cxx='none')

    # -------------------------------------------------------------------------
    # Checks for programs.
    # -------------------------------------------------------------------------
    CC_ERR=''
    CXX_ERR=''

    _vjsave_CFLAGS="$CFLAGS"
    _vjsave_CXXFLAGS="$CXXFLAGS"

    # If no alternate C compiler was specified, check to see if it is
    # necessary to force the use of a specific compiler on a given platform.
    if test "x${_alt_cc}" = "xnone" ; then
        CC_IRIX='cc'
        CC_PIC_IRIX='-KPIC'

        dnl # On IRIX, we have to use cc (not gcc).
        if test "$PLATFORM" = "IRIX" ; then
            CC_ERR='*** The library requires the IRIX MIPSpro C compiler ***'
        fi
    # Otherwise, force the use of the alternate C compiler.
    else
        CC="${_alt_cc}"
    fi

    # If no alternate C++ compiler was specified, check to see if it is
    # necessary to force the use of a specific compiler on a given platform.
    if test "x${_alt_cxx}" = "xnone" ; then
        CXX_HP='aCC'
        CXX_IRIX='CC'
        CXX_PIC_IRIX='-KPIC'

        # On HP-UX, we have to use the aCC C++ compiler.
        if test "$PLATFORM" = "HP" ; then
            CXX_ERR='*** The library requires the HP-UX aCC C++ compiler ***'
        # On Windows, we have to use the Microsoft Visual C++ compiler CL.EXE.
        elif test "$OS_TYPE" = "Win32" -a "x$USE_GCC" != "xyes" ; then
            DPP_PROG_MSVCCC
            CXX_ERR='*** The library requires the MS Visual C++ compiler ***'
        # On IRIX, we have to use CC (not g++).
        elif test "x$PLATFORM" = "xIRIX" ; then
            CXX_ERR='*** The library requires the IRIX MIPSpro C++ compiler ***'
        fi
    # Otherwise, force the use of the alternate C++ compiler.
    else
        CXX="${_alt_cxx}"
    fi

    DPP_PROG_CC($CC, , AC_MSG_ERROR($CC_ERR))
    DPP_PROG_CXX($CXX, , AC_MSG_ERROR($CXX_ERR))
    DPP_GET_EXT

    if test "x$GXX" = "xyes" ; then
        # If we are using GCC as the compiler, we need to be using at least
        # egcs 1.1.2.  A newer version (e.g., 2.95.2) is even better.
        DPP_GPLUSPLUS_MODERN(AC_MSG_ERROR(*** VR Juggler requires a modern G++ ***))
    fi

    # Now check to see if the compiler accepts the -pthread option.
    if test "x$1" != "xSPROC" -a "x$1" != "WIN32" -a \
            "x$NSPR_THREADS_ENABLED" != "xyes"
    then
        DPP_CC_PTHREAD_ARG

        if test "x$CC_ACCEPTS_PTHREAD" = "xno" ; then
            DPP_CC_PTHREADS_ARG
        fi
    fi

    # Ensure that the C++ compiler we've found is capable of compiling the
    # newer C++ features that we need.
    DPP_CXX_TEMPLATES(AC_MSG_ERROR(*** The library requires C++ template support ***))
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_MTREE_GEN(file-prefix, path, platform [, ISA])
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_MTREE_LIB_GEN,
[
    mtree_path=`echo $2 | sed -e 's|/| |g'`

    # Ensure that the release directory exists since configure would not
    # create it otherwise.
    for dir in $mtree_path ; do
        full_path="${full_path}${dir}/"
        if test ! -d "$full_path" ; then
            mkdir "$full_path"
        fi
    done

    # Create the VJ.lib.dist file in release/mtree.  This is generated from
    # scratch since it cannot be generalized into a template using our
    # library directory scheme.  The only time this file needs to be generated
    # is when configure is run, so it is not set up to be one of the commands
    # that config.status can execute.
    _lib_file="$2/$1.lib.dist"
    _set_line="/set type=dir mode=$DIR_PERMS uname=$OWNER_NAME gname=$GROUP_NAME"

    # Ensure that the file exists and has zero length.  Then write out the
    # /set line which is going to be there no matter what.
    echo "creating ${_lib_file}"
    echo >${_lib_file}
    echo "${_set_line}" >>${_lib_file}
    echo "" >>${_lib_file}

    # On IRIX, it's easy to compile for different ABI/ISA combinations, so we
    # just make cases for all -n32/-64/-mips3/-mips4 settings.
    if test "x$3" = "xIRIX" ; then
        cat >>${_lib_file} <<END_IRIX_DIST
.
    lib32
        mips3
            debug
            ..
            opt
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
        ..
    ..
    lib64
        mips3
            debug
            ..
            opt
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
        ..
    ..
..
END_IRIX_DIST
    # If a value for $4 has been given (which is the case for some platforms),
    # create a subdirectory in lib for that value.
    elif test "x$4" != "x" ; then
        cat >>${_lib_file} <<END_ISA_DIST
.
    lib
        $4
            debug
            ..
            opt
            ..
        ..
    ..
..
END_ISA_DIST
    # In all other cases, just make a simple directory tree for debugging and
    # optimized libraries.
    else
        cat >>${_lib_file} <<END_DIST
.
    lib
        debug
        ..
        opt
        ..
    ..
..
END_DIST
    fi
])
