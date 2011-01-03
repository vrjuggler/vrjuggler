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

package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;

/**
 * A property sheet for a ConfigDefinition object.
 */
public class ConfigDefinitionPropertySheet
   extends JTable
{
   public ConfigDefinitionPropertySheet()
   {
      // Populate the PropertyEditorManager's list of valid editors
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);

      // Make resizing work intuitively
      setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

      // Setup the table
      mTableModel = new ConfigDefinitionTableModel();
      setModel(mTableModel);
//      getColumnModel().getColumn(1).setCellEditor(
//                                    new ChunkDescPropertyValueEditor());
      setBackground(UIManager.getColor("Menu"));
      setRowHeight(getRowHeight()+4);
//      getColumnModel().getColumn(0).setCellRenderer(
//                           new ChunkDescPropertyNameRenderer());
//      getColumnModel().getColumn(1).setCellRenderer(
//                           new ChunkDescPropertyValueRenderer());

      // Make sure we know when the selection changes
      ListSelectionModel rowSM = getSelectionModel();
      rowSM.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent e)
         {
            if (! e.getValueIsAdjusting())
            {
               fireActionEvent();
            }
         }
      });
   }

   public void addActionListener(ActionListener listener)
   {
      mListenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      mListenerList.remove(ActionListener.class, listener);
   }

   private void fireActionEvent()
   {
      Object[] listeners = mListenerList.getListenerList();
      ActionEvent e = new ActionEvent(this, 0, "PropertySelectionChanged");
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            ((ActionListener)listeners[i+1]).actionPerformed(e);
         }
      }
   }

   public PropertyDefinition getSelectedProperty()
   {
      ListSelectionModel lsm = getSelectionModel();
      if (lsm.isSelectionEmpty())
      {
         return null;
      }
      else
      {
         int row = lsm.getMinSelectionIndex();
         return mTableModel.getPropertyDefinition(row);
      }
   }

   public void setDefinition(ConfigDefinition def)
   {
      mTableModel.setDefinition(def);
   }

   public ConfigDefinition getDefinition()
   {
      return mTableModel.getDefinition();
   }

   /**
    * The table model for the property sheet.
    */
   private ConfigDefinitionTableModel mTableModel;

   /**
    * All listeners interested in this model.
    */
   private EventListenerList mListenerList = new EventListenerList();
}
