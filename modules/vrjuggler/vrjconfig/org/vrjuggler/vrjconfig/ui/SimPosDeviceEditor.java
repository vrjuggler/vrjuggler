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
import org.vrjuggler.jccl.config.event.*;

/**
 * A component suitable for editing a simulator device.
 */
public class SimPosDeviceEditor
   extends JPanel
{
   /**
    * Creates a new sim device editor with no element to edit.
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
      List elts = getBroker().getElements(context);
      if (elts.size() > 0)
      {
         List proxies = ConfigUtilities.getElementsWithDefinition(elts, "KeyboardProxy");
         for (Iterator itr = proxies.iterator(); itr.hasNext(); )
         {
            ConfigElement proxy = (ConfigElement)itr.next();
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
    * Makes the SimPosDevice element active.
    */
   public void setDevice(ConfigElement device)
   {
      // Make sure the element is a SimPosition element.
      if (device != null && (!device.getDefinition().getToken().equals("simulated_positional_device")))
      {
         throw new IllegalArgumentException("device must be a SimPosition element");
      }

      // Change the device
      ConfigElement old = this.device;
      if (this.device != null)
      {
         this.device.removeConfigElementListener(deviceListener);
      }
      this.device = device;
      if (this.device != null)
      {
         this.device.addConfigElementListener(deviceListener);
      }

      // Let the model know
      actionModel.setElement(this.device);
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
   private ConfigBroker getBroker()
   {
      return new ConfigBrokerProxy();
   }

   /**
    * Gets a valid text string for a KeyModPair using the current device.
    */
   protected String getKeyModPairText(int modKey, int key)
   {
      ConfigElement key_mod_pair_elt = (ConfigElement)device.getProperty("key_pair", 0);
      String mod_str = getModString(key_mod_pair_elt, modKey);
      String key_str = getKeyString(key_mod_pair_elt, key);

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

   protected String getModString(ConfigElement elt, int mod)
   {
      Map enums = elt.getDefinition().getPropertyDefinition("modifier_key").getEnums();
      for (Iterator itr=enums.keySet().iterator(); itr.hasNext(); )
      {
         String label = (String)itr.next();
         if (enums.get(label).equals(new Integer(mod)))
         {
            return label;
         }
      }
      return "";
   }

   protected String getKeyString(ConfigElement elt, int key)
   {
      Map enums = elt.getDefinition().getPropertyDefinition("key").getEnums();
      for (Iterator itr=enums.keySet().iterator(); itr.hasNext(); )
      {
         String label = (String)itr.next();
         if (enums.get(label).equals(new Integer(key)))
         {
            return label;
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
    * The config element of the device we are currently editing.
    */
   private ConfigElement device;

   /**
    * Our view into the configuration.
    */
   private ConfigContext context;

   /**
    * Table model for the actions table.
    */
   private ActionTableModel actionModel = new ActionTableModel();

   /**
    * Listener for the edited config element.
    */
   private DeviceListener deviceListener = new DeviceListener();

   /**
    * Specialized listener on the config element.
    */
   private class DeviceListener
      implements ConfigElementListener
   {
      public void nameChanged(ConfigElementEvent evt)
      {
         System.err.println("SimDeviceEditor.deviceListener: Device changed!");
      }

      public void propertyValueChanged(ConfigElementEvent evt) { nameChanged(evt); }
      public void propertyValueAdded(ConfigElementEvent evt) { nameChanged(evt); }
      public void propertyValueRemoved(ConfigElementEvent evt) { nameChanged(evt); }
   }

   /**
    * Specialized table model for the action mappings.
    */
   private class ActionTableModel
      extends AbstractTableModel
   {
      public void setElement(ConfigElement element)
      {
         mElement = element;
         fireTableDataChanged();
      }

      public ConfigElement getElement()
      {
         return mElement;
      }

      public int getRowCount()
      {
         if (mElement != null)
         {
            return mElement.getPropertyValueCount("key_pair");
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
         if (mElement != null)
         {
            if (col == 0)
            {
               PropertyDefinition prop_def = mElement.getDefinition().getPropertyDefinition("key_pair");
               return prop_def.getPropertyValueDefinition(row);
            }
            else
            {
               return mElement.getProperty("key_pair", row);
            }
         }
         return null;
      }

      public void setValueAt(Object value, int row, int col)
      {
         mElement.setProperty("key_pair", row, value);
         fireTableCellUpdated(row, col);
      }

      private ConfigElement mElement;
      private String[] columnNames = { "Action", "Key Combo" };
   }

   /**
    * Customized table cell renderer for keymod pair config elements.
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

         if (value instanceof ConfigElement)
         {
            ConfigElement elt = (ConfigElement)value;
            if (elt.getDefinition().getToken().equals("key_modifier_pair"))
            {
               String text = getKeyModPairText(
                                 ((Integer)elt.getProperty("modifier_key", 0)).intValue(),
                                 ((Integer)elt.getProperty("key", 0)).intValue());
               setText(text);
            }
         }

         return this;
      }
   }

   /**
    * Specialized editor for KeyModPair config elements.
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
         mKeyElement = (ConfigElement)value;
         catcher = new KeyCatcher();
         catcher.requestFocus();
         return catcher;
      }

      public Object getCellEditorValue()
      {
         // Update the element with the new keys
         mKeyElement.setProperty("modifier_key", 0,
                                 getJavaToVRJMod(catcher.getLastModKey()));
         mKeyElement.setProperty("key", 0,
                                 getJavaToVRJKey(catcher.getLastKey()));
         return mKeyElement;
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
         PropertyDefinition prop_def = mKeyElement.getDefinition().getPropertyDefinition("modifier_key");
         Object key;

         switch (javaKey)
         {
         case KeyEvent.VK_ALT:
            key = prop_def.getEnums().get("ALT");
         case KeyEvent.VK_SHIFT:
            key = prop_def.getEnums().get("SHIFT");
         case KeyEvent.VK_CONTROL:
            key = prop_def.getEnums().get("CTRL");
         default:
            key = prop_def.getEnums().get("NONE");
         }

         return (Integer)key;
      }

      /**
       * Converts the given java key code to a VRJ key code.
       */
      private Integer getJavaToVRJKey(int javaKey)
      {
         PropertyDefinition prop_def = mKeyElement.getDefinition().getPropertyDefinition("key");
         Object key;

         switch (javaKey)
         {
         case KeyEvent.VK_UP:
            key = prop_def.getEnums().get("KEY_UP");
         case KeyEvent.VK_DOWN:
            key = prop_def.getEnums().get("KEY_DOWN");
         case KeyEvent.VK_LEFT:
            key = prop_def.getEnums().get("KEY_LEFT");
         case KeyEvent.VK_RIGHT:
            key = prop_def.getEnums().get("KEY_RIGHT");
         case KeyEvent.VK_CONTROL:
            key = prop_def.getEnums().get("KEY_CTRL");
         case KeyEvent.VK_SHIFT:
            key = prop_def.getEnums().get("KEY_SHIFT");
         case KeyEvent.VK_ALT:
            key = prop_def.getEnums().get("KEY_ALT");
         case KeyEvent.VK_1:
            key = prop_def.getEnums().get("KEY_1");
         case KeyEvent.VK_2:
            key = prop_def.getEnums().get("KEY_2");
         case KeyEvent.VK_3:
            key = prop_def.getEnums().get("KEY_3");
         case KeyEvent.VK_4:
            key = prop_def.getEnums().get("KEY_4");
         case KeyEvent.VK_5:
            key = prop_def.getEnums().get("KEY_5");
         case KeyEvent.VK_6:
            key = prop_def.getEnums().get("KEY_6");
         case KeyEvent.VK_7:
            key = prop_def.getEnums().get("KEY_7");
         case KeyEvent.VK_8:
            key = prop_def.getEnums().get("KEY_8");
         case KeyEvent.VK_9:
            key = prop_def.getEnums().get("KEY_9");
         case KeyEvent.VK_0:
            key = prop_def.getEnums().get("KEY_0");
         case KeyEvent.VK_A:
            key = prop_def.getEnums().get("KEY_A");
         case KeyEvent.VK_B:
            key = prop_def.getEnums().get("KEY_B");
         case KeyEvent.VK_C:
            key = prop_def.getEnums().get("KEY_C");
         case KeyEvent.VK_D:
            key = prop_def.getEnums().get("KEY_D");
         case KeyEvent.VK_E:
            key = prop_def.getEnums().get("KEY_E");
         case KeyEvent.VK_F:
            key = prop_def.getEnums().get("KEY_F");
         case KeyEvent.VK_G:
            key = prop_def.getEnums().get("KEY_G");
         case KeyEvent.VK_H:
            key = prop_def.getEnums().get("KEY_H");
         case KeyEvent.VK_I:
            key = prop_def.getEnums().get("KEY_I");
         case KeyEvent.VK_J:
            key = prop_def.getEnums().get("KEY_K");
         case KeyEvent.VK_K:
            key = prop_def.getEnums().get("KEY_J");
         case KeyEvent.VK_L:
            key = prop_def.getEnums().get("KEY_L");
         case KeyEvent.VK_M:
            key = prop_def.getEnums().get("KEY_M");
         case KeyEvent.VK_N:
            key = prop_def.getEnums().get("KEY_N");
         case KeyEvent.VK_O:
            key = prop_def.getEnums().get("KEY_O");
         case KeyEvent.VK_P:
            key = prop_def.getEnums().get("KEY_P");
         case KeyEvent.VK_Q:
            key = prop_def.getEnums().get("KEY_Q");
         case KeyEvent.VK_R:
            key = prop_def.getEnums().get("KEY_R");
         case KeyEvent.VK_S:
            key = prop_def.getEnums().get("KEY_S");
         case KeyEvent.VK_T:
            key = prop_def.getEnums().get("KEY_T");
         case KeyEvent.VK_U:
            key = prop_def.getEnums().get("KEY_U");
         case KeyEvent.VK_V:
            key = prop_def.getEnums().get("KEY_V");
         case KeyEvent.VK_W:
            key = prop_def.getEnums().get("KEY_W");
         case KeyEvent.VK_X:
            key = prop_def.getEnums().get("KEY_X");
         case KeyEvent.VK_Y:
            key = prop_def.getEnums().get("KEY_Y");
         case KeyEvent.VK_Z:
            key = prop_def.getEnums().get("KEY_Z");
         case KeyEvent.VK_ESCAPE:
            key = prop_def.getEnums().get("KEY_ESC");
         default:
            key = prop_def.getEnums().get("KEY_NONE");
         }

         return (Integer)key;
      }

      private ConfigElement mKeyElement;
      private KeyCatcher catcher;
   }
}
