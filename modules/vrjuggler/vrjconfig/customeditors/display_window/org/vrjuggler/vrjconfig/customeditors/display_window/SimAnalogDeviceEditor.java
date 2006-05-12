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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.Color;
import java.awt.Window;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.*;
import javax.swing.table.AbstractTableModel;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorHelpers;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;
import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


public class SimAnalogDeviceEditor
   extends JSplitPane
   implements SimDeviceEditor
            , ConfigElementListener
            , EditorConstants
{
   public SimAnalogDeviceEditor()
   {
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public void setKeyboardEditorPanel(KeyboardEditorPanel panel)
   {
      if ( panel == null && mKeyboardEditor != null )
      {
         // Unregister our listener for events from mKeyboardEditor ...
      }

      mKeyboardEditor = panel;

      if ( mKeyboardEditor != null )
      {
         // Register our listener for events from mKeyboardEditor ...
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( ! elt.getDefinition().getToken().equals(SIM_ANALOG_DEVICE_TYPE) )
      {
         throw new IllegalArgumentException("Invalid config element type '" +
                                            elt.getDefinition().getToken() +
                                            "'!  Expected " +
                                            SIM_ANALOG_DEVICE_TYPE);
      }

      mContext = ctx;
      mElement = elt;
      mElement.addConfigElementListener(this);

      if ( mProxyGraph != null )
      {
         this.remove(mProxyGraph);
         mProxyGraph.editorClosing();
         mProxyGraph = null;
      }

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();

      List allowed_types = new ArrayList(2);
      allowed_types.add(0, repos.get(ANALOG_PROXY_TYPE));
      allowed_types.add(1, repos.get(SIM_ANALOG_DEVICE_TYPE));

      mProxyGraph = new ProxyEditorUI(allowed_types);
      mProxyGraph.setConfig(ctx, elt);
      this.add(mProxyGraph, JSplitPane.LEFT);

      int inc_units =
         mElement.getPropertyValueCount(INCREMENT_KEYPRESS_PROPERTY);
      int dec_units =
         mElement.getPropertyValueCount(DECREMENT_KEYPRESS_PROPERTY);
      int units = Math.min(inc_units, dec_units);

      if ( inc_units != dec_units )
      {
         Window parent =
            (Window) SwingUtilities.getAncestorOfClass(Window.class, this);
         JOptionPane.showMessageDialog(
            parent,
            "Invalid simulated analog device config element '" +
               mElement.getName() + "'\nNumber of increment key presses (" +
               inc_units +
               ") does not equal the number of decrement key presses (" +
               dec_units + ")\nOnly Units 0 through " + units +
               " will be editable.",
            "Invalid Config Element", JOptionPane.WARNING_MESSAGE
         );
      }

      mTableModel = new AnalogUnitTableModel(mElement);
      mUnitTable.setModel(mTableModel);

      for ( int i = 0; i < units; ++i )
      {
         ConfigElement inc_elt =
            (ConfigElement) mElement.getProperty(INCREMENT_KEYPRESS_PROPERTY,
                                                 i);
         ConfigElement dec_elt =
            (ConfigElement) mElement.getProperty(DECREMENT_KEYPRESS_PROPERTY,
                                                 i);
         inc_elt.addConfigElementListener(this);
         dec_elt.addConfigElementListener(this);
      }
   }

   public JComponent getEditor()
   {
      return this;
   }

   public void editorClosing()
   {
      if ( mCurEditor != null )
      {
         mCurEditor.editorClosing();
         mCurEditor = null;
      }

      if ( mElement != null )
      {
         int units = mTableModel.getRowCount();

         for ( int i = 0; i < units; ++i )
         {
            ConfigElement inc_elt =
               (ConfigElement) mElement.getProperty(INCREMENT_KEYPRESS_PROPERTY,
                                                    i);
            ConfigElement dec_elt =
               (ConfigElement) mElement.getProperty(DECREMENT_KEYPRESS_PROPERTY,
                                                    i);
            inc_elt.removeConfigElementListener(this);
            dec_elt.removeConfigElementListener(this);
         }

         mElement.removeConfigElementListener(this);
         mElement = null;
      }
   }

   public void nameChanged(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
      mTableModel.fireTableRowsInserted(e.getIndex(), e.getIndex());
      ConfigElement new_elt = (ConfigElement) e.getValue();
      new_elt.addConfigElementListener(this);
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      ConfigElement src_elt = (ConfigElement) e.getSource();
      if ( src_elt.getDefinition().getToken().equals(KEY_MODIFIER_PAIR_TYPE) )
      {
         int row = mElement.getPropertyValueIndex(INCREMENT_KEYPRESS_PROPERTY,
                                                  src_elt);

         if ( row == -1 )
         {
            row = mElement.getPropertyValueIndex(DECREMENT_KEYPRESS_PROPERTY,
                                                 src_elt);
         }

         mTableModel.fireTableRowsUpdated(row, row);
      }
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
      int from_index = Math.min(e.getIndex0(), e.getIndex1());
      int to_index   = Math.max(e.getIndex0(), e.getIndex1());
      mTableModel.fireTableRowsUpdated(from_index, to_index);
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
      if ( mUnitTable.getSelectedRow() == e.getIndex() )
      {
         mUnitTable.clearSelection();
      }

      mTableModel.fireTableRowsDeleted(e.getIndex(), e.getIndex());
   }

   private void jbInit() throws Exception
   {
      this.setOneTouchExpandable(false);
      this.setOrientation(VERTICAL_SPLIT);
      this.setResizeWeight(0.5);

      mUnitTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mUnitTable.setCellSelectionEnabled(true);
      ListSelectionModel table_selection = mUnitTable.getSelectionModel();
      table_selection.addListSelectionListener(new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               selectedUnitChanged();
            }
         }
      );
      mUnitTable.getColumnModel().addColumnModelListener(
         new TableColumnModelListener()
         {
            public void columnAdded(TableColumnModelEvent e)
            {
            }

            public void columnMarginChanged(ChangeEvent e)
            {
            }

            public void columnMoved(TableColumnModelEvent e)
            {
            }

            public void columnRemoved(TableColumnModelEvent e)
            {
            }

            public void columnSelectionChanged(ListSelectionEvent e)
            {
               selectedUnitChanged();
            }
         }
      );

      double[][] sizes =
         {
            {TableLayout.PREFERRED, TableLayout.FILL},
            {TableLayout.FILL}
         };
      mKeyBindingPanel.setLayout(new TableLayout(sizes));
      mKeyBindingPanel.setBorder(
         new TitledBorder(BorderFactory.createEtchedBorder(
                             Color.white, new Color(142, 142, 142)
                          ),
                          "Analog Unit Key Bindings", TitledBorder.CENTER,
                          TitledBorder.DEFAULT_POSITION)
      );

      mKeyBindingPanel.add(
         mUnitScrollPane,
         new TableLayoutConstraints(0, 0, 0, 0,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.FULL)
      );

      this.add(mKeyBindingPanel, JSplitPane.RIGHT);
   }

   // NOTE: This method may be invoked multiple times as a result of the
   // separation between the row selection and column selection listeners.  I
   // don't think there is a way to avoid that with JTables.  Fortunately,
   // KeyPressEditor.setConfig() is designed to allow multiple invocation
   // during the lifetime of any given instance.
   private void selectedUnitChanged()
   {
      int row = mUnitTable.getSelectedRow();
      int col = mUnitTable.getSelectedColumn();

      if ( row == -1 || col == -1 || col == AnalogUnitTableModel.LABEL_COL )
      {
         if ( mCurEditor != null )
         {
            mCurEditor.setEnabled(false);
         }
      }
      else
      {
         if ( mCurEditor == null )
         {
            mCurEditor = new KeyPressEditor();
            mKeyBindingPanel.add(
               mCurEditor,
               new TableLayoutConstraints(1, 0, 1, 0,
                                          TableLayoutConstraints.CENTER,
                                          TableLayoutConstraints.TOP)
            );

            mKeyBindingPanel.revalidate();
            mKeyBindingPanel.repaint();
         }

         mCurEditor.setEnabled(true);

         String prop_name =
            (col == AnalogUnitTableModel.INCREMENT_COL ? INCREMENT_KEYPRESS_PROPERTY
                                                       : DECREMENT_KEYPRESS_PROPERTY);
         ConfigElement elt = (ConfigElement) mElement.getProperty(prop_name,
                                                                  row);
         mCurEditor.setConfig(mContext, elt);
      }
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private AnalogUnitTableModel mTableModel = null;

   private KeyboardEditorPanel mKeyboardEditor = null;
   private KeyPressEditor mCurEditor = null;

   private ProxyEditorUI mProxyGraph = null;
   private JPanel mKeyBindingPanel = new JPanel();
   private JTable mUnitTable = new JTable();
   private JScrollPane mUnitScrollPane = new JScrollPane(mUnitTable);

   private static class AnalogUnitTableModel
      extends AbstractTableModel
   {
      public static final int LABEL_COL     = 0;
      public static final int INCREMENT_COL = 1;
      public static final int DECREMENT_COL = 2;

      private static final int COLUMN_COUNT = 3;

      public AnalogUnitTableModel(ConfigElement simAnalogElt)
      {
         mAnalogElt = simAnalogElt;
      }

      public int getRowCount()
      {
         int inc_count =
            mAnalogElt.getPropertyValueCount(INCREMENT_KEYPRESS_PROPERTY);
         int dec_count =
            mAnalogElt.getPropertyValueCount(DECREMENT_KEYPRESS_PROPERTY);
         return Math.min(inc_count, dec_count);
      }

      public int getColumnCount()
      {
         return COLUMN_COUNT;
      }

      public String getColumnName(int columnIndex)
      {
         String name = "";

         switch (columnIndex)
         {
            case LABEL_COL:
               name = "Unit";
               break;
            case INCREMENT_COL:
               name = "Increment Key Press";
               break;
            case DECREMENT_COL:
               name = "Decrement Key Press";
               break;
         }

         return name;
      }

      public Object getValueAt(int row, int column)
      {
         Object value = null;

         switch (column)
         {
            case LABEL_COL:
               value = "Unit " + row;
               break;
            case INCREMENT_COL:
               ConfigElement inc_elt =
                  (ConfigElement) mAnalogElt.getProperty(INCREMENT_KEYPRESS_PROPERTY,
                                                         row);
               Integer inc_key =
                  (Integer) inc_elt.getProperty(KEY_PROPERTY, 0);
               Integer inc_mod =
                  (Integer) inc_elt.getProperty(MODIFIER_KEY_PROPERTY, 0);
               value = EditorHelpers.getKeyPressText(inc_key.intValue(),
                                                     inc_mod.intValue());
               break;
            case DECREMENT_COL:
               ConfigElement dec_elt =
                  (ConfigElement) mAnalogElt.getProperty(DECREMENT_KEYPRESS_PROPERTY,
                                                         row);
               Integer dec_key =
                  (Integer) dec_elt.getProperty(KEY_PROPERTY, 0);
               Integer dec_mod =
                  (Integer) dec_elt.getProperty(MODIFIER_KEY_PROPERTY, 0);
               value = EditorHelpers.getKeyPressText(dec_key.intValue(),
                                                     dec_mod.intValue());
               break;
         }

         return value;
      }

      private ConfigElement mAnalogElt = null;
   }
}
