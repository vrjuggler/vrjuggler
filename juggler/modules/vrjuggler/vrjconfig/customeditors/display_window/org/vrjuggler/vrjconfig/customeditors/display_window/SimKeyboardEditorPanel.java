/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


public class SimKeyboardEditorPanel
   extends JPanel
   implements EditorConstants
{
   private static Map sSimDevProxyTypeMap;

   static
   {
      sSimDevProxyTypeMap = new HashMap();
      sSimDevProxyTypeMap.put(SIM_ANALOG_DEVICE_TYPE, ANALOG_PROXY_TYPE);
      sSimDevProxyTypeMap.put(SIM_DIGITAL_DEVICE_TYPE, DIGITAL_PROXY_TYPE);
      sSimDevProxyTypeMap.put(SIM_POS_DEVICE_TYPE, POSITION_PROXY_TYPE);
   }

   public SimKeyboardEditorPanel()
   {
      mDeviceList.setModel(new DefaultListModel());

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository def_repos = broker.getRepository();

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
         if ( ! kbdElt.getDefinition().getToken().equals(KEYBOARD_MOUSE_TYPE) )
         {
            throw new IllegalArgumentException("Element named '" +
                                               kbdElt.getName() +
                                               "' is not of type " +
                                               KEYBOARD_MOUSE_TYPE);
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
//         System.out.println("Looking for pointers to " + kbdElt.getName());
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
//               System.out.println("Proxy: " + proxy_elt.getName());
//               System.out.println("\tPointing at " + kbd_ptr.getTarget());
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
                     // and ad it to the list model for mDeviceList.
//                     System.out.println("\tSim Device: " + sim_elt.getName());
//                     System.out.println("\tPointing at " +
//                                        proxy_ptr.getTarget());
                     if ( proxy_ptr != null &&
                          proxy_ptr.getTarget() != null &&
                          proxy_ptr.getTarget().equals(proxy_elt.getName()) )
                     {
//                        System.out.println("\t\tMatched " +
//                                           proxy_elt.getName());
                        SimDeviceConfig def_cfg = new SimDeviceConfig(ctx,
                                                                      sim_elt);
                        DefaultListModel model =
                           (DefaultListModel) mDeviceList.getModel();
                        model.addElement(def_cfg);
                     }
                  }
               }
            }
         }
      }
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

   private void jbInit() throws Exception
   {
      this.setLayout(mMainLayout);
      mDevicePanel.setLayout(mDevicePanelLayout);
      mDeviceSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
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
      mDeviceButtonPanel.add(mAddSimDeviceButton);
      mDeviceButtonPanel.add(mRemoveSimDeviceButton);
      mDeviceList.addListSelectionListener(
         new SimKeyboardEditorPanel_mDeviceList_listSelectionAdapter(this)
      );
      mDeviceList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mDevicePanel.add(mDeviceButtonPanel, BorderLayout.NORTH);
      mDevicePanel.add(mDeviceScrollPane, BorderLayout.CENTER);
      mKeyboardEditor.setMinimumSize(new Dimension(500, 225));
      mKeyboardEditor.setPreferredSize(new Dimension(500, 225));
      mDeviceSplitPane.add(mEmptyEditor, JSplitPane.LEFT);
      mDeviceSplitPane.add(mKeyboardEditor, JSplitPane.RIGHT);
      this.add(mMainSplitPane, BorderLayout.CENTER);
      mMainSplitPane.add(mDevicePanel, JSplitPane.LEFT);
      mMainSplitPane.add(mDeviceSplitPane, JSplitPane.RIGHT);
      mDeviceScrollPane.getViewport().add(mDeviceList);
   }

   private ConfigContext mContext              = null;
   private ConfigElement keyboardDeviceElement = null;
   private ConfigElement keyboardProxyElement  = null;
   private Map           mSimDevProxyDefMap    = new HashMap();
   private String        mDataSourceName       = null;

   private Map mSimEditorMap = new HashMap();
   private SimDeviceEditor mCurSimEditor = null;
   private KeyboardEditorPanel mKeyboardEditor = new KeyboardEditorPanel();

   private JPanel mEmptyEditor = new JPanel();

   private JSplitPane mMainSplitPane = new JSplitPane();
   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mDevicePanel = new JPanel();
   private BorderLayout mDevicePanelLayout = new BorderLayout();
   private JPanel mDeviceButtonPanel = new JPanel();
   private JButton mAddSimDeviceButton = new JButton();
   private JButton mRemoveSimDeviceButton = new JButton();
   private JScrollPane mDeviceScrollPane = new JScrollPane();
   private JList mDeviceList = new JList();
   private JSplitPane mDeviceSplitPane = new JSplitPane();

   void mAddSimDeviceButton_actionPerformed(ActionEvent event)
   {
      if ( keyboardDeviceElement == null )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition kbd_def =
            broker.getRepository().get(KEYBOARD_MOUSE_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         // Create the new KEYBOARD_MOUSE_TYPE config element.  We won't bother
         // to ask the user for a name since they probably won't care anyway.
         keyboardDeviceElement = factory.createUnique(kbd_def, mContext);

         // Create a proxy to the new KEYBOARD_MOUSE_TYPE config element.
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
      SimDeviceConfig sim_dev_cfg = dlg.showDialog();

      if ( sim_dev_cfg != null )
      {
         DefaultListModel model = (DefaultListModel) mDeviceList.getModel();
         model.addElement(sim_dev_cfg);
      }
   }

   void mRemoveSimDeviceButton_actionPerformed(ActionEvent event)
   {
      SimDeviceConfig sim_dev_cfg =
         (SimDeviceConfig) mDeviceList.getSelectedValue();
      ConfigElement dev_elt = sim_dev_cfg.getDevice();

      if ( mCurSimEditor != null )
      {
         mDeviceSplitPane.remove(mCurSimEditor.getEditor());
      }

      mDeviceSplitPane.add(mEmptyEditor, JSplitPane.LEFT);
      mCurSimEditor = null;

      mDeviceSplitPane.revalidate();
      mDeviceSplitPane.repaint();

      mDeviceList.clearSelection();
      ((DefaultListModel) mDeviceList.getModel()).removeElement(sim_dev_cfg);

      ConfigBroker broker = new ConfigBrokerProxy();
      broker.remove(mContext, sim_dev_cfg.getAlias());
      broker.remove(mContext, sim_dev_cfg.getProxy());
      broker.remove(mContext, sim_dev_cfg.getDevice());
   }

   void mDeviceList_valueChanged(ListSelectionEvent listSelectionEvent)
   {
      if ( mCurSimEditor != null )
      {
         mDeviceSplitPane.remove(mCurSimEditor.getEditor());
         mDeviceSplitPane.add(mEmptyEditor, JSplitPane.LEFT);
         mCurSimEditor = null;
      }

      Object selection = mDeviceList.getSelectedValue();

      if ( selection != null )
      {
         SimDeviceConfig sim_dev_cfg = (SimDeviceConfig) selection;
         ConfigElement dev_elt       = sim_dev_cfg.getDevice();

         // Look up the editor for this config element's type.
         Object value = mSimEditorMap.get(dev_elt.getDefinition());

         if ( value != null )
         {
            Class editor_class = (Class) value;

            try
            {
               mCurSimEditor = (SimDeviceEditor) editor_class.newInstance();
               mCurSimEditor.setConfigElement(dev_elt);
               mDeviceSplitPane.remove(mEmptyEditor);
               mDeviceSplitPane.add(mCurSimEditor.getEditor(),
                                    JSplitPane.LEFT);
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
