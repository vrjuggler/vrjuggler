/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Component;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorHelpers;


public class KeyPressEditor
   extends JPanel
   implements ConfigElementListener
            , EditorConstants
{
   private static final int KEY_TYPE    = 0;
   private static final int BUTTON_TYPE = 1;
   private static final int MOTION_TYPE = 2;

   private static final int MOUSE_LEFT    = 0;
   private static final int MOUSE_RIGHT   = 1;
   private static final int MOUSE_FORWARD = 2;
   private static final int MOUSE_BACK    = 3;

   private static final Integer NONE  = new Integer(MODKEY_NONE_INT_VALUE);
   private static final Integer ALT   = new Integer(MODKEY_ALT_INT_VALUE);
   private static final Integer CTRL  = new Integer(MODKEY_CTRL_INT_VALUE);
   private static final Integer SHIFT = new Integer(MODKEY_SHIFT_INT_VALUE);
   private static final Integer ANY   = new Integer(MODKEY_ANY_INT_VALUE);

   private static final Integer MOUSE_LEFT_KEY =
      new Integer(MOUSE_NEGX_INT_VALUE);
   private static final Integer MOUSE_RIGHT_KEY =
      new Integer(MOUSE_POSX_INT_VALUE);
   private static final Integer MOUSE_FORWARD_KEY =
      new Integer(MOUSE_NEGY_INT_VALUE);
   private static final Integer MOUSE_BACK_KEY =
      new Integer(MOUSE_POSY_INT_VALUE);

   public KeyPressEditor()
   {
      mKeyTypeChooser.addItem("Keyboard");
      mKeyTypeChooser.addItem("Mouse Button");
      mKeyTypeChooser.addItem("Mouse Motion");

      addModifierItems(mButtonModifierChooser);
      addModifierItems(mMotionModifierChooser);

      mMotionChooser.addItem(MOUSE_LEFT_KEY);
      mMotionChooser.addItem(MOUSE_RIGHT_KEY);
      mMotionChooser.addItem(MOUSE_FORWARD_KEY);
      mMotionChooser.addItem(MOUSE_BACK_KEY);

      try
      {
         initUI();
         this.setEnabled(false);
         mButtonModifierChooser.setRenderer(new ModifierCellRenderer());
         mMotionModifierChooser.setRenderer(new ModifierCellRenderer());
         mMotionChooser.setRenderer(new MouseMotionCellRenderer());
         mButtonChooser.setModel(new SpinnerNumberModel(1, FIRST_MOUSE_BUTTON,
                                                        LAST_MOUSE_BUTTON, 1));
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_base = IMAGE_BASE;

         mMotionIcons[MOUSE_LEFT] =
            new ImageIcon(loader.getResource(img_base + "/mouse-left.png"));
         mMotionIcons[MOUSE_RIGHT] =
            new ImageIcon(loader.getResource(img_base + "/mouse-right.png"));
         mMotionIcons[MOUSE_FORWARD] =
            new ImageIcon(loader.getResource(img_base + "/mouse-forward.png"));
         mMotionIcons[MOUSE_BACK] =
            new ImageIcon(loader.getResource(img_base + "/mouse-back.png"));
      }
      catch (Exception ex)
      {
         for ( int i = 0; i < mMotionIcons.length; ++i )
         {
            mMotionIcons[i] = null;
         }
      }
   }

   public void setEnabled(boolean enabled)
   {
      mKeyTypeChooser.setEnabled(enabled);
      mKeyField.setEnabled(enabled);
      mButtonModifierChooser.setEnabled(enabled);
      mButtonChooser.setEnabled(enabled);
      mMotionModifierChooser.setEnabled(enabled);
      mMotionChooser.setEnabled(enabled);
   }

   public void setConfig(ConfigContext ctx, ConfigElement element)
   {
      if ( element != null )
      {
         if ( ! element.getDefinition().getToken().equals(KEY_MODIFIER_PAIR_TYPE) )
         {
            throw new IllegalArgumentException(
               "element is not of type " + KEY_MODIFIER_PAIR_TYPE + ": " +
               element.getDefinition().getToken()
            );
         }
      }

      if ( mElement != null )
      {
         mElement.removeConfigElementListener(this);
      }

      mContext = ctx;
      mElement = element;

      if ( mElement != null )
      {
         mElement.addConfigElementListener(this);
         this.setEnabled(true);

         int key = ((Integer) mElement.getProperty(KEY_PROPERTY, 0)).intValue();

         if ( key == KEY_NONE_INT_VALUE )
         {
            mNoneKeyButton.setSelected(true);
            mKeyField.setEnabled(false);
            mKeyTypeChooser.setSelectedIndex(KEY_TYPE);
         }
         else if ( EditorHelpers.isJugglerMouseMotion(key) )
         {
            mKeyTypeChooser.setSelectedIndex(MOTION_TYPE);
         }
         else if ( EditorHelpers.isJugglerMouseButton(key) )
         {
            mKeyTypeChooser.setSelectedIndex(BUTTON_TYPE);
         }
         else
         {
            mKeyTypeChooser.setSelectedIndex(KEY_TYPE);
         }
      }
      else
      {
         this.setEnabled(false);

         if ( mCurEditor != null )
         {
            this.remove(mCurEditor);
            this.revalidate();
            this.repaint();

            mCurEditor = null;
         }
      }
   }

   public void nameChanged(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      Integer key_obj = (Integer) mElement.getProperty(KEY_PROPERTY, 0);
      int key = key_obj.intValue();

      int key_type = mKeyTypeChooser.getSelectedIndex();
      int new_key_type = -1;

      if ( EditorHelpers.isJugglerMouseMotion(key) )
      {
         new_key_type = MOTION_TYPE;
      }
      else if ( EditorHelpers.isJugglerMouseButton(key) )
      {
         new_key_type = BUTTON_TYPE;
      }
      else
      {
         new_key_type = KEY_TYPE;
      }

      if ( new_key_type != key_type )
      {
         mKeyTypeChooser.setSelectedIndex(key_type);
      }
      else
      {
         Integer mod_obj =
            (Integer) mElement.getProperty(MODIFIER_KEY_PROPERTY, 0);

         switch (key_type)
         {
            case KEY_TYPE:
               mKeyField.setKeyPressText(key, mod_obj.intValue());
               break;
            case MOTION_TYPE:
               setSelectedButton(key);
               mButtonModifierChooser.setSelectedItem(mod_obj);
               break;
            case BUTTON_TYPE:
               mMotionChooser.setSelectedItem(key_obj);
               mMotionModifierChooser.setSelectedItem(mod_obj);
               break;
         }
      }
   }

   public void setSelectedButton(int jugglerKey)
   {
      switch (jugglerKey)
      {
         case MOUSE_BUT1_INT_VALUE:
            mButtonChooser.setValue(new Integer(1));
            break;
         case MOUSE_BUT2_INT_VALUE:
            mButtonChooser.setValue(new Integer(2));
            break;
         case MOUSE_BUT3_INT_VALUE:
            mButtonChooser.setValue(new Integer(3));
            break;
      }
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
      /* Nothing to do. */ ;
   }

   public void editorClosing()
   {
      if ( mElement != null )
      {
         mElement.removeConfigElementListener(this);
         mElement = null;
      }
   }

   private void initUI()
      throws Exception
   {
      this.setLayout(new BorderLayout());

      double[][] sizes =
         {
            {TableLayout.MINIMUM, TableLayout.FILL},
            {TableLayout.PREFERRED, TableLayout.PREFERRED}
         };
      mKeyEditor.setLayout(new TableLayout(sizes));

      mButtonEditor.setLayout(new GridLayout(2, 2));
      mMotionEditor.setLayout(new GridLayout(2, 2));

      mNoneKeyButton.setText("None");
      mNoneKeyButton.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mKeyField.setEnabled(! mNoneKeyButton.isSelected());

               if ( mNoneKeyButton.isSelected() )
               {
                  updateElement(KEY_NONE_INT_VALUE, KEY_NONE_INT_VALUE);
               }
            }
         }
      );
      mKeyLabel.setLabelFor(mKeyField);
      mKeyLabel.setText("Enter key combination below:");
      mKeyLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mKeyEditor.add(mKeyLabel,
                     new TableLayoutConstraints(0, 0, 1, 0,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));
      mKeyEditor.add(mNoneKeyButton,
                     new TableLayoutConstraints(0, 1, 0, 1,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));
      mKeyEditor.add(mKeyField,
                     new TableLayoutConstraints(1, 1, 1, 1,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));

      mButtonModifierLabel.setLabelFor(mButtonModifierChooser);
      mButtonModifierLabel.setText("Button Modifier:");
      mButtonModifierLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mButtonChooserLabel.setLabelFor(mButtonChooser);
      mButtonChooserLabel.setText("Mouse Button:");
      mButtonChooserLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mButtonEditor.add(mButtonModifierLabel);
      mButtonEditor.add(mButtonModifierChooser);
      mButtonEditor.add(mButtonChooserLabel);
      mButtonEditor.add(mButtonChooser);

      mMotionModifierLabel.setLabelFor(mMotionModifierChooser);
      mMotionModifierLabel.setText("Motion Modifier:");
      mMotionModifierLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mMotionChooserLabel.setLabelFor(mMotionChooser);
      mMotionChooserLabel.setText("Mouse Motion:");
      mMotionChooserLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mMotionEditor.add(mMotionModifierLabel);
      mMotionEditor.add(mMotionModifierChooser);
      mMotionEditor.add(mMotionChooserLabel);
      mMotionEditor.add(mMotionChooser);

      mKeyTypeChooser.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               keyTypeChanged();
            }
         }
      );
      mButtonChooser.addChangeListener(new ChangeListener()
         {
            public void stateChanged(ChangeEvent e)
            {
               buttonValueChanged();
            }
         }
      );
      mMotionChooser.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               motionValueChanged();
            }
         }
      );
      mButtonModifierChooser.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               modifierValueChanged();
            }
         }
      );
      mMotionModifierChooser.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               modifierValueChanged();
            }
         }
      );

      this.add(mKeyTypeChooser, BorderLayout.NORTH);
   }

   private void addModifierItems(JComboBox chooser)
   {
      chooser.addItem(NONE);
      chooser.addItem(ALT);
      chooser.addItem(CTRL);
      chooser.addItem(SHIFT);
      chooser.addItem(ANY);
   }

   private void keyTypeChanged()
   {
      if ( mCurEditor != null )
      {
         this.remove(mCurEditor);
      }

      Integer key_obj = (Integer) mElement.getProperty(KEY_PROPERTY, 0);
      Integer modifier_obj =
         (Integer) mElement.getProperty(MODIFIER_KEY_PROPERTY, 0);
      int key = key_obj.intValue();

      switch (mKeyTypeChooser.getSelectedIndex())
      {
         case KEY_TYPE:
            mCurEditor = mKeyEditor;
            mKeyField.setKeyPressText(key, modifier_obj.intValue());
            break;
         case BUTTON_TYPE:
            mCurEditor = mButtonEditor;

            mButtonModifierChooser.setSelectedItem(modifier_obj);

            if ( EditorHelpers.isJugglerMouseButton(key) )
            {
               setSelectedButton(key);
            }
            else
            {
               mButtonChooser.setValue(new Integer(1));
               buttonValueChanged();
            }

            break;
         case MOTION_TYPE:
            mCurEditor = mMotionEditor;

            if ( EditorHelpers.isJugglerMouseMotion(key) )
            {
               mMotionChooser.setSelectedItem(key_obj);
            }
            else
            {
               mMotionChooser.setSelectedItem(null);
            }

            mMotionModifierChooser.setSelectedItem(modifier_obj);

            break;
      }

      this.add(mCurEditor, BorderLayout.SOUTH);
      this.revalidate();
      this.repaint();
   }

   public void buttonValueChanged()
   {
      int button = ((Number) mButtonChooser.getValue()).intValue();
      int mod = ((Integer) mButtonModifierChooser.getSelectedItem()).intValue();

      switch (button)
      {
         case 1:
            updateElement(MOUSE_BUT1_INT_VALUE, mod);
            break;
         case 2:
            updateElement(MOUSE_BUT2_INT_VALUE, mod);
            break;
         case 3:
            updateElement(MOUSE_BUT3_INT_VALUE, mod);
            break;
      }
   }

   private void motionValueChanged()
   {
      if ( mMotionChooser.getSelectedItem() != null )
      {
         Integer key = (Integer) mMotionChooser.getSelectedItem();
         Integer mod = (Integer) mMotionModifierChooser.getSelectedItem();
         updateElement(key.intValue(), mod.intValue());
      }
   }

   private void modifierValueChanged()
   {
      int key = ((Integer) mElement.getProperty(KEY_PROPERTY, 0)).intValue();
      int key_type = mKeyTypeChooser.getSelectedIndex();

      if ( key_type == BUTTON_TYPE &&
           mButtonModifierChooser.getSelectedItem() != null )
      {
         int mod =
            ((Integer) mButtonModifierChooser.getSelectedItem()).intValue();
         updateElement(key, mod);
      }
      else if ( key_type == MOTION_TYPE &&
                mMotionModifierChooser.getSelectedItem() != null )
      {
         int mod =
            ((Integer) mMotionModifierChooser.getSelectedItem()).intValue();
         updateElement(key, mod);
      }
   }

   /**
    * Updates our config element's key/modifier settings using the given
    * VR Juggler key codes.
    *
    * @param key        the VR Juggler key to be used for the new value of
    *                   the <code>EditorConstants.KEY_PROPERTY</code> value
    * @param mod        the VR Juggler key to be used for the new value of
    *                   the <code>EditorConstants.MODIFIER_KEY_PROPERTY</code>
    *                   value
    */
   private void updateElement(int key, int mod)
   {
      Integer cur_mod =
         (Integer) mElement.getProperty(MODIFIER_KEY_PROPERTY, 0);

      if ( cur_mod.intValue() != mod )
      {
         mElement.setProperty(MODIFIER_KEY_PROPERTY, 0, new Integer(mod),
                              mContext);
      }

      Integer cur_key = (Integer) mElement.getProperty(KEY_PROPERTY, 0);

      if ( cur_key.intValue() != key )
      {
         mElement.setProperty(KEY_PROPERTY, 0, new Integer(key), mContext);
      }
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private String[]    mMotionLabels = {"Left", "Right", "Forward", "Back"};
   private ImageIcon[] mMotionIcons  = new ImageIcon[mMotionLabels.length];

   private JComponent mCurEditor = null;

   private JComboBox mKeyTypeChooser = new JComboBox();
   private JPanel mKeyEditor = new JPanel();
   private JLabel mKeyLabel = new JLabel();
   private JCheckBox mNoneKeyButton = new JCheckBox();
   private KeyPressField mKeyField = new KeyPressField();
   private JPanel mMotionEditor = new JPanel();
   private JLabel mMotionModifierLabel = new JLabel();
   private JComboBox mMotionModifierChooser = new JComboBox();
   private JLabel mMotionChooserLabel = new JLabel();
   private JComboBox mMotionChooser = new JComboBox();
   private JPanel mButtonEditor = new JPanel();
   private JLabel mButtonModifierLabel = new JLabel();
   private JComboBox mButtonModifierChooser = new JComboBox();
   private JLabel mButtonChooserLabel = new JLabel();
   private JSpinner mButtonChooser = new JSpinner();

   private class KeyPressField
      extends JTextField
   {
      public KeyPressField()
      {
         this.setEditable(false);

         this.addKeyListener(new KeyAdapter()
            {
               public void keyPressed(KeyEvent e)
               {
                  if ( EditorHelpers.isAllowedJavaKey(e.getKeyCode()) )
                  {
                     int key =
                        EditorHelpers.convertJavaKeyToJuggler(e.getKeyCode());
                     int mod =
                        EditorHelpers.convertJavaModifiersToJuggler(
                           e.getModifiers()
                        );

                     // Action keys are those such as "Home," "Up," etc.  They
                     // will have modifier keys associated with them if any
                     // are pressed.
                     if ( e.isActionKey() )
                     {
                        setKeyPressText(key, mod);
                        updateElement(key, mod);
                        mNoKeyPressed = false;
                     }
                     else
                     {
                        int java_key = e.getKeyCode();

                        // If the key pressed is not a modifier key, then it
                        // is an allowed key (action keys are handled above),
                        // and we can update the config element.
                        if ( java_key != KeyEvent.VK_ALT &&
                             java_key != KeyEvent.VK_CONTROL &&
                             java_key != KeyEvent.VK_META &&
                             java_key != KeyEvent.VK_SHIFT )
                        {
                           setKeyPressText(key, mod);
                           updateElement(key, mod);
                           mNoKeyPressed = false;
                        }
                        // If the key pressed is a modifier key, then we just
                        // update the text we are displaying.
                        else
                        {
                           setText(EditorHelpers.getModifierText(mod));
                           mNoKeyPressed = true;
                        }
                     }
                  }
               }

               public void keyReleased(KeyEvent e)
               {
                  // The value of mNoKeyPressed indicates whether a
                  // non-modifier key was pressed before this event was
                  // generated.  If no non-modifier key was pressed, then
                  // this release event is associated with the user releasing
                  // a modifier key.
                  if ( mNoKeyPressed )
                  {
                     // If there are other modifiers left pressed, update the
                     // displayed text to reflect the current modifier set.
                     if ( e.getModifiers() != 0 )
                     {
                        int mod =
                           EditorHelpers.convertJavaModifiersToJuggler(
                              e.getModifiers()
                           );
                        setText(EditorHelpers.getModifierText(mod));
                     }
                     // If no buttons are pressed, then set the text back to
                     // whatever is in the config element.
                     else
                     {
                        Integer key =
                           (Integer) mElement.getProperty(KEY_PROPERTY, 0);
                        Integer mod =
                           (Integer) mElement.getProperty(MODIFIER_KEY_PROPERTY,
                                                          0);
                        setKeyPressText(key.intValue(), mod.intValue());
//                        setText("NONE");

                        mNoKeyPressed = false;
                     }
                  }
               }

               private boolean mNoKeyPressed = false;
            }
         );
      }

      /**
       * Updates the text displayed by this component to be the string
       * combination of the given VR Juggler key+modifier values.
       */
      public void setKeyPressText(int jugglerKey, int jugglerModifier)
      {
         setText(EditorHelpers.getKeyPressText(jugglerKey, jugglerModifier));
      }
   }

   private class MouseMotionCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public MouseMotionCellRenderer()
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
            int array_index;

            if ( value.equals(MOUSE_FORWARD_KEY) )
            {
               array_index = MOUSE_FORWARD;
            }
            else if ( value.equals(MOUSE_BACK_KEY) )
            {
               array_index = MOUSE_BACK;
            }
            else if ( value.equals(MOUSE_LEFT_KEY) )
            {
               array_index = MOUSE_LEFT;
            }
            else
            {
               array_index = MOUSE_RIGHT;
            }

            setIcon(mMotionIcons[array_index]);
            setText(mMotionLabels[array_index]);
         }
         else
         {
            setIcon(null);
            setText("");
         }

         setFont(list.getFont());

         return this;
      }
   }

   private static class ModifierCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public ModifierCellRenderer()
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

         if ( ALT.equals(value) )
         {
            setText("ALT");
         }
         else if ( CTRL.equals(value) )
         {
            setText("CTRL");
         }
         else if ( SHIFT.equals(value) )
         {
            setText("SHIFT");
         }
         else if ( NONE.equals(value) )
         {
            setText("NONE");
         }
         else if ( ANY.equals(value) )
         {
            setText("ANY");
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
