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
package org.vrjuggler.vrjconfig;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

public class ConfigDefinitionEditor
   extends JPanel
{
   public ConfigDefinitionEditor()
   {
      // Init the GUI
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
/*
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mSaveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/save.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mSaveBtn.setText("Save");
      }
*/
   }

   /**
    * Gets a handle to the configuration broker service.
    */
   private synchronized ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         mBroker = new ConfigBrokerProxy();
      }
      return mBroker;
   }

   /**
    * JBuilder GUI initialization.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
/*
      mToolbar.add(mSaveBtn);
      mSaveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // Save everything.
            ConfigDefinition def =
               getBroker().getRepository().get("input_manager");
            try
            {
               getBroker().saveDefinition(def);
            }
            catch(Exception ex)
            {
            }
         }
      });
*/
      mToolbarPanel.setLayout(new BorderLayout());
/*
      mToolbar.setBorder(BorderFactory.createEtchedBorder());
      mToolbar.setFloatable(false);
 */     
      mTitleLbl.setBackground(UIManager.getColor("textHighlight"));
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 18));
      mTitleLbl.setForeground(UIManager.getColor("textHighlightText"));
      mTitleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      mTitleLbl.setOpaque(true);
      mTitleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      mTitleLbl.setText("VRJConfig Config Definition Editor");
      
      mToolbarPanel.add(mTitleLbl, BorderLayout.NORTH);
//      mToolbarPanel.add(mToolbar, BorderLayout.CENTER);
      
      this.add(mToolbarPanel,  BorderLayout.NORTH);
      this.add(mDefReposEditor,  BorderLayout.CENTER);
   }

   // JBuilder GUI variables
   private BorderLayout mBaseLayout = new BorderLayout();
   private JPanel mToolbarPanel = new JPanel();
   private JLabel mTitleLbl = new JLabel();
//   private JToolBar mToolbar = new JToolBar();
//   private JButton mSaveBtn = new JButton();
   private ConfigDefinitionRepositoryEditor mDefReposEditor =
      new ConfigDefinitionRepositoryEditor();

   /** A handle to the configuration broker. */
   private ConfigBroker mBroker;
}
