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

package org.vrjuggler.tweek.iconviewer;

import javax.swing.tree.TreeModel;
import java.awt.*;
import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeNode;
import org.vrjuggler.tweek.beans.PanelBean;


/**
 * Container for a collection of icons representing Beans or other
 * BeanIconPanel instances with more Beans.
 */
public class BeanIconPanel
{
   public BeanIconPanel (DefaultMutableTreeNode node, String name)
   {
      this.name = name;
      init(node);
   }

   public BeanIconPanel (DefaultMutableTreeNode node)
   {
      name = node.getUserObject().toString();
      init(node);
   }

   public DefaultMutableTreeNode getNode ()
   {
      return node;
   }

   public JComponent getComponent ()
   {
      if ( node.isLeaf() && node.getUserObject() != null )
      {
         return ((PanelBean) node.getUserObject()).getComponent();
      }
      else
      {
         return mPanel;
      }
   }

   public PanelBean getBean ()
   {
      if ( node.isLeaf() )
      {
         return (PanelBean) node.getUserObject();
      }
      else
      {
         return null;
      }
   }

   public String getName ()
   {
      return name;
   }

   public void addIcon (JButton icon)
   {
      icon.setBackground(Color.white);
      icon.setVerticalTextPosition(JLabel.BOTTOM);
      icon.setHorizontalTextPosition(JLabel.CENTER);
      icon.setHorizontalAlignment(JLabel.CENTER);
      icon.setMinimumSize(new Dimension(64, 64));
      icon.setPreferredSize(new Dimension(64, 64));
      icon.setMaximumSize(new Dimension(96, 96));
      mPanel.add(icon, null);
   }

   private void init (DefaultMutableTreeNode node)
   {
      this.node = node;

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
   }

   private void jbInit() throws Exception
   {
      mIconLayout.setAlignment(FlowLayout.LEFT);
      mPanel.setBackground(Color.white);
      mPanel.setLayout(mIconLayout);
   }

   private DefaultMutableTreeNode node  = null;
   private String                 name  = null;

   private JPanel     mPanel      = new JPanel();
   private FlowLayout mIconLayout = new FlowLayout();
}
