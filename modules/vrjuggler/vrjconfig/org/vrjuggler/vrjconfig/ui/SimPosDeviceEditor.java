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
      ConfigChunk key_mod_pair_chunk = device.getProperty("keyPairs", 0).getEmbeddedChunk();
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
         if (de.getValue().getInt() == mod)
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
         if (de.getValue().getInt() == key)
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
               return chunk.getProperty("keyPairs", row).getEmbeddedChunk();
            }
         }
         return null;
      }

      public void setValueAt(Object value, int row, int col)
      {
         VarValue vv = new VarValue((ConfigChunk)value);
         chunk.setProperty("keyPairs", row, vv);
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
                                 chunk.getProperty("modKey").getInt(),
                                 chunk.getProperty("key").getInt());
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
                              new VarValue(getJavaToVRJMod(catcher.getLastModKey())));
         keyChunk.setProperty("key", 0,
                              new VarValue(getJavaToVRJKey(catcher.getLastKey())));
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
            String text = getKeyModPairText(getJavaToVRJMod(lastModKey),
                                            getJavaToVRJKey(lastKey));
            setText(text);
            updateUI();
         }

         private int lastModKey = KeyEvent.VK_UNDEFINED;
         private int lastKey = KeyEvent.VK_UNDEFINED;
      }

      /**
       * Converts the given java key code to a VRJ key code for a modifier key.
       */
      private int getJavaToVRJMod(int javaKey)
      {
         PropertyDesc desc = keyChunk.getPropertyDesc("modKey");

         switch (javaKey)
         {
         case KeyEvent.VK_ALT:
            return desc.getEnumValue("ALT").getInt();
         case KeyEvent.VK_SHIFT:
            return desc.getEnumValue("SHIFT").getInt();
         case KeyEvent.VK_CONTROL:
            return desc.getEnumValue("CTRL").getInt();
         default:
            return desc.getEnumValue("NONE").getInt();
         }
      }

      /**
       * Converts the given java key code to a VRJ key code.
       */
      private int getJavaToVRJKey(int javaKey)
      {
         PropertyDesc desc = keyChunk.getPropertyDesc("key");
         switch (javaKey)
         {
         case KeyEvent.VK_UP:
            return desc.getEnumValue("KEY_UP").getInt();
         case KeyEvent.VK_DOWN:
            return desc.getEnumValue("KEY_DOWN").getInt();
         case KeyEvent.VK_LEFT:
            return desc.getEnumValue("KEY_LEFT").getInt();
         case KeyEvent.VK_RIGHT:
            return desc.getEnumValue("KEY_RIGHT").getInt();
         case KeyEvent.VK_CONTROL:
            return desc.getEnumValue("KEY_CTRL").getInt();
         case KeyEvent.VK_SHIFT:
            return desc.getEnumValue("KEY_SHIFT").getInt();
         case KeyEvent.VK_ALT:
            return desc.getEnumValue("KEY_ALT").getInt();
         case KeyEvent.VK_1:
            return desc.getEnumValue("KEY_1").getInt();
         case KeyEvent.VK_2:
            return desc.getEnumValue("KEY_2").getInt();
         case KeyEvent.VK_3:
            return desc.getEnumValue("KEY_3").getInt();
         case KeyEvent.VK_4:
            return desc.getEnumValue("KEY_4").getInt();
         case KeyEvent.VK_5:
            return desc.getEnumValue("KEY_5").getInt();
         case KeyEvent.VK_6:
            return desc.getEnumValue("KEY_6").getInt();
         case KeyEvent.VK_7:
            return desc.getEnumValue("KEY_7").getInt();
         case KeyEvent.VK_8:
            return desc.getEnumValue("KEY_8").getInt();
         case KeyEvent.VK_9:
            return desc.getEnumValue("KEY_9").getInt();
         case KeyEvent.VK_0:
            return desc.getEnumValue("KEY_0").getInt();
         case KeyEvent.VK_A:
            return desc.getEnumValue("KEY_A").getInt();
         case KeyEvent.VK_B:
            return desc.getEnumValue("KEY_B").getInt();
         case KeyEvent.VK_C:
            return desc.getEnumValue("KEY_C").getInt();
         case KeyEvent.VK_D:
            return desc.getEnumValue("KEY_D").getInt();
         case KeyEvent.VK_E:
            return desc.getEnumValue("KEY_E").getInt();
         case KeyEvent.VK_F:
            return desc.getEnumValue("KEY_F").getInt();
         case KeyEvent.VK_G:
            return desc.getEnumValue("KEY_G").getInt();
         case KeyEvent.VK_H:
            return desc.getEnumValue("KEY_H").getInt();
         case KeyEvent.VK_I:
            return desc.getEnumValue("KEY_I").getInt();
         case KeyEvent.VK_J:
            return desc.getEnumValue("KEY_K").getInt();
         case KeyEvent.VK_K:
            return desc.getEnumValue("KEY_J").getInt();
         case KeyEvent.VK_L:
            return desc.getEnumValue("KEY_L").getInt();
         case KeyEvent.VK_M:
            return desc.getEnumValue("KEY_M").getInt();
         case KeyEvent.VK_N:
            return desc.getEnumValue("KEY_N").getInt();
         case KeyEvent.VK_O:
            return desc.getEnumValue("KEY_O").getInt();
         case KeyEvent.VK_P:
            return desc.getEnumValue("KEY_P").getInt();
         case KeyEvent.VK_Q:
            return desc.getEnumValue("KEY_Q").getInt();
         case KeyEvent.VK_R:
            return desc.getEnumValue("KEY_R").getInt();
         case KeyEvent.VK_S:
            return desc.getEnumValue("KEY_S").getInt();
         case KeyEvent.VK_T:
            return desc.getEnumValue("KEY_T").getInt();
         case KeyEvent.VK_U:
            return desc.getEnumValue("KEY_U").getInt();
         case KeyEvent.VK_V:
            return desc.getEnumValue("KEY_V").getInt();
         case KeyEvent.VK_W:
            return desc.getEnumValue("KEY_W").getInt();
         case KeyEvent.VK_X:
            return desc.getEnumValue("KEY_X").getInt();
         case KeyEvent.VK_Y:
            return desc.getEnumValue("KEY_Y").getInt();
         case KeyEvent.VK_Z:
            return desc.getEnumValue("KEY_Z").getInt();
         case KeyEvent.VK_ESCAPE:
            return desc.getEnumValue("KEY_ESC").getInt();
         default:
            return desc.getEnumValue("KEY_NONE").getInt();
         }
      }

      private ConfigChunk keyChunk;
      private KeyCatcher catcher;
   }
}
