dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
dnl File:          abi.m4,v
dnl Date modified: 2005/06/03 13:57:58
dnl Version:       1.18
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

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
dnl
dnl Variables defined:
dnl     ABI          - The ABI chosen.  It will have one of the following
dnl                    values: 64, N32, ELF, WIN32
dnl     ISA          - The ISA chosen.  It will have one of the following
dnl                    values: mips3, mips4, i386
dnl     LIBBITSUF    - The suffix for some library directories (which are
dnl                    typically named in $LDFLAGS).  If it has a value, it
dnl                    will be 32 or 64 which only has meaning on IRIX.
dnl     ABI_FLAGS    - Any extra compiler flags needed for compiling with the
dnl                    named ABI/ISA setting.  This should be appended to
dnl                    $CFLAGS and $CXXFLAGS for all compile and link tests
dnl                    that Autconf performs.
dnl     DPP_ABI_TYPE - The argument given to --with-abi.
dnl ===========================================================================

dnl abi.m4,v 1.18 2005/06/03 13:57:58 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Define a macro DPP_ABI_CFG for setting up the configuration parameters
dnl for a given ABI.
dnl
dnl Usage:
dnl     DPP_ABI_CFG([ ABI [, ISA [, library-suffix [, extra-flags]]]])
dnl
dnl Arguments:
dnl     ABI            - The application binary interface to use.  This
dnl                      argument is optional.
dnl     ISA            - The instruction set architecture for the target CPU.
dnl                      This argument is optional.
dnl     library-suffix - The bit-width suffix for the library directory
dnl                      tied to the ABI.  This is only meaningful on IRIX.
dnl                      This argument is optional.
dnl     extra-flags    - Extra compiler flags related to the ABI and ISA.
dnl                      This argument is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ABI_CFG], [ ABI=$1 ISA=$2 LIBBITSUF=$3 ABI_FLAGS=$4 _EXTRA_FLAGS=$4 ; ])

dnl ---------------------------------------------------------------------------
dnl Adds the --with-abi option and configure the ABI/ISA settings based on the
dnl value given.  Substitution on the variables $ABI, $ISA and $LIBBITSUF is
dnl also performed.
dnl 
dnl Usage:
dnl     DPP_ABI_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_ABI_SETUP],
[
   AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

   dnl -----------------------------------------------------------------------
   dnl Define the binary format.  Possible values are the following:
   dnl     N32_M3        - On IRIX, use N32 mips3 binaries
   dnl     N32_M4        - On IRIX, use N32 mips4 binaries
   dnl     64_M3         - On IRIX, use 64-bit mips3 binaries
   dnl     64_M4         - On IRIX, use 64-bit mips4 binaries
   dnl     ELF_i386      - On an i386 OS (e.g., Linux/i386), use ELF binaries
   dnl     ELF_x86_64    - On an AMD64/x86_64/x64 OS (e.g., Linux/amd64), use
   dnl                     ELF binaries
   dnl     WIN32_i386    - On an i386 Win32 OS, use Win32 binaries
   dnl     COFF_ALPHA    - On an Alpha, use COFF binaries
   dnl     ELF_ALPHA     - On an Alpha, use ELF binaries
   dnl     HP            - On HP-UX, use PA-RISC binaries
   dnl     HP64          - On HP-UX, use 64-bit PA-RISC binaries
   dnl     DARWIN_PPC    - On Darwin, use PowerPC binaries
   dnl -----------------------------------------------------------------------
   AC_ARG_WITH(abi,
               [  --with-abi=<N32_M3|N32_M4|64_M3|64_M4|ELF_i386|ELF_x86_64|WIN32_i386|COFF_ALPHA|ELF_ALPHA|HP|HP64|DARWIN_PPC>
                          Define the Application Binary
                          Interface to use],
               DPP_ABI_TYPE="$withval")

   dnl Set default values for these before checking $DPP_ABI_TYPE.
   dnl Currently, these are for non-IRIX platforms.
   ABI=''
   LIBBITSUF=''

   dnl Based on the value of $abi_type, set $ABI for later use and the
   dnl Makefile substitution variables $LIBBITSUF and $ISA.
   case "x$DPP_ABI_TYPE" in
      x64_M3)
         DPP_ABI_CFG('64', 'mips3', '64', '-64 -mips3')
         ;;
      x64_M4)
         DPP_ABI_CFG('64', 'mips4', '64', '-64 -mips4')
         ;;
      xN32_M3)
         DPP_ABI_CFG('N32', 'mips3', '32', '-n32 -mips3')
         ;;
      xN32_M4)
         DPP_ABI_CFG('N32', 'mips4', '32', '-n32 -mips4')
         ;;
      xELF_i386)
         DPP_ABI_CFG('ELF', 'i386')
         ;;
      xELF_x86_64)
         DPP_ABI_CFG('ELF', 'x86_64', '64')
         ;;
      xWIN32_i386)
         DPP_ABI_CFG('WIN32')
         ;;
      xCOFF_ALPHA)
         DPP_ABI_CFG('COFF', 'alpha')
         ;;
      xELF_ALPHA)
         DPP_ABI_CFG('ELF', 'alpha')
         ;;
      xHP)
         DPP_ABI_CFG('HP', 'pa-risc')
         ;;
      xHP64)
         DPP_ABI_CFG('HP64', 'pa-risc', '/pa20_64', '+DD64')
         ;;
      xDARWIN_PPC)
         DPP_ABI_CFG('Mach-O', 'powerpc')
         ;;
   esac

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables $ABI, $ISA and $LIBBITSUF.
   dnl -----------------------------------------------------------------------
   AC_SUBST(ABI)
   AC_SUBST(ISA)
   AC_SUBST(LIBBITSUF)
])
