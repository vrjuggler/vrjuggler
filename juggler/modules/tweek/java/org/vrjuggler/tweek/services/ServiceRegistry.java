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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

package org.vrjuggler.tweek.services;

import java.util.HashMap;
import java.util.Vector;


/**
 * Simple registry used to hold all the known services in the system.  Services
 * must have an associated name so that they can be loaded dynamically and
 * retrieved easily.
 *
 * @since 1.0
 */
public class ServiceRegistry
{
   public static ServiceRegistry instance ()
   {
      if ( m_instance == null )
      {
         m_instance = new ServiceRegistry();
      }

      return m_instance;
   }

   public void registerService (String service_name, Object service_bean)
   {
      m_registry.put(service_name, service_bean);
   }

   public Object getService (String service_name)
   {
      return m_registry.get(service_name);
   }

   public Vector getAllServices ()
   {
      return new Vector(m_registry.values());
   }

   /**
    * Default constructor that is protected so that this class can be used as
    * a singleton.
    */
   protected ServiceRegistry()
   {
   }

   protected static ServiceRegistry m_instance = null;

   private HashMap m_registry = new HashMap();
}
