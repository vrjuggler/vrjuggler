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

package org.vrjuggler.tweek.gui;

import java.awt.event.*;
import javax.swing.*;
import org.vrjuggler.tweek.text.MessageDocument;


/**
 * A GUI panel that displays messages written to the encapsulated
 * MessagePanelDocument instance.
 */
public class MessagePanel extends JScrollPane
{
   public MessagePanel(MessageDocument document)
   {
      mMsgDocument = document;
      mTextPane.setDocument(document);
      jbInit();
   }

   // =========================================================================
   // Private methods.
   // =========================================================================

   private void jbInit()
   {
      mTextPane.setEditable(false);
      mTextPane.addMouseListener(new PopupListener());
      mPopupClearItem.setText("Clear Messages");
      mPopupClearItem.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            popupClearSelected(e);
         }
      });
      this.getViewport().add(mTextPane, null);
      mMsgPanelMenu.add(mPopupClearItem);
   }

   private void popupClearSelected(ActionEvent e)
   {
      mMsgDocument.clear();
   }

   // =========================================================================
   // Private classes.
   // =========================================================================

   private class PopupListener extends MouseAdapter
   {
      public void mousePressed (MouseEvent e)
      {
         maybeShowPopup(e);
      }

      private void maybeShowPopup (MouseEvent e)
      {
         if ( e.isPopupTrigger() )
         {
            mMsgPanelMenu.show(e.getComponent(), e.getX(), e.getY());
         }
      }
   }

   // =========================================================================
   // Private member variables.
   // =========================================================================

   private MessageDocument mMsgDocument = null;
   private JTextPane       mTextPane    = new JTextPane();

   private JPopupMenu mMsgPanelMenu   = new JPopupMenu();
   private JMenuItem  mPopupClearItem = new JMenuItem();
}
