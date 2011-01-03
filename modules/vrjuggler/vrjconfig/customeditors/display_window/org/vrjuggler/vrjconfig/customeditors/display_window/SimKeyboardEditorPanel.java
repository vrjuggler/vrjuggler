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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.commoneditors.DeviceConfig;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;


public class SimKeyboardEditorPanel
   extends JPanel
   implements EditorConstants
{
   /**
    * Mapping from simulated device config element type to device proxy
    * config element type.  These simulated device types rely on input received
    * through a proxy to a keyboard/mouse input handler.
    */
   private static Map sSimDevProxyTypeMap;

   /**
    * Mapping from non-keyboard simulated device config element type to device
    * proxy config element type.  These simulated device types do not rely on
    * keyboard/mouse input and instead get their input through other means.
    */
   private static Map sSimDevNoKbdProxyTypeMap;

   /**
    * Mapping from simulated device config element type to a SimDeviceEditor
    * type that is used to edit the configuration for that simulated device
    * type.
    */
   private static Map sSimDevEditorMap;

   static
   {
      // Set up the mapping for keyboard-based simulated device types and
      // their proxies.
      sSimDevProxyTypeMap = new HashMap();
      sSimDevProxyTypeMap.put(SIM_ANALOG_DEVICE_TYPE, ANALOG_PROXY_TYPE);
      sSimDevProxyTypeMap.put(SIM_DIGITAL_DEVICE_TYPE, DIGITAL_PROXY_TYPE);
      sSimDevProxyTypeMap.put(SIM_POS_DEVICE_TYPE, POSITION_PROXY_TYPE);

      // Set up the mapping for non-keyboard simulated device types and their
      // proxies.
      sSimDevNoKbdProxyTypeMap = new HashMap();
      sSimDevNoKbdProxyTypeMap.put(SIM_RELATIVE_POS_DEVICE_TYPE,
                                   POSITION_PROXY_TYPE);

      // Set up the mapping for each simulated device type config elements and
      // its SimDeviceEditor implementation type.
      sSimDevEditorMap = new HashMap();
      sSimDevEditorMap.put(SIM_ANALOG_DEVICE_TYPE,
                           SimAnalogDeviceEditor.class);
      sSimDevEditorMap.put(SIM_DIGITAL_DEVICE_TYPE,
                           SimDigitalDeviceEditor.class);
      sSimDevEditorMap.put(SIM_POS_DEVICE_TYPE, SimPositionDeviceEditor.class);
   }

   public SimKeyboardEditorPanel()
   {
      mDeviceList.setModel(new DefaultListModel());

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository def_repos = broker.getRepository();

      // Set up the mapping from the simulated device type ConfigDefinition
      // reference to the device proxy type ConfigDefinition reference.  The
      // map mSimDevProxyDefMap will have a direct correlation to
      // sSimDevProxyTypeMap in terms of its key/value pairs.  In
      // sSimDevProxyTypeMap, the mapping is from String to String.  In
      // mSimDevProxyDefMap, the mapping is from ConfigDefinition to
      // ConfigDefinition.
      // XXX: This is done here because I am not sure if it is safe to use the
      // Config Broker during static initialization.
      for ( Iterator i = sSimDevProxyTypeMap.keySet().iterator();
            i.hasNext(); )
      {
         String dev_type   = (String) i.next();
         String proxy_type = (String) sSimDevProxyTypeMap.get(dev_type);
         mSimDevProxyDefMap.put(def_repos.get(dev_type),
                                def_repos.get(proxy_type));
      }

      try
      {
         jbInit();
         mAllProxyEditor.getDeviceProxyEditor().getGraph().setPortsVisible(true);
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   /**
    * Sets up the configuration information for this panel.  This is primarily
    * a convenience method intended to eliminate the need for the caller to
    * invoke the methods setConfig(ConfigContext,ConfigElement) and
    * setDataSourceName(String) in succession.  In general, this should only
    * be invoked once for a given instance of this class.  At the moment, the
    * implementation of this method expects to be invoked only once.
    *
    * @param ctx                the config context that will be used by this
    *                           panel
    * @param kbdElt             the keyboard/mouse device config element that
    *                           serves as the basis for what this panel edits
    * @param dataSourceName     the name of the data source that will be used
    *                           by this panel for adding new config elements
    *                           and removing existing config elements
    *
    * @see #setConfig(ConfigContext, ConfigElement)
    * @see #setDataSourceName(String)
    */
   public void setConfig(ConfigContext ctx, ConfigElement kbdElt,
                         String dataSourceName)
   {
      setConfig(ctx, kbdElt);
      setDataSourceName(dataSourceName);
   }

   /**
    * Sets up the configuration information for this panel.  In general, this
    * should only be invoked once for a given instance of this class.  At
    * the moment, the implementation of this method expects to be invoked only
    * once.
    *
    * @param ctx        the config context that will be used by this panel
    * @param kbdElt     the keyboard/mouse device config element that serves
    *                   as the basis for what this panel edits
    */
   public void setConfig(ConfigContext ctx, ConfigElement kbdElt)
   {
      mContext = ctx;

      // If the ConfigElement reference we are given is null, then there is
      // nothing for this panel to do.
      if ( kbdElt == null )
      {
         mRemoveSimDeviceButton.setEnabled(false);
      }
      // If the given ConfigElement reference is not null, then we will not
      // allow a new keyboard/mouse device config element to be created.
      // The amount of looping that happens in here is insane...
      else
      {
         if ( ! kbdElt.getDefinition().getToken().equals(KEYBOARD_MOUSE_DEVICE_TYPE) )
         {
            throw new IllegalArgumentException("Element named '" +
                                               kbdElt.getName() +
                                               "' is not of type " +
                                               KEYBOARD_MOUSE_DEVICE_TYPE);
         }

         keyboardDeviceElement = kbdElt;

         ConfigBroker broker = new ConfigBrokerProxy();
         List elements = broker.getElements(ctx);
         String proxy_type = KEYBOARD_MOUSE_PROXY_TYPE;
         String proxy_ptr_prop = KEYBOARD_MOUSE_PROXY_PTR_PROPERTY;

         List sim_dev_elts = new ArrayList();
         java.util.Set sim_dev_types = sSimDevProxyTypeMap.keySet();

         // Pull out all the config elements that are simulated devices
         // that we handle.
         for ( Iterator i = elements.iterator(); i.hasNext(); )
         {
            ConfigElement elt = (ConfigElement) i.next();

            if ( sim_dev_types.contains(elt.getDefinition().getToken()) )
            {
               sim_dev_elts.add(elt);
            }
         }

         // Search through all the elements in this context looking for
         // simulated devices that make use of kbdElt for keyboard/mouse
         // input.
         for ( Iterator i = elements.iterator(); i.hasNext(); )
         {
            ConfigElement proxy_elt = (ConfigElement) i.next();

            // If the current config element is a keyboard proxy, then we
            // need to see if it points at kbdElt first.
            if ( proxy_elt.getDefinition().getToken().equals(proxy_type) )
            {
               ConfigElementPointer kbd_ptr =
                  (ConfigElementPointer) proxy_elt.getProperty(DEVICE_PROPERTY,
                                                               0);

               // If keyboard proxy refers to the element kbdElt, then we will
               // search for config elements for simulated devices that refer
               // to that proxy.
               if ( kbd_ptr != null && kbd_ptr.getTarget() != null &&
                    kbd_ptr.getTarget().equals(kbdElt.getName()) )
               {
                  keyboardProxyElement = proxy_elt;

                  // Loop over all the simulated device config elements we
                  // know about looking for those that refer to the keyboard
                  // proxy proxy_elt.
                  for ( Iterator j = sim_dev_elts.iterator(); j.hasNext(); )
                  {
                     ConfigElement sim_elt = (ConfigElement) j.next();

                     ConfigElementPointer proxy_ptr =
                        (ConfigElementPointer) sim_elt.getProperty(proxy_ptr_prop,
                                                                   0);

                     // If proxy_ptr refers to proxy_elt, then we have found
                     // a simulated device config element that gets its input
                     // from kbdElt.  We need to create a holder for sim_elt
                     // and add it to the list model for mDeviceList.
                     if ( proxy_ptr != null &&
                          proxy_ptr.getTarget() != null &&
                          proxy_ptr.getTarget().equals(proxy_elt.getName()) )
                     {
                        DeviceConfig def_cfg = new DeviceConfig(ctx, sim_elt);
                        DefaultListModel model =
                           (DefaultListModel) mDeviceList.getModel();
                        model.addElement(def_cfg);
                     }
                  }
               }
            }
         }
      }

      mAllProxyEditor.setConfig(mContext, null);
   }

   /**
    * Updates the name of the data source being used by this panel for
    * adding and removing config elements.  This may be invoked at any time.
    *
    * @param dataSourceName     the name of the data source that will be used
    *                           by this panel for adding new config elements
    *                           and removing existing config elements
    */
   public void setDataSourceName(String dataSourceName)
   {
      mDataSourceName = dataSourceName;
   }

   public ConfigElement getKeyboardDeviceElement()
   {
      return keyboardDeviceElement;
   }

   public void editorClosing()
   {
      for ( Iterator e = mSimEditorCache.keySet().iterator(); e.hasNext(); )
      {
         SimDeviceEditor editor =
            (SimDeviceEditor) mSimEditorCache.get(e.next());
         editor.editorClosing();
      }

      mSimEditorCache.clear();
   }

   private void jbInit() throws Exception
   {
      this.setLayout(mMainLayout);
      mDevicePanel.setLayout(mDevicePanelLayout);
      mAddSimDeviceButton.setText("Create Sim Device");
      mAddSimDeviceButton.setToolTipText("Create a new simulator device configuration");
      mAddSimDeviceButton.addActionListener(
         new SimKeyboardEditorPanel_mAddSimDeviceButton_actionAdapter(this)
      );
      mRemoveSimDeviceButton.setEnabled(false);
      mRemoveSimDeviceButton.setText("Remove Sim Device");
      mRemoveSimDeviceButton.setToolTipText("Remove the selected simulator device configuration");
      mRemoveSimDeviceButton.addActionListener(
         new SimKeyboardEditorPanel_mRemoveSimDeviceButton_actionAdapter(this)
      );
      mAddSimDeviceButton.setAlignmentX(0.5f);
      mRemoveSimDeviceButton.setAlignmentX(0.5f);
      mDeviceButtonPanel.add(mAddSimDeviceButton);
      mDeviceButtonPanel.add(mRemoveSimDeviceButton);
      mDeviceList.addListSelectionListener(
         new SimKeyboardEditorPanel_mDeviceList_listSelectionAdapter(this)
      );
      mDeviceList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mDevicePanel.add(mDeviceButtonPanel, BorderLayout.NORTH);
      mDevicePanel.add(mDeviceScrollPane, BorderLayout.CENTER);
      mAdvancedEditorPanel.setLayout(new BorderLayout());
      mAdvancedEditorPanel.add(mAllProxyEditor, BorderLayout.CENTER);
      mTabContainer.add(mDeviceEditorPane, "Simulator Devices");
      mTabContainer.add(mAdvancedEditorPanel, "Advanced");
      mDeviceEditorPane.add(mDevicePanel, JSplitPane.LEFT);
      mDeviceEditorPane.add(mEmptyEditor, JSplitPane.RIGHT);
      mDeviceScrollPane.getViewport().add(mDeviceList);
      this.add(mTabContainer, BorderLayout.CENTER);
   }

   private ConfigContext mContext              = null;
   private ConfigElement keyboardDeviceElement = null;
   private ConfigElement keyboardProxyElement  = null;
   private Map           mSimDevProxyDefMap    = new HashMap();
   private String        mDataSourceName       = null;

   private Map mSimEditorCache = new HashMap();

   private SimDeviceEditor mCurSimEditor = null;

   private JPanel mEmptyEditor = new JPanel();

   private BorderLayout mMainLayout = new BorderLayout();
   private JTabbedPane mTabContainer = new JTabbedPane();
   private JSplitPane mDeviceEditorPane = new JSplitPane();
   private JPanel mDevicePanel = new JPanel();
   private BorderLayout mDevicePanelLayout = new BorderLayout();
   private Box mDeviceButtonPanel = new Box(BoxLayout.Y_AXIS);
   private JButton mAddSimDeviceButton = new JButton();
   private JButton mRemoveSimDeviceButton = new JButton();
   private JScrollPane mDeviceScrollPane = new JScrollPane();
   private JList mDeviceList = new JList();
   private JPanel mAdvancedEditorPanel = new JPanel();
   private ProxyEditorUI mAllProxyEditor = new ProxyEditorUI();

   void mAddSimDeviceButton_actionPerformed(ActionEvent event)
   {
      if ( keyboardDeviceElement == null )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition kbd_def =
            broker.getRepository().get(KEYBOARD_MOUSE_DEVICE_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         // Create the new KEYBOARD_MOUSE_DEVICE_TYPE config element.  We won't
         // bother to ask the user for a name since they probably won't care
         // anyway.
         keyboardDeviceElement = factory.createUnique(kbd_def, mContext);

         // Create a proxy to the new KEYBOARD_MOUSE_DEVICE_TYPE config
         // element.
         ConfigDefinition kbd_proxy_def =
            broker.getRepository().get(KEYBOARD_MOUSE_PROXY_TYPE);
         keyboardProxyElement =
            factory.create(keyboardDeviceElement.getName() + " Proxy",
                           kbd_proxy_def);
         keyboardProxyElement.setProperty(DEVICE_PROPERTY, 0,
                                          keyboardDeviceElement.getName());

         // Add the newly created config elements to the data source that the
         // user selected.
         broker.add(mContext, keyboardDeviceElement, mDataSourceName);
         broker.add(mContext, keyboardProxyElement, mDataSourceName);
      }

      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class, this);

      // Instead of having SimDeviceCreateDialog have yet another resource
      // chooser, it will use the same data source that is currently in use
      // for this panel.  As far as the user is concerned, this should make
      // sense.  Furthermore, the device to be created will need to reference
      // keyboardProxyElement, so it would generally be best to have all those
      // config elements in the same file.
      SimDeviceCreateDialog dlg =
         new SimDeviceCreateDialog(parent, mContext, keyboardProxyElement,
                                   mSimDevProxyDefMap, mDataSourceName);
      DeviceConfig sim_dev_cfg = dlg.showDialog();

      if ( sim_dev_cfg != null )
      {
         DefaultListModel model = (DefaultListModel) mDeviceList.getModel();
         model.addElement(sim_dev_cfg);
      }
   }

   void mRemoveSimDeviceButton_actionPerformed(ActionEvent event)
   {
      DeviceConfig sim_dev_cfg = (DeviceConfig) mDeviceList.getSelectedValue();
      ConfigElement dev_elt = sim_dev_cfg.getDevice();

      if ( mCurSimEditor != null )
      {
         mDeviceEditorPane.remove(mCurSimEditor.getEditor());
      }

      mDeviceEditorPane.add(mEmptyEditor, JSplitPane.RIGHT);
      mCurSimEditor = null;

      mDeviceEditorPane.revalidate();
      mDeviceEditorPane.repaint();

      mDeviceList.clearSelection();
      ((DefaultListModel) mDeviceList.getModel()).removeElement(sim_dev_cfg);

      ConfigBroker broker = new ConfigBrokerProxy();

      List aliases = sim_dev_cfg.getAliases();
      for ( Iterator i = aliases.iterator(); i.hasNext(); )
      {
         broker.remove(mContext, (ConfigElement) i.next());
      }

      List proxies = sim_dev_cfg.getProxies();
      for ( Iterator i = proxies.iterator(); i.hasNext(); )
      {
         broker.remove(mContext, (ConfigElement) i.next());
      }

      broker.remove(mContext, sim_dev_cfg.getDevice());
   }

   void mDeviceList_valueChanged(ListSelectionEvent listSelectionEvent)
   {
      if ( mCurSimEditor != null )
      {
         mDeviceEditorPane.remove(mCurSimEditor.getEditor());
         mDeviceEditorPane.add(mEmptyEditor, JSplitPane.RIGHT);
         mCurSimEditor = null;
      }

      Object selection = mDeviceList.getSelectedValue();

      if ( selection != null )
      {
         DeviceConfig sim_dev_cfg = (DeviceConfig) selection;
         ConfigElement dev_elt    = sim_dev_cfg.getDevice();

         Object cached_editor = mSimEditorCache.get(dev_elt);

         if ( cached_editor != null )
         {
            mCurSimEditor = (SimDeviceEditor) cached_editor;
            mDeviceEditorPane.remove(mEmptyEditor);
            mDeviceEditorPane.add(mCurSimEditor.getEditor(), JSplitPane.RIGHT);
         }
         else
         {
            // Look up the editor for this config element's type.
            Object value =
               sSimDevEditorMap.get(dev_elt.getDefinition().getToken());

            if ( value != null )
            {
               Class editor_class = (Class) value;

               try
               {
                  mCurSimEditor = (SimDeviceEditor) editor_class.newInstance();
                  mCurSimEditor.setConfig(mContext, dev_elt);
                  mDeviceEditorPane.remove(mEmptyEditor);
                  mDeviceEditorPane.add(mCurSimEditor.getEditor(),
                                        JSplitPane.RIGHT);
                  mSimEditorCache.put(dev_elt, mCurSimEditor);
               }
               catch (Exception ex)
               {
                  ex.printStackTrace();
               }
            }
            else
            {
               /* No editor! */ ;
            }
         }
      }

      mRemoveSimDeviceButton.setEnabled(selection != null);
   }

   private class SimDeviceRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public SimDeviceRenderer()
      {
         setOpaque(true);
         setHorizontalAlignment(LEFT);
         setVerticalAlignment(CENTER);
      }

      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean cellHasFocus)
      {
         if ( isSelected )
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         setFont(list.getFont());
         setText(value.toString());

         return this;
      }
   }
}

class SimKeyboardEditorPanel_mAddSimDeviceButton_actionAdapter
   implements ActionListener
{
   private SimKeyboardEditorPanel adaptee;
   SimKeyboardEditorPanel_mAddSimDeviceButton_actionAdapter(SimKeyboardEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mAddSimDeviceButton_actionPerformed(actionEvent);
   }
}

class SimKeyboardEditorPanel_mRemoveSimDeviceButton_actionAdapter
   implements ActionListener
{
   private SimKeyboardEditorPanel adaptee;
   SimKeyboardEditorPanel_mRemoveSimDeviceButton_actionAdapter(SimKeyboardEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mRemoveSimDeviceButton_actionPerformed(actionEvent);
   }
}

class SimKeyboardEditorPanel_mDeviceList_listSelectionAdapter
   implements ListSelectionListener
{
   private SimKeyboardEditorPanel adaptee;
   SimKeyboardEditorPanel_mDeviceList_listSelectionAdapter(SimKeyboardEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void valueChanged(ListSelectionEvent listSelectionEvent)
   {
      adaptee.mDeviceList_valueChanged(listSelectionEvent);
   }
}
