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
package org.vrjuggler.vrjconfig.wizard;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.tweek.ui.PropertySheet;
import org.vrjuggler.vrjconfig.ui.Placer;
import org.vrjuggler.vrjconfig.ui.placer.*;

/**
 * Allows the user to move components about using Drag-and-Drop semantics. Such
 * a panel is useful for allowing the user to specify where simulator windows
 * should be placed and how they should be sized.
 * <p>
 * Components placed within this panel are moveable using the mouse.
 */
public class SimDisplayPane
   extends AbstractWizardStep
   implements WizardPane
{
   /**
    * Let the WizardStep factory know we exist.
    */
   static
   {
      WizardStepFactory.registerWizardStep(SimDisplayPane.class);
   }

   public JComponent getGUI()
   {
      if (panel == null)
      {
         panel = new SimDisplayPanePanel();
      }
      return panel;
   }

   public void setNumDisplays(int numDisplays)
   {
      int oldValue = this.numDisplays;
      this.numDisplays = numDisplays;
//      firePropertyChange("numDisplays", oldValue, numDisplays);
   }

   public int getNumDisplays()
   {
      return numDisplays;
   }

   public void setBlah(boolean blah)
   {
      this.blah = blah;
   }

   public boolean getBlah()
   {
      return blah;
   }

   private boolean blah = false;

   /**
    * The number of displays this panel is managing.
    */
   private int numDisplays = 1;

   private transient SimDisplayPanePanel panel = null;
}

class SimDisplayPanePanel
   extends JPanel
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel placerPanel = new JPanel();
   BorderLayout placerLayout = new BorderLayout();
   JToolBar placerToolbar = new JToolBar();
   PropertySheet wndPropSheet = new PropertySheet();
   JButton addBtn = new JButton();

   /**
    * Our window placer.
    */
   Placer wndPlacer = new Placer();

   /**
    * The data model for our window placer.
    */
   SimDisplayPlacerModel model = new SimDisplayPlacerModel();

   public SimDisplayPanePanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      ClassLoader loader = BeanJarClassLoader.instance();
      addBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/images/add24.gif")));

      // Setup the display placer
      wndPlacer.setModel(model);

      // Connect the display placer to the property sheet
      wndPlacer.addPlacerSelectionListener(new PlacerSelectionListener()
      {
         public void valueChanged(PlacerSelectionEvent evt)
         {
            System.out.println("Selected: "+wndPlacer.getSelectedIndex());
            // show the object's properties
            wndPropSheet.setBean(wndPlacer.getSelectedValue());
         }
      });
   }

   private void jbInit() throws Exception
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
      this.add(placerPanel,  BorderLayout.CENTER);
      placerPanel.add(wndPlacer, BorderLayout.CENTER);
      placerPanel.add(placerToolbar,  BorderLayout.SOUTH);
      placerToolbar.add(addBtn, null);
      this.add(wndPropSheet,  BorderLayout.EAST);
   }
}

/**
 * A specialized placer model for our sim displays.
 */
class SimDisplayPlacerModel
   extends AbstractPlacerModel
{
   /**
    * List of all displays in this model.
    */
   private List displays = new ArrayList();

   /**
    * Adds a new display to this model at the front.
    */
   public void add(Object obj)
   {
      displays.add(0, obj);
//      fireItemInserted(0, obj);
   }

   public void remove(int idx)
   {
      Object old = displays.remove(idx);
//      fireItemRemoved(idx, old);
   }

   public Object getElement(int idx)
   {
      return displays.get(idx);
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
}
