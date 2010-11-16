dnl Doozer++ is (C) Copyright 2000-2010 by Iowa State University
dnl Distributed under the GNU Lesser General Public License 2.1.  (See
dnl accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
dnl
dnl Original Author:
dnl   Patrick Hartling

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
dnl Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl Boston, MA 02110-1301, USA.

dnl ===========================================================================
dnl Provide functionality for application binary interface (ABI) and
dnl instruction set architecture (ISA) configurations.  The code here performs
dnl substitution for the variables $ABI, $ISA and $LIBBITUF (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_ABI_CFG   - Set the values for $ABI, $ISA, $LIBBITSUF and
dnl                     $ABI_FLAGS respectively.  These variables are
dnl                     described next.
dnl     DPP_ABI_SETUP - Add the --with-abi option and configure the ABI/ISA
dnl                     settings based on the value given.
dnl
dnl Command-line options added:
dnl     --with-abi - Choose the ABI/ISA setting for compiling.  This can take
dnl                  one of the following values (stored in $DPP_ABI_TYPE):
dnl
dnl                      N32_M3 (N32/mips3)
dnl                      N32_M4 (N32/mips4)
dnl                      64_M3 (64/mips3)
dnl                      64_M4 (64/mips4)
dnl                      ELF_i386 (ELF/i386)
dnl                      WIN32_i386 (WIN32/i386)
dnl                      COFF_ALPHA (COFF/alpha)
dnl                      ELF_ALPHA (ELF/alpha)
dnl                      HP (pa-risc)
dnl                      DARWIN_PPC (Mach-O/PowerPC)
dnl                      DARWIN_i386 (Mach-O/i386)
dnl                      DARWIN_x86_64 (Mach-O/x86_64)
dnl                      DARWIN_UNIVERSAL (Mach-O/universal)
dnl
dnl Variables defined:
dnl     ABI                 - The ABI chosen.
dnl     ISA                 - The ISA chosen.
dnl     LIBBITSUF           - The suffix for some library directories (which
dnl                           are typically named in $LDFLAGS).
dnl     ABI_FLAGS           - Any extra compiler flags needed for compiling
dnl                           with the named ABI/ISA setting. This should be
dnl                           appended to $CFLAGS and $CXXFLAGS for all
dnl                           compile and link tests that Autconf performs.
dnl     PLATFORM_SDK        - Subset of $ABI_FLAGS providing information about
dnl                           the platform SDK required for building the
dnl                           desired ABI/ISA combination(s).
dnl     UNIVERSAL_ARCH_LIST - The list of architectures that will be supported
dnl                           by universal binaries.
dnl     DPP_ABI_TYPE        - The argument given to --with-abi.
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl Define a macro DPP_ABI_CFG for setting up the configuration parameters
dnl for a given ABI.
dnl
dnl Usage:
dnl     DPP_ABI_CFG([ ABI [, ISA [, library-suffix [, extra-flags [, platform-sdk]]]]])
dnl
dnl Arguments:
dnl     ABI            - The application binary interface to use.  This
dnl                      argument is optional.
dnl     ISA            - The instruction set architecture for the target CPU.
dnl                      This argument is optional.
dnl     library-suffix - The bit-width suffix for the library directory tied
dnl                      to the ABI. This argument is optional. If the
dnl                      variable $LIBBITSUF already has a value, then the
dnl                      parameter supplied to this macro is ignored.
dnl     extra-flags    - Extra compiler flags related to the ABI and ISA.
dnl                      This argument is optional.
dnl     platform-sdk   - Yet more compler flags related to the ABI and ISA.
dnl                      These are used to identify the platform SDK needed
dnl                      for targeting the desird ABI/ISA combination. This
dnl                      argument is optional and is appended to $ABI_FLAGS
dnl                      and $_EXTRA_FLAGS.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ABI_CFG],
[
   ABI=$1
   ISA=$2
   : ${LIBBITSUF:=$3}
   ABI_FLAGS="$4 $5"
   _EXTRA_FLAGS="$4 $5"
   PLATFORM_SDK=$5
])

dnl ---------------------------------------------------------------------------
dnl Adds the --with-abi option and configure the ABI/ISA settings based on the
dnl value given.  Substitution on the variables $ABI, $ISA and $LIBBITSUF is
dnl also performed. For generating universal binaries on Mac OS X, the default
dnl set of architectures is 32-bit PowerPC and Intel. This can be overridden
dnl by setting the variable UNIVERSAL_ARCH_LIST to a space-separated list of
dnl architecture names supported by the Apple version of GCC. For example, to
dnl target 32-bit PowerPC, 32-bit Intel, and 64-bit Intel, do the following:
dnl
dnl    UNIVERSAL_ARCH_LIST='ppc i386 x86_64'
dnl    DPP_ABI_SETUP
dnl
dnl See the Mac OS X arch(3) man page for more details about architecture
dnl names.
dnl 
dnl Usage:
dnl     DPP_ABI_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ABI_SETUP],
[
   AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

   dnl -----------------------------------------------------------------------
   dnl Define the binary format.  Possible values are the following:
   dnl     N32_M3           - On IRIX, use N32 mips3 binaries
   dnl     N32_M4           - On IRIX, use N32 mips4 binaries
   dnl     64_M3            - On IRIX, use 64-bit mips3 binaries
   dnl     64_M4            - On IRIX, use 64-bit mips4 binaries
   dnl     ELF_i386         - On an i386 OS (e.g., Linux/i386), use ELF
   dnl                        binaries
   dnl     ELF_x86_64       - On an AMD64/x86_64/x64 OS (e.g., Linux/amd64),
   dnl                        use ELF binaries
   dnl     WIN32_i386       - On an i386 Win32 OS, use Win32 binaries
   dnl     COFF_ALPHA       - On an Alpha, use COFF binaries
   dnl     ELF_ALPHA        - On an Alpha, use ELF binaries
   dnl     HP               - On HP-UX, use PA-RISC binaries
   dnl     HP64             - On HP-UX, use 64-bit PA-RISC binaries
   dnl     DARWIN_PPC       - On Darwin, use PowerPC binaries
   dnl     DARWIN_i386      - On Darwin, use i386 binaries
   dnl     DARWIN_x86_64    - On Darwin, use AMD64/EM64T binaries
   dnl     DARWIN_UNIVERSAL - On Darwin, use Mach-O universal binaries
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(abi,
               [  --with-abi=<N32_M3|N32_M4|64_M3|64_M4|ELF_i386|ELF_x86_64|WIN32_i386|COFF_ALPHA|ELF_ALPHA|HP|HP64|DARWIN_PPC|DARWIN_i386|DARWIN_x86_64|DARWIN_UNIVERSAL>
                          Define the Application Binary
                          Interface to use],
               DPP_ABI_TYPE="$withval")

   AC_ARG_WITH([macosx-sdk],
               [  --with-macosx-sdk=<PATH>
                          Path to Mac OS X platform SDK   [default=none]],
               [osx_platform_sdk="$withval"], [dpp_platform_sdk=''])

   if test "x$osx_platform_sdk" != "x" ; then
      changequote(<<, >>)
      min_osx_ver=`echo $osx_platform_sdk | sed -e 's/.*MacOSX\(10\.[^u]*\)u*\.sdk.*/\1/'`
      changequote([, ])
      osx_sdk_flags="-isysroot $osx_platform_sdk -mmacosx-version-min=$min_osx_ver"
   fi

   dnl Based on the value of $abi_type, set $ABI for later use and the
   dnl Makefile substitution variables $LIBBITSUF and $ISA.
   case "x$DPP_ABI_TYPE" in
      x64_M3)
         DPP_ABI_CFG([64], [mips3], [64], [-64 -mips3])
         ;;
      x64_M4)
         DPP_ABI_CFG([64], [mips4], [64], [-64 -mips4])
         ;;
      xN32_M3)
         DPP_ABI_CFG([N32], [mips3], [32], [-n32 -mips3])
         ;;
      xN32_M4)
         DPP_ABI_CFG([N32], [mips4], [32], [-n32 -mips4])
         ;;
      xELF_i386)
         DPP_ABI_CFG([ELF], [i386], , [-m32])
         ;;
      xELF_x86_64)
         DPP_ABI_CFG([ELF], [x86_64], [64], [-m64])
         ;;
      xWIN32_i386)
         DPP_ABI_CFG([WIN32])
         ;;
      xCOFF_ALPHA)
         DPP_ABI_CFG([COFF], [alpha])
         ;;
      xELF_ALPHA)
         DPP_ABI_CFG([ELF], [alpha])
         ;;
      xHP)
         DPP_ABI_CFG([HP], [pa-risc])
         ;;
      xHP64)
         DPP_ABI_CFG([HP64], [pa-risc], [/pa20_64], [+DD64])
         ;;
      xDARWIN_PPC)
         DPP_ABI_CFG([Mach-O], [powerpc], , [-arch ppc], [$osx_sdk_flags])
         ;;
      xDARWIN_i386)
         DPP_ABI_CFG([Mach-O], [i386], , [-arch i386], [$osx_sdk_flags])
         ;;
      xDARWIN_x86_64)
         DPP_ABI_CFG([Mach-O], [x86_64], , [-arch x86_64], [$osx_sdk_flags])
         ;;
      xDARWIN_UNIVERSAL)
         : ${UNIVERSAL_ARCH_LIST='ppc i386'}
         dpp_universal_arch_flags=''
         for a in $UNIVERSAL_ARCH_LIST ; do
            dpp_universal_arch_flags="$dpp_universal_arch_flags -arch $a"
         done
         DPP_ABI_CFG([Mach-O], [universal], , [$dpp_universal_arch_flags],
                     [$osx_sdk_flags])
         ;;
   esac

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables $ABI, $ISA and $LIBBITSUF.
   dnl -----------------------------------------------------------------------
   AC_SUBST(ABI)
   AC_SUBST(ISA)
   AC_SUBST(LIBBITSUF)
   AC_SUBST(PLATFORM_SDK)
   AC_SUBST(UNIVERSAL_ARCH_LIST)
])
