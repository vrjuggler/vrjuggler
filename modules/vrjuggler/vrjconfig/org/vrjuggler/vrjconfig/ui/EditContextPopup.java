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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.ui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigContextEvent;
import org.vrjuggler.jccl.config.event.ConfigContextListener;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class EditContextPopup
   extends JPanel
{
   public EditContextPopup()
   {
      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      contextTable.setModel(model);
      contextTable.getColumnModel().getColumn(0).setMaxWidth(50);
      contextTable.setShowGrid(false);
      contextTable.setRowSelectionAllowed(false);
      contextTable.setColumnSelectionAllowed(false);
      contextTable.setCellSelectionEnabled(false);
   }

   public void show(JComponent invoker, int x, int y)
   {
      Container parent = null;
      if (invoker != null)
      {
         parent = invoker.getParent();
         Point invoker_origin = invoker.getLocationOnScreen();
         x += invoker_origin.x;
         y += invoker_origin.y;
      }

      // Get the root pane of the invoker
      JRootPane root = invoker.getRootPane();
      Point root_pos = new Point(x, y);
      SwingUtilities.convertPointFromScreen(root_pos, root);
      setLocation(root_pos);
      pack();
      setVisible(true);
      root.getLayeredPane().add(this, JLayeredPane.POPUP_LAYER, 0);
   }

   public void setVisible(boolean b)
   {
      // no-op?
      if (b == isVisible())
      {
         return ;
      }

      // When this is hidden, it really needs to be removed from its parent
      if (b == false)
      {
         Container parent = getParent();
         Rectangle r = getBounds();
         if (parent != null)
         {
            // Repaint the affected area in the parent
            parent.remove(this);
            parent.repaint(r.x, r.y, r.width, r.height);
         }
      }
      else
      {
         super.setVisible(b);
      }
   }

   /**
    * Resizes this component to its preferred size.
    */
   public void pack()
   {
      setSize(getPreferredSize());
   }

   public void setConfigContext(ConfigContext context)
   {
      model.setConfigContext(context);
      contextTable.getColumnModel().getColumn(0).sizeWidthToFit();
   }

   public ConfigContext getConfigContext()
   {
      return model.getConfigContext();
   }

   private void doAddNew()
   {
      fireAction("New");
   }

   private void doAddExisting()
   {
      fireAction("Open");
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
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      this.setOpaque(true);
      this.setDoubleBuffered(true);
      titlePnl.setLayout(new BorderLayout());
      lblTitle.setText("Please pick the files in your configuration.");
      lblTitle.setOpaque(false);
      titlePnl.setOpaque(false);
      this.add(titlePnl, BorderLayout.NORTH);
      titlePnl.add(lblTitle, null);
      this.add(contextTableScrollPane, BorderLayout.CENTER);
      contextTableScrollPane.getViewport().setView(contextTable);
   }

   // JBuilder GUI variables
   private JPanel titlePnl = new JPanel();
   private JLabel lblTitle = new JLabel();
   private JScrollPane contextTableScrollPane = new JScrollPane();
   private JTable contextTable = new JTable()
   {
      public Dimension getPreferredScrollableViewportSize()
      {
         Dimension pref_size = getPreferredSize();
         pref_size.height += 13;
         return pref_size;
      }
   };

   private ContextTableModel model = new ContextTableModel();

   /**
    * Table model for the context table.
    */
   class ContextTableModel
      extends AbstractTableModel
      implements ConfigContextListener
   {
      private static final int MIN_ROWS = 5;

      public void setConfigContext(ConfigContext context)
      {
         if (this.context != null)
         {
            this.context.removeConfigContextListener(this);
         }
         this.context = context;
         if (this.context != null)
         {
            this.context.addConfigContextListener(this);
         }
         fireTableDataChanged();
      }

      public ConfigContext getConfigContext()
      {
         return context;
      }

      public String getColumnName(int col)
      {
         return columnNames[col];
      }

      public Class getColumnClass(int col)
      {
         switch (col)
         {
         case 0:
            return Boolean.class;
         case 1:
            return String.class;
         default:
            return null;
         }
      }

      public int getColumnCount()
      {
         return columnNames.length;
      }

      public int getRowCount()
      {
         int rows = broker.getResourceNames().size();
         if (rows < MIN_ROWS)
         {
            rows = MIN_ROWS;
         }
         return rows;
      }

      public Object getValueAt(int row, int col)
      {
         // Resources are sorted in our table
         List resources = new ArrayList(broker.getResourceNames());
         Collections.sort(resources);

         if (row < resources.size())
         {
            if (col == 0)
            {
               boolean active = false;
               if (context != null)
               {
                  active = context.contains((String)resources.get(row));
               }
               return new Boolean(active);
            }
            else
            {
               return resources.get(row);
            }
         }
         else
         {
            return null;
         }
      }

      public boolean isCellEditable(int row, int col)
      {
         int num_resources = broker.getResourceNames().size();
         if (col == 0 && row < num_resources)
         {
            return true;
         }
         return false;
      }

      public void setValueAt(Object value, int row, int col)
      {
         // Resources are sorted in our table
         List resources = new ArrayList(broker.getResourceNames());
         Collections.sort(resources);

         if (col == 0)
         {
            String resource = (String)resources.get(row);
            boolean active = ((Boolean)value).booleanValue();
            if (active)
            {
               context.add(resource);
            }
            else
            {
               context.remove(resource);
            }
         }
      }

      /**
       * Called whenever a resources is added to the context we are modeling.
       */
      public void resourceAdded(ConfigContextEvent evt)
      {
         int idx = getRowFor(evt.getResource());
         if (idx != -1)
         {
            if (idx == getRowCount() && idx < MIN_ROWS)
            {
               fireTableRowsUpdated(idx, idx);
            }
            else
            {
               fireTableRowsInserted(idx, idx);
            }
         }
      }

      /**
       * Called whenever a resource is removed from the context we are modeling.
       */
      public void resourceRemoved(ConfigContextEvent evt)
      {
         int idx = getRowFor(evt.getResource());
         if (idx != -1)
         {
            if (idx < MIN_ROWS && getRowCount() <= MIN_ROWS)
            {
               fireTableRowsUpdated(idx, MIN_ROWS-1);
            }
            else
            {
               fireTableRowsDeleted(idx, idx);
            }
         }
      }

      /**
       * Gets the row of the table for the given resource.
       */
      private int getRowFor(String resource)
      {
         // Resources are sorted in our table
         List resources = new ArrayList(broker.getResourceNames());
         Collections.sort(resources);
         return resources.indexOf(resource);
      }

      private String[] columnNames = new String[] { "Active", "Resource Name" };
      private ConfigContext context = null;
      private ConfigBroker broker = new ConfigBrokerProxy();
   }
}
