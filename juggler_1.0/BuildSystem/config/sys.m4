dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++
dnl
dnl Original Author:
dnl   Patrick Hartling
dnl ---------------------------------------------------------------------------
dnl VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
dnl File:          sys.m4,v
dnl Date modified: 2001/08/28 22:46:00
dnl Version:       1.39.2.2
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Determine the canonical system type and define a ton of variables for
dnl system-dependent settings.  The platforms supported in this file are AIX,
dnl Cygnus-Win32, FreeBSD, HP-UX, IRIX, Linux, and Solaris.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_SYSTEM_SETUP
dnl     DPP_SYSTEM_SUBST - Add final options to $CFLAGS and $CXXFLAGS based on
dnl                        the results of DPP_SYSTEM_SETUP.
dnl
dnl Variables defined:
dnl     AR              - The archiver program (ar(1) for example).
dnl     ARFLAGS         - Basic flags passed to $AR.
dnl     AR_NAME_FLAG    - The flag passed to $AR to specify the name of the
dnl                       archive being generated.
dnl     CFLAGS          - All options passed to the C compiler.
dnl     CXXFLAGS        - All options passed to the C++ compiler.
dnl     DEP_GEN_FLAG    - Compiler flag for generating source file
dnl                       dependencies.
dnl     DEPEND_FLAGS    - Flags for building dependencies (typically with
dnl                       makedepend(1)).
dnl     DEPEND_EXTRAS   - Miscellaneous flags that the dependency generator
dnl                       may or may not recognize.  If it does not
dnl                       recognize them, they will be ignored.
dnl     DYLIB_NAME_FLAG - Flag used for naming a compiled dynamic library.
dnl     EXE_NAME_FLAG   - Flag used for naming a compiled executable file.
dnl     INCLUDES        - Extensions to the compiler's default include path.
dnl     LD              - The linker program (ld(1) for example).
dnl     LIBS            - Libraries needed for linking.
dnl     LDOPTS          - Basic options for $LD.
dnl     LDOPTS_DBG      - Options for $LD when making a library with debugging
dnl                       symbols.
dnl     LDOPTS_OPT      - Options for $LD when making an optimized library.
dnl     OBJ_NAME_FLAG   - Flag used for naming a compiled object file.
dnl     OBJ_BUILD_FLAG  - Flag passed to compiler to build a compiled object
dnl                       file.
dnl
dnl     DBG_FLAGS       - Complier flags for adding debugging symbols.
dnl     OPT_FLAGS       - Complier flags for compiling optimized code.
dnl
dnl     LDFLAGS_STATIC  - Linker flags that invoke static linking.
dnl     LDFLAGS_DYNAMIC - Linker flags that invoke dynamic linking.
dnl
dnl     LIB_PREFIX      - Prefix for library names.
dnl     STATICLIB_EXT   - Filename extension for static library names.
dnl     DYNAMICLIB_EXT  - Filename extension for dynamic library names.
dnl
dnl     ABI_LIST        - The list of supported ABI/ISA combinations.
dnl     OS_TYPE         - The type of the target platform (UNIX or Win32).
dnl     PLATFORM        - The vendor name of the target platform.
dnl     DSOREGFILE
dnl     DSOVERSION
dnl     DSOVERSIONOPTS
dnl
dnl     OS_REL_STR       - The full, unmodified string giving the operating
dnl                        system version number.
dnl     OS_REL_NUM       - The numerical value of the operating system
dnl                        version number.
dnl     OS_REL_NUM_MAJOR - The major component of the OS version number.
dnl     OS_REL_NUM_MINOR - The minor component of the OS version number.
dnl
dnl Possible preprocssor symbols defined:
dnl     WIN32
dnl     _MBCS
dnl     IRIXREL      - Defined to the string "IRIX5" or "IRIX6" based on the
dnl                    determined version of IRIX.
dnl ===========================================================================

dnl sys.m4,v 1.39.2.2 2001/08/28 22:46:00 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Based on the given target host and CPU, set up the system-specific
dnl configuration for commonly used tools.
dnl
dnl Usage:
dnl     DPP_SYSTEM_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SYSTEM_SETUP,
[
    dnl We use AC_CANONICAL SYSTEM so that we can find out information about
    dnl the build, arget and host platforms rather than only the host.
    AC_REQUIRE([AC_CANONICAL_SYSTEM])

    dnl Operating system release information.  This may contain anything and
    dnl should be treated as a string.
    OS_REL_STR=`uname -r`

    dnl Operating system version number only (any miscellaneous text is
    dnl stripped).  This will only contain a number and can be treated as a
    dnl float-point value.
    changequote(<<, >>)
    OS_REL_NUM=`uname -r | sed -e 's/^[^0-9]*\([0-9][0-9]*\.[0-9][0-9]*\)[^0-9]*$/\1/'`
    changequote([, ])

    dnl Major and minor version numbers from $OS_REL_NUM separated for use
    dnl with the C preprocessor.  Using cut(1) is kind of a hack, but at least
    dnl it's a clean one.
    OS_REL_NUM_MAJOR=`echo "$OS_REL_NUM" | cut -f1 -d'.'`
    OS_REL_NUM_MINOR=`echo "$OS_REL_NUM" | cut -f1 -d' ' | cut -f2 -d'.'`

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

    dnl IRIX junk.
    DSOREGFILE=''
    DSOVERSION=''
    DSOVERSIONOPTS=''

    USE_MAKEDEPEND='N'

    dnl List of supported ABIs on a given platform.
    ABI_LIST=''

    dnl General type of operating system (e.g., UNIX, Win32) and actual
    dnl platform name.
    OS_TYPE='UNIX'
    PLATFORM=''

    dpp_opt_level=${OPT_LEVEL-2}

    dnl The OS for which this is being configured.  The operating systems are
    dnl listed alphabetically.
    case $target_os in
        dnl IBM RS/6000 running AIX.
        aix*)
            dnl If no ABI has been set yet, default to ELF with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('ELF', $target_cpu)
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -shared'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DBG_FLAGS='-g'
            DYNAMICLIB_EXT='so'

            PLATFORM='AIX'
            ;;
        dnl i386-class machine running Windows {98,NT} with Cygnus GNU-Win32.
        cygwin)
            dnl If no ABI has been set yet, default to WIN32 with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('WIN32')
            fi

            if test "x$USE_GCC" = "xyes" ; then
                AR='ar'
                ARFLAGS='-ruv'
                DBG_FLAGS='-g'

                LD='$(CXX) -shared'
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
                LDOPTS_DBG='/DEBUG /PDB:NONE'
                LDOPTS_OPT='/RELEASE'
                OBJ_NAME_FLAG='/Fo$[@]'
                OBJ_BUILD_FLAG='/c'
                EXE_NAME_FLAG='/OUT:$[@]'
                DYLIB_NAME_FLAG="$EXE_NAME_FLAG"

                USE_MAKEDEPEND='Y'
            fi

            LIB_PREFIX=''
            STATICLIB_EXT='lib'
            DYNAMICLIB_EXT='dll'

            ABI_LIST='WIN32'
            OS_TYPE='Win32'

            dpp_sys_name=`uname -s`

            dnl Determine the value for $PLATFORM based on what uname(1) tells
            dnl us.  We define our own values because Cygnus' uname(1) returns
            dnl somewhat annoying names.
            case "$dpp_sys_name" in
                dnl Windows NT 4.0 and 5.0 (aka Windows 2000).
                *NT-[[45]].0)
                    PLATFORM='WinNT'
                    ;;
                dnl Windows 95 and 98.
                *WIN_9*-4.*)
                    PLATFORM='Win9x'
                    ;;
                dnl Windows ME.
                *WIN_ME-4.*)
                    PLATFORM='WinMe'
                    ;;
                dnl Unknown version of Windows that may not yet be supported.
                *)
                    AC_MSG_WARN(Unknown Windows type $dpp_sys_name!)
                    ;;
            esac

            AC_DEFINE(WIN32, , [define if compiling on a Win32 platform])
            AC_DEFINE(_MBCS, , [define if compiling on a Win32 platform])
            ;;
        darwin1.*)
            dnl If no ABI has been set yet, default to MAC with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('MAC', $target_cpu)
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -dynamiclib'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DBG_FLAGS='-g'
            DYNAMICLIB_EXT='dylib'

            PLATFORM='Darwin'
            ;;
        dnl A machine running FreeBSD.
        freebsd*)
            dnl If no ABI has been set yet, default to ELF with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('ELF', $target_cpu)
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -shared'
            LDOPTS=''
            DBG_FLAGS='-g'
            OPT_FLAGS="-O$dpp_opt_level"
            DYNAMICLIB_EXT='so'

            ABI_LIST='ELF'
            PLATFORM='FreeBSD'
            ;;
        dnl HP PA-RISC machine running HP-UX 10.20.
        hpux10.20)
            dnl If no ABI has been set yet, default to HP PA-RISC.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('HP', 'PA-RISC')
            fi

            AR='ar'
            ARFLAGS='-ruv'

            if test "x$USE_GCC" != "xyes" ; then
                CFLAGS="-Aa $CFLAGS"
                CXXFLAGS="+p $CXXFLAGS"
            fi

            LD='ld'
            LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
            LDOPTS='-b'
            DBG_FLAGS='-g'
            OPT_FLAGS='+inline_level 3'
            DYNAMICLIB_EXT='sl'

            ABI_LIST='HP'
            PLATFORM='HP'
            ;;
        dnl HP PA-RISC machine running HP-UX 11.x.
        hpux11*)
            dnl If no ABI has been set yet, default to HP PA-RISC.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('HP', 'PA-RISC')
            fi

            AR='ar'
            ARFLAGS='-ruv'

            if test "x$USE_GCC" != "xyes" ; then
                CFLAGS="-Aa $CFLAGS"
                CXXFLAGS="-AA $CXXFLAGS"
                DEP_GEN_FLAG='-E +m'
                LD='$(CXX)'
                LDOPTS='-b'
            else
                LD='ld'
            fi

            LDFLAGS="$LDFLAGS -L/usr/lib/X11R6"
            DBG_FLAGS='-g'
            OPT_FLAGS=''
            DYNAMICLIB_EXT='sl'

            ABI_LIST='HP'
            PLATFORM='HP'
            ;;
        dnl A machine running IRIX 6.x.
        irix6*)
            dnl If no ABI has been set yet, default to N32_M3.
            if test "x$ABI" = "x" -a "x$USE_GCC" = "xno" -o "x$USE_GCC" = "x"
            then
                DPP_ABI_CFG('N32', 'mips3', '32', '-n32 -mips3')
            fi

            if test "x$USE_GCC" = "xyes" ; then
                AR='ar'
                ARFLAGS='-ruv'
                LDOPTS=''
                DBG_FLAGS='-g'
                OPT_FLAGS="-O$dpp_opt_level"

                dpp_cc_defs_file='/etc/compiler.defaults'

                if test -r "$dpp_cc_defs_file" ; then
                    dpp_cc_defs=`cat $dpp_cc_defs_file`
                    dpp_abi_val=`echo $dpp_cc_defs | sed -e 's/.*abi=\([[a-z0-9]]\{2,3\}\).*/\1/'`
                    dpp_isa_val=`echo $dpp_cc_defs | sed -e 's/.*isa=\(mips[[0-9]]\).*/\1/'`
 
                    dpp_abi_isa_name=''

                    if test "x$dpp_abi_val" = "xn32" ; then
                        dpp_abi_isa_name='N32_'
                    elif test "x$dpp_abi_val" = "x64" ; then
                        dpp_abi_isa_name='64_'
                    else
                        AC_MSG_WARN(*** Unknown ABI type $dpp_abi_val ***)
                    fi

                    if test "x$dpp_isa_val" = "xmips3" ; then
                        dpp_abi_isa_name="${dpp_abi_isa_name}M3"
                    elif test "x$dpp_isa_val" = "xmips4" ; then
                        dpp_abi_isa_name="${dpp_abi_isa_name}M4"
                    else
                        AC_MSG_WARN(*** Unknown ISA type $dpp_isa_val ***)
                    fi

                    ABI_LIST="$dpp_abi_isa_name"
                else
                    AC_MSG_WARN(*** Defaulting ABI list to N32_M3 ***)
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

                CFLAGS="$CFLAGS -ansi"
                CXXFLAGS="$CXXFLAGS $STDFLAGS -ansi"

                DBG_FLAGS="-g"
                _LD_REGOPTS='-check_registry $(DSOREGFILE)'
                LDOPTS="$LDOPTS -ansi $STDFLAGS -all ${_LD_REGOPTS} -elf -rdata_shared"

                DSOVERSION='sgi4.0'
                DSOVERSIONOPTS='-set_version $(DSOVERSION)'

                ABI_LIST='64_M4 64_M3 N32_M4 N32_M3'
            fi

            LD='$(CXX) -shared'
            DYNAMICLIB_EXT='so'

            PLATFORM='IRIX'

            if test $OS_REL_NUM_MAJOR -lt 6 ; then
                dpp_IRIXREL='IRIX5'
            else
                dpp_IRIXREL='IRIX6'
            fi

            AC_DEFINE_UNQUOTED(IRIXREL, "$dpp_IRIXREL",
                [define to \"IRIX5\" or \"IRIX6\" depending on IRIX version])
            ;;
        dnl A machine running Linux.
        linux*)
            dnl If no ABI has been set yet, default to ELF with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('ELF', $target_cpu)
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -shared'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DBG_FLAGS='-g'
            DYNAMICLIB_EXT='so'

            ABI_LIST='ELF'
            PLATFORM='Linux'
            ;;
        dnl DEC Alpha running Digital UNIX 4.0.
        osf4.0)
            dnl If no ABI has been set yet, default to ALPHA (whatever that
            dnl means).
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('ALPHA')
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -shared'
            LDOPTS=''
            OPT_FLAGS="-O$dpp_opt_level"
            DYNAMICLIB_EXT='so'

            ABI_LIST='ALPHA'
            PLATFORM='alpha-osf'
            ;;
        dnl A machine running Solaris (aka SunOS).
        solaris*)
            dnl If no ABI has been set yet, default to ELF with whatever the
            dnl CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG('ELF', $target_cpu)
            fi

            AR='ar'
            ARFLAGS='-ruv'
            LD='$(CXX) -G'
            LDOPTS=''
            DBG_FLAGS='-g'
            OPT_FLAGS="-O$dpp_opt_level"
            DYNAMICLIB_EXT='so'

            ABI_LIST='ELF'
            PLATFORM='Solaris'
            ;;
        dnl Unkown operating system (to config.guess).
        unknown)
            dnl If no ABI has been set yet, default to the OS (which will be
            dnl 'unknown') with whatever the CPU architecture is.
            if test "x$ABI" = "x" ; then
                DPP_ABI_CFG($target_os, $target_cpu)
            fi  

            PLATFORM='UNKNOWN'
            ;;
    esac

    if test "x$PLATFORM" = "x" ; then
        AC_MSG_WARN(Unknown target OS!)
        PLATFORM='UNKNOWN'
        OS_TYPE='UNKNOWN'
    fi

    dpp_platform="$PLATFORM"
    dpp_os_type="$OS_TYPE"
])

dnl ---------------------------------------------------------------------------
dnl Add final options to $CFLAGS and $CXXFLAGS that would not be needed during
dnl the rest of the configuration.  Substitution of most variables defined in
dnl DPP_SYSTEM_SETUP is done here.
dnl
dnl Usage:
dnl     DPP_SYSTEM_SUBST
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SYSTEM_SUBST,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])

    if test "x$dpp_os_type" = "xWin32" ; then
        DEPEND_EXTRAS="-D_WIN32 -D_M_IX86 $DEPEND_EXTRAS"
    fi

    dnl Provide the linker options for static and dynamic linking of
    dnl applications on various platforms.
    if test "x$dpp_platform" = "xIRIX" -a "x$USE_GCC" = "xno" -o \
            "x$USE_GCC" = "x"
    then
        LDFLAGS_STATIC='-B static'
        LDFLAGS_DYNAMIC='-B dynamic'
    dnl XXX: I don't know what to do for Win32...
    dnl elif test "x$dpp_os_type" = "xWin32" -a test "x$USE_GCC" != "xyes" ; then
    dnl ...
    dnl This is the case for GCC.
    else
        LDFLAGS_STATIC='-static'
        LDFLAGS_DYNAMIC=''
    fi

    dnl -----------------------------------------------------------------------
    dnl Do the substition step for the variables defined here.
    dnl -----------------------------------------------------------------------
    AC_SUBST(AR)
    AC_SUBST(ARFLAGS)
    AC_SUBST(AR_NAME_FLAG)
    AC_SUBST(CFLAGS)
    AC_SUBST(CXXFLAGS)
    AC_SUBST(DEP_GEN_FLAG)
    AC_SUBST(DEPEND_FLAGS)
    AC_SUBST(DEPEND_EXTRAS)
    AC_SUBST(INCLUDES)
    AC_SUBST(LD)
    AC_SUBST(LIBS)
    AC_SUBST(LDOPTS)
    AC_SUBST(LDOPTS_DBG)
    AC_SUBST(LDOPTS_OPT)
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

    AC_SUBST(ABI_LIST)
    AC_SUBST(OS_TYPE)
    AC_SUBST(PLATFORM)
    AC_SUBST(DSOREGFILE)
    AC_SUBST(DSOVERSION)
    AC_SUBST(DSOVERSIONOPTS)

    AC_SUBST(USE_MAKEDEPEND)
])
