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
import org.omg.CosNaming.NamingContextPackage.*;


public class CorbaService
{
   public CorbaService (String _host, int _port)
   {
      host = _host;
      port = _port;
   }

   public void init (String[] args) throws SystemException
   {
      Properties props = new Properties();
      props.put("org.omg.CORBA.ORBInitialHost", host);
      props.put("org.omg.CORBA.ORBInitialPort", String.valueOf(port));

      m_orb = ORB.init(args, props);

      org.omg.CORBA.Object init_ref = null;

      try
      {
         init_ref      = m_orb.resolve_initial_references("NameService");
         rootContext = NamingContextHelper.narrow(init_ref);

         NameComponent[] tweek_name_context = new NameComponent[1];
         tweek_name_context[0] = new NameComponent("tweek", "context");

         init_ref     = rootContext.resolve(tweek_name_context);
         localContext = NamingContextHelper.narrow(init_ref);
      }
      catch (UserException user_ex)
      {
         user_ex.printStackTrace();
      }
   }

   public String getHost ()
   {
      return host;
   }

   public int getPort ()
   {
      return port;
   }

   public NamingContext getLocalContext ()
   {
      return localContext;
   }

   /**
    * Retrieves a reference to the CORBA object known as the Subject Manager.
    * Using this reference, the caller can request references to subjects
    * registered with the manager.
    *
    * @return null if the Subject Manager reference cannot be retrieved.
    */
   public tweek.SubjectManager getSubjectManager ()
   {
      tweek.SubjectManager mgr = null;

      NameComponent[] name_comp = new NameComponent[1];
      name_comp[0] = new NameComponent("SubjectManager", "Object");

      try
      {
         org.omg.CORBA.Object ref = localContext.resolve(name_comp);
         mgr = tweek.SubjectManagerHelper.narrow(ref);
      }
      catch (InvalidName e)
      {
         e.printStackTrace();
      }
      catch (CannotProceed e)
      {
         e.printStackTrace();
      }
      catch (NotFound e)
      {
         e.printStackTrace();
      }

      return mgr;
   }

   private String host = null;
   private int    port = 0;

   private ORB           m_orb        = null;
   private NamingContext rootContext  = null;
   private NamingContext localContext = null;
}
