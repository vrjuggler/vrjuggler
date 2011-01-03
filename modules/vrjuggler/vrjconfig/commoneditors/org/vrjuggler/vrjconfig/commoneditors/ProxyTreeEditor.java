/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.BorderLayout;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;

import org.vrjuggler.vrjconfig.commoneditors.proxytree.*;


public class ProxyTreeEditor extends JPanel
{
    /** Toolbar for controling ProxyTree. */
   private JToolBar mToolbar = new JToolBar(); 
   private JScrollPane mProxyTreeScrollPane = new JScrollPane();
   
   private JButton mCreateBtn = new JButton();
   private JButton mRemoveBtn = new JButton();
   private JButton mAttachBtn = new JButton();
   private JButton mDisconnectBtn = new JButton();

   public ProxyTreeEditor(ProxyTree proxy_tree)
   {
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mCreateBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Add16.gif")));
         mRemoveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Delete16.gif")));
         mAttachBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Import16.gif")));
         mDisconnectBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Export16.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mCreateBtn.setText("New");
         mRemoveBtn.setText("Delete");
         mAttachBtn.setText("Attach");
         mDisconnectBtn.setText("Disconnect");
      }

      mCreateBtn.setToolTipText("Create New Proxy");
      mCreateBtn.setActionCommand("create");
      mCreateBtn.setFocusPainted(false);
      mRemoveBtn.setToolTipText("Remove Proxy");
      mRemoveBtn.setActionCommand("remove");
      mRemoveBtn.setFocusPainted(false);
      mAttachBtn.setToolTipText("Attach Existing Proxy");
      mAttachBtn.setActionCommand("attach");
      mAttachBtn.setFocusPainted(false);
      mDisconnectBtn.setToolTipText("Disconnect Proxy");
      mDisconnectBtn.setActionCommand("disconnect");
      mDisconnectBtn.setFocusPainted(false);

      mCreateBtn.addActionListener(proxy_tree);
      mRemoveBtn.addActionListener(proxy_tree);
      mAttachBtn.addActionListener(proxy_tree);
      mDisconnectBtn.addActionListener(proxy_tree);

      mToolbar.add(mCreateBtn, null);
      mToolbar.add(mRemoveBtn, null);
      mToolbar.add(mAttachBtn, null);
      mToolbar.add(mDisconnectBtn, null);
      
      mProxyTreeScrollPane.getViewport().add(proxy_tree, null);
      mProxyTreeScrollPane.setBorder(BorderFactory.createLoweredBevelBorder());

      Border lowered = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
      TitledBorder proxy_title = BorderFactory.createTitledBorder(lowered, "Station Proxys");
      proxy_title.setTitlePosition(TitledBorder.TOP);
      this.setBorder(proxy_title);

      this.setLayout(new BorderLayout());
      this.add(mProxyTreeScrollPane, BorderLayout.CENTER);
      this.add(mToolbar, BorderLayout.NORTH);
   }
}
