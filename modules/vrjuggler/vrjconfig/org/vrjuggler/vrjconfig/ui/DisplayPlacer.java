/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
package org.vrjuggler.vrjconfig.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.ui.PropertySheet;
import org.vrjuggler.vrjconfig.ui.placer.*;

/**
 * A component suitable for placing display windows.
 */
public class DisplayPlacer
   extends JPanel
{
   public DisplayPlacer()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // get the icons
      try
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         addBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/add24.gif")));
         removeBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/remove24.gif")));
      }
      catch (NullPointerException npe)
      {
         // Couldn't load the icons, use text instead
         addBtn.setText("Add");
         removeBtn.setText("Remove");
      }

      // Setup the display placer
      model.setConfigChunkDB(getConfigManager().getActiveConfig());
      wndPlacer.setModel(model);
      wndPlacer.setRenderer(new DisplayRenderer());
   }

   public void setDesktopSize(Dimension desktopSize)
   {
      wndPlacer.setDesktopSize(desktopSize);
   }

   public Dimension getDimensionSize()
   {
      return wndPlacer.getDesktopSize();
   }

   /**
    * Gets the currently selected displayWindow config chunk.
    *
    * @return  the config chunk for the currently selected display window;
    *          null if nothing is selected
    */
   public ConfigChunk getSelectedDisplay()
   {
      return (ConfigChunk)wndPlacer.getSelectedValue();
   }

   /**
    * Gets the placer used internally by this display placer.
    */
   public Placer getPlacer()
   {
      return wndPlacer;
   }

   /**
    * Adds a new display to the configuration.
    */
   protected void addNewDisplay()
   {
      // Create the new window
      ChunkDescDB all_descs = getConfigManager().getAllChunkDescs();
      ChunkDesc display_desc = (ChunkDesc)ConfigUtilities.getDescsWithToken(
                                    all_descs.getAll(), "displayWindow").get(0);
      ConfigChunk display_chunk = new ConfigChunk(display_desc);

      // Add the window to the database
      ConfigChunkDB active_config = getConfigManager().getActiveConfig();
      String name = active_config.getNewName(display_desc.getToken());
      display_chunk.setName(name);
      active_config.add(display_chunk);
   }

   /**
    * Removes the currently selected display from this configuration.
    */
   protected void removeSelectedDisplay()
   {
      int idx = wndPlacer.getSelectedIndex();
      if (idx != -1)
      {
         ConfigChunk display_chunk = (ConfigChunk)wndPlacer.getModel().getElement(idx);
         getConfigManager().getActiveConfig().remove(display_chunk);
      }
   }

   private ConfigManagerService getConfigManager()
   {
      return (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();
   }

   /**
    * Automatically generated JBuilder GUI init.
    */
   private void jbInit()
      throws Exception
   {
      placerPanel.setLayout(placerLayout);
      this.setLayout(baseLayout);
      placerPanel.setBorder(BorderFactory.createEtchedBorder());
      wndPlacer.setBorder(BorderFactory.createEtchedBorder());
      placerToolbar.setFloatable(false);
      addBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addNewDisplay();
         }
      });
      removeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeSelectedDisplay();
         }
      });
      this.add(placerPanel,  BorderLayout.CENTER);
      placerPanel.add(wndPlacer, BorderLayout.CENTER);
      placerPanel.add(placerToolbar,  BorderLayout.SOUTH);
      placerToolbar.add(addBtn, null);
      placerToolbar.add(Box.createHorizontalStrut(8), null);
      placerToolbar.add(removeBtn, null);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private JPanel placerPanel = new JPanel();
   private BorderLayout placerLayout = new BorderLayout();
   private JToolBar placerToolbar = new JToolBar();
   private JButton addBtn = new JButton();
   private JButton removeBtn = new JButton();

   /**
    * Our window placer.
    */
   private Placer wndPlacer = new Placer();

   /**
    * The data model for our window placer.
    */
   private DisplayPlacerModel model = new DisplayPlacerModel();

   /**
    * A specialized renderer for displays in the placer component.
    */
   class DisplayRenderer
      extends JPanel
      implements PlacerRenderer
   {
      private Placer placer;
      private int index;
      private boolean selected;
      private ImageIcon icon, scaledIcon;
      private JLabel iconLbl = new JLabel();

      public DisplayRenderer()
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         icon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/graphics_window.png"));
         scaledIcon = new ImageIcon();
      }

      public Component getPlacerRendererComponent(Placer placer,
                                                  Object value,
                                                  boolean selected,
                                                  boolean focused,
                                                  int index)
      {
         this.placer = placer;
         this.index = index;
         this.selected = selected;
         if (selected)
         {
            setForeground(placer.getSelectionForeground());
            setBackground(placer.getSelectionBackground());
         }
         else
         {
            setForeground(placer.getSelectionBackground());
            setBackground(placer.getSelectionForeground());
         }

         return this;
      }

      public void paintComponent(Graphics g)
      {
         super.paintComponent(g);
         if (placer != null && index >= 0)
         {
            Point pos = placer.getModel().getLocationOf(index);
            Dimension dim = placer.getModel().getSizeOf(index);

            // Check if we need to update the scaled image
            Image img = scaledIcon.getImage();
            if ((scaledIcon.getIconWidth() != dim.width) ||
                (scaledIcon.getIconHeight() != dim.height))
            {
               img = icon.getImage().getScaledInstance(dim.width, dim.height, Image.SCALE_DEFAULT);
               scaledIcon.setImage(img);
            }

            // Sanity check in case our scale failed
            if (img != null)
            {
               g.drawImage(img, 0, 0, null);
            }

            // Highlight the window nicely
            if (selected)
            {
               g.setColor(getBackground());
            }
            else
            {
               g.setColor(Color.white);
            }
            g.drawRect(0, 0, dim.width-1, dim.height-1);
            g.fillRect(0, 0, dim.width, 3);
         }
      }
   }
}

/**
 * A specialized placer model for our sim displays.
 */
class DisplayPlacerModel
   extends AbstractPlacerModel
   implements ChunkDBListener, ConfigChunkListener
{
   public void setConfigChunkDB(ConfigChunkDB db)
   {
      chunkDB = db;
      windows = ConfigUtilities.getChunksWithDescToken(chunkDB.getAll(),
                                                       "displayWindow");
      for (Iterator itr = windows.iterator(); itr.hasNext(); )
      {
         ConfigChunk chunk = (ConfigChunk)itr.next();
         chunk.addConfigChunkListener(this);
      }
      chunkDB.addChunkDBListener(this);
//      fireTableContentsChanged();
   }

   public Object getElement(int idx)
   {
      return windows.get(idx);
   }

   public int getIndexOf(Object obj)
   {
      return windows.indexOf(obj);
   }

   public Object getElementAt(Point pt)
   {
      int idx = getIndexOfElementAt(pt);
      if (idx != -1)
      {
         return getElement(idx);
      }
      return null;
   }

   public int getIndexOfElementAt(Point pt)
   {
      for (int i=0; i<windows.size(); ++i)
      {
         Point pos = getLocationOf(i);
         Dimension dim = getSizeOf(i);
         if (pt.x >= pos.x && pt.y >= pos.y &&
             pt.x < (pos.x + dim.width) && pt.y < (pos.y + dim.height))
         {
            return i;
         }
      }
      return -1;
   }

   public Dimension getSizeOf(int idx)
   {
      ConfigChunk window_chunk = (ConfigChunk)getElement(idx);
      int width = window_chunk.getProperty("size", 0).getInt();
      int height = window_chunk.getProperty("size", 1).getInt();
      return new Dimension(width, height);
   }

   public void setSizeOf(int idx, Dimension size)
   {
      ConfigChunk window_chunk = (ConfigChunk)getElement(idx);
      window_chunk.setProperty("size", 0, new VarValue(size.width));
      window_chunk.setProperty("size", 1, new VarValue(size.height));
   }

   public Point getLocationOf(int idx)
   {
      ConfigChunk window_chunk = (ConfigChunk)getElement(idx);
      int x = window_chunk.getProperty("origin", 0).getInt();
      int y = window_chunk.getProperty("origin", 1).getInt();
      return new Point(x, y);
   }

   public void setLocationOf(int idx, Point pt)
   {
      ConfigChunk window_chunk = (ConfigChunk)getElement(idx);
      window_chunk.setProperty("origin", 0, new VarValue(pt.x));
      window_chunk.setProperty("origin", 1, new VarValue(pt.y));
   }

   public void moveToFront(int idx)
   {
      // remove and reinsert at the front.
      windows.add(0, windows.remove(idx));
   }

   public int getSize()
   {
      return windows.size();
   }

   public void configChunkAdded(ChunkDBEvent evt)
   {
      ConfigChunk chunk = evt.getChunk();
      if (chunk.getDesc().getToken().equals("displayWindow"))
      {
         System.out.println("Adding a new displayWindow.");
         chunk.addConfigChunkListener(this);
         windows.add(0, chunk);
         fireItemsInserted(new int[] { 0 });
      }
   }

   public void configChunkRemoved(ChunkDBEvent evt)
   {
      ConfigChunk chunk = evt.getChunk();
      if (chunk.getDesc().getToken().equals("displayWindow"))
      {
         int idx = getIndexOf(chunk);
         if (idx != -1)
         {
            System.out.println("Removed a displayWindow.");
            chunk.removeConfigChunkListener(this);
            windows.remove(idx);
            fireItemsRemoved(new int[] { 0 }, new Object[] { chunk });
         }
      }
   }

   public void configChunkReplaced(ChunkDBEvent evt)
   {
   }

   public void configChunksCleared(ChunkDBEvent evt)
   {
      int[] idxs = new int[windows.size()];
      Object[] items = new Object[windows.size()];
      for (int i=0; i<windows.size(); ++i)
      {
         ConfigChunk chunk = (ConfigChunk)windows.get(i);
         chunk.removeConfigChunkListener(this);
         idxs[i] = i;
         items[i] = chunk;
      }
      windows.clear();
      fireItemsRemoved(idxs, items);
   }

   /**
    * Called whenever one of the displays contained within the model changes.
    */
   public void nameChanged(ConfigChunkEvent evt)
   {
      int idx = getIndexOf(evt.getSource());
      if (idx != -1)
      {
         fireItemsChanged(new int[] { idx });
      }
   }

   public void propertyValueChanged(ConfigChunkEvent evt) { nameChanged(evt); }
   public void propertyValueAdded(ConfigChunkEvent evt) { nameChanged(evt); }
   public void propertyValueRemoved(ConfigChunkEvent evt) { nameChanged(evt); }

   /**
    * The ConfigChunkDB we're working off of.
    */
   private ConfigChunkDB chunkDB;

   /**
    * The list of windows in the active db.
    */
   private List windows;
}
