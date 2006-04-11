/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TWEEK_DEFINES_H_
#define _TWEEK_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */

@TOP@

/* If using omniORB, define to the major version number (3, 4, etc.). */
#undef OMNIORB_VER

/* Define to include the file CORBA.h -- for example: <omniORB/CORBA.h> */
#undef TWEEK_INCLUDE_CORBA_H

/*
 * Define to an ORB version string used to initialize the ORB -- for
 * example: "omniORB3"
 */
#undef TWEEK_ORB_VER_STRING

/* ----------------- The following are needed by omniORB. ----------------- */

/* Define appropriately for the target architcture. */
#undef __alpha__
#undef __hppa__
#undef __mips__
#undef __powerpc__
#undef __sparc__
#undef __x86__

/* Define appropriately for the target operating system. */
#undef __aix__
#undef __darwin__
#undef __freebsd__
#undef __hpux__
#undef __irix__
#undef __linux__
#undef __osf1__
#undef __sunos__
#undef __WIN32__

/* Define only on Windows NT/2000. */
#undef __NT__

/* Define based on the major version number of the target operating system. */
#undef __OSVERSION__

@BOTTOM@

#endif   /* _TWEEK_DEFINES_H_ */
