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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigElementPointer;
import org.vrjuggler.jccl.config.event.ConfigElementAdapter;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;


public class InputWindowFrame
   extends JugglerWindowFrame
   implements EditorConstants
{
   public InputWindowFrame(String title, Dimension resolution,
                           Dimension desktopSize, ConfigContext ctx,
                           ConfigElement elt, boolean allowKbdEdit)
   {
      super(title, resolution, desktopSize, ctx, elt);

      mElementListener = new InputWindowFrame_this_configElementAdapter(this);
      mElement.addConfigElementListener(mElementListener);
      mAllowKbdEdit = allowKbdEdit;

      try
      {
         jbInit();

         if ( mAllowKbdEdit )
         {
            ConfigElementPointer kbd_ptr =
               (ConfigElementPointer) elt.getProperty(KEYBOARD_MOUSE_PTR_PROPERTY,
                                                      0);

            if ( kbd_ptr != null && kbd_ptr.getTarget() != null &&
                 ! kbd_ptr.getTarget().equals("") )
            {
               this.getContentPane().add(mEditButton, BorderLayout.NORTH);
            }
         }

         placeMyself();

         updateContextMenuItems();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         ClassLoader l = getClass().getClassLoader();
         ImageIcon img =
            new ImageIcon(l.getResource(IMAGE_BASE +
                                        "/vrjuggler-input-window-icon.png"));
         mIcon.setIcon(img);
//         mIcon.setImageFile(IMAGE_BASE +
//                            "/vrjuggler-input-window-icon.png");
      }
      catch(Exception ex)
      {
         /* Oh well. */ ;
      }
   }

   /**
    * Unregisters this object as a ConfigElementListener from the contained
    * mElement reference.  This is needed to prevent a circular reference
    * from existing forever between this object and its ConfigElement.
    *
    * Is this the best place (or way) to do this?
    */
/*
   public void removeNotify()
   {
      super.removeNotify();
      mElement.removeConfigElementListener(mElementListener);
   }
*/

   private void jbInit() throws Exception
   {
      this.getContentPane().setLayout(mMainLayout);
      this.addMouseListener(
         new InputWindowFrame_this_mouseInputAdapter(this)
      );
      mWinPropsItem.setText("Input Window Properties ...");
      mWinPropsItem.addActionListener(new InputWindowFrame_mWinPropsItem_actionAdapter(this));
//      this.addInternalFrameListener(new InputWindowFrame_this_internalFrameAdapter(this));
      mContextMenu.add(mWinPropsItem);
      mIcon.setHorizontalAlignment(SwingConstants.CENTER);
      mIcon.setVerticalAlignment(SwingConstants.CENTER);
      this.getContentPane().add(mIcon, BorderLayout.CENTER);
   }

   private void updateContextMenuItems()
   {
   }

   private boolean mAllowKbdEdit = false;
   private InputWindowFrame_this_configElementAdapter mElementListener = null;

   private BorderLayout mMainLayout = new BorderLayout();
   private JLabel mIcon = new JLabel();
   private JPopupMenu mContextMenu = new JPopupMenu();
   private JMenuItem mWinPropsItem = new JMenuItem();

   void this_mouseClicked(MouseEvent e)
   {
      checkForPopUp(e);
   }

   void this_mousePressed(MouseEvent e)
   {
      checkForPopUp(e);
   }

   void this_mouseReleased(MouseEvent e)
   {
      checkForPopUp(e);
   }

   private boolean checkForPopUp(MouseEvent e)
   {
      if ( e.isPopupTrigger() )
      {
         mContextMenu.show(e.getComponent(), e.getX(), e.getY());
         return true;
      }
      else
      {
         return false;
      }
   }

   void windowPropsEditSelected(ActionEvent e)
   {
      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                       getDesktopPane());
      InputWindowStartDialog dlg =
         new InputWindowStartDialog(parent, mContext, mElement, mDesktopSize);

      if ( dlg.showDialog() == DisplayWindowStartDialog.OK_OPTION )
      {
         Rectangle bounds = dlg.getInputWindowBounds();
         mElement.setProperty(ORIGIN_PROPERTY, 0, new Integer(bounds.x),
                              mContext);
         mElement.setProperty(ORIGIN_PROPERTY, 1, new Integer(bounds.y),
                              mContext);
         mElement.setProperty(SIZE_PROPERTY, 0, new Integer(bounds.width),
                              mContext);
         mElement.setProperty(SIZE_PROPERTY, 1, new Integer(bounds.height),
                              mContext);

         mElement.setProperty(LOCK_KEY_PROPERTY, 0, dlg.getLockKey(),
                              mContext);
         mElement.setProperty(START_LOCKED_PROPERTY, 0,
                              dlg.shouldStartLocked(), mContext);
         mElement.setProperty(SLEEP_TIME_PROPERTY, 0, dlg.getSleepTime(),
                              mContext);
      }
   }

/*
   void frameActivated(InternalFrameEvent e)
   {
      // XXX: Why do we have to do this?
      e.getInternalFrame().getGlassPane().setVisible(true);
   }
*/

   void windowNameChanged(ConfigElementEvent e)
   {
      this.setTitle(mElement.getName());
   }

   void windowPropertyChanged(ConfigElementEvent e)
   {
      // This constant name is just too long.
      String prop_name = KEYBOARD_MOUSE_PTR_PROPERTY;

      if ( mAllowKbdEdit && e.getProperty().equals(prop_name) )
      {
         ConfigElementPointer val =
            (ConfigElementPointer) mElement.getProperty(prop_name, 0);

         // If the config element pointer has is non-null and has a target,
         // check its current value and determine what to do with mEditButton.
         if ( val != null && val.getTarget() != null )
         {
            boolean in_layout = this.getContentPane().isAncestorOf(mEditButton);

            // If mEditButton is in the content pane and the new value of the
            // pointer is empty, then we need to remove mEditButton from the
            // content pane.
            if ( in_layout && val.getTarget().equals("") )
            {
               this.getContentPane().remove(mEditButton);
            }
            // if mEditButton is not in the content pane and the new value of
            // the pointer is non-empty, then we need to add mEditButton to
            // the content pane.
            else if ( ! in_layout && ! val.getTarget().equals("") )
            {
               this.getContentPane().add(mEditButton, BorderLayout.NORTH);
            }
         }
         // If the config element pointer has no value and mEditButton is in
         // the content pane, then mEditButton must be removed from the content
         // pane.
         else
         {
            if ( this.getContentPane().isAncestorOf(mEditButton) )
            {
               this.getContentPane().remove(mEditButton);
            }
         }

         this.revalidate();
         this.repaint();
      }
      else
      {
         updateContextMenuItems();
      }
   }
}

class InputWindowFrame_this_mouseInputAdapter
   extends MouseInputAdapter
{
   private InputWindowFrame adaptee;

   InputWindowFrame_this_mouseInputAdapter(InputWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }

   public void mouseClicked(MouseEvent e)
   {
      adaptee.this_mouseClicked(e);
   }

   public void mousePressed(MouseEvent e)
   {
      adaptee.this_mousePressed(e);
   }

   public void mouseReleased(MouseEvent e)
   {
      adaptee.this_mouseReleased(e);
   }
}

/*
class InputWindowFrame_this_internalFrameAdapter
   extends InternalFrameAdapter
{
   private InputWindowFrame adaptee;

   InputWindowFrame_this_internalFrameAdapter(InputWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void internalFrameActivated(InternalFrameEvent e)
   {
      adaptee.frameActivated(e);
   }
}
*/
class InputWindowFrame_mWinPropsItem_actionAdapter
   implements ActionListener
{
   private InputWindowFrame adaptee;

   InputWindowFrame_mWinPropsItem_actionAdapter(InputWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.windowPropsEditSelected(e);
   }
}

class InputWindowFrame_this_configElementAdapter
   extends ConfigElementAdapter
{
   private InputWindowFrame adaptee;

   InputWindowFrame_this_configElementAdapter(InputWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void nameChanged(ConfigElementEvent e)
   {
      adaptee.windowNameChanged(e);
   }
   public void propertyValueChanged(ConfigElementEvent e)
   {
      adaptee.windowPropertyChanged(e);
   }
}
