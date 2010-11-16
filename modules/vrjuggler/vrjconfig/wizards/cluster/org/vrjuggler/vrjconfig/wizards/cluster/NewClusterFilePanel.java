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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.wizards.cluster;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;

public class NewClusterFilePanel extends JPanel
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel centerPnl = new JPanel();

   private DefaultListModel lstModelNodes = new DefaultListModel();

   private ConfigContext mContext = null;
   private FileDataSource mFileSource = null;
   private ConfigBroker mBroker = null;
   private JPanel directionsPanel = new JPanel();
   private JFileChooser mFileChooser = new JFileChooser();
   private Box mMenuBox2;
   private Component spacer;
   private JButton browse = new JButton();
   private BorderLayout centerPanelBorderLayout = new BorderLayout();
   private JTextField textFilename = new JTextField();
   private JPanel jPanel1 = new JPanel();
   private BorderLayout borderLayout1 = new BorderLayout();
   private JPanel jPanel2 = new JPanel();
   private JPanel jPanel3 = new JPanel();
   private JLabel lblTitle = new JLabel();
   private JLabel lblDirections = new JLabel();
   private JLabel jLabel2 = new JLabel();
   private Component component1;

   private Map mWhiteBoard;

   public NewClusterFilePanel()
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
         jLabel2.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/wizards/cluster/images/ClusterIcon.png")));
      }
      catch(NullPointerException exp)
      {
         jLabel2.setText("");
      }
   }

   private void jbInit() throws Exception
   {
      component1 = Box.createVerticalStrut(150);
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      this.setLayout(baseLayout);
      directionsPanel.setBorder(BorderFactory.createEtchedBorder());
      lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
      lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
      lblTitle.setText("New Cluster Config File");
      lblDirections.setText("Select the file that you want to create.");
      jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
      this.add(centerPnl, BorderLayout.SOUTH);
      mMenuBox2 = Box.createHorizontalBox();
      spacer = Box.createHorizontalStrut(8);
      directionsPanel.setLayout(borderLayout1);
      browse.setText("Browse...");
      browse.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               //addNode_actionPerformed(e);
               browse();
            }
         });
      centerPnl.setLayout(centerPanelBorderLayout);
      this.add(directionsPanel, BorderLayout.NORTH);
      directionsPanel.add(jPanel2,  BorderLayout.CENTER);
      jPanel2.add(lblTitle, null);
      jPanel2.add(lblDirections, null);
      directionsPanel.add(jPanel3,  BorderLayout.EAST);
      jPanel3.add(jLabel2, null);
      this.add(jPanel1,  BorderLayout.CENTER);
      centerPnl.add(mMenuBox2, BorderLayout.CENTER);
      mMenuBox2.add(browse, null);
      mMenuBox2.add(spacer, null);
      mMenuBox2.add(textFilename, null);
      centerPnl.add(component1,  BorderLayout.SOUTH);
   }

   private void browse()
   {
      int result =
         mFileChooser.showSaveDialog(SwingUtilities.getAncestorOfClass(Frame.class,
                                                                       this));
      if (JFileChooser.APPROVE_OPTION == result)
      {
         String filename = mFileChooser.getSelectedFile().getAbsolutePath();
         textFilename.setText(filename);
      }
   }
   
   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      this.textFilename.setText("");
   }
   public boolean loadFile()
   {
      java.io.File temp_file = new java.io.File(textFilename.getText());

      if(temp_file.exists())
      {
         int result = JOptionPane.showConfirmDialog(this,"Do you want to load this existing file?.",
                                                    "ClusterWizard", JOptionPane.YES_NO_OPTION);
         if (JOptionPane.NO_OPTION == result)
         {
            return false;
         }
         else if (!temp_file.canWrite())
         {
            JOptionPane.showMessageDialog(this,"You can not write to this.","ClusterWizard",JOptionPane.ERROR_MESSAGE);
            return false;
         }
         // We have gotten a positive response so try to create it below
      }

      try
      {
         // Get handle to broker
         mBroker = new ConfigBrokerProxy();

         // Create New FileSource
         mWhiteBoard.put("datasource.name", "DefaultFileSource");
         if (!mBroker.containsDataSource((String)mWhiteBoard.get("datasource.name")))
         {
            mFileSource = FileDataSource.create(textFilename.getText(), mBroker.getRepository());
            mBroker.add( (String)mWhiteBoard.get("datasource.name"), mFileSource);
         }
      }
      catch (java.io.IOException exp)
      {
         exp.printStackTrace();
         return false;
      }
      return true;
   }
}
