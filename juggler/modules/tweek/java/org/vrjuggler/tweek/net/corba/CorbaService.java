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
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;


public class CorbaService
{
   /**
    * @param ns_uri The URI that can be used as the initial reference for the
    *               NameService.
    * @param
    */
   public CorbaService (String ns_host, int ns_port, String subcontext_id)
   {
      nameServiceHost  = ns_host;
      nameServicePort  = ns_port;
      namingSubcontext = subcontext_id;

      nameServiceURI = "corbaloc:iiop:1.0@" + ns_host + ":" + ns_port + "/NameService";

      System.out.println("nameServiceURI: " + nameServiceURI);
   }

   public void init (String[] args) throws SystemException
   {
      m_orb = ORB.init(args, null);
      m_orb_thread = new OrbThread(m_orb);
      m_orb_thread.start();

      try
      {
         m_root_poa = (POA) m_orb.resolve_initial_references("RootPOA");
         m_root_poa.the_POAManager().activate();
      }
      catch (org.omg.CORBA.ORBPackage.InvalidName name_ex)
      {
         System.err.println("ERROR: Could not get RootPOA: " +
                            name_ex.getMessage());
      }
      catch (org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
      {
         System.err.println("ERROR: Could not activate RootPOA: " +
                            ex.getMessage());
      }

      try
      {
         org.omg.CORBA.Object init_ref = null;

         init_ref    = m_orb.string_to_object(nameServiceURI);
         rootContext = NamingContextHelper.narrow(init_ref);

         if ( rootContext != null )
         {
            // XXX: Need to allow users to specify this through
            // namingSubcontext.
            NameComponent[] tweek_name_context = new NameComponent[1];
            tweek_name_context[0] = new NameComponent("tweek", "context");

            init_ref     = rootContext.resolve(tweek_name_context);
            localContext = NamingContextHelper.narrow(init_ref);
         }
         else
         {
            System.err.println("Failed to get root naming context!");
         }
      }
      catch (UserException user_ex)
      {
         user_ex.getMessage();
         user_ex.printStackTrace();
      }
   }

   public String toString ()
   {
      return nameServiceHost + ":" + nameServicePort + "/" + namingSubcontext;
   }

   public String getNameServiceURI ()
   {
      return nameServiceURI;
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

   public void registerObject (Servant servant, String name)
   {
      // Get object reference from the servant.
      try
      {
         org.omg.CORBA.Object ref = m_root_poa.servant_to_reference(servant);
      }
      catch (ServantNotActive ex)
      {
         ex.printStackTrace();
      }
      catch (WrongPolicy ex)
      {
         ex.printStackTrace();
      }
   }

   /**
    * Removes the given Servant object from the collection of activated CORBA
    * objects.  In CORBA terms, the servant is deactivated within the POA and
    * cannot be referenced after this occurs.
    */
   public void unregisterObject (Servant servant)
   {
/*
      // Get object reference from the servant and deactivate it within the
      // POA.
      try
      {
         org.omg.CORBA.Object ref = m_root_poa.servant_to_reference(servant);
         m_root_poa.deactivate_object(servant._object_id());
      }
      catch (ObjectNotActive ex)
      {
         ex.printStackTrace();
      }
      catch (ServantNotActive ex)
      {
         ex.printStackTrace();
      }
      catch (WrongPolicy ex)
      {
         ex.printStackTrace();
      }
*/
   }

   private class OrbThread extends Thread
   {
      public OrbThread (ORB orb)
      {
         m_orb = orb;
      }

      public void run ()
      {
         System.out.println("ORB thread starting");
         m_orb.run();
         System.out.println("ORB thread exiting");
      }

      private ORB m_orb = null;
   }

   private String nameServiceHost = null;
   private int    nameServicePort = 2809;
   private String nameServiceURI  = null;

   private String namingSubcontext = null;

   private ORB           m_orb        = null;
   private POA           m_root_poa   = null;
   private NamingContext rootContext  = null;
   private NamingContext localContext = null;

   private OrbThread m_orb_thread = null;
}
