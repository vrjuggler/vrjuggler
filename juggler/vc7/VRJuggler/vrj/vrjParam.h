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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VJ_PARAM_H_
#define _VJ_PARAM_H_


/*
 * Define to the 9-digit VR Juggler version integer.  This is the "internal
 * version number" that is intended for internal feature checks by VR Juggler
 * itself.
 */
#define __VJ_version 1001147

/*
 * This is the "human-readable" VR Juggler version _string_.  It is of the
 * form <major>.<minor>.<days since last major release>.
 */
namespace vrj
{
   const char* VJ_VERSION = "v1.1.147-0 'Kenny' (NSPR) HEAD Nov 15, 2003 13:19:20";
}


#endif	/* _VJ_PARAM_H_ */
