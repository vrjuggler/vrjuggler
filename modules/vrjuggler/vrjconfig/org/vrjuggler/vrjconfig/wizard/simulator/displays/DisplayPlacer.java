/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

package org.vrjuggler.vrjconfig.wizard.simulator.displays;

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
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.tweek.ui.PropertySheet;
import org.vrjuggler.vrjconfig.wizard.simulator.displays.placer.*;

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
         ClassLoader loader = getClass().getClassLoader();
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
      wndPlacer.setModel(model);
      wndPlacer.setRenderer(new DisplayRenderer());
   }

   public void setContext(ConfigContext context)
   {
      mContext = context;
      model.setElements(getBroker().getElements(mContext));
   }

   public ConfigContext getContext()
   {
      return mContext;
   }

   public void setDesktopSize(Dimension desktopSize)
   {
      model.setDesktopSize(desktopSize);
   }

   public Dimension getDimensionSize()
   {
      return model.getDesktopSize();
   }

   /**
    * Gets the currently selected displayWindow configuration element.
    *
    * @return  the config element for the currently selected display window;
    *          null if nothing is selected
    */
   public ConfigElement getSelectedDisplay()
   {
      return (ConfigElement)wndPlacer.getSelectedValue();
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
      ConfigDefinition display_def = getBroker().getRepository().get("display_window");
      // TODO: Compute a new, unique name for the new element
      String name = display_def.getToken();

      // Create the new window
      ConfigElementFactory factory = new ConfigElementFactory(getBroker().getRepository().getAllLatest());
      ConfigElement display_elt = factory.create(name, display_def);

      // Add the window to the database
      getBroker().add(mContext, display_elt);
   }

   /**
    * Removes the currently selected display from this configuration.
    */
   protected void removeSelectedDisplay()
   {
      int idx = wndPlacer.getSelectedIndex();
      if (idx != -1)
      {
         ConfigElement display_elt = (ConfigElement)wndPlacer.getModel().getElement(idx);
         getBroker().remove(mContext, display_elt);
      }
   }

   private ConfigBroker getBroker()
   {
      return new ConfigBrokerProxy();
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
    * Our view into the configuration.
    */
   private ConfigContext mContext = new ConfigContext();

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
         ClassLoader loader = getClass().getClassLoader();
         icon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/simulator/displays/images/graphics_window.png"));
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
{
   public void DisplayPlacerModel()
   {
      mDesktopSize = new Dimension(1600, 1200);
   }

   public void setElements(List elements)
   {
      mWindows = ConfigUtilities.getElementsWithDefinition(elements, "display_window");
      for (Iterator itr = mWindows.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         elt.addConfigElementListener(mChangeListener);
      }
//      chunkDB.addChunkDBListener(this);
//    fireTableContentsChanged();
   }

   public Object getElement(int idx)
   {
      return mWindows.get(idx);
   }

   public int getIndexOf(Object obj)
   {
      return mWindows.indexOf(obj);
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
      for (int i=0; i<mWindows.size(); ++i)
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
      ConfigElement window_elt = (ConfigElement)getElement(idx);
      int width = ((Integer)window_elt.getProperty("size", 0)).intValue();
      int height = ((Integer)window_elt.getProperty("size", 1)).intValue();
      return new Dimension(width, height);
   }

   public void setSizeOf(int idx, Dimension size)
   {
      ConfigElement window_elt = (ConfigElement)getElement(idx);
      System.out.println("--- Chunk "+window_elt.getName()+" new size=("+size.width+", "+size.height+")");
      System.out.println("\thashcode="+window_elt.hashCode());
      window_elt.setProperty("size", 0, new Integer(size.width));
      window_elt.setProperty("size", 1, new Integer(size.height));
   }

   public Point getLocationOf(int idx)
   {
      ConfigElement window_elt = (ConfigElement)getElement(idx);
      int x = ((Integer)window_elt.getProperty("origin", 0)).intValue();
      int y = ((Integer)window_elt.getProperty("origin", 1)).intValue();

      // Convert y from Juggler coords (bottom left is origin)
      int height = ((Integer)window_elt.getProperty("size", 1)).intValue();
      y = mDesktopSize.height - y - height;
      return new Point(x, y);
   }

   public void setLocationOf(int idx, Point pt)
   {
      ConfigElement window_elt = (ConfigElement)getElement(idx);

      // Convert y to Juggler coords (bottom left is origin)
      int height = ((Integer)window_elt.getProperty("size", 1)).intValue();
      int y = mDesktopSize.height - pt.y - height;

      window_elt.setProperty("origin", 0, new Integer(pt.x));
      window_elt.setProperty("origin", 1, new Integer(y));
   }

   public void moveToFront(int idx)
   {
      // remove and reinsert at the front.
      mWindows.add(0, mWindows.remove(idx));
   }

   public int getSize()
   {
      return mWindows.size();
   }

   public Dimension getDesktopSize()
   {
      return mDesktopSize;
   }

   public void setDesktopSize(Dimension desktopSize)
   {
      mDesktopSize = desktopSize;
      fireDesktopSizeChanged();
   }

   /** The list of windows in the active db. */
   private List mWindows;

   /** The size of the desktop in this model. */
   private Dimension mDesktopSize;

   /** The custom listener for changes to the displays. */
   private ChangeListener mChangeListener = new ChangeListener();

   /**
    * Custom listener for changes to the broker and the elements in the context.
    */
   private class ChangeListener
      extends ConfigElementAdapter
      implements ConfigListener
   {
      public void configElementAdded(ConfigEvent evt)
      {
         ConfigElement elt = evt.getElement();
         if (elt.getDefinition().getToken().equals("display_window"))
         {
            System.out.println("Adding a new displayWindow.");
            elt.addConfigElementListener(this);
            mWindows.add(0, elt);
            fireItemsInserted(new int[] { 0 });
         }
      }

      public void configElementRemoved(ConfigEvent evt)
      {
         ConfigElement elt = evt.getElement();
         if (elt.getDefinition().getToken().equals("display_window"))
         {
            int idx = getIndexOf(elt);
            if (idx != -1)
            {
               System.out.println("Removed a displayWindow.");
               elt.removeConfigElementListener(this);
               mWindows.remove(idx);
               fireItemsRemoved(new int[] { 0 }, new Object[] { elt });
            }
         }
      }

      /**
       * Called whenever one of the displays contained within the model changes.
       */
      public void nameChanged(ConfigElementEvent evt)
      {
         int idx = getIndexOf(evt.getSource());
         if (idx != -1)
         {
            fireItemsChanged(new int[] { idx });
         }
      }

   }
}
