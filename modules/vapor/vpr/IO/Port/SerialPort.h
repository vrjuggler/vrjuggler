/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_SERIAL_PORT_H_
#define _VPR_SERIAL_PORT_H_

/**
 * \file
 *
 * Include this class to get the full declaration of the platform-specific
 * class that is typedef'd to vpr::SerialPort.
 */

#include <vpr/vprConfig.h>

#ifndef VPR_SIMULATOR

#include <vpr/IO/Port/Port.h>
#include <vpr/IO/Port/SerialTypes.h>

// Include the bridge class.
#include <vpr/IO/Port/SerialPort_t.h>

#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR || VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_BOOST
#  ifdef VPR_OS_Windows
#     include <vpr/md/WIN32/IO/Port/SerialPortImplWin32.h>
#  else
#     include <vpr/md/POSIX/IO/Port/SerialPortImplTermios.h>
#  endif  /* VPR_OS_Windows */
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#  include <vpr/md/POSIX/IO/Port/SerialPortImplTermios.h>
#endif

#endif /* ifndef VPR_SIMULATOR */

#endif  /* _VPR_SERIAL_PORT_H_ */
