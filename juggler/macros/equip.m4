dnl **************** <VPR heading BEGIN do not edit this line> ****************
dnl
dnl VR Juggler Portable Runtime
dnl
dnl Original Authors:
dnl   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
dnl
dnl -----------------------------------------------------------------
dnl File:          $RCSfile$
dnl Date modified: $Date$
dnl Version:       $Revision$
dnl -----------------------------------------------------------------
dnl
dnl ***************** <VPR heading END do not edit this line> *****************

dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
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
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ---------------------------------------------------------------------------
dnl EQUIP_PATH([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl
dnl Test for VPR and then define EQIDL, and EQ_LIBS.
dnl ---------------------------------------------------------------------------
AC_DEFUN(EQUIP_PATH,
[
    dnl Get the cflags and libraries from the vpr-config script
    AC_ARG_WITH(equip-standalone,
                [  --with-equip-standalone=<PATH> Path to the standalone Equip installation [default=none]],
                eq_standalone="$withval", eq_standalone='none')

    AC_ARG_WITH(eqidl,
                [  --with-eqidl=<PATH>     Path to eqidl executable [default=none]],
                eqidl_prefix="$withval", eqidl_prefix="")

		dnl -----------------------------------------------
    dnl -- Checks for header files and build EQ_LIBS --
    dnl ----------------------------------------------- 
		DPP_LANG_SAVE
		DPP_LANG_CPLUSPLUS
		AC_CHECK_HEADERS(iostream hash_map hash_map.h)

		dso_save_CPPFLAGS="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $VPR_CXXFLAGS -I$eq_standalone/include -I$eq_standalone/include/Modules/equip/equip_runtime/include"
		AC_CHECK_HEADER(EquipExt/VectorStream.h, ,
                		AC_MSG_ERROR(*** Equip standalone required for DSO ***))
		CPPFLAGS="$dso_save_CPPFLAGS"
		INCLUDES="$INCLUDES -I$eq_standalone/include -I$eq_standalone/include/Modules/equip/equip_runtime/include"

		if test "x$OS_TYPE" = "xWin32" ; then
    		EQ_LIBS="-libpath:$eq_standalone/lib$LIBBITSUF eqStandalone.lib"
		else
    		EQ_LIBS="-L$eq_standalone/lib$LIBBITSUF -leqStandalone"
		fi

		DPP_LANG_RESTORE
    dnl ------------------------------------------------

        
		dnl -- Construct path to eqidl --
		EQIDL=''
		if test "x$eqidl_prefix" != "x" ; then
			EQIDL="$eqidl_prefix/eqidl"
		fi       
      
		dnl -- Try to find eqidl program --
    AC_PATH_PROG(EQIDL, eqidl, no)

    dnl Do a sanity check to ensure that $EQIDL actually works.
    if ! eval `$EQIDL >/dev/null 2>&1` ; then
        EQIDL='no'
    fi

    if test "$EQIDL" = "no" ; then
    		echo "*** The eqidl program was not found."
      	echo "*** Make sure to set --with-eqidl path"
    		EQIDL=""
				ifelse([$3], , :, [$3])
   	fi
    
    AC_SUBST(EQIDL)
    AC_SUBST(EQ_LIBS)
])
