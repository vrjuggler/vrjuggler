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

import java.awt.Component;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * A component suitable for editing a simulator device.
 */
public class SimPosDeviceEditor
   extends JPanel
{
   /**
    * Creates a new sim device editor with no chunk to edit.
    */
   public SimPosDeviceEditor(ConfigContext context)
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.context = context;

      // setup the keyboard proxy combo box
      List chunks = getConfigBroker().getChunks(context);
      if (chunks.size() > 0)
      {
         List proxies = ConfigUtilities.getChunksWithDescToken(
                                                   chunks, "KeyboardProxy");
         for (Iterator itr = proxies.iterator(); itr.hasNext(); )
         {
            ConfigChunk proxy = (ConfigChunk)itr.next();
            keyProxyCombo.addItem(proxy.getName());
         }
         keyProxyCombo.setSelectedIndex(0);
      }

      // setup the table
      actionTable.setModel(actionModel);
      actionTable.getColumnModel().getColumn(1).setCellRenderer(
                                                  new KeyModPairCellRenderer());
      actionTable.getColumnModel().getColumn(1).setCellEditor(
                                                  new KeyModPairCellEditor());
   }

   /**
    * Makes the SimPosDevice chunk active.
    */
   public void setDevice(ConfigChunk device)
   {
      // Make sure the chunk is a SimPosition chunk.
      if (device != null && (!device.getDesc().getToken().equals("SimPosition")))
      {
         throw new IllegalArgumentException("device must be a SimPosition chunk");
      }

      // Change the device
      ConfigChunk old = this.device;
      if (this.device != null)
      {
         this.device.removeConfigChunkListener(deviceListener);
      }
      this.device = device;
      if (this.device != null)
      {
         this.device.addConfigChunkListener(deviceListener);
      }

      // Let the model know
      actionModel.setChunk(this.device);
      firePropertyChange("device", old, this.device);
   }

   /**
    * Automatically generated JBuilder GUI init.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
      keyProxyPanel.setLayout(new BoxLayout(keyProxyPanel, BoxLayout.X_AXIS));
      keyProxyLabel.setText("Keyboard Proxy:");
      this.add(keyProxyPanel);
      keyProxyPanel.add(keyProxyLabel);
      keyProxyPanel.add(keyProxyCombo);
      keyProxyPanel.add(Box.createHorizontalStrut(8));
      keyProxyPanel.add(Box.createHorizontalGlue());
      this.add(actionScrollPane);
      actionScrollPane.setViewportView(actionTable);
   }

   /**
    * Helper class for getting the config broker.
    */
   private ConfigBroker getConfigBroker()
   {
      return new ConfigBrokerProxy();
   }

   /**
    * Gets a valid text string for a KeyModPair using the current device.
    */
   protected String getKeyModPairText(int modKey, int key)
   {
//      ConfigChunk key_mod_pair_chunk = device.getPropertyDesc("keyPairs").
//                                       getDefaultValue(0).getEmbeddedChunk();
      ConfigChunk key_mod_pair_chunk = (ConfigChunk)device.getProperty("keyPairs", 0);
      String mod_str = getModString(key_mod_pair_chunk, modKey);
      String key_str = getKeyString(key_mod_pair_chunk, key);

      // Filter the text given to us so that it looks sane
      StringBuffer text = new StringBuffer();

      // Don't display a modifier of NONE
      if (! mod_str.equals("NONE"))
      {
         text.append(mod_str).append(" + ");
      }

      // Strip off the KEY_ prefix to the key
      if (key_str.startsWith("KEY_"))
      {
         key_str = key_str.substring(4);
      }
      text.append(key_str);

      return text.toString();
   }

   protected String getModString(ConfigChunk chunk, int mod)
   {
      java.util.List enums = chunk.getPropertyDesc("modKey").getEnums();
      for (Iterator itr=enums.iterator(); itr.hasNext(); )
      {
         DescEnum de = (DescEnum)itr.next();
         if (de.getValue().equals(new Integer(mod)))
         {
            return de.getName();
         }
      }
      return "";
   }

   protected String getKeyString(ConfigChunk chunk, int key)
   {
      java.util.List enums = chunk.getPropertyDesc("key").getEnums();
      for (Iterator itr=enums.iterator(); itr.hasNext(); )
      {
         DescEnum de = (DescEnum)itr.next();
         if (de.getValue().equals(new Integer(key)))
         {
            return de.getName();
         }
      }
      return "";
   }

   //--- JBuilder GUI variables ---//
   private JPanel keyProxyPanel = new JPanel();
   private JLabel keyProxyLabel = new JLabel();
   private JComboBox keyProxyCombo = new JComboBox();
   private JScrollPane actionScrollPane = new JScrollPane();
   private JTable actionTable = new JTable();

   /**
    * The config chunk of the device we are currently editing.
    */
   private ConfigChunk device;

   /**
    * Our view into the configuration.
    */
   private ConfigContext context;

   /**
    * Table model for the actions table.
    */
   private ActionTableModel actionModel = new ActionTableModel();

   /**
    * Listener for the edited ConfigChunk.
    */
   private DeviceListener deviceListener = new DeviceListener();

   /**
    * Specialized listener on the ConfigChunk.
    */
   private class DeviceListener
      implements ConfigChunkListener
   {
      public void nameChanged(ConfigChunkEvent evt)
      {
         System.err.println("SimDeviceEditor.deviceListener: Device changed!");
      }

      public void propertyValueChanged(ConfigChunkEvent evt) { nameChanged(evt); }
      public void propertyValueAdded(ConfigChunkEvent evt) { nameChanged(evt); }
      public void propertyValueRemoved(ConfigChunkEvent evt) { nameChanged(evt); }
   }

   /**
    * Specialized table model for the action mappings.
    */
   private class ActionTableModel
      extends AbstractTableModel
   {
      public void setChunk(ConfigChunk chunk)
      {
         this.chunk = chunk;
         fireTableDataChanged();
      }

      public ConfigChunk getChunk()
      {
         return this.chunk;
      }

      public int getRowCount()
      {
         if (chunk != null)
         {
            return chunk.getNumPropertyValues("keyPairs");
         }
         return 0;
      }

      public int getColumnCount()
      {
         return 2;
      }

      public String getColumnName(int col)
      {
         return columnNames[col];
      }

      public boolean isCellEditable(int row, int col)
      {
         // Only column 1 is editable
         return (col == 1);
      }

      public Object getValueAt(int row, int col)
      {
         if (chunk != null)
         {
            if (col == 0)
            {
               return chunk.getPropertyDesc("keyPairs").getValueLabel(row);
            }
            else
            {
               return chunk.getProperty("keyPairs", row);
            }
         }
         return null;
      }

      public void setValueAt(Object value, int row, int col)
      {
         chunk.setProperty("keyPairs", row, value);
         fireTableCellUpdated(row, col);
      }

      private ConfigChunk chunk;
      private String[] columnNames = { "Action", "Key Combo" };
   }

   /**
    * Customized table cell renderer for keymod pair config chunks.
    */
   private class KeyModPairCellRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focused,
                                                     int row,
                                                     int col)
      {
         super.getTableCellRendererComponent(table, value, selected, focused,
                                             row, col);

         if (value instanceof ConfigChunk)
         {
            ConfigChunk chunk = (ConfigChunk)value;
            if (chunk.getDesc().getToken().equals("KeyModPair"))
            {
               String text = getKeyModPairText(
                                 ((Integer)chunk.getProperty("modKey")).intValue(),
                                 ((Integer)chunk.getProperty("key")).intValue());
               setText(text);
            }
         }

         return this;
      }
   }

   /**
    * Specialized editor for KeyModPair config chunks.
    */
   public class KeyModPairCellEditor
      extends AbstractCellEditor
      implements TableCellEditor
   {
      public Component getTableCellEditorComponent(JTable table,
                                                   Object value,
                                                   boolean selected,
                                                   int row,
                                                   int col)
      {
         System.out.println("Getting a cell editor");
         keyChunk = (ConfigChunk)value;
         catcher = new KeyCatcher();
         catcher.requestFocus();
         return catcher;
      }

      public Object getCellEditorValue()
      {
         // Update the chunk with the new keys
         keyChunk.setProperty("modKey", 0,
                              getJavaToVRJMod(catcher.getLastModKey()));
         keyChunk.setProperty("key", 0,
                              getJavaToVRJKey(catcher.getLastKey()));
         return keyChunk;
      }

      private class KeyCatcher
         extends JTextField
      {
         public KeyCatcher()
         {
            this.addKeyListener(new KeyAdapter()
            {
               public void keyPressed(KeyEvent evt)
               {
                  System.out.println("Key Pressed: "+evt.getKeyCode());
                  int key = evt.getKeyCode();
                  if (key == KeyEvent.VK_ALT ||
                      key == KeyEvent.VK_CONTROL ||
                      key == KeyEvent.VK_SHIFT)
                  {
                     lastModKey = key;
                     textChanged();
                  }
                  else
                  {
                     lastKey = key;
                     textChanged();
                     stopCellEditing();
                  }
               }

               public void keyTyped(KeyEvent evt)
               {
                  System.out.println("Key Typed");
                  if (evt.getKeyChar() == '\n')
                  {
                     stopCellEditing();
                  }
               }
            });
         }

         public int getLastModKey()
         {
            return lastModKey;
         }

         public int getLastKey()
         {
            return lastKey;
         }

         /**
          * Called whenever the keys have changed and we need to update our UI.
          */
         private void textChanged()
         {
            String text = getKeyModPairText(getJavaToVRJMod(lastModKey).intValue(),
                                            getJavaToVRJKey(lastKey).intValue());
            setText(text);
            updateUI();
         }

         private int lastModKey = KeyEvent.VK_UNDEFINED;
         private int lastKey = KeyEvent.VK_UNDEFINED;
      }

      /**
       * Converts the given java key code to a VRJ key code for a modifier key.
       */
      private Integer getJavaToVRJMod(int javaKey)
      {
         PropertyDesc desc = keyChunk.getPropertyDesc("modKey");
         Object key;

         switch (javaKey)
         {
         case KeyEvent.VK_ALT:
            key = desc.getEnumValue("ALT");
         case KeyEvent.VK_SHIFT:
            key = desc.getEnumValue("SHIFT");
         case KeyEvent.VK_CONTROL:
            key = desc.getEnumValue("CTRL");
         default:
            key = desc.getEnumValue("NONE");
         }

         return (Integer)key;
      }

      /**
       * Converts the given java key code to a VRJ key code.
       */
      private Integer getJavaToVRJKey(int javaKey)
      {
         PropertyDesc desc = keyChunk.getPropertyDesc("key");
         Object key;

         switch (javaKey)
         {
         case KeyEvent.VK_UP:
            key = desc.getEnumValue("KEY_UP");
         case KeyEvent.VK_DOWN:
            key = desc.getEnumValue("KEY_DOWN");
         case KeyEvent.VK_LEFT:
            key = desc.getEnumValue("KEY_LEFT");
         case KeyEvent.VK_RIGHT:
            key = desc.getEnumValue("KEY_RIGHT");
         case KeyEvent.VK_CONTROL:
            key = desc.getEnumValue("KEY_CTRL");
         case KeyEvent.VK_SHIFT:
            key = desc.getEnumValue("KEY_SHIFT");
         case KeyEvent.VK_ALT:
            key = desc.getEnumValue("KEY_ALT");
         case KeyEvent.VK_1:
            key = desc.getEnumValue("KEY_1");
         case KeyEvent.VK_2:
            key = desc.getEnumValue("KEY_2");
         case KeyEvent.VK_3:
            key = desc.getEnumValue("KEY_3");
         case KeyEvent.VK_4:
            key = desc.getEnumValue("KEY_4");
         case KeyEvent.VK_5:
            key = desc.getEnumValue("KEY_5");
         case KeyEvent.VK_6:
            key = desc.getEnumValue("KEY_6");
         case KeyEvent.VK_7:
            key = desc.getEnumValue("KEY_7");
         case KeyEvent.VK_8:
            key = desc.getEnumValue("KEY_8");
         case KeyEvent.VK_9:
            key = desc.getEnumValue("KEY_9");
         case KeyEvent.VK_0:
            key = desc.getEnumValue("KEY_0");
         case KeyEvent.VK_A:
            key = desc.getEnumValue("KEY_A");
         case KeyEvent.VK_B:
            key = desc.getEnumValue("KEY_B");
         case KeyEvent.VK_C:
            key = desc.getEnumValue("KEY_C");
         case KeyEvent.VK_D:
            key = desc.getEnumValue("KEY_D");
         case KeyEvent.VK_E:
            key = desc.getEnumValue("KEY_E");
         case KeyEvent.VK_F:
            key = desc.getEnumValue("KEY_F");
         case KeyEvent.VK_G:
            key = desc.getEnumValue("KEY_G");
         case KeyEvent.VK_H:
            key = desc.getEnumValue("KEY_H");
         case KeyEvent.VK_I:
            key = desc.getEnumValue("KEY_I");
         case KeyEvent.VK_J:
            key = desc.getEnumValue("KEY_K");
         case KeyEvent.VK_K:
            key = desc.getEnumValue("KEY_J");
         case KeyEvent.VK_L:
            key = desc.getEnumValue("KEY_L");
         case KeyEvent.VK_M:
            key = desc.getEnumValue("KEY_M");
         case KeyEvent.VK_N:
            key = desc.getEnumValue("KEY_N");
         case KeyEvent.VK_O:
            key = desc.getEnumValue("KEY_O");
         case KeyEvent.VK_P:
            key = desc.getEnumValue("KEY_P");
         case KeyEvent.VK_Q:
            key = desc.getEnumValue("KEY_Q");
         case KeyEvent.VK_R:
            key = desc.getEnumValue("KEY_R");
         case KeyEvent.VK_S:
            key = desc.getEnumValue("KEY_S");
         case KeyEvent.VK_T:
            key = desc.getEnumValue("KEY_T");
         case KeyEvent.VK_U:
            key = desc.getEnumValue("KEY_U");
         case KeyEvent.VK_V:
            key = desc.getEnumValue("KEY_V");
         case KeyEvent.VK_W:
            key = desc.getEnumValue("KEY_W");
         case KeyEvent.VK_X:
            key = desc.getEnumValue("KEY_X");
         case KeyEvent.VK_Y:
            key = desc.getEnumValue("KEY_Y");
         case KeyEvent.VK_Z:
            key = desc.getEnumValue("KEY_Z");
         case KeyEvent.VK_ESCAPE:
            key = desc.getEnumValue("KEY_ESC");
         default:
            key = desc.getEnumValue("KEY_NONE");
         }

         return (Integer)key;
      }

      private ConfigChunk keyChunk;
      private KeyCatcher catcher;
   }
}
