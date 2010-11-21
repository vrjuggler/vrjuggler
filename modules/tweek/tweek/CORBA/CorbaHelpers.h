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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TWEEK_CORBA_HELPERS_H_
#define _TWEEK_CORBA_HELPERS_H_

#include <tweek/tweekConfig.h>

#include <string>
#include TWEEK_INCLUDE_CORBA_H

#ifdef TWEEK_USE_TAO
#include <orbsvcs/CosNamingC.h>
#endif


namespace tweek
{

/**
 * Gets the root context for the CORBA Naming Service using the ORB's "resolve
 * initial references" capabilities and therefore requires that the reference
 * be accessible to the ORB through appropriate means.
 *
 * @param orb The reference to the ORB in this memory space.
 *
 * @pre  The Naming Service reference is available through the ORB's "resolve
 *       initial referneces" feature.
 * @post The root context is retrieved through the given ORB pointer.
 */
CosNaming::NamingContext_var getRootNamingContextByInitRef(CORBA::ORB_ptr orb);

/**
 * Gets the root context for the CORBA Naming Service using the given URI.
 *
 * @param orb            The reference to the ORB in this memory space.
 * @param nameServiceURI The URI to be used for looking up the Naming Service
 *                       reference.  This can be a corbaname or a corbaloc URI.
 *
 * @post The root context is retrieved through the given ORB pointer.
 */
CosNaming::NamingContext_var getRootNamingContextByURI(CORBA::ORB_ptr orb,
                                                       const std::string& nameServiceURI);

/**
 * Binds a local Naming Context for this memory space.
 *
 * @param parentContext The parent context within which the local context will
 *                      be created.
 * @param localId       The user-specified identifier for the context.
 *
 * @post A sub-context is created within the given root naming context.
 */
CosNaming::NamingContext_var bindLocalNamingContext(CosNaming::NamingContext_ptr parentContext,
                                                    const std::string& localId);

/**
 * Prints the given CORBA system exception to standard out (stdout) using
 * vpr::Debug.
 *
 * @param ex         The exception to be printed to stdout.
 * @param debugLevel The vpr::Debug output level at which the exception state
 *                   will be printed.
 */
void printSystemException(const CORBA::SystemException& ex,
                          const int debugLevel);

} // End of tweek namespace


#endif /* _TWEEK_CORBA_HELPERS_H_ */
