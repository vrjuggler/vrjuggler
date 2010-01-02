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

package org.vrjuggler.tweek.net.corba;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;


/**
 * This is a convenience class that wraps all CORBA functionality that will be
 * needed by Tweek client code.
 */
public class CorbaService
{
   /**
    * Creates and initializes a new CORBA ORB and activates the root
    * Portable Object Adapter (POA). The ORB itself runs in a thread that is
    * managed by this object.
    *
    * @param args Command line arguments to be processed by the ORB.
    *
    * @throws org.omg.SystemException
    *           A CORBA system error occurred.
    * @throws org.omg.PortableServer.POAManagerPackage.AdapterInactive
    *           The Portable Object Adapter (POA) is inactive and cannot be
    *           accessed.
    * @throws org.omg.CORBA.ORBPackage.InvalidName
    *           The root Portable Object Adapter (POA) could not be found.
    *           CORBA objects cannot be accessed.
    */
   public CorbaService(String[] args)
      throws SystemException
           , org.omg.PortableServer.POAManagerPackage.AdapterInactive
           , org.omg.CORBA.ORBPackage.InvalidName
   {
      mORB = ORB.init(args, null);
      mOrbThread = new OrbThread(mORB);
      mOrbThread.start();

      rootPOA = (POA) mORB.resolve_initial_references("RootPOA");
      rootPOA.the_POAManager().activate();
   }

   public ORB getORB()
   {
      return mORB;
   }

   public POA getRootPOA()
   {
      return rootPOA;
   }

   /**
    * Shuts down the ORB created by this object.  Depending on the given
    * argument, this call will (or will not) block until all pending events
    * are completed.
    *
    * Post conditin: If an ORB has been initialized, it will be shut down, its
    * thread will stop running, and the references will be set to null.
    *
    * @param wait_for_completion If true, the shutdown process for this
    *                            service will block until pending events are
    *                            completed.
    */
   public void shutdown (boolean wait_for_completion)
   {
      if ( mOrbThread != null && mORB != null )
      {
         rootPOA.destroy(true, wait_for_completion);
         mORB.shutdown(wait_for_completion);
         mORB       = null;
         mOrbThread = null;
      }
   }

   /**
    * Sets the Subject Manager reference that will be used with this CORBA
    * service object.
    */
   public void setSubjectManager(tweek.SubjectManager mgr)
   {
      this.subjectManager = mgr;
   }

   /**
    * Returns a reference to the CORBA object known as the Subject Manager.
    * Using this reference, the caller can request references to subjects
    * registered with the manager.
    *
    * @return null if the Subject Manager reference has not been set yet.
    */
   public tweek.SubjectManager getSubjectManager ()
   {
      return this.subjectManager;
   }

   /**
    * Registers the given servant with the local Portable Object Adaptor.
    * This is necessary for tweek::Observer implementations to be referenced
    * by their corresponding subject.
    *
    * @param servant The servant (interface implementation object) to register.
    * @param name    The name of the servant within the local POA.
    *
    * @return The ID of the servant within the POA.  This should be retained
    *         so that the servant can be unregistered later.
    */
   public byte[] registerObject(Servant servant, String name)
   {
      byte[] obj_id = null;

      // Get object reference from the servant.
      try
      {
         obj_id = rootPOA.activate_object(servant);
      }
      catch (ServantAlreadyActive ex)
      {
         System.err.println("WARNING: (CorbaService.registerObject() " +
                            "Servant already active in root POA!");
         ex.printStackTrace();
      }
      catch (WrongPolicy ex)
      {
         ex.printStackTrace();
      }

      return obj_id;
   }

   /**
    * Removes the identified Servant object from the collection of activated
    * CORBA objects.  In CORBA terms, the servant is deactivated within the
    * POA and cannot be referenced after this occurs.
    *
    * @param servant_id The unique identifier for the servant that is to be
    *                   deactivated.
    */
   public void unregisterObject(byte[] servant_id)
   {
      if ( servant_id != null )
      {
         // Deactivate the identified object within the root POA.
         try
         {
            rootPOA.deactivate_object(servant_id);
         }
         catch (ObjectNotActive ex)
         {
            System.err.println("WARNING: (CorbaService.unregisterObject()) " +
                               "Servant not active in root POA!");
            ex.printStackTrace();
         }
         catch (WrongPolicy ex)
         {
            ex.printStackTrace();
         }
      }
      else
      {
         System.err.println("WARNING: Tried to unregister null ID in " +
                            "CorbaService.unregisterObject()!");
      }
   }

   private class OrbThread extends Thread
   {
      public OrbThread (ORB orb)
      {
         mORB = orb;
      }

      public void run ()
      {
         System.out.println("ORB thread starting");
         mORB.run();
         System.out.println("ORB thread exiting");
      }

      private ORB mORB = null;
   }

   private tweek.SubjectManager subjectManager = null;

   private ORB mORB    = null;
   private POA rootPOA = null;

   private OrbThread mOrbThread = null;
}
