/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;


/**
 * This is a convenience class that wraps all CORBA functionality that will be
 * needed by Tweek client code.
 */
public class CorbaService
{
   /**
    * Creates a new instance of this class and initializes the URI that will
    * be used to contact the CORBA Naming Service.
    *
    * @param nsHost       The hostname (or IP address) of the machine where
    *                     the Naming Service is running.
    * @param nsPort       The port number on which the Naming Service is
    *                     listening.  Normally, this will be 2809.
    * @param iiopVer      The version of IIOP to use when communicating with
    *                     the Naming Service.  Common values are "1.0" and
    *                     "1.2".
    * @param subcontextId The identifier for the Naming subcontext.  This is
    *                     currently unused.
    */
   public CorbaService(String nsHost, int nsPort, String iiopVer,
                       String subcontextId)
   {
      nameServiceHost    = nsHost;
      nameServicePort    = nsPort;
      nameServiceIiopVer = iiopVer;
      namingSubcontext   = subcontextId;

      nameServiceURI = "corbaloc:iiop:" + iiopVer + "@" + nsHost + ":" +
                       nsPort + "/NameService";

      System.out.println("nameServiceURI: " + nameServiceURI);
   }

   /**
    * Initializes this ORB (the so-called "CORBA Service") by starting the
    * ORB thread and making the initial connection to the CORBA Naming Service.
    *
    * @param args Command line arguments to be processed by the ORB.
    *
    * @throws org.omg.SystemException A CORBA system error occurred.
    * @throws org.omg.CosNaming.NamingContextPackage.NotFound The requested
    *         naming context (the Tweek naming context in this case) cannot be
    *         found in the CORBA Naming Service.
    * @throws org.omg.CosNaming.NamingContextPackage.CannotProceed An operation
    *         cannot be performed on a specific naming context--the Tweek
    *         naming context in this case.
    * @throws org.omg.CosNaming.NamingContextPackage.InvalidName An invalid
    *         (non-existant) name was requested from the CORBA Naming Service.
    * @throws org.omg.CORBA.ORBPackage.InvalidName The root Portable Object
    *         Adapter (POA) could not be found.  CORBA objects cannot be
    *         accessed.
    * @throws org.omg.PortableServer.POAManagerPackage.AdapterInactive The
    *         Portable Object Adapter (POA) is inactive and cannot be accessed.
    */
   public void init(String[] args)
      throws SystemException, NotFound, CannotProceed, InvalidName,
             org.omg.CORBA.ORBPackage.InvalidName,
             org.omg.PortableServer.POAManagerPackage.AdapterInactive
   {
      mORB = ORB.init(args, null);
      mOrbThread = new OrbThread(mORB);
      mOrbThread.start();

      mRootPoa = (POA) mORB.resolve_initial_references("RootPOA");
      mRootPoa.the_POAManager().activate();

      org.omg.CORBA.Object init_ref = null;

      init_ref = mORB.string_to_object(nameServiceURI);
      rootContext = NamingContextHelper.narrow(init_ref);

      if (rootContext != null)
      {
         // XXX: Need to allow users to specify this through
         // namingSubcontext.
         NameComponent[] tweek_name_context = new NameComponent[1];
         tweek_name_context[0] = new NameComponent("tweek", "context");
         init_ref = rootContext.resolve(tweek_name_context);
         localContext = NamingContextHelper.narrow(init_ref);
      }
      else
      {
         System.err.println("Failed to get root naming context!");
      }
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
         mRootPoa.destroy(true, wait_for_completion);
         mORB.shutdown(wait_for_completion);
         mORB       = null;
         mOrbThread = null;
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
    * Retrieves a reference to all the CORBA objects that implement the
    * tweek.SubjectManager interface.
    *
    * NOTE: The implementation of this method is based on code found on page
    *       806 of <i>Advanced CORBA Programming with C++</i>.
    *
    * @return A list containing zero or more tweek.SubjectManager objects.
    */
   public List getSubjectManagerList()
   {
      ArrayList subj_mgrs = new ArrayList();

      if ( null != localContext )
      {
         int data_size = 100;

         BindingListHolder     list_holder = new BindingListHolder();
         BindingIteratorHolder iter_holder = new BindingIteratorHolder();

         // Get the list of objects (of any type) bound in localContext.
         localContext.list(data_size, list_holder, iter_holder);

         // Using the returned list of objects, populate subj_mgrs with any
         // objects that implement the tweek.SubjectManager interface.
         addSubjectManagers(list_holder.value, subj_mgrs);

         if ( null != iter_holder.value )
         {
            BindingIterator iter = iter_holder.value;

            while ( iter.next_n(data_size, list_holder) )
            {
               addSubjectManagers(list_holder.value, subj_mgrs);
            }

            iter.destroy();
         }
      }

      return subj_mgrs;
   }

   /**
    * Resolves all the CORBA objects implementing tweek.SubjectManager in
    * bindingList and stores the resulting tweek.SubjectManager object(s) in
    * mgrList.  If bindingList contains no such objects, mgrList will not be
    * modified.  All references added to mgrList are guaranteed to refer to
    * extant objects.
    */
   private void addSubjectManagers(Binding[] bindingList, List mgrList)
   {
      Binding binding;

      for ( int i = 0; i < bindingList.length; ++i )
      {
         binding = bindingList[i];

         // We do not care about anything that is a naming context.
         if ( BindingType.ncontext != binding.binding_type )
         {
            // Furthermore, we only care about SubjectManager instances.
            if ( binding.binding_name[0].id.startsWith("SubjectManager") )
            {
               NameComponent name_comp[] = binding.binding_name;

               try
               {
                  org.omg.CORBA.Object ref = localContext.resolve(name_comp);
                  tweek.SubjectManager mgr = tweek.SubjectManagerHelper.narrow(ref);

                  // Do not present invalid Subject Manager references to the
                  // user.  This little test is pretty sweet--I just hope it's
                  // fast.
                  try
                  {
                     if ( ! mgr._non_existent() )
                     {
                        mgrList.add(mgr);
                     }
                  }
                  // CORBA system exceptions mean that the current Subject
                  // Manager reference is not available, so we cannot add it
                  // to mgrList.
                  catch (org.omg.CORBA.SystemException ex)
                  {
                     // Ignore the exception.
                  }
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
            }
         }
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
         obj_id = mRootPoa.activate_object(servant);
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
            mRootPoa.deactivate_object(servant_id);
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

   private String nameServiceHost    = null;
   private int    nameServicePort    = 2809;
   private String nameServiceIiopVer = "1.0";
   private String nameServiceURI     = null;

   private String namingSubcontext = null;

   private tweek.SubjectManager subjectManager = null;

   private ORB           mORB         = null;
   private POA           mRootPoa     = null;
   private NamingContext rootContext  = null;
   private NamingContext localContext = null;

   private OrbThread mOrbThread = null;
}
