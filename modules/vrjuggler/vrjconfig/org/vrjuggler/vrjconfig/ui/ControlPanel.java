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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

/**
 * This panel lays out icons within it WinXP control panel style.
 */
public class ControlPanel
   extends JComponent
{
   /**
    * Creates a new ControlPanel with an empty model.
    */
   public ControlPanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Init the model to something useful
      model = new DefaultControlPanelModel();

      // Setup the model listener and do the initial hookup
      model.addListDataListener(modelListener);
   }

   /**
    * Sets the data model this ControlPanel should use.
    */
   public void setModel(ControlPanelModel model)
   {
      if (model == null)
      {
         throw new IllegalArgumentException("model cannot be null");
      }
      ControlPanelModel old = this.model;
      this.model.removeListDataListener(modelListener);
      this.model = model;
      this.model.addListDataListener(modelListener);
      firePropertyChange("model", old, model);
      rebuildUI();
   }

   /**
    * Gets the data model this ControlPanel is using.
    */
   public ControlPanelModel getModel()
   {
      return model;
   }

   /**
    * Sets the title used in the control panel.
    */
   public void setTitle(String title)
   {
      String old = getTitle();
      titleLbl.setText(title);
      firePropertyChange("title", old, title);
   }

   /**
    * Gets the title used in the control panel.
    */
   public String getTitle()
   {
      return titleLbl.getText();
   }

   /**
    * Sets the watermark.
    */
   public void setWatermark(Icon watermark)
   {
      Icon old = getWatermark();
      this.watermark.setIcon(watermark);
      firePropertyChange("watermark", old, watermark);
   }

   /**
    * Gets the watermark.
    */
   public Icon getWatermark()
   {
      return watermark.getIcon();
   }

   public void addActionListener(ActionListener listener)
   {
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   protected void fireAction(String command, int id)
   {
      ActionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            if (evt == null)
            {
               evt = new ActionEvent(this, id, command);
            }
            ((ActionListener)listeners[i+1]).actionPerformed(evt);
         }
      }
   }

//   protected void paintComponent(Graphics g)
//   {
//      // Do the normal paint thing
//      super.paintComponent(g);
//
//      // Paint the watermark first if we have one
//      if (watermark != null)
//      {
//         int w = watermark.getIconWidth();
//         int h = watermark.getIconHeight();
//         Dimension size = this.getSize();
//
//         System.out.println("Drawing the watermark at ("+(size.width-w)+", "+(size.height - h)+")");
//         g.drawImage(watermark.getImage(),
//                     size.width - w,
//                     size.height - h,
//                     null);
//      }
//   }

   private void rebuildUI()
   {
      // First clear the listener status on all the component we don't care
      // about anymore
      for (int i=0; i<iconPanel.getComponentCount(); ++i)
      {
         ((ControlPanelItem)iconPanel.getComponent(i)).removeActionListener(actionListener);
      }

      // Remove all the old items
      iconPanel.removeAll();

      // Add in the new items
      for (int i=0; i<model.getSize(); ++i)
      {
         ControlPanelItem item = new ControlPanelItem(model.getElementAt(i),
                                                      model.getIconAt(i));
         item.addActionListener(actionListener);
         iconPanel.add(item);
      }
      iconPanel.revalidate();
      iconPanel.repaint();
   }

   public void setSize(Dimension newSize)
   {
      // Paint the watermark first if we have one
      if (watermark != null)
      {
         int w = watermark.getIcon().getIconWidth();
         int h = watermark.getIcon().getIconHeight();
         watermark.setLocation(newSize.width - w, newSize.height - h);
      }

      super.setSize(newSize);
   }

   /**
    * JBuilder automatically generated UI initialization.
    */
   private void jbInit()
      throws Exception
   {
      actionBox = Box.createVerticalBox();
      this.setLayout(baseLayout);
//      defaultLayer.setLayout(defaultLayerLayout);
      titleLbl.setFont(new Font("sans serif", Font.BOLD, 32));
      titleLbl.setText("");
      titleLbl.setBorder(BorderFactory.createEmptyBorder(20, 10, 0, 0));
      titleLbl.setForeground(Color.black);
      iconPanel.setOpaque(true);
      iconPanel.setBackground(Color.white);
      actionPanel.setLayout(actionPanelLayout);
      actionPanel.setBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)));
      iconBasePanel.setBackground(Color.white);
      iconBasePanel.setLayout(iconBasePanelLayout);
      iconPanelLayout.setHorizontalGap(15);
      iconPanelLayout.setVerticalGap(15);
      iconPanel.setLayout(iconPanelLayout);
      iconPanel.setBorder(BorderFactory.createEmptyBorder(20, 10, 10, 10));
//      this.add(layeredPane, BorderLayout.CENTER);
//      layeredPane.add(defaultLayer, JLayeredPane.DEFAULT_LAYER);
      this.add(actionPanel,  BorderLayout.WEST);
      actionPanel.add(actionBox, BorderLayout.NORTH);
      this.add(iconBasePanel, BorderLayout.CENTER);
      iconBasePanel.add(titleLbl, BorderLayout.NORTH);
      iconBasePanel.add(iconPanel, BorderLayout.CENTER);
//      layeredPane.add(watermark, new Integer(-1));
   }

   //--- JBuilder automatically generated UI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
//   private JLayeredPane layeredPane = new JLayeredPane();
//   private BorderLayout defaultLayerLayout = new BorderLayout();
//   private JPanel defaultLayer = new JPanel();
   private JLabel titleLbl = new JLabel();
   private JPanel actionPanel = new JPanel();
   private BorderLayout actionPanelLayout = new BorderLayout();
   private Box actionBox;
   private BorderLayout iconBasePanelLayout = new BorderLayout();
   private JPanel iconBasePanel = new JPanel();
   private JPanel iconPanel = new JPanel();
   private ControlPanelLayout iconPanelLayout = new ControlPanelLayout();
   private JLabel watermark = new JLabel();

   /**
    * The data model describing this control panel.
    */
   private ControlPanelModel model;

   /**
    * The watermark image.
    */
//   private ImageIcon watermark;

   /**
    * The listener for the data model so that we can update the UI when the
    * model changes.
    */
   private ModelListener modelListener = new ModelListener();

   /**
    * The listener for actions on the contained control panel items.
    */
   private ItemActionListener actionListener = new ItemActionListener();

   /**
    * Specialized listener for the model so that we make sure the UI gets
    * updated whenever the model changes.
    */
   private class ModelListener
      implements ListDataListener
   {
      public void intervalAdded(ListDataEvent evt)
      {
         for (int i=evt.getIndex0(); i<=evt.getIndex1(); ++i)
         {
            ControlPanelItem item = new ControlPanelItem(model.getElementAt(i),
                                                         model.getIconAt(i));
            item.addActionListener(actionListener);
            iconPanel.add(item, i);
         }
      }

      public void intervalRemoved(ListDataEvent evt)
      {
         for (int i=evt.getIndex1(); i>=evt.getIndex0(); --i)
         {
            ControlPanelItem item = (ControlPanelItem)iconPanel.getComponent(i);
            item.removeActionListener(actionListener);
            iconPanel.remove(i);
         }
      }

      public void contentsChanged(ListDataEvent evt)
      {
         for (int i=evt.getIndex0(); i<=evt.getIndex1(); ++i)
         {
            ControlPanelItem item = (ControlPanelItem)iconPanel.getComponent(i);
            item.setValue(model.getElementAt(i));
            item.setIcon(model.getIconAt(i));
         }
      }
   }

   /**
    * Specialized action listener that checks for action on the contained items.
    */
   private class ItemActionListener
      implements ActionListener
   {
      public void actionPerformed(ActionEvent evt)
      {
         ControlPanelItem item = (ControlPanelItem)evt.getSource();
         int idx = model.getIndexOf(item.getValue());

         // Sanity check that the item we got an event for is in the model
         if (idx != -1)
         {
            fireAction("clicked", idx);
         }
      }
   }

   /**
    * Helper component class that is used to render each item on the control
    * panel.
    */
   private class ControlPanelItem
      extends JComponent
   {
      public ControlPanelItem(Object value, Icon icon)
      {
         // init the UI
         setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
         iconBtn.setOpaque(false);
         iconBtn.setBorder(BorderFactory.createEmptyBorder());
         iconBtn.setMinimumSize(new Dimension(64, 64));
         iconBtn.setMaximumSize(new Dimension(64, 64));
         iconBtn.setPreferredSize(new Dimension(64, 64));
         labelBtn.setOpaque(false);
         labelBtn.setBorder(BorderFactory.createEmptyBorder());
         labelBtn.setFont(new Font("sans serif", Font.BOLD, 16));

         // setup listeners
         iconBtn.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               // Forward on to interested parties
               fireAction(String.valueOf(ControlPanelItem.this.value));
            }
         });
         labelBtn.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               // Forward on to interested parties
               fireAction(String.valueOf(ControlPanelItem.this.value));
            }
         });

         // add in the internal components
         add(iconBtn);
         add(Box.createHorizontalStrut(8));
         add(labelBtn);

         // setup the default values
         setValue(value);
         setIcon(icon);
      }

      public Object getValue()
      {
         return value;
      }

      public void setValue(Object newValue)
      {
         this.value = newValue;
         labelBtn.setText((this.value == null) ? "" : this.value.toString());
      }

      public Icon getIcon()
      {
         return iconBtn.getIcon();
      }

      public void setIcon(Icon icon)
      {
         iconBtn.setIcon(icon);
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

      /**
       * The value object being represented by this item.
       */
      private Object value;

      private JButton iconBtn = new JButton();
      private JButton labelBtn = new JButton();

      /**
       * Listeners on this item.
       */
      private EventListenerList listenerList = new EventListenerList();
   }
}
