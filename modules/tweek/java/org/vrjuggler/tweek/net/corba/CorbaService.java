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

package org.vrjuggler.tweek.net.corba;

import java.util.Properties;
import org.omg.CORBA.*;
import org.omg.CosNaming.*;


public class CorbaService
{
   public CorbaService (String init_host, int init_port)
   {
      initHost = init_host;
      initPort = init_port;
   }

   public void init (String[] args) throws SystemException
   {
      Properties props = new Properties();
      props.put("org.omg.CORBA.ORBInitialHost", initHost);
      props.put("org.omg.CORBA.ORBInitialPort", String.valueOf(initPort));

      m_orb = ORB.init(args, props);

      org.omg.CORBA.Object init_ref = null;

      try
      {
         init_ref      = m_orb.resolve_initial_references("NameService");
         namingContext = NamingContextHelper.narrow(init_ref);
      }
      catch (UserException user_ex)
      {
         user_ex.printStackTrace();
      }
   }

   public String getInitHost ()
   {
      return initHost;
   }

   public int getInitPort ()
   {
      return initPort;
   }

   public org.omg.CORBA.Object getReference (String id, String kind)
      throws SystemException, UserException
   {
      NameComponent[] name_comp = new NameComponent[1];

      name_comp[0] = new NameComponent(id, kind);

      org.omg.CORBA.Object ref = null;

      ref = namingContext.resolve(name_comp);

      return ref;
   }

   private String initHost = null;
   private int    initPort = 0;

   private ORB           m_orb         = null;
   private NamingContext namingContext = null;
}
