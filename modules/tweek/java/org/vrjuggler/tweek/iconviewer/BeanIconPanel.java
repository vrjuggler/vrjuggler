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
 *
 * @since 1.0
 */
public class BeanIconPanel extends JPanel
{
   public BeanIconPanel (DefaultMutableTreeNode _node, String _name)
   {
      name = _name;
      init(_node);
   }

   public BeanIconPanel (DefaultMutableTreeNode _node)
   {
      name = _node.getUserObject().toString();
      init(_node);
   }

   public DefaultMutableTreeNode getNode ()
   {
      return node;
   }

   public JComponent getComponent ()
   {
      if ( node.isLeaf() )
      {
         return ((PanelBean) node.getUserObject()).getComponent();
      }
      else
      {
         return this;
      }
   }

   public String getName ()
   {
      return name;
   }

   public void addIcon (JLabel icon)
   {
      icon.setVerticalTextPosition(JLabel.BOTTOM);
      icon.setHorizontalTextPosition(JLabel.CENTER);
      icon.setHorizontalAlignment(JLabel.CENTER);
      icon.setBorder(BorderFactory.createEtchedBorder());
      icon.setMinimumSize(new Dimension(64, 64));
      icon.setPreferredSize(new Dimension(64, 64));
      icon.setMaximumSize(new Dimension(96, 96));
      this.add(icon, null);
   }

   private void init (DefaultMutableTreeNode _node)
   {
      node = _node;

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
      m_icon_layout.setAlignment(FlowLayout.LEFT);
      this.setBackground(Color.white);
      this.setLayout(m_icon_layout);
   }

   private DefaultMutableTreeNode node  = null;
   private String                 name  = null;

   private FlowLayout m_icon_layout = new FlowLayout();
}
