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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.jccl.rtrc;

import java.util.List;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import org.omg.CORBA.BAD_PARAM;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import org.vrjuggler.jccl.config.*;
import tweek.*;
import jccl.*;

import java.io.*;
import java.util.*;
import org.vrjuggler.jccl.config.io.*;

/**
 * This is a RTRCDataSourceBroker implementation that does the real work behind
 * the scenes. It runs as a tweek service which is used by the
 * RTRCDataSourceBrokerProxy class.
 *
 * @version 1.0
 */
public class RTRCDataSourceBrokerImpl
   extends TweekFrameAdapter
   implements CommunicationListener
            , RTRCDataSourceBroker
{
   public RTRCDataSourceBrokerImpl()
   {
      System.out.println("RTRCDataSourceBroker started");
   }

   /**
    * Implements the Tweek CommunicationListener interface needed for being
    * informed of connections and disconnections with remote CORBA servers.
    */
   public void connectionOpened(CommunicationEvent e)
   {
      // The first thing to do is get the CORBA service object from the
      // event.  We need this so we know to whom we are are connecting.  Once
      // we have the CORBA service, we get its Subject Manager since that's
      // what contains the actual subjects we need.
      CorbaService corba_service = e.getCorbaService();
      SubjectManager mgr         = corba_service.getSubjectManager();

      Subject subject = mgr.getSubject("CorbaRemoteReconfig");

      // Try to narrow the Subjet object to a SliderSubject object.  If this
      // fails, it throws a CORBA BAD_PARAM exception.  In that case, we open
      // a dialog box saying that the narrowing failed.
      try
      {
         RemoteReconfigSubject temp =
            RemoteReconfigSubjectHelper.narrow(subject);
         if (temp != null)
         {
            add(temp);
         }
      }
      catch (BAD_PARAM narrow_ex)
      {
         JOptionPane.showMessageDialog(null,
                                       "Failed to narrow subject to SliderSubject",
                                       "SliderSubject Narrow Error",
                                       JOptionPane.ERROR_MESSAGE);
      }
   }
   
   /**
    * Catch an event when we lose a CORBA connection that a RTRDDataSource is
    * using.
    */
   public void connectionClosed(CommunicationEvent e)
   {
      //TODO: Find a way to close the RTRCDataSource that has lost its connection.
      //disconnect();
   }

   /**
    * Catch a frame close event to cleanly shutdown our CORBA subjects.
    */
   public boolean frameClosing(TweekFrameEvent e)
   {
      //TODO: Find a way to close the RTRCDataSource that has lost its
      //connection.
      //disconnect();
      return true;
   }

   /**
    * Create a new RTRCDataSource for a new RemoteReconfigSubject. And add it
    * to the list of active RTRCDataSources.
    */
   private void add(RemoteReconfigSubject subject)
   {
      System.out.println("Adding a new RTRC Interface.");
      
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
     
      // Create a new RTRCDataSource to communicate with the application.
      RTRCDataSource data_src = new RTRCDataSource(repos, subject);
      mRTRCDataSources.add(data_src);
   }

   /**
    * Returns a list of all active RTRCDataSources.
    */
   public List getDataSources()
   {
      return mRTRCDataSources;
   }   

   /** Contains all active RTRCDataSources, i.e. all applications that we can connect to */
   private List mRTRCDataSources = new ArrayList();
}
