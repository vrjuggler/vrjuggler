/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
import java.awt.Color;
import java.awt.Font;
import java.awt.Window;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.vrjconfig.commoneditors.EditorHelpers;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;
import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


public class SimPositionDeviceEditor
   extends JSplitPane
   implements SimDeviceEditor
            , ConfigElementListener
            , EditorConstants
{
   private static final int COLUMN0 = 1;
   private static final int COLUMN1 = 3;
   private static final int COLUMN2 = 5;

   private static final int LABEL_TOP_ROW    = 1;
   private static final int FIELD_TOP_ROW    = 2;
   private static final int LABEL_MIDDLE_ROW = 4;
   private static final int FIELD_MIDDLE_ROW = 5;
   private static final int LABEL_BOTTOM_ROW = 7;
   private static final int FIELD_BOTTOM_ROW = 8;

   public SimPositionDeviceEditor()
   {
      for ( int i = 0; i < mKeyPairs.length; ++i )
      {
         mKeyPairs[i] = new KeyPair();
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
      if ( ! elt.getDefinition().getToken().equals(SIM_POS_DEVICE_TYPE) )
      {
         throw new IllegalArgumentException("Invalid config element type '" +
                                            elt.getDefinition().getToken() +
                                            "'!  Expected " +
                                            SIM_POS_DEVICE_TYPE);
      }

      mContext = ctx;
      mElement = elt;

      if ( mProxyGraph != null )
      {
         this.remove(mProxyGraph);
         mProxyGraph.editorClosing();
         mProxyGraph = null;
      }

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();

      List allowed_types = new ArrayList(2);
      allowed_types.add(0, repos.get(POSITION_PROXY_TYPE));
      allowed_types.add(1, repos.get(SIM_POS_DEVICE_TYPE));

      mProxyGraph = new ProxyEditorUI(allowed_types);
      mProxyGraph.setConfig(ctx, elt);
      this.add(mProxyGraph, JSplitPane.LEFT);

      for ( int i = 0; i < mKeyPairs.length; ++i )
      {
         ConfigElement key_pair_elt =
            (ConfigElement) mElement.getProperty(KEY_PAIR_PROPERTY, i);
         key_pair_elt.addConfigElementListener(this);
         mKeyPairs[i].button.setEnabled(true);
         String press_text = EditorHelpers.getKeyPressText(key_pair_elt);
         mKeyPairs[i].button.setText(press_text + " ...");
         mKeyPairs[i].button.setToolTipText(press_text);
      }
   }

   public JComponent getEditor()
   {
      return this;
   }

   public void editorClosing()
   {
      if ( mElement != null )
      {
         for ( int i = 0; i < mKeyPairs.length; ++i )
         {
            ConfigElement key_pair_elt =
               (ConfigElement) mElement.getProperty(KEY_PAIR_PROPERTY, i);
            key_pair_elt.removeConfigElementListener(this);
         }

         mElement = null;
      }
   }

   public void nameChanged(ConfigElementEvent e)
   {
      /* Do nothing. */ ;
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
      /* Do nothing. */ ;
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      ConfigElement src_elt = (ConfigElement) e.getSource();

      if ( src_elt.getDefinition().getToken().equals(KEY_MODIFIER_PAIR_TYPE) )
      {
         int index = mElement.getPropertyValueIndex(KEY_PAIR_PROPERTY, src_elt);
         String press_text =
            EditorHelpers.getKeyPressText((ConfigElement) e.getSource());
         mKeyPairs[index].button.setText(press_text + " ...");
         mKeyPairs[index].button.setToolTipText(press_text);

         Integer key = (Integer) src_elt.getProperty(KEY_PROPERTY, 0);
         Integer mod = (Integer) src_elt.getProperty(MODIFIER_KEY_PROPERTY, 0);

         Color button_bg = UIManager.getColor("Button.background");

         int unit_count = mElement.getPropertyValueCount(KEY_PAIR_PROPERTY);
         boolean found_error = false;
         for ( int i = 0; i < unit_count; ++i )
         {
            if ( i != index )
            {
               ConfigElement cur_unit =
                  (ConfigElement) mElement.getProperty(KEY_PAIR_PROPERTY, i);
               Integer cur_key =
                  (Integer) cur_unit.getProperty(KEY_PROPERTY, 0);
               Integer cur_mod =
                  (Integer) cur_unit.getProperty(MODIFIER_KEY_PROPERTY, 0);

               if ( key.equals(cur_key) && mod.equals(cur_mod) )
               {
                  mKeyPairs[i].button.setBackground(Color.red);
                  found_error = true;
               }
               else
               {
                  mKeyPairs[i].button.setBackground(button_bg);
               }
            }
         }

         if ( found_error )
         {
            mKeyPairs[index].button.setBackground(Color.red);
            mErrorMsgLabel.setText("Configuration contains an error: " +
                                   "duplicate key bindings for units");
            mBindingContainer.add(mErrorMsgLabel, BorderLayout.NORTH);
         }
         else
         {
            if ( mBindingContainer.isAncestorOf(mErrorMsgLabel) )
            {
               mBindingContainer.remove(mErrorMsgLabel);
            }

            mKeyPairs[index].button.setBackground(button_bg);
         }

         mBindingContainer.revalidate();
         mBindingContainer.repaint();
      }
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
      /* Do nothing. */ ;
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
      /* Do nothing. */ ;
   }

   private void jbInit() throws Exception
   {
      int btn_font_size = 10;

      double[][] main_sizes =
         {
            {0.5, 0.5},
            {TableLayout.FILL}
         };

      mKeyBindingPanel.setLayout(new TableLayout(main_sizes));

      double p = TableLayout.PREFERRED;
      double[][] editor_sizes =
         {
            {5, 0.33, 5, 0.33, 5, 0.33, 5},
            {5, p, p, 5, p, p, 5, p, p, 5}
         };

      mTranslateBindingPanel.setLayout(new TableLayout(editor_sizes));
      mTranslateBindingPanel.setBorder(
         new TitledBorder(BorderFactory.createEtchedBorder(
                             Color.white, new Color(142, 142, 142)
                          ),
                          "Translation Bindings", TitledBorder.CENTER,
                          TitledBorder.DEFAULT_POSITION)
      );

      mRotateBindingPanel.setLayout(new TableLayout(editor_sizes));
      mRotateBindingPanel.setBorder(
         new TitledBorder(BorderFactory.createEtchedBorder(
                             Color.white, new Color(142, 142, 142)
                          ),
                          "Rotation Bindings", TitledBorder.CENTER,
                          TitledBorder.DEFAULT_POSITION)
      );

      mKeyBindingPanel.add(
         mTranslateBindingPanel,
         new TableLayoutConstraints(0, 0, 0, 0,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.FULL)
      );
      mKeyBindingPanel.add(
         mRotateBindingPanel,
         new TableLayoutConstraints(1, 0, 1, 0,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.FULL)
      );

      mKeyPairs[FORWARD_VALUE_INDEX].label.setText("Forward");
      mKeyPairs[FORWARD_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[FORWARD_VALUE_INDEX].button
      );
      mKeyPairs[FORWARD_VALUE_INDEX].button.setFont(new Font("Dialog",
                                                             Font.PLAIN,
                                                             btn_font_size));
      mKeyPairs[FORWARD_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[FORWARD_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(FORWARD_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[BACK_VALUE_INDEX].label.setText("Back");
      mKeyPairs[BACK_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[BACK_VALUE_INDEX].button
      );
      mKeyPairs[BACK_VALUE_INDEX].button.setFont(new Font("Dialog", Font.PLAIN,
                                                          btn_font_size));
      mKeyPairs[BACK_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[BACK_VALUE_INDEX].button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(BACK_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[LEFT_VALUE_INDEX].label.setText("Left");
      mKeyPairs[LEFT_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[LEFT_VALUE_INDEX].button
      );
      mKeyPairs[LEFT_VALUE_INDEX].button.setFont(new Font("Dialog", Font.PLAIN,
                                                          btn_font_size));
      mKeyPairs[LEFT_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[LEFT_VALUE_INDEX].button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(LEFT_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[RIGHT_VALUE_INDEX].label.setText("Right");
      mKeyPairs[RIGHT_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[RIGHT_VALUE_INDEX].button
      );
      mKeyPairs[RIGHT_VALUE_INDEX].button.setFont(new Font("Dialog",
                                                           Font.PLAIN,
                                                           btn_font_size));
      mKeyPairs[RIGHT_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[RIGHT_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(RIGHT_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[UP_VALUE_INDEX].label.setText("Up");
      mKeyPairs[UP_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[UP_VALUE_INDEX].button
      );
      mKeyPairs[UP_VALUE_INDEX].button.setFont(new Font("Dialog", Font.PLAIN,
                                                        btn_font_size));
      mKeyPairs[UP_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[UP_VALUE_INDEX].button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(UP_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[DOWN_VALUE_INDEX].label.setText("Down");
      mKeyPairs[DOWN_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[DOWN_VALUE_INDEX].button
      );
      mKeyPairs[DOWN_VALUE_INDEX].button.setFont(new Font("Dialog", Font.PLAIN,
                                                          btn_font_size));
      mKeyPairs[DOWN_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[DOWN_VALUE_INDEX].button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(DOWN_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].label.setText("Right");
      mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].button.setFont(
         new Font("Dialog", Font.PLAIN, btn_font_size)
      );
      mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_RIGHT_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_LEFT_VALUE_INDEX].label.setText("Left");
      mKeyPairs[ROTATE_LEFT_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_LEFT_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_LEFT_VALUE_INDEX].button.setFont(
         new Font("Dialog", Font.PLAIN, btn_font_size)
      );
      mKeyPairs[ROTATE_LEFT_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_LEFT_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_LEFT_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_UP_VALUE_INDEX].label.setText("Up");
      mKeyPairs[ROTATE_UP_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_UP_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_UP_VALUE_INDEX].button.setFont(new Font("Dialog",
                                                               Font.PLAIN,
                                                               btn_font_size));
      mKeyPairs[ROTATE_UP_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_UP_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_UP_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_DOWN_VALUE_INDEX].label.setText("Down");
      mKeyPairs[ROTATE_DOWN_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_DOWN_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_DOWN_VALUE_INDEX].button.setFont(new Font("Dialog",
                                                               Font.PLAIN,
                                                               btn_font_size));
      mKeyPairs[ROTATE_DOWN_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_DOWN_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_DOWN_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].label.setText(
         "Counter-Clockwise"
      );
      mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].button.setFont(
         new Font("Dialog", Font.PLAIN, btn_font_size)
      );
      mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX);
            }
         }
      );
      mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].label.setText("Clockwise");
      mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].label.setLabelFor(
         mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].button
      );
      mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].button.setFont(
         new Font("Dialog", Font.PLAIN, btn_font_size)
      );
      mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].button.setEnabled(false);
      mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].button.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               openEditor(ROTATE_CLOCKWISE_VALUE_INDEX);
            }
         }
      );

      mTranslateBindingPanel.add(
         mKeyPairs[UP_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN0, LABEL_TOP_ROW,
                                    COLUMN0, LABEL_TOP_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[UP_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN0, FIELD_TOP_ROW,
                                    COLUMN0, FIELD_TOP_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[LEFT_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN0, LABEL_MIDDLE_ROW,
                                    COLUMN0, LABEL_MIDDLE_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[LEFT_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN0, FIELD_MIDDLE_ROW,
                                    COLUMN0, FIELD_MIDDLE_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[DOWN_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN0, LABEL_BOTTOM_ROW,
                                    COLUMN0, LABEL_BOTTOM_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[DOWN_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN0, FIELD_BOTTOM_ROW,
                                    COLUMN0, FIELD_BOTTOM_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[FORWARD_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN1, LABEL_TOP_ROW,
                                    COLUMN1, LABEL_TOP_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[FORWARD_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN1, FIELD_TOP_ROW,
                                    COLUMN1, FIELD_TOP_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[BACK_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN1, LABEL_BOTTOM_ROW,
                                    COLUMN1, LABEL_BOTTOM_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[BACK_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN1, FIELD_BOTTOM_ROW,
                                    COLUMN1, FIELD_BOTTOM_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[RIGHT_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN2, LABEL_MIDDLE_ROW,
                                    COLUMN2, LABEL_MIDDLE_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mTranslateBindingPanel.add(
         mKeyPairs[RIGHT_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN2, FIELD_MIDDLE_ROW,
                                    COLUMN2, FIELD_MIDDLE_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );

      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN0, LABEL_TOP_ROW,
                                    COLUMN0, LABEL_TOP_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN0, FIELD_TOP_ROW,
                                    COLUMN0, FIELD_TOP_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_LEFT_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN0, LABEL_MIDDLE_ROW,
                                    COLUMN0, LABEL_MIDDLE_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_LEFT_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN0, FIELD_MIDDLE_ROW,
                                    COLUMN0, FIELD_MIDDLE_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_UP_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN1, LABEL_TOP_ROW,
                                    COLUMN1, LABEL_TOP_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_UP_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN1, FIELD_TOP_ROW,
                                    COLUMN1, FIELD_TOP_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_DOWN_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN1, LABEL_BOTTOM_ROW,
                                    COLUMN1, LABEL_BOTTOM_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_DOWN_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN1, FIELD_BOTTOM_ROW,
                                    COLUMN1, FIELD_BOTTOM_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN2, LABEL_TOP_ROW,
                                    COLUMN2, LABEL_TOP_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_CLOCKWISE_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN2, FIELD_TOP_ROW,
                                    COLUMN2, FIELD_TOP_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].label,
         new TableLayoutConstraints(COLUMN2, LABEL_MIDDLE_ROW,
                                    COLUMN2, LABEL_MIDDLE_ROW,
                                    TableLayoutConstraints.CENTER,
                                    TableLayoutConstraints.CENTER)
      );
      mRotateBindingPanel.add(
         mKeyPairs[ROTATE_RIGHT_VALUE_INDEX].button,
         new TableLayoutConstraints(COLUMN2, FIELD_MIDDLE_ROW,
                                    COLUMN2, FIELD_MIDDLE_ROW,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );

      mErrorMsgLabel.setOpaque(true);
      mErrorMsgLabel.setBackground(Color.red);
      mErrorMsgLabel.setForeground(Color.white);
      mErrorMsgLabel.setFont(mErrorMsgLabel.getFont().deriveFont(Font.BOLD));

      mBindingContainer.setLayout(new BorderLayout());
      mBindingContainer.add(mKeyBindingScrollPane, BorderLayout.CENTER);

      this.setOneTouchExpandable(false);
      this.setOrientation(VERTICAL_SPLIT);
      this.setResizeWeight(0.5);
      this.add(mBindingContainer, JSplitPane.RIGHT);
   }

   private void openEditor(int index)
   {
      ConfigElement elt =
         (ConfigElement) mElement.getProperty(KEY_PAIR_PROPERTY, index);
      Window parent = (Window) SwingUtilities.getAncestorOfClass(Window.class,
                                                                 this);
      KeyPressEditorDialog dlg = new KeyPressEditorDialog(parent, mContext,
                                                          elt);
      dlg.setVisible(true);
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private KeyboardEditorPanel mKeyboardEditor = null;

   private ProxyEditorUI mProxyGraph = new ProxyEditorUI();
   private JPanel mBindingContainer = new JPanel();
   private JPanel mKeyBindingPanel = new JPanel();
   private JScrollPane mKeyBindingScrollPane =
      new JScrollPane(mKeyBindingPanel);
   private JLabel mErrorMsgLabel = new JLabel();
   private JPanel mTranslateBindingPanel = new JPanel();
   private JPanel mRotateBindingPanel = new JPanel();
   private KeyPair[] mKeyPairs = new KeyPair[12];

   private static class KeyPair
   {
      public JLabel  label  = new JLabel();
      public JButton button = new JButton();
   }
}
