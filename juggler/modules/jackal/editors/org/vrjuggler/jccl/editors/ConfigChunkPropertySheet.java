/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.PropertyDesc;

/**
 * A property sheet for a config chunk object.
 */
public class ConfigChunkPropertySheet
   extends JTable
{
   public ConfigChunkPropertySheet()
   {
      // Populate the PropertyEditorManager's list of valid editors
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);

      // Make resizing work intuitively
      setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

      // Setup the table
      tableModel = new ConfigChunkTableModel();
      setModel(tableModel);
      getColumnModel().getColumn(1).setCellEditor(
                                    new PropertyValueEditor());
      setBackground(UIManager.getColor("Menu"));
      setRowHeight(getRowHeight()+4);
      getColumnModel().getColumn(0).setCellRenderer(
                           new PropertyNameRenderer());
      getColumnModel().getColumn(1).setCellRenderer(
                           new PropertyValueRenderer());

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
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   private void fireActionEvent()
   {
      Object[] listeners = listenerList.getListenerList();
      ActionEvent e = new ActionEvent(this, 0, "PropertySelectionChanged");
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            ((ActionListener)listeners[i+1]).actionPerformed(e);
         }
      }
   }

   public PropertyDesc getSelectedProperty()
   {
      ListSelectionModel lsm = getSelectionModel();
      if (lsm.isSelectionEmpty())
      {
         return null;
      }
      else
      {
         int row = lsm.getMinSelectionIndex();
         return tableModel.getPropertyDesc(row);
      }
   }

   public void setConfigChunk(ConfigChunk chunk)
   {
      tableModel.setConfigChunk(chunk);
   }

   public ConfigChunk getConfigChunk()
   {
      return tableModel.getConfigChunk();
   }

   /**
    * This method is called by the EmbeddedChunkCellEditor to let the property
    * sheet know that an embedded ConfigChunk hyperlink has been selected.
    */
   public void embeddedChunkSelected(ConfigChunk chunk)
   {
      fireEmbeddedChunkSelected(chunk);
   }

   /**
    * Notifies all listeners that the given embedded ConfigChunk has been
    * selected.
    */
   protected void fireEmbeddedChunkSelected(ConfigChunk chunk)
   {
      Object[] listeners = listenerList.getListenerList();
      ChunkSelectionEvent e = new ChunkSelectionEvent(this, chunk);
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkSelectionListener.class)
         {
            ((ChunkSelectionListener)listeners[i+1]).chunkSelected(e);
         }
      }
   }

   public void addChunkSelectionListener(ChunkSelectionListener listener)
   {
      listenerList.add(ChunkSelectionListener.class, listener);
   }

   public void removeChunkSelectionListener(ChunkSelectionListener listener)
   {
      listenerList.remove(ChunkSelectionListener.class, listener);
   }

   /**
    * The table model for the property sheet.
    */
   private ConfigChunkTableModel tableModel;

   /**
    * All listeners interested in this model.
    */
   private EventListenerList listenerList = new EventListenerList();
}
