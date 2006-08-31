dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
dnl -----------------------------------------------------------------
dnl File:          compiler.m4,v
dnl Date modified: 2006/07/10 11:40:05
dnl Version:       1.46
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Determine which C and C++ compilers to use for compiling.  The
dnl capabilities of the C++ compiler are tested here as well to ensure that
dnl the proper language support is available.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_COMPILER_FLAG       - Check if the given compiler accepts a given
dnl                               flag.
dnl     DPP_SETUP_COMPILER      - Setup software development environment for
dnl                               building C and C++ code.
dnl     DPP_PROG_CC             - Check for the existence and usability of a C
dnl                               compiler.
dnl     DPP_PROG_CC_NOSTDINC    - Check if the C compiler supports clearing
dnl                               the default include path.
dnl     DPP_PROG_CC_ANSI        - Check if the C compiler supports -ansi.
dnl     DPP_PROG_CC_PIPE        - Check if the C compiler supports -pipe.
dnl     DPP_PROG_CC_PROF_P      - Check if the C compiler supports -p.
dnl     DPP_PROG_CC_PROF_PG     - Check if the C compiler supports -p.
dnl     DPP_GET_EXT             - Get the extensions for object files and
dnl                               executables.
dnl     DPP_PROG_CXX            - Check for the existence and usability of a
dnl                               C++ compiler.
dnl     DPP_PROG_CXX_NOSTDINCXX - Check if the C++ compiler supports clearing
dnl                               the default C++ include path.
dnl     DPP_PROG_CXX_ANSI       - Check if the C++ compiler supports -ansi.
dnl     DPP_PROG_CXX_PIPE       - Check if the C++ compiler supports -pipe.
dnl     DPP_PROG_CXX_PROF_P     - Check if the C++ compiler supports -p.
dnl     DPP_PROG_CXX_PROF_PG    - Check if the C++ compiler supports -pg.
dnl
dnl Variables defined:
dnl     AR                   - The archiver program (ar(1) for example).
dnl     ARFLAGS              - Basic flags passed to $AR.
dnl     AR_NAME_FLAG         - The flag passed to $AR to specify the name of
dnl                            the archive being generated.
dnl     CC                   - The C compiler.
dnl     CC_NOSTDINC_FLAGS    - The flag(s) that will clear the default include
dnl                            path for the C compiler (if any).
dnl     CC_ANSI_FLAGS        - The flag(s) to require ANSI C compliance.
dnl     CC_PIPE_FLAGS        - The flag(s) to use pipes instead of temporary
dnl                            files when compiling.
dnl     CXX_NOSTDINCXX_FLAGS - The flag(s) that will clear the default include
dnl                            path for the C++ compiler (if any).
dnl     CFLAGS               - All options passed to the C compiler.
dnl     CFLAGS_DYNLIB        - Position-independent code generation flag for
dnl                            the C compiler.  Substituted automatically.
dnl     CPP                  - The C compiler's preprocessor.
dnl     CXX                  - The C++ compiler.
dnl     CXX_ANSI_FLAGS       - The flag(s) to require ANSI C++ compliance.
dnl     CXX_PIPE_FLAGS       - The flag(s) to use pipes instead of temporary
dnl                            files when compiling.
dnl     CXXFLAGS             - All options passed to the C++ compiler.
dnl     CXXFLAGS_DYNLIB      - Position-independent code generation flag for
dnl                            the C++ compiler.  Substituted automatically.
dnl     CXXCPP               - The C++ compiler's preprocessor.
dnl     C_DLL                - The command to generate a DLL (shared library)
dnl                            from C object files.
dnl     CXX_DLL              - The command to generate a DLL (shared library)
dnl                            from C++ object files.
dnl     C_PLUGIN             - The command to generate a C plug-in (code loaded
dnl                            at runtime, usually with dlopen(3) or equivalent
dnl                            system call).
dnl     CXX_PLUGIN           - The command to generate a C++ plug-in (code
dnl                            loaded at runtime, usually with dlopen(3) or
dnl                            equivalent system call).
dnl     DEP_GEN_FLAG         - Compiler flag for generating source file
dnl                            dependencies.
dnl     DEPEND_FLAGS         - Flags for building dependencies (typically with
dnl                            makedepend(1)).
dnl     DEPEND_EXTRAS        - Miscellaneous flags that the dependency
dnl                            generator may or may not recognize.  If it does
dnl                            not recognize them, they will be ignored.
dnl     DYLIB_NAME_FLAG      - Flag used for naming a compiled dynamic library.
dnl     EXE_NAME_FLAG        - Flag used for naming a compiled executable file.
dnl     INCLUDES             - Extensions to the compiler's default include
dnl                            path.
dnl     LD                   - The linker program (ld(1) for example).
dnl     LIBS                 - Libraries needed for linking.
dnl     LDOPTS               - Basic options for $LD.
dnl     LDOPTS_DBG           - Options for $LD when making a library with
dnl                            debugging symbols.
dnl     LDOPTS_OPT           - Options for $LD when making an optimized
dnl                            library.
dnl     LDOPTS_PROF          - Options for $LD when making a profiled library.
dnl     OBJ_NAME_FLAG        - Flag used for naming a compiled object file.
dnl     OBJ_BUILD_FLAG       - Flag passed to compiler to build a compiled
dnl                            object file.
dnl
dnl     DBG_FLAGS            - Complier flags for adding debugging symbols.
dnl     OPT_FLAGS            - Complier flags for compiling optimized code.
dnl
dnl     LDFLAGS_STATIC       - Linker flags that invoke static linking.
dnl     LDFLAGS_DYNAMIC      - Linker flags that invoke dynamic linking.
dnl
dnl     LIB_PREFIX           - Prefix for library names.
dnl     STATICLIB_EXT        - Filename extension for static library names.
dnl     DYNAMICLIB_EXT       - Filename extension for dynamic library names.
dnl
dnl     DSOREGFILE
dnl     DSOVERSION
dnl     DSOVERSIONOPTS
dnl
dnl Possible preprocssor symbols defined:
dnl     WIN32
dnl     _MBCS
dnl ===========================================================================

dnl compiler.m4,v 1.46 2006/07/10 11:40:05 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Check if the given compiler accepts a given flag.  This can be used for
dnl all compiler flags except -c.
dnl
dnl Usage:
dnl     DPP_COMPILER_FLAG(compiler, flag, cache-var)
dnl
dnl Arguments:
dnl     compiler  - The compiler to use for the check.
dnl     flag      - The flag to check.
dnl     cache-var - The cache variable used to store the result of the test.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_COMPILER_FLAG],
[
   AC_CACHE_CHECK([whether $1 accepts $2], $3,
[echo 'void f(){;}' > conftest.$ac_ext
if test -z "`$1 $2 -c conftest.$ac_ext 2>&1`"; then
  $3='yes'
else
  $3='no'
fi
rm -f conftest*
])
])

dnl ---------------------------------------------------------------------------
dnl Based on the given detected host and CPU, set up the system-specific
dnl configuration for commonly used software developer tools.
dnl
dnl Usage:
dnl     DPP_SETUP_COMPILER
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_SETUP_COMPILER],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])

   dnl Archive library command (generally ar(1)) and associated flags.
   dnl $AR_NAME_FLAG is an extra (optional) argument that allows $AR to
   dnl name its output file.
   AR=''
   ARFLAGS=''
   AR_NAME_FLAG=''

   dnl Flag for the compiler to generate dependencies
   DEP_GEN_FLAG='-M'

   dnl Shared library command (usually ld(1)) and associated flags.
   LD=''
   LDOPTS=''
   LDOPTS_DBG=''
   LDOPTS_OPT=''
   LDOPTS_PROF=''
   C_DLL=''
   CXX_DLL=''
   C_PLUGIN=''
   CXX_PLUGIN=''
   OBJC_DLL=''
   OBJCXX_DLL=''
   OBJC_PLUGIN=''
   OBJCXX_PLUGIN=''

   dnl Flag used for naming a compiled object file.
   OBJ_NAME_FLAG='-o $[@]'
   OBJ_BUILD_FLAG='-c'
   EXE_NAME_FLAG='-o $[@]'
   DYLIB_NAME_FLAG="$EXE_NAME_FLAG"

   dnl Library naming conventions (prefix, extension for static libraries,
   dnl extension for dynamic libraries respectively).
   LIB_PREFIX='lib'
   STATICLIB_EXT='a'
   DYNAMICLIB_EXT=''

   dnl Compiler object file debugging flags and optimization flags
   dnl respectively.
   DBG_FLAGS=''
   OPT_FLAGS=''

   dnl Initialize these here just to be safe.  Subsequent code will fill in
   dnl the details.
   C_WARNS_LEVEL_0=''
   C_WARNS_LEVEL_1=''
   C_WARNS_LEVEL_2=''
   C_WARNS_LEVEL_3=''
   C_WARNS_LEVEL_4=''
   C_WARNS_LEVEL_5=''
   CXX_WARNS_LEVEL_0=''
   CXX_WARNS_LEVEL_1=''
   CXX_WARNS_LEVEL_2=''
   CXX_WARNS_LEVEL_3=''
   CXX_WARNS_LEVEL_4=''
   CXX_WARNS_LEVEL_5=''
   OBJC_WARNS_LEVEL_0=''
   OBJC_WARNS_LEVEL_1=''
   OBJC_WARNS_LEVEL_2=''
   OBJC_WARNS_LEVEL_3=''
   OBJC_WARNS_LEVEL_4=''
   OBJC_WARNS_LEVEL_5=''
   OBJCXX_WARNS_LEVEL_0=''
   OBJCXX_WARNS_LEVEL_1=''
   OBJCXX_WARNS_LEVEL_2=''
   OBJCXX_WARNS_LEVEL_3=''
   OBJCXX_WARNS_LEVEL_4=''
   OBJCXX_WARNS_LEVEL_5=''

   dnl IRIX junk.
   DSOREGFILE=''
   DSOVERSION=''
   DSOVERSIONOPTS=''

   USE_MAKEDEPEND='N'

   dpp_opt_level=${OPT_LEVEL-2}

   dnl The OS for which this is being configured.  The operating systems are
   dnl listed alphabetically.
   case $target_os in
      dnl IBM RS/6000 running AIX.
      aix*)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
      cygwin)
         if test "x$ICC" = "xyes" ; then
            DPP_SETUP_ICC($dpp_opt_level)
         elif test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)
         else
            AR='link /lib'
            ARFLAGS="$ARFLAGS /nologo"
            AR_NAME_FLAG='/out:'
            CFLAGS="$CFLAGS /nologo"
            CXXFLAGS="$CXXFLAGS /nologo"
            DBG_FLAGS='/Od /GZ /Z7'
            DEP_GEN_FLAG='/FD'
            OPT_FLAGS="/Ogityb$dpp_opt_level /GB"
            LD='link /dll'
            LDOPTS="$LDOPTS /nologo"
            LDOPTS_DBG='/DEBUG'
            LDOPTS_OPT='/RELEASE'
            C_DLL='link /dll'
            CXX_DLL='link /dll'
            C_PLUGIN='link /dll'
            CXX_PLUGIN='link /dll'
            OBJ_NAME_FLAG='/Fo$[@]'
            OBJ_BUILD_FLAG='/c'
            EXE_NAME_FLAG='/OUT:$[@]'
            DYLIB_NAME_FLAG="$EXE_NAME_FLAG"

            USE_MAKEDEPEND='Y'
         fi

         DEPEND_EXTRAS="-D_WIN32 -D_M_IX86 $DEPEND_EXTRAS"
         LIB_PREFIX=''
         STATICLIB_EXT='lib'
         DYNAMICLIB_EXT='dll'

         AC_DEFINE(WIN32, , [define if compiling on a Win32 platform])
         AC_DEFINE(_MBCS, , [define if compiling on a Win32 platform])
         ;;
      dnl A machine running Mac OS X.
      darwin*)
         AR='libtool'
         ARFLAGS='-static'
         AR_NAME_FLAG='-o '
         LD='$(CXX) -dynamiclib'
         LDOPTS=''
         C_DLL='$(CC) -dynamiclib'
         CXX_DLL='$(CXX) -dynamiclib'
         C_PLUGIN='$(CC) -bundle'
         CXX_PLUGIN='$(CXX) -bundle'
         OBJC_DLL='$(OBJC) -dynamiclib'
         OBJCXX_DLL="$CXX_DLL"
         OBJC_PLUGIN='$(OBJC) -bundle'
         OBJCXX_PLUGIN="$CXX_PLUGIN"
         OPT_FLAGS="-O$dpp_opt_level"
         DBG_FLAGS='-g'
         DYNAMICLIB_EXT='dylib'
         CFLAGS="-fno-common $CFLAGS"
         CXXFLAGS="-fno-common $CXXFLAGS"
         OBJCFLAGS="-fno-common $OBJCFLAGS"
         OBJCXXFLAGS="-fno-common $OBJCXXFLAGS"
         ;;
      dnl A machine running FreeBSD.
      freebsd*)
         if test "x$ICC" = "xyes" ; then
            DPP_SETUP_ICC($dpp_opt_level)
         else
            DPP_SETUP_GCC($dpp_opt_level)
         fi
         DYNAMICLIB_EXT='so'
         ;;
      dnl HP PA-RISC machine running HP-UX 10.20.
      hpux10.20)
         DPP_SETUP_GCC($dpp_opt_level)

         if test "x$USE_GCC" != "xyes" ; then
            CFLAGS="-Aa $CFLAGS"
            CXXFLAGS="+p $CXXFLAGS"
         fi

         LD='ld'
         C_DLL='ld'
         CXX_DLL='ld'
         C_PLUGIN='ld'
         CXX_PLUGIN='ld'
         LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
         LDOPTS='-b'
         OPT_FLAGS='+inline_level 3'
         DYNAMICLIB_EXT='sl'
         ;;
      dnl HP PA-RISC machine running HP-UX 11.x.
      hpux11*)
         DPP_SETUP_GCC($dpp_opt_level)

         if test "x$USE_GCC" != "xyes" ; then
            CFLAGS="-Aa $CFLAGS"
            CXXFLAGS="-AA $CXXFLAGS"
            DEP_GEN_FLAG='-E +m'
            LD='$(CXX)'
            C_DLL='$(CC)'
            CXX_DLL='$(CXX)'
            C_PLUGIN='$(CC)'
            CXX_PLUGIN='$(CXX)'
            LDOPTS='-b'
         else
            LD='ld'
            C_DLL='ld'
            CXX_DLL='ld'
            C_PLUGIN='ld'
            CXX_PLUGIN='ld'
         fi

         LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
         OPT_FLAGS=''
         DYNAMICLIB_EXT='sl'
         ;;
      dnl A machine running IRIX 6.x.
      irix6*)
         if test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)

            dpp_cc_defs_file='/etc/compiler.defaults'

            if test -r "$dpp_cc_defs_file" ; then
               dpp_cc_defs=`cat $dpp_cc_defs_file`
               dpp_abi_val=`echo $dpp_cc_defs | sed -e 's/.*abi=\([[a-z0-9]]\{2,3\}\).*/\1/'`
               dpp_isa_val=`echo $dpp_cc_defs | sed -e 's/.*isa=\(mips[[0-9]]\).*/\1/'`
 
               dpp_abi_isa_name=''

               if test "x$dpp_abi_val" = "xn32" ; then
                  dpp_abi_isa_name='N32_'
                  dpp_abi_val='N32'
                  dpp_bit_suffix='32'
               elif test "x$dpp_abi_val" = "x64" ; then
                  dpp_abi_isa_name='64_'
                  dpp_bit_suffix='64'
               else
                  AC_MSG_WARN([*** Unknown ABI type $dpp_abi_val ***])
               fi

               if test "x$dpp_isa_val" = "xmips3" ; then
                  dpp_abi_isa_name="${dpp_abi_isa_name}M3"
               elif test "x$dpp_isa_val" = "xmips4" ; then
                  dpp_abi_isa_name="${dpp_abi_isa_name}M4"
               else
                  AC_MSG_WARN([*** Unknown ISA type $dpp_isa_val ***])
               fi

               DPP_ABI_CFG([$dpp_abi_val], [$dpp_isa_val], [$dpp_bit_suffix])
               ABI_LIST="$dpp_abi_isa_name"
            else
               AC_MSG_WARN([*** Defaulting ABI list to N32_M3 ***])
               ABI_LIST='N32_M3'
            fi
         else
            STDFLAGS=''

            if test "x$dpp_std" = "xyes" ; then
               STDFLAGS='-LANG:std'
            fi

            dnl Set compiler flags and the location of $DSOREGFILE
            dnl depending on $ABI.
            if test "x$ABI" = "x64" ; then
               DSOREGFILE='/usr/lib64/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -OPT:Olimit=0"
            elif test "x$ABI" = "xN32" ; then
               DSOREGFILE='/usr/lib32/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -OPT:Olimit=0"
            elif test "x$ABI" = "xO32" ; then
               DSOREGFILE='/usr/lib/so_locations'
               OPT_FLAGS="-O$dpp_opt_level -Olimit 0"
            fi

            AR='$(CXX)'
            ARFLAGS="$STDFLAGS -J6 -ar -WR,-v -o"

            CXXFLAGS="$CXXFLAGS $STDFLAGS"

            DBG_FLAGS="-g"
dnl            _LD_REGOPTS='-check_registry $(DSOREGFILE)'
            LDOPTS="$LDOPTS $STDFLAGS -all ${_LD_REGOPTS} -elf -rdata_shared"
            LDFLAGS_STATIC='-B static'
            LDFLAGS_DYNAMIC='-B dynamic'


            DSOVERSION='sgi4.0'
            DSOVERSIONOPTS='-set_version $(DSOVERSION)'

            ABI_LIST='64_M4 64_M3 N32_M4 N32_M3'
         fi

         LD='$(CXX) -shared'
         C_DLL='$(CC) -shared'
         CXX_DLL='$(CXX) -shared'
         C_PLUGIN='$(CC) -shared'
         CXX_PLUGIN='$(CXX) -shared'
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running Linux.
      linux*)
         if test "x$ICC" = "xyes" ; then
            DPP_SETUP_ICC($dpp_opt_level)
         else
            DPP_SETUP_GCC($dpp_opt_level)
         fi
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running NetBSD.
      netbsd*)
         if test "x$ICC" = "xyes" ; then
            DPP_SETUP_ICC($dpp_opt_level)
         else
            DPP_SETUP_GCC($dpp_opt_level)
         fi
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running OpenBSD.
      openbsd*)
         if test "x$ICC" = "xyes" ; then
            DPP_SETUP_ICC($dpp_opt_level)
         else
            DPP_SETUP_GCC($dpp_opt_level)
         fi
         DYNAMICLIB_EXT='so'
         ;;
      dnl DEC Alpha running Digital UNIX 4.0.
      osf4.0)
         DPP_SETUP_GCC($dpp_opt_level)
         DYNAMICLIB_EXT='so'
         ;;
      dnl A machine running Solaris (aka SunOS).
      solaris*)
         dnl GCC.
         if test "x$USE_GCC" = "xyes" ; then
            DPP_SETUP_GCC($dpp_opt_level)
         dnl Sun WorkShop.
         else
            AR='ar'
            ARFLAGS='-ruv'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DBG_FLAGS='-g'
            DEP_GEN_FLAG='-xM'
         fi

         LD='$(CXX) -G'
         C_DLL='$(CC) -G'
         CXX_DLL='$(CXX) -G'
         C_PLUGIN='$(CC) -G'
         CXX_PLUGIN='$(CXX) -G'
         DYNAMICLIB_EXT='so'
         ;;
   esac

   AC_SUBST(AR)
   AC_SUBST(ARFLAGS)
   AC_SUBST(AR_NAME_FLAG)
   AC_SUBST(CFLAGS)
   AC_SUBST(CXXFLAGS)
   AC_SUBST(C_DLL)
   AC_SUBST(CXX_DLL)
   AC_SUBST(C_PLUGIN)
   AC_SUBST(CXX_PLUGIN)
   AC_SUBST(DEP_GEN_FLAG)
   AC_SUBST(DEPEND_FLAGS)
   AC_SUBST(DEPEND_EXTRAS)
   AC_SUBST(INCLUDES)
   AC_SUBST(LD)
   AC_SUBST(LIBS)
   AC_SUBST(LDOPTS)
   AC_SUBST(LDOPTS_DBG)
   AC_SUBST(LDOPTS_OPT)
   AC_SUBST(LDOPTS_PROF)
   AC_SUBST(OBJCFLAGS)
   AC_SUBST(OBJCXXFLAGS)
   AC_SUBST(OBJC_DLL)
   AC_SUBST(OBJCXX_DLL)
   AC_SUBST(OBJC_PLUGIN)
   AC_SUBST(OBJCXX_PLUGIN)
   AC_SUBST(OBJ_NAME_FLAG)
   AC_SUBST(OBJ_BUILD_FLAG)
   AC_SUBST(EXE_NAME_FLAG)
   AC_SUBST(DYLIB_NAME_FLAG)

   AC_SUBST(DBG_FLAGS)
   AC_SUBST(OPT_FLAGS)

   AC_SUBST(LDFLAGS_STATIC)
   AC_SUBST(LDFLAGS_DYNAMIC)

   AC_SUBST(LIB_PREFIX)
   AC_SUBST(STATICLIB_EXT)
   AC_SUBST(DYNAMICLIB_EXT)

   AC_SUBST(C_WARNS_LEVEL_0)
   AC_SUBST(C_WARNS_LEVEL_1)
   AC_SUBST(C_WARNS_LEVEL_2)
   AC_SUBST(C_WARNS_LEVEL_3)
   AC_SUBST(C_WARNS_LEVEL_4)
   AC_SUBST(C_WARNS_LEVEL_5)
   AC_SUBST(CXX_WARNS_LEVEL_0)
   AC_SUBST(CXX_WARNS_LEVEL_1)
   AC_SUBST(CXX_WARNS_LEVEL_2)
   AC_SUBST(CXX_WARNS_LEVEL_3)
   AC_SUBST(CXX_WARNS_LEVEL_4)
   AC_SUBST(CXX_WARNS_LEVEL_5)
   AC_SUBST(OBJC_WARNS_LEVEL_0)
   AC_SUBST(OBJC_WARNS_LEVEL_1)
   AC_SUBST(OBJC_WARNS_LEVEL_2)
   AC_SUBST(OBJC_WARNS_LEVEL_3)
   AC_SUBST(OBJC_WARNS_LEVEL_4)
   AC_SUBST(OBJC_WARNS_LEVEL_5)
   AC_SUBST(OBJCXX_WARNS_LEVEL_0)
   AC_SUBST(OBJCXX_WARNS_LEVEL_1)
   AC_SUBST(OBJCXX_WARNS_LEVEL_2)
   AC_SUBST(OBJCXX_WARNS_LEVEL_3)
   AC_SUBST(OBJCXX_WARNS_LEVEL_4)
   AC_SUBST(OBJCXX_WARNS_LEVEL_5)

   AC_SUBST(DSOREGFILE)
   AC_SUBST(DSOVERSION)
   AC_SUBST(DSOVERSIONOPTS)

   AC_SUBST(USE_MAKEDEPEND)
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
dnl     DPP_PROG_CC([C-compiler [, extra-flags [, action-if-not-found [, path ]]]])
dnl
dnl Arguments:
dnl     C-compiler          - Force the check to look for this C compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C compiler cannot be
dnl                           found or does not work.  This is optional.
dnl     path                - Extra path information for finding the C
dnl                           compiler.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC],
[
   dpp_save_CFLAGS="$CFLAGS"
   CFLAGS="$CFLAGS $2 $ABI_FLAGS"

   dnl Get the platform-specific compiler hint value.
   dpp_cc_var="CC_$dpp_platform"
   dpp_cc_val=`eval echo '$''{'$dpp_cc_var'}'`

   if test "x$dpp_cc_val" != "x" -a "x$CC" = "x" ; then
      CC="$dpp_cc_val"
   fi

   if test "x$1" != "x" -o "x$CC" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_CC_found_val=$1
         AC_CHECK_PROG([dpp_CC_check_val], $1, $1, , $4)
      else
         dpp_CC_found_val="$CC"
         AC_CHECK_PROG([dpp_CC_check_val], [$CC], [$CC], , $4)
      fi

      if test "x$dpp_CC_check_val" = "x$dpp_CC_found_val" ; then
         CC="$dpp_CC_found_val"
      else
         dpp_CC_found='no'

         dnl If the caller provided a list of fallbacks in the $CC_FALLBACKS
         dnl variable, loop over them and try to use another C compiler.
         if test "x$CC_FALLBACKS" != "x" ; then
            for dpp_cc in $CC_FALLBACKS ; do
               AC_CHECK_PROG([dpp_CC_check_val], [$dpp_cc], [$dpp_cc], , $4)
               if test "x$dpp_CC_check_val" = "x$dpp_cc" ; then
                  CC="$dpp_cc"
                  dpp_CC_found='yes'
                  break
               fi
            done
         fi

         dnl No viable C compiler was found.
         if test "x$dpp_CC_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_PROG_CC
   AC_PROG_CPP
   DPP_C_COMPILER_INTEL

   CFLAGS="$dpp_save_CFLAGS"

   if test "x$ICC" = "xyes" ; then
      CFLAGS_DYNLIB='-KPIC'
   dnl If GCC will be the C compiler, -fPIC is the position-independent code
   dnl generation option.
   elif test "x$GCC" = "xyes" ; then
      CFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_cc_pic_var="CC_PIC_$dpp_platform"
      CFLAGS_DYNLIB=`eval echo '$''{'$dpp_cc_pic_var'}'`
   fi

   CFLAGS_DYNLIB="$CFLAGS_DYNLIB -DPIC"

   AC_SUBST(CFLAGS_DYNLIB)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports clearing the default include path.  If
dnl -nostdinc is not supporte, try -I- as a fallback.  Depending on the
dnl results of the tests, $CC_NOSTDINC_FLAGS will be set to '-nostdinc',
dnl '-I-', or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_NOSTDINC
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC_NOSTDINC],
[
   DPP_COMPILER_FLAG([${CC-cc}], [-nostdinc], [dpp_cv_prog_cc_nostdinc])

   dnl If the compiler supports -nostdinc, use it to clear the include path.
   if test "x$dpp_cv_prog_cc_nostdinc" = "xyes" ; then
      dpp_cc_nostdinc='yes'
      CC_NOSTDINC_FLAGS='-nostdinc'
   dnl Otherwise, check if -I- will do the job.
   else
      dpp_cc_nostdinc='no'

      DPP_COMPILER_FLAG([${CC-cc}], [-I-], [dpp_cv_prog_cc_inc_clear])

      if test "x$dpp_cv_prog_cc_nostdinc" = "xyes" ; then
         dpp_cc_inc_clear='yes'
         CC_NOSTDINC_FLAGS='-I-'
      else
         CC_NOSTDINC_FLAGS=''
         dpp_cc_inc_clear='no'
      fi
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -ansi.  Based on the results of the test,
dnl $CC_ANSI_FLAGS will be set to '-ansi' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_ANSI
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC_ANSI],
[
   DPP_COMPILER_FLAG([${CC-cc}], [-ansi], [dpp_cv_prog_cc_ansi])

   if test "x$dpp_cv_prog_cc_ansi" = "xyes" ; then
      dpp_cc_ansi='yes'
      CC_ANSI_FLAGS='-ansi'
   else
      dpp_cc_ansi='no'
      CC_ANSI_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -pipe.  Based on the results of the test,
dnl $CC_PIPE_FLAGS will be set to '-pipe' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PIPE
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC_PIPE],
[
   DPP_COMPILER_FLAG([${CC-cc}], [-pipe], [dpp_cv_prog_cc_pipe])

   if test "x$dpp_cv_prog_cc_pipe" = "xyes" ; then
      dpp_cc_pipe='yes'
      CC_PIPE_FLAGS='-pipe'
   else
      dpp_cc_pipe='no'
      CC_PIPE_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -p.  Based on the results of the test,
dnl $CC_PROF_FLAGS will be set to '-p' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PROF_P
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC_PROF_P],
[
   DPP_COMPILER_FLAG([${CC-cc}], [-p], [dpp_cv_prog_cc_prof_p])

   if test "x$dpp_cv_prog_cc_prof_p" = "xyes" ; then
      dpp_cc_prof_p='yes'
      CC_PROF_FLAGS='-p'
   else
      dpp_cc_prof_p='no'
      CC_PROF_FLAGS=''
   fi

   AC_SUBST(CC_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C compiler supports -pg.  Based on the results of the test,
dnl $CC_PROF_FLAGS will be set to '-pg' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CC_PROF_PG
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CC_PROF_PG],
[
   DPP_COMPILER_FLAG([${CC-cc}], [-pg], [dpp_cv_prog_cc_prof_pg])

   if test "x$dpp_cv_prog_cc_prof_pg" = "xyes" ; then
      dpp_cc_prof_pg='yes'
      CC_PROF_FLAGS='-pg'
   else
      dpp_cc_prof_pg='no'
      CC_PROF_FLAGS=''
   fi

   AC_SUBST(CC_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Get the extensions for object files and executables.  The extension
dnl strings are stored in $OBJEXT and $EXEEXT.
dnl
dnl Usage:
dnl     DPP_GET_EXT
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_GET_EXT],
[
   AC_REQUIRE([DPP_PROG_CC])

   dnl Determine the suffix for object file names.
   AC_OBJEXT

   dnl Determine the suffix for executable file names.
   AC_EXEEXT
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
dnl     DPP_PROG_CXX([C++-compiler [, extra-flags [, action-if-not-found [, path]]]])
dnl
dnl Arguments:
dnl     C++-compiler        - Force the check to look for this C++ compiler.
dnl                           This is optional.
dnl     extra-flags         - Extra compiler flags to use when checking.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the C++ compiler cannot be
dnl                           found or does not work.  This is optional.
dnl     path                - Extra path information for finding the C++
dnl                           compiler.  This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX],
[
   dpp_save_CXXFLAGS="$CXXFLAGS"
   CXXFLAGS="$CXXFLAGS $2 $ABI_FLAGS"

   dnl Get the platform-specific compiler hint value.
   dpp_cxx_var="CXX_$dpp_platform"
   dpp_cxx_val=`eval echo '$''{'$dpp_cxx_var'}'`

   if test "x$dpp_cxx_val" != "x" -a "x$CXX" = "x" ; then
      CXX="$dpp_cxx_val"
   fi

   if test "x$1" != "x" -o "x$CXX" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_CXX_found_val=$1
         AC_CHECK_PROG([dpp_CXX_check_val], $1, $1, , $4)
      else
         dpp_CXX_found_val="$CXX"
         AC_CHECK_PROG([dpp_CXX_check_val], [$CXX], [$CXX], , $4)
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
               AC_CHECK_PROG([dpp_CXX_check_val], [$dpp_cxx], [$dpp_cxx], , $4)
               if test "x$dpp_CXX_check_val" = "x$dpp_cxx" ; then
                  CXX="$dpp_cxx"
                  dpp_CXX_found='yes'
                  break
               fi
            done
         fi

         dnl No viable C++ compiler was found.
         if test "x$dpp_CXX_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_PROG_CXX
   AC_PROG_CXXCPP
   DPP_CXX_COMPILER_INTEL

   CXXFLAGS="$dpp_save_CXXFLAGS"

   if test "x$ICPC" = "xyes" ; then
      CXXFLAGS_DYNLIB='-KPIC'
   dnl If GCC's g++ will be the C++ compiler, -fPIC is the
   dnl position-independent code generation option.
   elif test "x$GXX" = "xyes" ; then
      CXXFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_cxx_pic_var="CXX_PIC_$dpp_platform"
      CXXFLAGS_DYNLIB=`eval echo '$''{'$dpp_cxx_pic_var'}'`
   fi

   CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -DPIC"

   AC_SUBST(CXXFLAGS_DYNLIB)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports clearing the default C++ include path.
dnl Depending olnt he results of the test, $CXX_NOSTDINCXX_FLAGS will be set
dnl to '-nostdinc++' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_NOSTDINCXX
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX_NOSTDINCXX],
[
   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS
   DPP_COMPILER_FLAG([${CXX-c++}], [-nostdinc++], [dpp_cv_prog_cxx_nostdinc])
   DPP_LANG_RESTORE

   if test "x$dpp_cv_prog_cxx_nostdincxx" = "xyes" ; then
      dpp_cxx_nostdincxx='yes'
      CXX_NOSTDINCXX_FLAGS='-nostdinc++'
   else
      dpp_cxx_nostdincxx='no'
      CXX_NOSTDINCXX_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -ansi.  Based on the results of the
dnl test, $CXX_ANSI_FLAGS will be set to '-ansi' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_ANSI
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX_ANSI],
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-ansi], [dpp_cv_prog_cxx_ansi])

   if test "x$dpp_cv_prog_cxx_ansi" = "xyes" ; then
      dpp_cxx_ansi='yes'
      CXX_ANSI_FLAGS='-ansi'
   else
      dpp_cxx_ansi='no'
      CXX_ANSI_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -pipe.  Based on the results of the
dnl test, $CXX_PIPE_FLAGS will be set to '-pipe' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PIPE
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX_PIPE],
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-pipe], [dpp_cv_prog_cxx_pipe])

   if test "x$dpp_cv_prog_cxx_pipe" = "xyes" ; then
      dpp_cxx_pipe='yes'
      CXX_PIPE_FLAGS='-pipe'
   else
      dpp_cxx_pipe='no'
      CXX_PIPE_FLAGS=''
   fi
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -p.  Based on the results of the test,
dnl $CXX_PROF_FLAGS will be set to '-p' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PROF_P
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX_PROF_P],
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-p], [dpp_cv_prog_cxx_prof_p])

   if test "x$dpp_cv_prog_cxx_prof_p" = "xyes" ; then
      dpp_cxx_prof_p='yes'
      CXX_PROF_FLAGS='-p'
   else
      dpp_cxx_prof_p='no'
      CXX_PROF_FLAGS=''
   fi

   AC_SUBST(CXX_PROF_FLAGS)
])

dnl ---------------------------------------------------------------------------
dnl Check if the C++ compiler supports -pg.  Based on the results of the test,
dnl $CXX_PROF_FLAGS will be set to '-pg' or ''.
dnl
dnl Usage:
dnl     DPP_PROG_CXX_PROF_PG
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_CXX_PROF_PG],
[
   DPP_COMPILER_FLAG([${CXX-c++}], [-pg], [dpp_cv_prog_cxx_prof_pg])

   if test "x$dpp_cv_prog_cxx_prof_pg" = "xyes" ; then
      dpp_cxx_prof_pg='yes'
      CXX_PROF_FLAGS='-pg'
   else
      dpp_cxx_prof_pg='no'
      CXX_PROF_FLAGS=''
   fi

   AC_SUBST(CXX_PROF_FLAGS)
])

m4_define([AC_LANG(Objective-C)],
[ac_ext=m
ac_cpp='$OBJCCPP $OBJCPPFLAGS'
ac_compile='$OBJC -c $OBJCFLAGS $OBJCPPFLAGS conftest.$ac_ext >&AS_MESSAGE_LOG_FD'
ac_link='$OBJC -o conftest$ac_exeext $OBJCFLAGS $OBJCPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS >&AS_MESSAGE_LOG_FD'
ac_compiler_gnu=$ac_cv_objc_compiler_gnu
])

m4_define([_AC_LANG_ABBREV(Objective-C)], [objc])

m4_define([_AC_LANG_PREFIX(Objective-C)], [OBJC])

m4_define([AC_LANG(Objective-C++)],
[ac_ext=mm
ac_cpp='$OBJCXXCPP $OBJCXXPPFLAGS'
ac_compile='$OBJCXX -c $OBJCXXFLAGS $OBJCXXPPFLAGS conftest.$ac_ext >&AS_MESSAGE_LOG_FD'
ac_link='$CXX -o conftest$ac_exeext $OBJCXXFLAGS $OBJCXXPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS >&AS_MESSAGE_LOG_FD'
ac_compiler_gnu=$ac_cv_objcxx_compiler_gnu
])

m4_copy([AC_LANG_SOURCE(C)], [AC_LANG_SOURCE(Objective-C)])

m4_copy([AC_LANG_PROGRAM(C)], [AC_LANG_PROGRAM(Objective-C)])

m4_define([_AC_LANG_ABBREV(Objective-C++)], [objcxx])

m4_define([_AC_LANG_PREFIX(Objective-C++)], [OBJCXX])

m4_copy([AC_LANG_SOURCE(C)], [AC_LANG_SOURCE(Objective-C++)])

m4_copy([AC_LANG_PROGRAM(C)], [AC_LANG_PROGRAM(Objective-C++)])

m4_define([_AC_PROG_OBJC_G],
[ac_test_OBJCFLAGS=${OBJCFLAGS+set}
ac_save_OBJCFLAGS=$OBJCFLAGS
OBJCFLAGS="-g"
AC_CACHE_CHECK(whether $OBJC accepts -g, ac_cv_prog_objc_g,
               [_AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
                                   [ac_cv_prog_objc_g=yes],
                                   [ac_cv_prog_objc_g=no])])
if test "$ac_test_OBJCFLAGS" = set; then
  OBJCFLAGS=$ac_save_OBJCFLAGS
elif test $ac_cv_prog_objc_g = yes; then
  if test "$GNU_OBJC" = yes; then
    OBJCFLAGS="-g -O2"
  else
    OBJCFLAGS="-g"
  fi
else
  if test "$GNU_OBJC" = yes; then
    OBJCFLAGS="-O2"
  else
    OBJCFLAGS=
  fi
fi[]dnl
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of an Objective-C compiler (or the
dnl given Objective-C compiler if one is specified) and how to run its
dnl preprocessor. A platform-specific hint for the Objective-C compiler can be
dnl given by setting a value for the variable $OBJC_<PLATFORM> (e.g.,
dnl $OBJC_Linux). A value for the Objective-C-compiler argument overrides any
dnl hint given, however.  A list of fallbacks can be provided by setting a
dnl value for the variable $OBJC_FALLBACKS.  This must be in the format of an
dnl sh list.
dnl
dnl Usage:
dnl     DPP_PROG_OBJC([Objective-C-compiler [, extra-flags [, action-if-not-found [, path ]]]])
dnl
dnl Arguments:
dnl     Objective-C-compiler - Force the check to look for this Objective-C
dnl                            compiler. This is optional.
dnl     extra-flags          - Extra compiler flags to use when checking.  This
dnl                            is optional.
dnl     action-if-not-found  - The action to take if the Objective-C compiler
dnl                            cannot be found or does not work. This is
dnl                            optional.
dnl     path                 - Extra path information for finding the
dnl                            Objective-C compiler. This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_OBJC],
[
   dpp_save_OBJCFLAGS="$OBJCFLAGS"
   OBJCFLAGS="$OBJCFLAGS $2 $ABI_FLAGS"

   dnl Get the platform-specific compiler hint value.
   dpp_objc_var="OBJC_$dpp_platform"
   dpp_objc_val=`eval echo '$''{'$dpp_objc_var'}'`

   if test "x$dpp_objc_val" != "x" -a "x$OBJC" = "x" ; then
      OBJC="$dpp_objc_val"
   fi

   if test "x$1" != "x" -o "x$OBJC" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_OBJC_found_val=$1
         AC_CHECK_PROG([dpp_OBJC_check_val], $1, $1, , $4)
      else
         dpp_OBJC_found_val="$OBJC"
         AC_CHECK_PROG([dpp_OBJC_check_val], [$OBJC], [$OBJC], , $4)
      fi

      if test "x$dpp_OBJC_check_val" = "x$dpp_OBJC_found_val" ; then
         OBJC="$dpp_OBJC_found_val"
      else
         dpp_OBJC_found='no'

         dnl If the caller provided a list of fallbacks in the $OBJC_FALLBACKS
         dnl variable, loop over them and try to use another Objective-C
         dnl compiler.
         if test "x$OBJC_FALLBACKS" != "x" ; then
            for dpp_objc in $OBJC_FALLBACKS ; do
               AC_CHECK_PROG([dpp_OBJC_check_val], [$dpp_objc], [$dpp_objc], ,
                             $4)
               if test "x$dpp_OBJC_check_val" = "x$dpp_objc" ; then
                  OBJC="$dpp_objc"
                  dpp_OBJC_found='yes'
                  break
               fi
            done
         fi

         dnl No viable Objective-C compiler was found.
         if test "x$dpp_OBJC_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_LANG_PUSH([Objective-C])

   AC_CHECK_TOOL(OBJC, gcc)
   if test -z "$OBJC" ; then
      AC_CHECK_TOOL(OBJC, cc)
   fi
   if test -z "$OBJC" ; then
      AC_CHECK_PROG(OBJC, cc, cc)
   fi

   test -z "$OBJC" && AC_MSG_FAILURE([no acceptable Objective-C compiler found in \$PATH])
dnl   AC_PROG_CPP

   m4_expand_once([_AC_COMPILER_EXEEXT])[]dnl
   m4_expand_once([_AC_COMPILER_OBJEXT])[]dnl
   _AC_LANG_COMPILER_GNU
   GNU_OBJC=`test $ac_compiler_gnu = yes && echo yes`
   _AC_PROG_OBJC_G

   OBJCFLAGS="$dpp_save_OBJCFLAGS"

   dnl If GCC will be the Objective-C compiler, -fPIC is the
   dnl position-independent code generation option.
   if test "x$GNU_OBJC" = "xyes" ; then
      OBJCFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_objc_pic_var="OBJC_PIC_$dpp_platform"
      OBJCFLAGS_DYNLIB=`eval echo '$''{'$dpp_objc_pic_var'}'`
   fi

   OBJCFLAGS_DYNLIB="$OBJCFLAGS_DYNLIB -DPIC"

   AC_LANG_POP([Objective-C])

   AC_SUBST(OBJCFLAGS_DYNLIB)
])

m4_define([_AC_PROG_OBJCXX_G],
[ac_test_OBJCXXFLAGS=${OBJCXXFLAGS+set}
ac_save_OBJCXXFLAGS=$OBJCXXFLAGS
OBJCXXFLAGS="-g"
AC_CACHE_CHECK(whether $OBJCXX accepts -g, ac_cv_prog_objcxx_g,
               [_AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
                                   [ac_cv_prog_objcxx_g=yes],
                                   [ac_cv_prog_objcxx_g=no])])
if test "$ac_test_OBJCXXFLAGS" = set; then
  OBJCXXFLAGS=$ac_save_OBJCXXFLAGS
elif test $ac_cv_prog_objcxx_g = yes; then
  if test "$GNU_OBJCXX" = yes; then
    OBJCXXFLAGS="-g -O2"
  else
    OBJCXXFLAGS="-g"
  fi
else
  if test "$GNU_OBJCXX" = yes; then
    OBJCXXFLAGS="-O2"
  else
    OBJCXXFLAGS=
  fi
fi[]dnl
])

dnl ---------------------------------------------------------------------------
dnl Check for the existence and usability of an Objective-C++ compiler (or the
dnl given Objective-C++ compiler if one is specified) and how to run its
dnl preprocessor. A platform-specific hint for the Objective-C++ compiler can
dnl be given by setting a value for the variable $OBJCXX_<PLATFORM> (e.g.,
dnl $OBJCXX_Linux). A value for the Objective-C++-compiler argument overrides
dnl any hint given, however. A list of fallbacks can be provided by setting a
dnl value for the variable $OBJCXX_FALLBACKS. This must be in the format of an
dnl sh list.
dnl
dnl Usage:
dnl     DPP_PROG_OBJCXX([Objective-C++-compiler [, extra-flags [, action-if-not-found [, path ]]]])
dnl
dnl Arguments:
dnl     Objective-C++-compiler - Force the check to look for this Objective-C++
dnl                              compiler. This is optional.
dnl     extra-flags            - Extra compiler flags to use when checking.
dnl                              This is optional.
dnl     action-if-not-found    - The action to take if the Objective-C++
dnl                              compiler cannot be found or does not work.
dnl                              This is optional.
dnl     path                   - Extra path information for finding the
dnl                              Objective-C++ compiler. This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PROG_OBJCXX],
[
   dpp_save_OBJCXXFLAGS="$OBJCXXFLAGS"
   OBJCXXFLAGS="$OBJCXXFLAGS $2 $ABI_FLAGS"

   dnl Get the platform-specific compiler hint value.
   dpp_objcxx_var="OBJCXX_$dpp_platform"
   dpp_objcxx_val=`eval echo '$''{'$dpp_objcxx_var'}'`

   if test "x$dpp_objcxx_val" != "x" -a "x$OBJCXX" = "x" ; then
      OBJCXX="$dpp_objcxx_val"
   fi

   if test "x$1" != "x" -o "x$OBJCXX" != "x" ; then
      if test "x$1" != "x" ; then
         dpp_OBJCXX_found_val=$1
         AC_CHECK_PROG([dpp_OBJCXX_check_val], $1, $1, , $4)
      else
         dpp_OBJCXX_found_val="$OBJCXX"
         AC_CHECK_PROG([dpp_OBJCXX_check_val], [$OBJCXX], [$OBJCXX], , $4)
      fi

      if test "x$dpp_OBJCXX_check_val" = "x$dpp_OBJCXX_found_val" ; then
         OBJCXX="$dpp_OBJCXX_found_val"
      else
         dpp_OBJCXX_found='no'

         dnl If the caller provided a list of fallbacks in the
         dnl $OBJCXX_FALLBACKS variable, loop over them and try to use another
         dnl Objective-C++ compiler.
         if test "x$OBJCXX_FALLBACKS" != "x" ; then
            for dpp_objcxx in $OBJCXX_FALLBACKS ; do
               AC_CHECK_PROG([dpp_OBJCXX_check_val], [$dpp_objcxx],
                             [$dpp_objcxx], , $4)
               if test "x$dpp_OBJCXX_check_val" = "x$dpp_objcxx" ; then
                  OBJCXX="$dpp_objcxx"
                  dpp_OBJCXX_found='yes'
                  break
               fi
            done
         fi

         dnl No viable Objective-C++ compiler was found.
         if test "x$dpp_OBJCXX_found" = "xno" ; then
            ifelse([$3], , :, [$3])
         fi
      fi
   fi

   AC_LANG_PUSH([Objective-C++])

   AC_CHECK_TOOL(OBJCXX, gcc)
   if test -z "$OBJCXX" ; then
      AC_CHECK_TOOL(OBJCXX, cc)
   fi
   if test -z "$OBJCXX" ; then
      AC_CHECK_PROG(OBJCXX, cc, cc)
   fi

   test -z "$OBJCXX" && AC_MSG_FAILURE([no acceptable Objective-C++ compiler found in \$PATH])
dnl   AC_PROG_CPP

   m4_expand_once([_AC_COMPILER_EXEEXT])[]dnl
   m4_expand_once([_AC_COMPILER_OBJEXT])[]dnl
   _AC_LANG_COMPILER_GNU
   GNU_OBJCXX=`test $ac_compiler_gnu = yes && echo yes`
   _AC_PROG_OBJCXX_G

   OBJCXXFLAGS="$dpp_save_OBJCXXFLAGS"

   dnl If GCC will be the Objective-C++ compiler, -fPIC is the
   dnl position-independent code generation option.
   if test "x$GNU_OBJCXX" = "xyes" ; then
      OBJCXXFLAGS_DYNLIB='-fPIC'
   dnl Otherwise, get the platform-specific compiler PIC option hint value.
   else
      dpp_objcxx_pic_var="OBJCXX_PIC_$dpp_platform"
      OBJCXXFLAGS_DYNLIB=`eval echo '$''{'$dpp_objcxx_pic_var'}'`
   fi

   OBJCXXFLAGS_DYNLIB="$OBJCXXFLAGS_DYNLIB -DPIC"

   AC_LANG_POP([Objective-C++])

   AC_SUBST(OBJCXXFLAGS_DYNLIB)
])
