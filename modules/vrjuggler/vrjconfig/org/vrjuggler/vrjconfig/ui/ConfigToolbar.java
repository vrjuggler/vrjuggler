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
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.PopupButton;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class ConfigToolbar
   extends JComponent
{
   public ConfigToolbar()
   {
      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         openBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/open.gif")));
         saveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/save.gif")));
         saveAsBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveas.gif")));
         saveAllBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveall.gif")));
         expandBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/expand_toolbar.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         openBtn.setText("Open");
         saveBtn.setText("Save");
         saveAsBtn.setText("Save As");
         saveAllBtn.setText("Save All");
         expandBtn.setText("Expand");
      }

      // Get some of the icons we care about
      ImageIcon newChunkIcon = null;
      ImageIcon newDescIcon = null;
      try
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         newChunkIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/newchunk.gif"));
         newDescIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/newdesc.gif"));
      }
      catch (Exception e)
      {
         newChunkIcon = new ImageIcon();
         newDescIcon = new ImageIcon();
      }

      // Setup the new popup button
      JPopupMenu new_popup = new JPopupMenu();
      JMenuItem chunk_menu_item = new JMenuItem("New Configuration", newChunkIcon);
      JMenuItem desc_menu_item = new JMenuItem("New Config Format", newDescIcon);
      chunk_menu_item.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("New ConfigDB");
         }
      });
      desc_menu_item.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("New DescDB");
         }
      });
      new_popup.add(chunk_menu_item);
      new_popup.add(desc_menu_item);
      newPopupBtn.setPopupMenu(new_popup);
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   public void setConfigContext(ConfigContext context)
   {
      this.context = context;
   }

   public ConfigContext getConfigContext()
   {
      return context;
   }

   public void addActionListener(ActionListener listener)
   {
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   protected void fireAction(String command)
   {
      ActionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            if (evt == null)
            {
               evt = new ActionEvent(this, 0, command);
            }
            ((ActionListener)listeners[i+1]).actionPerformed(evt);
         }
      }
   }

   protected void toggleContextEditor()
   {
      boolean show_editor = expandBtn.isSelected();
      EditContextPopup contextEditor = getContextEditor();
      if (! show_editor)
      {
         contextEditor.setVisible(false);
         contextEditor.setPreferredSize(null);
      }
      else
      {
         contextEditor.setConfigContext(getConfigContext());
         Dimension pref_size = contextEditor.getPreferredSize();
         pref_size.width = this.getWidth();
         contextEditor.setPreferredSize(pref_size);
         contextEditor.setBackground(Color.pink);
         contextEditor.show(this, 0, this.getHeight());
      }
   }

   private EditContextPopup getContextEditor()
   {
      if (contextEditor == null)
      {
         contextEditor = new EditContextPopup();
      }
      return contextEditor;
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      titleLbl.setBackground(UIManager.getColor("textHighlight"));
      titleLbl.setFont(new java.awt.Font("Serif", 1, 18));
      titleLbl.setForeground(Color.black);
      titleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      titleLbl.setOpaque(true);
      titleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      titleLbl.setText("VRJConfig");
      toolbar.setBorder(BorderFactory.createEtchedBorder());
      toolbar.setFloatable(false);
      openBtn.setToolTipText("Open Configuration");
      openBtn.setActionCommand("Open");
      openBtn.setFocusPainted(false);
      saveBtn.setToolTipText("Save Configuration");
      saveBtn.setActionCommand("Save");
      saveBtn.setFocusPainted(false);
      saveAsBtn.setToolTipText("Save Configuration As");
      saveAsBtn.setActionCommand("SaveAs");
      saveAsBtn.setFocusPainted(false);
      saveAllBtn.setToolTipText("Save All Open Configurations");
      saveAllBtn.setActionCommand("SaveAll");
      saveAllBtn.setFocusPainted(false);
      expandBtn.setToolTipText("Expand Toolbar");
      expandBtn.setActionCommand("Expand");
      expandBtn.setFocusPainted(false);
      openBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("Open");
         }
      });
      saveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("Save");
         }
      });
      saveAsBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("SaveAs");
         }
      });
      saveAllBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction("SaveAll");
         }
      });
      expandBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            toggleContextEditor();
         }
      });
      this.add(titleLbl, BorderLayout.NORTH);
      this.add(toolbar, BorderLayout.CENTER);
      toolbar.add(newPopupBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.add(saveAllBtn, null);
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(expandBtn, null);
   }

   // JBuilder GUI variables
   private JLabel titleLbl = new JLabel();
   private PopupButton newPopupBtn = new PopupButton();
   private JToolBar toolbar = new JToolBar();
   private JButton openBtn = new JButton();
   private JButton saveBtn = new JButton();
   private JButton saveAsBtn = new JButton();
   private JButton saveAllBtn = new JButton();
   private JToggleButton expandBtn = new JToggleButton();

   private ConfigContext context = new ConfigContext();
   private EditContextPopup contextEditor;
}
