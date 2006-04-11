/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
package org.vrjuggler.vrjconfig.wizards.cluster;

import java.util.*;
import javax.swing.*;

import org.vrjuggler.tweek.wizard.*;

/**
 * Creates the clustered simulator configuration.
 */
public class CreateClusteredSimDevicesStep
   extends AbstractWizardStep
   implements WizardPane
{
   /**
    * Creates a new CreateClusteredSimDevicesStep with nothing.
    */
   public CreateClusteredSimDevicesStep()
   {
      cwp = null;
   }

   /**
    * Gets the GUI component for this pane.
    */
   public JComponent getGUI()
   {
      if (cwp == null)
      {
         // If this is the first time on this step...
         cwp = new CreateClusteredSimDevicesPanel();
         cwp.init(mWhiteBoard);
         cwp.copySimFilesToContext();
         cwp.createDisplayElements();
      }
      cwp.loadLists();
      return cwp;
   }

   public boolean onExiting()
   {
     return(cwp.saveFile());
   }

   public List getRequiredClasses()
   {
      List classes = new ArrayList();
      classes.add(CreateClusteredSimDevicesPanel.class.getName());
      classes.add(CreateClusteredSimDevicesPanel.class.getName()+"$1");
      classes.add(CreateClusteredSimDevicesPanel.class.getName()+"$2");
      classes.add(CreateClusteredSimDevicesPanel.class.getName()+"$3");
      return classes;
   }

   public List getRequiredResources()
   {
      List res = new ArrayList();
      res.add("org/vrjuggler/vrjconfig/wizard/cluster/images/ClusterIcon.png");
      return res;
   }

   /**
    * The JPanel that makes up this pane's UI.
    */
   private transient CreateClusteredSimDevicesPanel cwp;
   private String mFileSourceName = new String("DefaultFileSource");
   private String mConfigFilePath =
       new String("${VJ_BASE_DIR}/share/vrjuggler/data/configFiles/");
   private String mHeadEventWindowName = "Head Event Window";
   private String mSimViewEventWindowName = "Sim View Cameras Control";
   private String mWandEventWindowName = "Wand Event Window";
}
