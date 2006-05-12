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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

/**
 * This component acts very much like a button with an extra "popup" button
 * attached to its side. When the popup button is clicked, a popup menu is
 * displayed allowing the user to choose between many different options. The
 * main part of the button when clicked invokes the first item on the popup
 * menu as a sort of shortcut.
 */
public class PopupButton
   extends JComponent
{
   public PopupButton()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Init the icons
      ClassLoader loader = BeanJarClassLoader.instance();
      try
      {
         arrowBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/popdown.gif")));
      }
      catch (NullPointerException npe)
      {
         // ignore for now ...
      }

      // Default to an empty popup menu
      JPopupMenu menu = new JPopupMenu();
      menu.add(new JMenuItem(""));
      setPopupMenu(menu);
   }

   /**
    * Sets the popup menu associated with this button.
    *
    * @param menu      the menu to pop up when the popup button is depressed
    *
    * @throws IllegalArgumentException     if menu is null
    */
   public void setPopupMenu(JPopupMenu menu)
   {
      // assert that menu is not null
      if (menu == null)
      {
         throw new IllegalArgumentException("menu must not be null");
      }

      // Make the switch, ensuring that the listener status is updated
      if (popupMenu != null)
      {
         popupMenu.removePopupMenuListener(popupListener);
      }
      menu.setVisible(false);
      popupMenu = menu;
      popupMenu.addPopupMenuListener(popupListener);

      // Set the main icon to the first item in the menu
      JMenuItem first_item = (JMenuItem)popupMenu.getComponent(0);
      iconBtn.setIcon(first_item.getIcon());
   }

   public JPopupMenu getPopupMenu()
   {
      return popupMenu;
   }

   public Dimension getMinimumSize()
   {
      return baseBox.getMinimumSize();
   }

   public Dimension getMaximumSize()
   {
      return baseBox.getMaximumSize();
   }

   public Dimension getPreferredSize()
   {
      return baseBox.getPreferredSize();
   }

   /**
    * Internal helper that makes sure the popup menu is visible.
    */
   protected void showPopupMenu()
   {
      if (! popupMenu.isVisible())
      {
         popupMenu.getSelectionModel().setSelectedIndex(0);
         popupMenu.show(arrowBtn, 0, arrowBtn.getHeight());
      }
   }

   /**
    * JBuilder automatically generated UI initialization.
    */
   private void jbInit()
      throws Exception
   {
      baseBox = Box.createHorizontalBox();
      this.setLayout(baseLayout);
      this.setBorder(BorderFactory.createEtchedBorder());
//      this.setMaximumSize(new Dimension(40, 32));
//      this.setMinimumSize(new Dimension(40, 32));
//      this.setPreferredSize(new Dimension(40, 32));
//      iconBtn.setMaximumSize(new Dimension(24, 24));
//      iconBtn.setMinimumSize(new Dimension(24, 24));
//      iconBtn.setPreferredSize(new Dimension(24, 24));
//      arrowBtn.setMaximumSize(new Dimension(12, 24));
//      arrowBtn.setMinimumSize(new Dimension(12, 24));
//      arrowBtn.setPreferredSize(new Dimension(12, 24));
      iconBtn.setMargin(new Insets(0, 0, 0, 0));
      iconBtn.setBorderPainted(false);
      iconBtn.setFocusPainted(false);
      arrowBtn.setMargin(new Insets(0, 0, 0, 0));
      arrowBtn.setBorderPainted(false);
      arrowBtn.setFocusPainted(false);
      arrowBtn.addChangeListener(new ChangeListener()
      {
         public void stateChanged(ChangeEvent evt)
         {
            // Throw up the popup menu if this button has been pressed
            if (arrowBtn.getModel().isArmed() &&
                arrowBtn.getModel().isPressed() &&
                ! arrowBtn.getModel().isSelected())
            {
               showPopupMenu();
            }
         }
      });
      iconBtn.addChangeListener(new ChangeListener()
      {
         public void stateChanged(ChangeEvent evt)
         {
            arrowBtn.setSelected(iconBtn.getModel().isPressed() &&
                                 iconBtn.getModel().isArmed());
         }
      });
      iconBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            JMenuItem first_item = (JMenuItem)popupMenu.getComponent(0);
            first_item.doClick();
         }
      });
      this.add(baseBox, BorderLayout.CENTER);
      baseBox.add(iconBtn, null);
      baseBox.add(arrowBtn, null);
   }

   //--- JBuilder automatically generated UI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private Box baseBox;
   private JButton iconBtn = new JButton();
   private JToggleButton arrowBtn = new JToggleButton()
   {
      public Dimension getMaximumSize()
      {
         Dimension size = super.getMaximumSize();
         Dimension icon_size = iconBtn.getMaximumSize();
         if (icon_size.height > size.height)
         {
            size.height = icon_size.height;
         }
         return size;
      }

      public Dimension getPreferredSize()
      {
         Dimension size = super.getPreferredSize();
         Dimension icon_size = iconBtn.getPreferredSize();
         if (icon_size.height > size.height)
         {
            size.height = icon_size.height;
         }
         return size;
      }
   };

   /**
    * The popup menu that is shown when the popup button is pressed.
    */
   private JPopupMenu popupMenu;

   /**
    * Listens for change to the popup manu.
    */
   private PopupListener popupListener = new PopupListener();

   /**
    * Specialized listener for the popup menu.
    */
   class PopupListener
      implements PopupMenuListener
   {
      public void popupMenuCanceled(PopupMenuEvent evt)
      {
         arrowBtn.getModel().setSelected(false);
      }

      public void popupMenuWillBecomeInvisible(PopupMenuEvent evt)
      {
         arrowBtn.getModel().setSelected(false);
      }

      public void popupMenuWillBecomeVisible(PopupMenuEvent evt)
      {
      }
   }
}
