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

dnl ---------------------------------------------------------------------------
dnl Variables set in Makefiles found in the application trees.  All variables
dnl listed are for use only in the Makefile.in's found in that tree unless
dnl otherwise noted.  They are used in the following way:
dnl
dnl     * APP_EXTRA_FLAGS: Extra flags for the compiler and linker.
dnl
dnl Usage
dnl     VJ_APP_COMPILER(C-compiler, C-flags, C++-compiler, C++-flags, debug-flags, optimization-flags, base-dir, defs, includes, extra-flags)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_APP_COMPILER,
[
    if test "$OS_TYPE" = "UNIX" ; then
        APP_CC="$1"
        APP_CXX="$3"
    elif test "$OS_TYPE" = "Win32" ; then
        if test "x$DPP_USING_MSVCCC" = "xyes" ; then
            APP_CC='cl'
            APP_CXX='cl'
        else
            APP_CC="$1"
            APP_CXX="$3"
        fi
    fi

    APP_CFLAGS="$2"
    APP_CXXFLAGS="$4"
    APP_DEBUG_FLAGS="$5"
    APP_OPTIM_FLAGS="$6"
    APP_DEFS="$8"
    APP_INCLUDES=ifelse([$7], , "$9", "-I\$($7)/include $9")
    APP_EXTRA_FLAGS="$10"

    AC_SUBST(APP_CC)
    AC_SUBST(APP_CFLAGS)
    AC_SUBST(APP_CXX)
    AC_SUBST(APP_CXXFLAGS)
    AC_SUBST(APP_DEBUG_FLAGS)
    AC_SUBST(APP_OPTIM_FLAGS)
    AC_SUBST(APP_DEFS)
    AC_SUBST(APP_INCLUDES)
    AC_SUBST(APP_EXTRA_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Variables set in Makefiles found in the application trees.  All variables
dnl listed are for use only in the Makefile.in's found in that tree unless
dnl otherwise noted.  They are used in the following way:
dnl
dnl     * APP_BASIC_LIBS: The basic libraries being built in this system.
dnl     * APP_EXTRA_LIBS_BEGIN: Options used at the beginning of the list of
dnl       extra libraries.
dnl     * APP_EXTRA_LIBS_END: Options used at the end of the list of extra
dnl       libraries.
dnl     * APP_EXTRA_LIBS: A basic set of extra libraries and linker flags
dnl       needed for compiling.  These are put after those listed in the
dnl       previous variables in the compile line.
dnl
dnl Usage
dnl     VJ_APP_LINKER(linker, link-flags, base-dir-name, basic-libs, extra-libs)
dnl ---------------------------------------------------------------------------
AC_DEFUN(VJ_APP_LINKER,
[
    if test "$OS_TYPE" = "UNIX" ; then
        APP_LINK="$1"
        APP_LINK_FLAGS="$2"

        for lib in $4 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS -l$lib"
        done

        APP_EXTRA_LIBS="$5"

        if test "$PLATFORM" = "IRIX" ; then
            APP_BASIC_LIBS_BEGIN="-B static -L\$($3)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END='-B dynamic'
            APP_BASIC_LIBS_BEGIN_INST="-B dynamic -L\$($3)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($3)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-B dynamic'
            APP_EXTRA_LIBS_END=''
        elif test "x$GXX" = "xyes" -a "x$PLATFORM" != "xDarwin" ; then
            APP_BASIC_LIBS_BEGIN="-Wl,-Bstatic -L\$($3)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END="-Wl,-Bdynamic"
            APP_BASIC_LIBS_BEGIN_INST="-Wl,-Bdynamic -L\$($3)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($3)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-Wl,-Bdynamic'
            APP_EXTRA_LIBS_END=''
        else
            # For now, we are disabling static linking for the sample
            # applications when compiled in a developer's build tree.  This
            # is only the case on non-IRIX platforms and non-GCC compilers.
            APP_BASIC_LIBS_BEGIN="-L\$($3)/lib$LIBBITSUF"
            APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"
        fi
    elif test "$OS_TYPE" = "Win32" ; then
        if test "x$DPP_USING_MSVCCC" = "xyes" ; then
            APP_LINK='link'
        else
            APP_lINK="$1"
        fi

        for lib in $4 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS ${lib}.lib"
        done

        APP_BASIC_LIBS_BEGIN="-libpath:\$($3)/lib/debug"
        APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"
    fi

    AC_SUBST(APP_LINK)
    AC_SUBST(APP_LINK_FLAGS)
    AC_SUBST(APP_BASIC_LIBS_BEGIN)
    AC_SUBST(APP_BASIC_LIBS_END)
    AC_SUBST(APP_BASIC_LIBS_BEGIN_INST)
    AC_SUBST(APP_BASIC_LIBS_END_INST)
    AC_SUBST(APP_BASIC_LIBS)
    AC_SUBST(APP_BASIC_EXT_LIBS)
    AC_SUBST(APP_EXTRA_LIBS_BEGIN)
    AC_SUBST(APP_EXTRA_LIBS_END)
    AC_SUBST(APP_EXTRA_LIBS_BEGIN_INST)
    AC_SUBST(APP_EXTRA_LIBS_END_INST)
    AC_SUBST(APP_EXTRA_LIBS)
])
