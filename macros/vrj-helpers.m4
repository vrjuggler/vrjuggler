dnl ************ <auto-copyright.pl BEGIN do not edit this line> ************
dnl
dnl VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

AC_DEFUN([VJ_STRIP_JAR_PATH],
[
   dnl Helper function that removes any path information leading up to a JAR
   dnl file name (i.e, a file that ends in .jar).
   strip_jar_path ( ) {
      jar_list=`echo $[1] | sed -e "s/$JCPS/ /g"`

      changequote(<<, >>)
      for jar in $jar_list ; do
         no_path_jar=`echo $jar | sed -e 's/^.*[/\]\([A-Za-z0-9_][A-Za-z0-9_.-]*\.jar\)/\1/'`
         no_path_jar_list="$no_path_jar_list $no_path_jar"
      done
      changequote([, ])

      dnl Strip off any leading spaces that would have been added in the above
      dnl loop.
      retval=`echo "$no_path_jar_list" | sed -e 's/^  *//'`
      echo "$retval"
   }

   $1=`strip_jar_path $2`
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_PROG_CC_PROF_FLAG([prefer-gnu])
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_PROG_CC_PROF_FLAG],
[
    if test "x$GCC" = "xyes" ; then
        if test "x$1" = "xyes" ; then
            DPP_PROG_CC_PROF_PG

            if test "x$CC_PROF_FLAGS" = "x" ; then
                DPP_PROG_CC_PROF_P
            fi
        else
            DPP_PROG_CC_PROF_P

            if test "x$CC_PROF_FLAGS" = "x" ; then
                DPP_PROG_CC_PROF_PG
            fi
        fi

        if test "x$CC_PROF_FLAGS" = "x" ; then
            BUILD_PROF_C='N'
        else
            BUILD_PROF_C='Y'
        fi
    else
        BUILD_PROF_C='N'
    fi

    AC_SUBST(BUILD_PROF_C)
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_PROG_CXX_PROF_FLAG([prefer-gnu])
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_PROG_CXX_PROF_FLAG],
[
    vrj_prefer_gnu_prof=$1

    if test "x$GXX" = "xyes" ; then
        if test "x$vrj_prefer_gnu_prof" = "xyes" ; then
            DPP_PROG_CXX_PROF_PG

            if test "x$CXX_PROF_FLAGS" = "x" ; then
                DPP_PROG_CXX_PROF_P
            fi
        else
            DPP_PROG_CXX_PROF_P

            if test "x$CXX_PROF_FLAGS" = "x" ; then
                DPP_PROG_CXX_PROF_PG
            fi
        fi

        if test "x$CXX_PROF_FLAGS" = "x" ; then
            BUILD_PROF_CXX='N'
        else
            BUILD_PROF_CXX='Y'
            LDOPTS_PROF="$LDOPTS_PROF $CXX_PROF_FLAGS"
        fi
    else
        BUILD_PROF_CXX='N'
    fi

    AC_SUBST(BUILD_PROF_CXX)
])

AC_DEFUN([VJ_COMPILER_SETUP],
[
    DPP_PREREQ([1.99.21])

    AC_REQUIRE([DPP_WITH_GCC])
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
        # On IRIX, we can use MIPSpro or GCC.  We default to MIPSpro.
        if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
            CC_IRIX='cc'
            CC_PIC_IRIX='-KPIC'
        # On Solaris/SPARC, we can use Sun WorkShop or GCC.  We default to
        # WorkShop.
        elif test "x$PLATFORM" = "xSolaris" -a "x$USE_GCC" != "xyes" \
               -a "x$ISA" = "xsparc"
        then
            CC_Solaris='cc'
            CC_PIC_Solaris='-KPIC'
        fi
    # Otherwise, force the use of the alternate C compiler.
    else
        CC="${_alt_cc}"
    fi

    # If no alternate C++ compiler was specified, check to see if it is
    # necessary to force the use of a specific compiler on a given platform.
    if test "x${_alt_cxx}" = "xnone" ; then
        CXX_HP='aCC'

        # On HP-UX, we have to use the aCC C++ compiler.
        if test "$PLATFORM" = "HP" ; then
            CXX_ERR='*** The library requires the HP-UX aCC C++ compiler ***'
        # On Windows, we have to use the Microsoft Visual C++ compiler CL.EXE.
        elif test "$OS_TYPE" = "Win32" -a "x$USE_GCC" != "xyes" ; then
            # Prefer the use of cygcl over msvccc.
            DPP_PROG_CYGCL( , , [DPP_PROG_MSVCCC])
            CXX_ERR='*** The library requires the MS Visual C++ compiler ***'
        # On IRIX, we can use MIPSpro or GCC.  We default to MIPSpro.
        elif test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
            CXX_IRIX='CC'
            CXX_PIC_IRIX='-KPIC'
        # On Soalris/SPARC, we can use WorkShop or GCC.  We default to
        # WorkShop.
        elif test "x$PLATFORM" = "xSolaris" -a "x$USE_GCC" != "xyes" \
               -a "x$ISA" = "xsparc"
        then
            CXX_Solaris='CC'
            CXX_PIC_Solaris='-KPIC'
        fi
    # Otherwise, force the use of the alternate C++ compiler.
    else
        CXX="${_alt_cxx}"
    fi

    DPP_PROG_CC([$CC], , [AC_MSG_ERROR($CC_ERR)])
    DPP_PROG_CXX([$CXX], , [AC_MSG_ERROR($CXX_ERR)])
    DPP_SETUP_COMPILER
    DPP_GET_EXT

    # GCC is our compiler (g++ will compile the C++ code).
    if test "x$GXX" = "xyes" ; then
        # If we are using GCC as the compiler, we need to be using version
        # 3.0 or newer.
        DPP_GPLUSPLUS_VER(3.0,
                          [AC_MSG_ERROR([*** VR Juggler requires a modern G++ ***])])
    fi

    # Test to see if the C and C++ compilers support the -pipe (or equivalent)
    # argument.  If one or both of them do, then it is added to $CFLAGS and
    # $CXXFLAGS respectively.
    DPP_PROG_CC_PIPE
    DPP_PROG_CXX_PIPE
    CFLAGS="$CFLAGS $CC_PIPE_FLAGS"
    CXXFLAGS="$CXXFLAGS $CXX_PIPE_FLAGS"

    # Add visibility flag if supported.
    #DPP_COMPILER_FLAG($CXX, [-fvisibility=hidden], dpp_cv_prog_cc_visibility)
    #if test "x$dpp_cv_prog_cc_visibility" = "xyes" ; then
    #    CXX_VISIBILITY_FLAGS='-fvisibility=hidden'
    #fi

    # The following sets up the warning levels for the compilers we support.
    # The levels we want for C++ are as follows (in increasing order of
    # noisiness):
    #
    #    0: Supress all warnings
    #    1: Basic warnings such as unused/unreferenced variables,
    #       uninitialziezd variables, initializer list disordering
    #    2: Overloaded virtual functions, sign promotion, non-virtual
    #       destructor
    #    3: Private constructor(s) and destructor, inability to inline
    #    4: Shadowing of methods, variables, etc.; unreachable code; ...
    #    5: Recommendations from Scott Meyers, cast issues, etc.
    if test "x$ICPC" = "xyes" ; then
        # Doozer++ gives us reasonable defaults for C code, and we will just
        # use those settings for C++.
        CXX_WARNS_LEVEL_0="$C_WARNS_LEVEL_0"
        CXX_WARNS_LEVEL_1="$C_WARNS_LEVEL_1"
        CXX_WARNS_LEVEL_2="$C_WARNS_LEVEL_2"
        CXX_WARNS_LEVEL_3="$C_WARNS_LEVEL_3"
        CXX_WARNS_LEVEL_4="$C_WARNS_LEVEL_4"
        CXX_WARNS_LEVEL_5="$C_WARNS_LEVEL_5"
    elif test "x$GXX" = "xyes" ; then
        # Doozer++ gives us reasonable defaults for C code.
        CXX_WARNS_LEVEL_0="-w"
        CXX_WARNS_LEVEL_1="-Wall"
        CXX_WARNS_LEVEL_2="$CXX_WARNS_LEVEL_1 -W -Woverloaded-virtual -Wsign-promo -Wnon-virtual-dtor"
        CXX_WARNS_LEVEL_3="$CXX_WARNS_LEVEL_2 -Winline -Wctor-dtor-privacy"
        CXX_WARNS_LEVEL_4="$CXX_WARNS_LEVEL_3 -Wshadow -Wunreachable-code"
        CXX_WARNS_LEVEL_5="$CXX_WARNS_LEVEL_4 -Weffc++ -Wold-style-cast"
    # MIPSpro is our compiler.  The warning levels go in reverse here be we
    # disable fewer warnings at higher levels.
    elif test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
        C_WARNS_LEVEL_0='-woff all'
        C_WARNS_LEVEL_1=''
        C_WARNS_LEVEL_2=''
        C_WARNS_LEVEL_3=''
        C_WARNS_LEVEL_4=''
        C_WARNS_LEVEL_5=''

        # The warnings we care about:
        #    1174: Unreferenced variable
        #    1183: Unsigned int compared to 0
        #    1552: Unused variable
        #    1681: Virtual function overload
        #    3303: Meaningless return type qualifier
        CXX_WARNS_LEVEL_0='-woff all'
        CXX_WARNS_LEVEL_1='-woff 1183,1681,3303'
        CXX_WARNS_LEVEL_2='-woff 1183,3303'
        CXX_WARNS_LEVEL_3='-woff 3303'
        CXX_WARNS_LEVEL_4=''
        CXX_WARNS_LEVEL_5=''
    # Visual C++ is our compiler.
    elif test "$OS_TYPE" = "Win32" -a "x$USE_GCC" != "xyes" ; then
        C_WARNS_LEVEL_0='/w'
        C_WARNS_LEVEL_1='/W1'
        C_WARNS_LEVEL_2='/W2'
        C_WARNS_LEVEL_3='/W3'
        C_WARNS_LEVEL_4='/W4'
        C_WARNS_LEVEL_5='/Wall'

        CXX_WARNS_LEVEL_0='/w'
        CXX_WARNS_LEVEL_1='/W1'
        CXX_WARNS_LEVEL_2='/W2'
        CXX_WARNS_LEVEL_3='/W3'
        CXX_WARNS_LEVEL_4='/W4'
        CXX_WARNS_LEVEL_5='/Wall'
    fi

    if test "x$GCC" = "xyes" ; then
        OPT_FLAGS="$OPT_FLAGS -fno-strict-aliasing"
    fi

    # Ensure that the C++ compiler we've found is capable of compiling the
    # newer C++ features that we need.
    DPP_CXX_TEMPLATES([AC_MSG_ERROR([*** The library requires C++ template support ***])])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will remove all duplicate strings
dnl from arg-list and assign the result to variable.  The given argument list
dnl is searched from beginning to end.  The first instance of a given argument
dnl is saved, and any subsequent duplicates are removed.  This macro is
dnl suitable for use with compiler arguments where the precendence of arguments
dnl is usually based on the first instance.
dnl
dnl Usage:
dnl     VJ_STRIP_DUPLICATE_ARGS(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                stripped list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of arguments from which duplicates will be removd.
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_STRIP_DUPLICATE_ARGS],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0;
   for ( i = 1; i <= NF; i++ )
   {
      # Here, valid_list acts like a string-based dictionary of all the
      # command-line arguments passwed to awk(1).  These come in as $1, $2,
      # etc.  That is the reason for the $i expressions.
      # On the other hand, arg_list acts as an ordered list and will contain
      # only those arguments not previously added.
      if ( ! valid_list[$i] )
      {
         valid_list[$i] = 1;
         arg_list[j] = $i;
         j++;
      }
   }

   # Now, build result as a space-separated string of everything in arg_list.
   result = arg_list[0]
   for ( i = 1; i < j; i++ )
      result = sprintf("%s %s", result, arg_list[i]);

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will remove all duplicate strings
dnl from arg-list and assign the result to variable.  The given argument list
dnl is searched from the end to the beginning.  The first instance of a given
dnl argument is saved, and any subsequent duplicates are removed.  This macro
dnl is suitable for use with linker flags where flags that appear at the end
dnl usually satisfy dependencies of preceding flags.
dnl
dnl Usage:
dnl     VJ_STRIP_DUPLICATE_ARGS_REVERSE(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                stripped list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of arguments from which duplicates will be removd.
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_STRIP_DUPLICATE_ARGS_REVERSE],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0;
   for ( i = NF; i >= 1; i-- )
   {
      # Here, valid_list acts like a string-based dictionary of all the
      # command-line arguments passwed to awk(1).  These come in as $1, $2,
      # etc.  That is the reason for the $i expressions.
      # On the other hand, arg_list acts as an ordered list and will contain
      # only those arguments not previously added.
      if ( ! valid_list[$i] )
      {
         valid_list[$i] = 1;
         arg_list[j] = $i;
         j++;
      }
   }

   # Now, build result as a space-separated string of everything in arg_list.
   result = arg_list[j - 1]
   for ( i = j - 2; i >= 0; i-- )
      result = sprintf("%s %s", result, arg_list[i]);

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl This defines a handy little macro that will "clean up" linker arguments.
dnl The basic process is simple: move all path arguments (-L, /libpath:) to
dnl the beginning and move all library arguments (-l, *.lib, -framework <X>,
dnl -pthread[s]) to the end.  Order is preserved, but duplicate arguments are
dnl removed.
dnl
dnl Usage:
dnl     VJ_CLEAN_LINKRE_ARGS(variable, arg-list)
dnl
dnl Arguments:
dnl     variable - The name of the variable that will contain the resulting
dnl                clean list of arguments.  This should NOT begin with a $
dnl                (dollar sign) character.
dnl     arg-list - The list of linker arguments that will be "cleansed."
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_CLEAN_LINKER_ARGS],
[
   changequote(<<, >>)

   $1=<<`echo $2 | awk '{
   j = 0
   k = 0

   # Go through the list in forward order and extract the linker path
   # arguments.  We only want those that are not duplicated.
   for ( i = 1; i <= NF; i++ )
   {
      if ( $i ~ /^-L/ || $i ~ /[-\/]libpath:/ )
      {
         if ( ! valid_path_list[$i] )
         {
            valid_path_list[$i] = 1
            path_list[k] = $i
            k++
         }
      }
   }

   # Go through the list in reverse order and extract the library
   # arguments.  We only want those that are not duplicated.
   for ( i = NF; i >= 1; i-- )
   {
      if ( $i ~ /^-l/ || $i ~ /\.[Ll][Ii][Bb]$/ || $i ~ /^-pthread/ )
      {
         if ( ! valid_list[$i] )
         {
            valid_list[$i] = 1
            arg_list[j] = $i
            j++
         }
      }
      # Ignore linker path arguments.  Those are already stored in path_list
      # from above.
      else if ( $i !~ /^-L/ && $i !~ /[-\/]libpath:/ )
      {
         arg_list[j] = $i
         j++
      }
   }

   # Put the path arguments at the beginning of result.
   if ( k > 0 )
   {
      result = path_list[0]
      for ( i = 1; i < k; i++ )
         result = sprintf("%s %s", result, path_list[i])
   }

   # Now, build result as a space-separated string of everything in arg_list.
   if ( j > 0 )
   {
      result = sprintf("%s %s", result, arg_list[j - 1])
      for ( i = j - 2; i >= 0; i-- )
         result = sprintf("%s %s", result, arg_list[i])
   }

   # This gives the final argument string that will be assigned to the sh
   # variable.
   print result
}' -`>>

   changequote([, ])
])

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_MTREE_GEN(file-prefix, path, platform [, ISA])
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_MTREE_LIB_GEN],
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
            profiled
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
    ..
    lib64
        mips3
            debug
            ..
            opt
            ..
            profiled
            ..
        ..
        mips4
            debug
            ..
            opt
            ..
            profiled
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
    lib$LIBBITSUF
        $4
            debug
            ..
            opt
            ..
            profiled
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
        profiled
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
dnl     * APP_EXTRA_CFLAGS: Extra flags for the compiler.
dnl
dnl Usage
dnl     VJ_APP_COMPILER(C-compiler, C-flags, C++-compiler, C++-flags, debug-flags, optimization-flags, base-dir, defs, includes, extra-flags)
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_APP_COMPILER],
[
    if test "$OS_TYPE" = "UNIX" ; then
        APP_CC="$1"
        APP_CXX="$3"
    elif test "$OS_TYPE" = "Win32" ; then
        if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
        then
            APP_CC='cl'
            APP_CXX='cl'
        else
            APP_CC="$1"
            APP_CXX="$3"
        fi
    fi

    APP_CXX_PLUGIN="$CXX_PLUGIN"
    APP_CFLAGS="$2"
    APP_CXXFLAGS="$4"
    APP_DEBUG_CFLAGS="$5 -D_DEBUG"
    APP_OPTIM_CFLAGS="$6 -D_OPT -DNDEBUG"
    APP_DEFS="$8"
    APP_INCLUDES=ifelse([$7], , "$9", "-I\$($7)/include $9")
    APP_EXTRA_CFLAGS="$10"

    AC_SUBST(APP_CC)
    AC_SUBST(APP_CFLAGS)
    AC_SUBST(APP_CXX)
    AC_SUBST(APP_CXXFLAGS)
    AC_SUBST(APP_CXX_PLUGIN)
    AC_SUBST(APP_DEBUG_CFLAGS)
    AC_SUBST(APP_OPTIM_CFLAGS)
    AC_SUBST(APP_DEFS)
    AC_SUBST(APP_INCLUDES)
    AC_SUBST(APP_EXTRA_CFLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Variables set in Makefiles found in the application trees.  All variables
dnl listed are for use only in the Makefile.in's found in that tree unless
dnl otherwise noted.  They are used in the following way:
dnl
dnl     * APP_LINK: The command that will be used to link applications.
dnl     * APP_LINK_FLAGS: Flags passed to the linker.
dnl     * APP_DEBUG_LDFLAGS: Debugging linker flags.
dnl     * APP_OPTIM_LDFLAGS: Optimizing linker flags.
dnl     * PROJ_BASE_DIR_ENV: The name of the environment variable that deifnes
dnl       the base installation directory.
dnl     * BASIC_LIBS: The platform-indepenedent names of the basic libraries
dnl       that must be linked (for example, "GL X11 m").
dnl     * APP_EXTRA_LIBS: A basic set of extra libraries and linker flags
dnl       needed for compiling.  These are put after those listed in the
dnl       previous variables in the compile line.
dnl
dnl Usage
dnl     VJ_APP_LINKER(linker, link-flags, debug-flags, optimization-flags, base-dir-name, basic-libs, extra-libs)
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_APP_LINKER],
[
    AC_REQUIRE([DPP_PROG_LINKER])

    if test "x$OS_TYPE" = "xUNIX" ; then
        APP_LINK="$1"
        APP_LINK_FLAGS="$2"

        for lib in $6 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS -l$lib"
        done

        APP_EXTRA_LIBS="$7"

        dnl IRIX without GNU linker.
        if test "x$PLATFORM" = "xIRIX" -a "x$GNU_LD" != "xyes" ; then
            APP_LINKALL_ON='-all'
            APP_LINKALL_OFF=''

            APP_BASIC_LIBS_BEGIN="-B static \$(LINKALL_ON) -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END='$(LINKALL_OFF) -B dynamic'
            APP_BASIC_LIBS_BEGIN_INST="-B dynamic -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($5)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-B dynamic'
            APP_EXTRA_LIBS_END=''
        dnl UNIX-based system (but not Darwin) using GNU linker.
        elif test "x$GNU_LD" = "xyes" -a "x$PLATFORM" != "xDarwin" ; then
            if test "x$PLATFORM" = "xIRIX" ; then
                APP_LINKALL_ON='-Wl,-all'
                APP_LINKALL_OFF=''
            else
                APP_LINKALL_ON='-Wl,--whole-archive'
                APP_LINKALL_OFF='-Wl,--no-whole-archive'
            fi

            APP_BASIC_LIBS_BEGIN="-Wl,-Bstatic \$(LINKALL_ON) -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END="\$(LINKALL_OFF) -Wl,-Bdynamic"
            APP_BASIC_LIBS_BEGIN_INST="-Wl,-Bdynamic -L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_END_INST="-Wl,-rpath,\$($5)/lib$LIBBITSUF"

            APP_EXTRA_LIBS_BEGIN='-Wl,-Bdynamic'
            APP_EXTRA_LIBS_END=''
        else
            # For now, we are disabling static linking for the sample
            # applications when compiled in a developer's build tree.  This
            # is only the case on non-IRIX platforms and non-GNU linkers.
            APP_BASIC_LIBS_BEGIN="-L\$($5)/lib$LIBBITSUF"
            APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"
        fi
    elif test "$OS_TYPE" = "Win32" ; then
        APP_LINKALL_ON=''
        APP_LINKALL_OFF=''

        if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
        then
            APP_LINK='link'
        else
            APP_lINK="$1"
        fi

        APP_LINK_FLAGS="$2"

        for lib in $6 ; do
            APP_BASIC_LIBS="$APP_BASIC_LIBS ${lib}.lib"
        done

        APP_BASIC_LIBS_BEGIN="-libpath:\$($5)/lib/debug"
        APP_BASIC_LIBS_BEGIN_INST="$APP_BASIC_LIBS_BEGIN"

        APP_EXTRA_LIBS="$7"
    fi

    APP_DEBUG_LFLAGS="$3"
    APP_OPTIM_LFLAGS="$4"

    AC_SUBST(APP_LINK)
    AC_SUBST(APP_DEBUG_LFLAGS)
    AC_SUBST(APP_OPTIM_LFLAGS)
    AC_SUBST(APP_EXTRA_LFLAGS)
    AC_SUBST(APP_LINK_FLAGS)
    AC_SUBST(APP_LINKALL_ON)
    AC_SUBST(APP_LINKALL_OFF)
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

dnl ---------------------------------------------------------------------------
dnl Set up variables for substituation in a makefile that will be used in
dnl conjunction with Doozer (not Doozer++).
dnl
dnl Usage:
dnl    VJ_APP_LINKER_DOOZER(linker, link-flags, debug-flags, optimization-flags, basic librar(y|ies), extra-libraries)
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_APP_LINKER_DOOZER],
[
   AC_REQUIRE([DPP_PROG_LINKER])

   APP_BASIC_LIBS_BEGIN='$(STATIC_ON) $(LINKALL_ON)'
   APP_BASIC_LIBS=$5
   APP_BASIC_LIBS_END='$(LINKALL_OFF) $(STATIC_OFF)'

   dnl Values used for installation. 
   APP_BASIC_LIBS_BEGIN_INST=''
   APP_BASIC_LIBS_END_INST=''

   APP_EXTRA_LIBS=$6

   if test "x$OS_TYPE" = "xUNIX" ; then
      APP_LINK=$1
      APP_LINK_FLAGS=$2

      dnl IRIX without GNU linker.
      if test "x$PLATFORM" = "xIRIX" -a "x$GNU_LD" != "xyes" ; then
         APP_LINKALL_ON='-all'
         APP_LINKALL_OFF=''

         APP_LINK_STATIC_ON='-B static'
         APP_LINK_STATIC_OFF='-B dynamic'

         APP_EXTRA_LIBS_BEGIN='-B dynamic'
         APP_EXTRA_LIBS_END=''
      dnl UNIX-based system (but not Darwin) using GNU linker.
      elif test "x$GNU_LD" = "xyes" -a "x$PLATFORM" != "xDarwin" ; then
         if test "x$PLATFORM" = "xIRIX" ; then
            APP_LINKALL_ON='-Wl,-all'
            APP_LINKALL_OFF=''
         else
            APP_LINKALL_ON='-Wl,--whole-archive'
            APP_LINKALL_OFF='-Wl,--no-whole-archive'
         fi

         APP_LINK_STATIC_ON='-Wl,-Bstatic'
         APP_LINK_STATIC_OFF='-Wl,-Bdynamic'

         APP_EXTRA_LIBS_BEGIN='-Wl,-Bdynamic'
         APP_EXTRA_LIBS_END=''
      fi
   elif test "$OS_TYPE" = "Win32" ; then
      APP_LINKALL_ON=''
      APP_LINKALL_OFF=''

      if test "x$DPP_USING_CYGCL" = "xyes" -o "x$DPP_USING_MSVCCC" = "xyes"
      then
         APP_LINK='link'
      else
         APP_lINK=$1
      fi

      APP_LINK_FLAGS=$2
   fi

   APP_DEBUG_LFLAGS=$3
   APP_OPTIM_LFLAGS=$4

   AC_SUBST(APP_LINK)
   AC_SUBST(APP_DEBUG_LFLAGS)
   AC_SUBST(APP_OPTIM_LFLAGS)
   AC_SUBST(APP_EXTRA_LFLAGS)
   AC_SUBST(APP_LINK_FLAGS)
   AC_SUBST(APP_LINKALL_ON)
   AC_SUBST(APP_LINKALL_OFF)
   AC_SUBST(APP_LINK_STATIC_ON)
   AC_SUBST(APP_LINK_STATIC_OFF)
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

dnl ---------------------------------------------------------------------------
dnl Usage:
dnl     VJ_VERSION_GROK(version-file)
dnl ---------------------------------------------------------------------------
AC_DEFUN([VJ_VERSION_GROK],
[
    version_file="$srcdir/$1"

    MAJOR_VERSION=0
    MINOR_VERSION=0
    MICRO_VERSION=0

    if test -r "$version_file" ; then
        ver_str=`head -1 "$version_file"`
        ver_num=`echo "$ver_str" | sed -e 's/^\(.*\) @.*@$/\1/'`
        ver_str_sep=`echo "$ver_num" | sed -e 's/\./ /g'`
        MAJOR_VERSION=`echo "$ver_str_sep" | sed -e 's/^\(..*\) ..* ..*$/\1/'`
        MINOR_VERSION=`echo "$ver_str_sep" | sed -e 's/^..* \(..*\) ..*$/\1/'`
        MICRO_VERSION=`echo "$ver_str_sep" | sed -e 's/^..* ..* \(..*\)-..*$/\1/'`
    fi

    AC_SUBST(MAJOR_VERSION)
    AC_SUBST(MINOR_VERSION)
    AC_SUBST(MICRO_VERSION)
])
