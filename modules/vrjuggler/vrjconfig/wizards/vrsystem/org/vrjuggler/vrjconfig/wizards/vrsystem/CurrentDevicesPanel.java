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
package org.vrjuggler.vrjconfig.wizards.vrsystem;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.*;
import java.awt.Font;
import java.awt.Frame;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.SimpleEditorDialog;
import org.vrjuggler.vrjconfig.commoneditors.CustomEditorDialog;
import info.clearthought.layout.*;

public class CurrentDevicesPanel extends JPanel implements EditorConstants, ConfigListener
{
   private Icon mAddIcon = null;
   private Icon mRemoveIcon = null;
   private Icon mDisabledRemoveIcon = null;
   private Icon mEditIcon = null;
   private Icon mDisabledEditIcon = null;
   private TableLayout mRightPanelLayout = null;
   private JPanel mRightPanel = new JPanel();

   private JPanel mDirectionsPanel = new JPanel(new BorderLayout());
   private JLabel mTitleLbl = new JLabel();
   private JTextArea mDirectionsLbl = new JTextArea();
   private JPanel mTopPanel = new JPanel();
   private JLabel mDeviceIcon = new JLabel();
   private JList mDeviceList = new JList();
   private DeviceListModel mDeviceListModel = null;
   private JScrollPane mDeviceListScrollPane = new JScrollPane();
   private JButton mAddDeviceBtn = new JButton();
   private JButton mRemoveDeviceBtn = new JButton();
   private JButton mEditDeviceBtn = new JButton();
   private Map mWhiteBoard;
   private ConfigContext mConfigContext = null;

   public CurrentDevicesPanel()
   {
      try
      {

         jbInit();
         
         DefinitionListRenderer renderer2 = new DefinitionListRenderer();
         mDeviceList.setCellRenderer(renderer2);
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mDeviceIcon.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/devices64.png")));
      }
      catch(NullPointerException exp)
      {
         mDeviceIcon.setText("");
      }
   }
   
   /**
    * Gives notification that a configuration element was added to the broker.
    *
    * @param evt     the config event
    */
   public void configElementAdded(ConfigEvent evt)
   {
      //XXX: This needs to change since it is very slow
      //     to rebuild the model everytime.
      mDeviceListModel = new DeviceListModel(mConfigContext);
      mDeviceList.setModel(mDeviceListModel);
   }

   /**
    * Gives notification that a config element was removed from the broker.
    *
    * @param evt     the config event
    */
   public void configElementRemoved(ConfigEvent evt)
   {
      //XXX: This needs to change since it is very slow
      //     to rebuild the model everytime.
      mDeviceListModel = new DeviceListModel(mConfigContext);
      mDeviceList.setModel(mDeviceListModel);
   }

   private void jbInit() throws Exception
   {
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.CENTER);
      mTitleLbl.setText("Input Device Manager");
      mDirectionsLbl.setText("All currently configured input devices such as "
                           + "trackers and button devices are listed below.");
      mDirectionsLbl.setLineWrap(true);
      mDirectionsLbl.setEditable(false);
      mDirectionsLbl.setBackground(mTitleLbl.getBackground());

      mDirectionsPanel.add(mTitleLbl, BorderLayout.NORTH);
      mDirectionsPanel.add(mDirectionsLbl, BorderLayout.CENTER);
      
      mTopPanel.setLayout(new BorderLayout());
      mTopPanel.setBorder(BorderFactory.createEtchedBorder());
      mTopPanel.add(mDirectionsPanel, BorderLayout.CENTER);
      mTopPanel.add(mDeviceIcon, BorderLayout.EAST);
      
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mAddIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/add.gif"));
         mRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove.gif"));
         mDisabledRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove_disabled.gif"));
         mEditIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/edit.gif"));
         mDisabledEditIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/edit_disabled.gif"));
         
         mAddDeviceBtn.setIcon(mAddIcon);
         mRemoveDeviceBtn.setIcon(mRemoveIcon);
         mRemoveDeviceBtn.setDisabledIcon(mDisabledRemoveIcon);
         mEditDeviceBtn.setIcon(mEditIcon);
         mEditDeviceBtn.setDisabledIcon(mDisabledEditIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mAddDeviceBtn.setText("Add");
         mRemoveDeviceBtn.setText("Remove");
         mEditDeviceBtn.setText("Edit");
      }
      
      mAddDeviceBtn.setToolTipText("Configure a new input device.");
      mAddDeviceBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddDeviceBtn_actionPerformed(e);
         }
      });
      
      mRemoveDeviceBtn.setEnabled(false);
      mRemoveDeviceBtn.setToolTipText("Remove the currently selected input device.");
      mRemoveDeviceBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mRemoveDeviceBtn_actionPerformed(e);
         }
      });
      
      mEditDeviceBtn.setEnabled(false);
      mEditDeviceBtn.setToolTipText("Edit the currently selected input device.");
      mEditDeviceBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mEditDeviceBtn_actionPerformed(e);
         }
      });

      mDeviceListScrollPane.getViewport().add(mDeviceList, null);
      mDeviceListScrollPane.setBorder(BorderFactory.createLoweredBevelBorder());
      mDeviceList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mDeviceList.addListSelectionListener(new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               if (e.getValueIsAdjusting() == false)
               {
                  // We need to ensure that the selected item is
                  // a valid ConfigDefinition.
                  if (mDeviceList.getSelectedIndex() != -1 &&
                      mDeviceList.getSelectedValue() instanceof ConfigDefinition)
                  {
                     mRemoveDeviceBtn.setEnabled(true);
                     mEditDeviceBtn.setEnabled(true);
                  }
                  else
                  {
                     mRemoveDeviceBtn.setEnabled(false);
                     mEditDeviceBtn.setEnabled(false);
                  }
               }
            }
         });

      double[][] right_size = {{25, TableLayout.FILL},
                              {25, 25, 25, TableLayout.FILL}};
      mRightPanelLayout = new TableLayout(right_size);
      
      mRightPanel.setLayout(mRightPanelLayout);
      mRightPanel.add(mAddDeviceBtn,
                             new TableLayoutConstraints(0, 0, 0, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mRightPanel.add(mRemoveDeviceBtn,
                             new TableLayoutConstraints(0, 1, 0, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mRightPanel.add(mEditDeviceBtn,
                             new TableLayoutConstraints(0, 2, 0, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      
      setLayout(new BorderLayout());
      add(mTopPanel, BorderLayout.NORTH);
      add(mDeviceListScrollPane, BorderLayout.CENTER);
      add(mRightPanel, BorderLayout.EAST);
   }
   
   private void mAddDeviceBtn_actionPerformed(ActionEvent e)
   {
      final JDialog dialog = new JDialog();
      dialog.setSize(590, 430);
      dialog.setModal(true);
      dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
      dialog.getContentPane().setLayout(new BorderLayout());

      try
      {
         Wizard wizard = WizardLoader.loadWizard(
            "${VJ_BASE_DIR}/share/vrjuggler/beans/wizards/NewDeviceWizard.jar"
         );

         wizard.setMap(mWhiteBoard);
         
         WizardViewerBean viewer = new WizardViewerBean(mWhiteBoard);
         viewer.setWizard(wizard);
         viewer.setSidebarImage(new ImageIcon(this.getClass().getClassLoader().getResource(
                     "org/vrjuggler/tweek/wizard/images/juggler_sidebar.png")));
         viewer.addWizardViewListener(new WizardViewListener()
         {
            public void wizardStarted(WizardViewEvent evt) {}

            public void wizardCancelled(WizardViewEvent evt)
            {
               dialog.setVisible(false);
               dialog.dispose();
            }

            public void wizardFinished(WizardViewEvent evt)
            {
               dialog.setVisible(false);
               dialog.dispose();
            }
         });
         dialog.getContentPane().add(viewer, BorderLayout.CENTER);
         dialog.setVisible(true);
      }
      catch(IOException ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }
   }

   private void mRemoveDeviceBtn_actionPerformed(ActionEvent e)
   {
      Object elm = mDeviceList.getSelectedValue();
      if (null != elm && elm instanceof ConfigElement)
      {
         getBroker().remove(mConfigContext, (ConfigElement)elm);
      }
   }

   private void mEditDeviceBtn_actionPerformed(ActionEvent e)
   {
      Object obj = mDeviceList.getSelectedValue();
      if (null != obj && obj instanceof ConfigElement)
      {
         ConfigElement elm = (ConfigElement)obj;

         ConfigDefinition def = elm.getDefinition();
         String token = def.getToken();

         List list = CustomEditorRegistry.findEditors(token);
        
         Color start_color = new Color(160, 160, 180);

         Object color = UIManager.get( "window" );
         if(null != color && color instanceof Color)
         {
            start_color = (Color)color;
         }
         else
         {
            System.out.println("Could not get the desktop color from the  UIManager.");
         }

         if(null == list || list.size() == 0)
         {
            System.out.println("No CustomEditors registered for token: " + token);
            
            JScrollPane scroll_pane = new JScrollPane();
            PropertySheet element_prop_sheet =
               PropertySheetFactory.instance().makeSheet(mConfigContext, elm, start_color);
            
            scroll_pane.getViewport().removeAll();
            scroll_pane.getViewport().add(element_prop_sheet, null);
            
            Frame parent = 
               (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                         CurrentDevicesPanel.this);
            SimpleEditorDialog dlg = new SimpleEditorDialog(parent,
                                                                  scroll_pane,
                                                                  elm.getDefinition().getName() + " Editor");
            int status = dlg.showDialog();

            if ( status == SimpleEditorDialog.CANCEL_OPTION )
            {
               //mToolbar.doUndoUntil(save_point);
            }
         }
         else
         {
            CustomEditor editor = (CustomEditor)list.get(0);
            editor.setConfig(mConfigContext, elm);
            
            Frame parent = 
               (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                         CurrentDevicesPanel.this);
            CustomEditorDialog dlg = new CustomEditorDialog(parent,
                                                            editor);
            int status = dlg.showDialog();

            if ( status == CustomEditorDialog.CANCEL_OPTION )
            {
               //mToolbar.doUndoUntil(save_point);
            }
         }
      }
   }

   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      
      if (null == mWhiteBoard)
      {
         System.out.println("WhiteBoard null.");
      }
      mConfigContext = (ConfigContext)mWhiteBoard.get("context");
      mDeviceListModel = new DeviceListModel(mConfigContext);
      mDeviceList.setModel(mDeviceListModel);
   }

   /** Reference to the ConfigBroker used in this object. */
   private ConfigBroker mBroker = null;

   /**
    * Gets a handle to the configuration broker.
    */
   private ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }
   
   public class DeviceListModel
      extends AbstractListModel
      //implements CaveModelListener
   {
      private ConfigContext mConfigContext = null;
      private List mDeviceList = new ArrayList();
      private ConfigBroker mBroker = new ConfigBrokerProxy();

      public DeviceListModel(ConfigContext context)
      {
         mConfigContext = context;

         List elms = mBroker.getElements(mConfigContext);
         mDeviceList.clear();

         for (Iterator itr = elms.iterator() ; itr.hasNext() ; )
         {
            ConfigElement elm = (ConfigElement)itr.next();
            if (elm.getDefinition().isOfType(POSITIONAL_DEVICE_TYPE) ||
                elm.getDefinition().isOfType(DIGITAL_DEVICE_TYPE) ||
                elm.getDefinition().isOfType(ANALOG_DEVICE_TYPE))
            {
               mDeviceList.add(elm);
            }
         }
      }
      
      public Object getElementAt(int index)
      {
         if (0 == mDeviceList.size())
         {
            return "[No input devices, click the add button to configure a new device.]";
         }
         return mDeviceList.get(index);
      }

      public int getSize()
      {
         if (0 == mDeviceList.size())
         {
            return 1;
         }
         return mDeviceList.size();
      }
   }

   /**
    * Custom ListCellRenderer for proxies.
    */
   public class DefinitionListRenderer extends DefaultListCellRenderer
   {
      /** Icon used for position nodes. */
      private Icon mPositionalIcon = null;

      /** Icon used for position nodes. */
      private Icon mDigitalIcon = null;

      /** Icon used for position nodes. */
      private Icon mAnalogIcon = null;

      /**
       * Constructor.
       */
      public DefinitionListRenderer()
      {
         ClassLoader loader = getClass().getClassLoader();

         // Load the icons.
         mPositionalIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/vrsystem/images/position16.png"));
         mDigitalIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/vrsystem/images/digital16.png"));
         mAnalogIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/vrsystem/images/analog16.png"));
      }

      /**
       * Creates new TreeNodeRenderer for given value.
       *
       * @param list     JList which is asking for renderer.
       * @param value    Value of node being renderer.
       * @param selected Is the value selected.
       * @param hasFocus Does the value have focus.
       */
      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index, boolean selected,
                                                    boolean hasFocus)
      {
         //DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;

         // NOTE: DefaultTreeCellRenderer extends JLabel and returns 
         // itself, if we call the following method to set everything up
         // we can then set the Icon to whatever we want and return this.
         // Get the default renderer for the passed value because we know that the UserObject is
         // either a ProxyType or a DeviceUnit which both implement the toString method.
         super.getListCellRendererComponent(list, value, index, selected, hasFocus);

         if (value instanceof String)
         {
            setText((String)value);
            if (!getFont().isItalic())
            {
               setFont(getFont().deriveFont(Font.BOLD + Font.ITALIC));
            }
            return this;
         }
         
         ConfigDefinition config_def = ((ConfigElement)value).getDefinition();

         // Set the correct Icon for this node in the tree.
         if (config_def.isOfType(POSITIONAL_DEVICE_TYPE))
         {
            setIcon(mPositionalIcon);
         }
         else if (config_def.isOfType(ANALOG_DEVICE_TYPE))
         {
            setIcon(mAnalogIcon);
         }
         else if (config_def.isOfType(DIGITAL_DEVICE_TYPE))
         {
            setIcon(mDigitalIcon);
         }

         setText(config_def.getName());

         return this;
      }
   }
}
