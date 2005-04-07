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

import java.awt.Color;
import java.awt.Component;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;
import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


public class SimDigitalDeviceEditor
   extends JSplitPane
   implements SimDeviceEditor
            , ConfigElementListener
            , EditorConstants
{
   public SimDigitalDeviceEditor()
   {
      try
      {
         jbInit();
         mUnitList.setCellRenderer(new UnitCellRenderer());
         mUnitList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
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
      if ( ! elt.getDefinition().getToken().equals(SIM_DIGITAL_DEVICE_TYPE) )
      {
         throw new IllegalArgumentException("Invalid config element type '" +
                                            elt.getDefinition().getToken() +
                                            "'!  Expected " +
                                            SIM_DIGITAL_DEVICE_TYPE);
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
      allowed_types.add(0, repos.get(DIGITAL_PROXY_TYPE));
      allowed_types.add(1, repos.get(SIM_DIGITAL_DEVICE_TYPE));

      mProxyGraph = new ProxyEditorUI(allowed_types);
      mProxyGraph.setConfig(ctx, elt);
      this.add(mProxyGraph, JSplitPane.LEFT);

      int units = mElement.getPropertyValueCount(KEY_PAIR_PROPERTY);

      for ( int i = 0; i < units; ++i )
      {
         mUnitListModel.addElement(mElement.getProperty(KEY_PAIR_PROPERTY, i));
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
      ConfigElement src_elt = (ConfigElement) e.getSource();
      mUnitListModel.add(e.getIndex(),
                         src_elt.getProperty(KEY_PAIR_PROPERTY, e.getIndex()));
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
      ConfigElement src_elt = (ConfigElement) e.getSource();
      int from_index = Math.min(e.getIndex0(), e.getIndex1());
      int to_index   = Math.max(e.getIndex0(), e.getIndex1());
      mUnitListModel.removeRange(from_index, to_index);

      for ( int i = from_index; i < to_index; ++i )
      {
         mUnitListModel.add(i, src_elt.getProperty(KEY_PAIR_PROPERTY, i));
      }
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
      ConfigElement src_elt = (ConfigElement) e.getSource();

      if ( mUnitList.getSelectedIndex() == e.getIndex() )
      {
         mUnitList.setSelectedIndex(-1);
      }

      mUnitListModel.removeElementAt(e.getIndex());
   }

   private void jbInit() throws Exception
   {
      this.setOneTouchExpandable(false);
      this.setOrientation(VERTICAL_SPLIT);
      this.setResizeWeight(0.5);

      mUnitList.addListSelectionListener(new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               selectedUnitChanged();
            }
         }
      );

      double[][] sizes =
         {
            {0.5, 0.5},
            {TableLayout.FILL}
         };
      mKeyBindingPanel.setLayout(new TableLayout(sizes));
      mKeyBindingPanel.setBorder(
         new TitledBorder(BorderFactory.createEtchedBorder(
                             Color.white, new Color(142, 142, 142)
                          ),
                          "Digital Unit Key Bindings", TitledBorder.CENTER,
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

   private void selectedUnitChanged()
   {
      Object selected_obj = mUnitList.getSelectedValue();

      if ( selected_obj == null )
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
         mCurEditor.setConfig(mContext, (ConfigElement) selected_obj);
      }
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private KeyboardEditorPanel mKeyboardEditor = null;
   private KeyPressEditor mCurEditor = null;

   private ProxyEditorUI mProxyGraph = null;
   private JPanel mKeyBindingPanel = new JPanel();
   private DefaultListModel mUnitListModel = new DefaultListModel();
   private JList mUnitList = new JList(mUnitListModel);
   private JScrollPane mUnitScrollPane = new JScrollPane(mUnitList);

   private static class UnitCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public UnitCellRenderer()
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

         if ( value != null )
         {
            ConfigElement unit_elt = (ConfigElement) value;
            setText("Unit " + index + " (" + unit_elt.getName() + ")");
         }
         else
         {
            setText("");
         }

         setFont(list.getFont());

         return this;
      }
   }
}
