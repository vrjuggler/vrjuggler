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

      // Init the model to something stupid
      model = new AbstractControlPanelModel()
      {
         public Object getElementAt(int index) { return null; }
         public Icon getIconAt(int index)      { return null; }
         public int getSize()                  { return 0; }
      };

      // Setup the model listener and do the initial hookup
      modelListener = new ModelListener();
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

   private void rebuildUI()
   {
      iconPanel.removeAll();
      for (int i=0; i<model.getSize(); ++i)
      {
         ControlPanelItem item = new ControlPanelItem(model.getElementAt(i),
                                                      model.getIconAt(i));
         iconPanel.add(item);
      }
   }

   /**
    * JBuilder automatically generated UI initialization.
    */
   private void jbInit()
      throws Exception
   {
      actionBox = Box.createVerticalBox();
      this.setLayout(baseLayout);
      titleLbl.setFont(new Font("serif", Font.BOLD, 32));
      titleLbl.setText("");
      titleLbl.setBorder(BorderFactory.createEmptyBorder(20, 10, 0, 0));
//      titleLbl.setOpaque(true);
//      titleLbl.setBackground(Color.white);
      iconPanel.setOpaque(true);
      iconPanel.setBackground(Color.white);
      actionPanel.setLayout(actionPanelLayout);
      actionPanel.setBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)));
      iconBasePanel.setBackground(Color.white);
      iconBasePanel.setLayout(iconBasePanelLayout);
      iconPanelLayout.setHorizontalGap(15);
      iconPanelLayout.setVerticalGap(15);
      iconPanel.setLayout(iconPanelLayout);
      iconPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
      this.add(actionPanel,  BorderLayout.WEST);
      actionPanel.add(actionBox, BorderLayout.NORTH);
      this.add(iconBasePanel, BorderLayout.CENTER);
      iconBasePanel.add(titleLbl, BorderLayout.NORTH);
      iconBasePanel.add(iconPanel, BorderLayout.CENTER);
   }

   //--- JBuilder automatically generated UI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private JLabel titleLbl = new JLabel();
   private JPanel actionPanel = new JPanel();
   private BorderLayout actionPanelLayout = new BorderLayout();
   private Box actionBox;
   private BorderLayout iconBasePanelLayout = new BorderLayout();
   private JPanel iconBasePanel = new JPanel();
   private JPanel iconPanel = new JPanel();
   private ControlPanelLayout iconPanelLayout = new ControlPanelLayout();

   /**
    * The data model describing this control panel.
    */
   private ControlPanelModel model;

   /**
    * The listener for the data model so that we can update the UI when th
    * model changes.
    */
   private ModelListener modelListener;

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
            iconPanel.add(item, i);
         }
      }

      public void intervalRemoved(ListDataEvent evt)
      {
         for (int i=evt.getIndex1(); i>=evt.getIndex0(); --i)
         {
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
         labelBtn.setFont(new Font("serif", Font.BOLD, 16));
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

      /**
       * The value object being represented by this item.
       */
      private Object value;

      private JButton iconBtn = new JButton();
      private JButton labelBtn = new JButton();
   }

//   /**
//    * Table model that wraps the ControlPanel model so that the control panel
//    * can be displayed like a table, but used like a list.
//    */
//   private class ControlPanelTableModel
//      extends AbstractTableModel
//   {
//      public ControlPanelTableModel(ControlPanelModel model)
//      {
//         setModel(model);
//      }
//
//      public void setModel(ControlPanelModel model)
//      {
//         this.model = model;
//         fireTableDataChanged();
//      }
//
//      public ControlPanelModel getModel()
//      {
//         return model;
//      }
//
//      public int getRowCount()
//      {
//         return (model.getSize() / getColumnCount()) + 1;
//      }
//
//      public int getColumnCount()
//      {
//         return 2;
//      }
//
//      public Object getValueAt(int row, int col)
//      {
//         int idx = 2 *row + col;
//         if (idx <= model.getSize())
//         {
//            return model.getElementAt(idx);
//         }
//         return null;
//      }
//
//      private ControlPanelModel model;
//   }
}
