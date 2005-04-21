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
package org.vrjuggler.vrjconfig.wizard.vrsystem;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.*;
import java.awt.Frame;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.controlpanel.CustomEditorDialog;
import org.vrjuggler.vrjconfig.controlpanel.WizardLoader;
import info.clearthought.layout.*;
import org.vrjuggler.vrjconfig.customeditors.cave.*;

public class ScreenPanel extends JPanel implements EditorConstants
{
   private JPanel mBottomPanel = new JPanel();
   private TableLayout mBottomPanelLayout = null;
   private JLabel mDeviceIcon = new JLabel();
   private JPanel mDirectionsPanel = new JPanel();
   private JLabel mTitleLbl = new JLabel();
   private JLabel mDirectionsLbl = new JLabel();
   private JPanel mTopPanel = new JPanel();
   private ConfigContext mConfigContext = null;
   private Map mWhiteBoard = null;

   private ScreenDisplay mScreenDisplay = new ScreenDisplay();
   private CaveModel mCaveModel = null;

   public ScreenPanel()
   {
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mDeviceIcon.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/devices64.png")));
      }
      catch(NullPointerException exp)
      {
         mDeviceIcon.setText("");
      }
   }

   public void setClusterConfig(boolean clusterConfig)
   {
      mScreenDisplay.setClusterConfig(clusterConfig);
   }
   
   private void jbInit() throws Exception
   {
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      mTitleLbl.setText("User Settings");
      mDirectionsLbl.setText("In order for VR Juggler to know how to "
            + "display graphics from the users point of view, you must "
            + "set the following values.");
      
      mDirectionsPanel.add(mTitleLbl, null);
      mDirectionsPanel.add(mDirectionsLbl, null);

      mTopPanel.setLayout(new BorderLayout());
      mTopPanel.setBorder(BorderFactory.createEtchedBorder());
      mTopPanel.add(mDirectionsPanel, BorderLayout.CENTER);
      mTopPanel.add(mDeviceIcon, BorderLayout.EAST);
      
      setLayout(new BorderLayout());
      add(mTopPanel, BorderLayout.NORTH);
      add(mScreenDisplay, BorderLayout.CENTER);
   }
   
   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      
      if (null == mWhiteBoard)
      {
         System.out.println("WhiteBoard null.");
      }
      mConfigContext = (ConfigContext)mWhiteBoard.get("context");

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
      
      mCaveModel = new CaveModel(mConfigContext);

      mWhiteBoard.put("CaveModel", mCaveModel);
      mScreenDisplay.setConfig(mConfigContext, mCaveModel);
   }

   /** Reference to the ConfigBroker used in this object. */
   private ConfigBroker mBroker = null;

   /**
    * Gets a handle to the configuration broker.
    */
   private ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }
}
