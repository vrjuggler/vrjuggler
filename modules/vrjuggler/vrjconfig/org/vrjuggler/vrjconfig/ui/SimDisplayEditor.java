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
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.ui.PropertySheet;
import org.vrjuggler.vrjconfig.ui.placer.*;

/**
 * A component suitable for editing Simulator display windows.
 */
public class SimDisplayEditor
   extends JPanel
{
   public SimDisplayEditor()
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
      wndPlacer.setModel(model);
      wndPlacer.setRenderer(new SimDisplayRenderer());

      // Connect the display placer to the property sheet
      wndPlacer.addPlacerSelectionListener(new PlacerSelectionListener()
      {
         public void valueChanged(PlacerSelectionEvent evt)
         {
            // show the object's properties
            wndPropSheet.setBean(wndPlacer.getSelectedValue());
         }
      });
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
      wndPropSheet.setPreferredSize(new Dimension(150, 300));
      addBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            model.add(new SimDisplay(50,50));
            wndPlacer.repaint();
         }
      });
      removeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            int idx = wndPlacer.getSelectedIndex();
            if (idx != -1)
            {
               model.remove(idx);
            }
         }
      });
      this.add(placerPanel,  BorderLayout.CENTER);
      placerPanel.add(wndPlacer, BorderLayout.CENTER);
      placerPanel.add(placerToolbar,  BorderLayout.SOUTH);
      placerToolbar.add(addBtn, null);
      placerToolbar.add(removeBtn, null);
      this.add(wndPropSheet,  BorderLayout.EAST);
   }

   //--- JBuilder GUI variables ---//
   BorderLayout baseLayout = new BorderLayout();
   JPanel placerPanel = new JPanel();
   BorderLayout placerLayout = new BorderLayout();
   JToolBar placerToolbar = new JToolBar();
   PropertySheet wndPropSheet = new PropertySheet();
   JButton addBtn = new JButton();
   JButton removeBtn = new JButton();

   /**
    * Our window placer.
    */
   Placer wndPlacer = new Placer();

   /**
    * The data model for our window placer.
    */
   SimDisplayPlacerModel model = new SimDisplayPlacerModel();

   /**
    * A specialized renderer for simulator displays in the placer component.
    */
   class SimDisplayRenderer
      extends JPanel
      implements PlacerRenderer
   {
      private Placer placer;
      private int index;
      private boolean selected;
      private ImageIcon icon, scaledIcon;
      private JLabel iconLbl = new JLabel();

      public SimDisplayRenderer()
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

            g.drawImage(img, 0, 0, null);
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
class SimDisplayPlacerModel
   extends AbstractPlacerModel
   implements PropertyChangeListener
{
   /**
    * List of all displays in this model.
    */
   private List displays = new ArrayList();

   /**
    * The size of the desktop in which the windows are placed.
    */
   private Dimension mDesktopSize = new Dimension(1600, 1200);

   /**
    * Adds a new display to this model at the front.
    */
   public void add(Object obj)
   {
      SimDisplay disp = (SimDisplay)obj;
      disp.addPropertyChangeListener(this);
      displays.add(0, obj);
      fireItemsInserted(new int[] { 0 });
   }

   public void remove(int idx)
   {
      SimDisplay old = (SimDisplay)displays.remove(idx);
      old.removePropertyChangeListener(this);
      fireItemsRemoved(new int[] { idx }, new Object[] { old });
   }

   public Object getElement(int idx)
   {
      return displays.get(idx);
   }

   public int getIndexOf(Object obj)
   {
      return displays.indexOf(obj);
   }

   public Object getElementAt(Point pt)
   {
      int idx = getIndexOfElementAt(pt);
      if (idx != -1)
      {
         return displays.get(idx);
      }
      return null;
   }

   public int getIndexOfElementAt(Point pt)
   {
      for (int i=0; i<displays.size(); ++i)
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
      SimDisplay d = (SimDisplay)getElement(idx);
      return new Dimension(d.getWidth(), d.getHeight());
   }

   public void setSizeOf(int idx, Dimension size)
   {
      SimDisplay d = (SimDisplay)getElement(idx);
      d.setWidth(size.width);
      d.setHeight(size.height);
   }

   public Point getLocationOf(int idx)
   {
      SimDisplay d = (SimDisplay)getElement(idx);
      return new Point(d.getX(), d.getY());
   }

   public void setLocationOf(int idx, Point pt)
   {
      SimDisplay d = (SimDisplay)getElement(idx);
      d.setX(pt.x);
      d.setY(pt.y);
   }

   public void moveToFront(int idx)
   {
      // remove and reinsert at the front.
      displays.add(0, displays.remove(idx));
   }

   public int getSize()
   {
      return displays.size();
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

   /**
    * Called whenever one of the displays contained within the model changes.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      int idx = getIndexOf((SimDisplay)evt.getSource());
      if (idx != -1)
      {
         fireItemsChanged(new int[] { idx });
      }
   }
}
