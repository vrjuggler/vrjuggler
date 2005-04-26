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
import java.awt.Component;
import java.awt.event.*;
import java.awt.GridLayout;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import javax.swing.*;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;

public class ChooseClusterPanel extends JPanel
{
   private JPanel mDirectionsPanel = new JPanel(new BorderLayout());
   private JLabel mTitleLbl = new JLabel();
   private JTextArea mDirectionsLbl = new JTextArea();
   private JPanel mTopPanel = new JPanel();
   private JLabel mDeviceIcon = new JLabel();
   private JPanel mClusterPanel = new JPanel();
   private ButtonGroup mClusterGroup = new ButtonGroup();
   private JRadioButton mNoClusterBtn = new JRadioButton();
   private JRadioButton mClusterBtn = new JRadioButton();
   private Map mWhiteBoard;

   public ChooseClusterPanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

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

   private void jbInit() throws Exception
   {
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.CENTER);
      mTitleLbl.setText("Virtual Reality Configuration Wizard");
      mDirectionsLbl.setText("Select the type of VR system you want to configure.");
      mDirectionsLbl.setLineWrap(true);
      mDirectionsLbl.setEditable(false);
      mDirectionsLbl.setBackground(mTitleLbl.getBackground());

      mDirectionsPanel.add(mTitleLbl, BorderLayout.NORTH);
      mDirectionsPanel.add(mDirectionsLbl, BorderLayout.CENTER);

      mTopPanel.setLayout(new BorderLayout());
      mTopPanel.setBorder(BorderFactory.createEtchedBorder());
      mTopPanel.add(mDirectionsPanel, BorderLayout.CENTER);
      mTopPanel.add(mDeviceIcon, BorderLayout.EAST);

      setLayout(new BorderLayout());
      add(mTopPanel, BorderLayout.NORTH);
      add(mClusterPanel, BorderLayout.CENTER);
      
      // Put the radio buttons in a column in a panel.
      mClusterPanel.setLayout(new GridLayout(0, 1));
      mClusterGroup.add(mNoClusterBtn);
      mClusterGroup.add(mClusterBtn);
      mClusterPanel.add(mNoClusterBtn);
      mClusterPanel.add(mClusterBtn);
      
      mNoClusterBtn.setSelected(true);
      
      mClusterBtn.setText("Cluster configuration.");
      mClusterBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mClusterBtn_actionPerformed(e);
         }
      });

      mNoClusterBtn.setText("Standalone configuration.");
      mNoClusterBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mNoClusterBtn_actionPerformed(e);
         }
      });
   }
   
   void mClusterBtn_actionPerformed(ActionEvent e)
   {
      mWhiteBoard.put("cluster_configuration", new Boolean(true));
   }
   
   void mNoClusterBtn_actionPerformed(ActionEvent e)
   {
      mWhiteBoard.put("cluster_configuration", new Boolean(false));
   }
      
   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      mWhiteBoard.put("cluster_configuration", new Boolean(false));
   }
}
