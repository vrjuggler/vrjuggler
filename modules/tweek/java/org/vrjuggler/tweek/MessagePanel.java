/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

package org.vrjuggler.tweek;

import java.awt.event.*;
import java.awt.Color;
import java.util.Vector;
import javax.swing.*;
import javax.swing.text.*;


/**
 *
 */
public class MessagePanel
{
   public static final int STATUS  = 0;
   public static final int WARNING = 1;
   public static final int ERROR   = 2;

   // This must be kept in sync with the above styles.
   private static final int MSG_TYPE_COUNT = 3;

   public static MessagePanel instance ()
   {
      if ( mInstance == null )
      {
         mInstance = new MessagePanel();
      }

      return mInstance;
   }

   public void addMessageAdditionListener (MessageAdditionListener l)
   {
      mListeners.add(l);
   }

   public void removeMessageAdditionListener (MessageAdditionListener l)
   {
      mListeners.remove(l);
   }

   public void printStatus (String msg)
   {
      print(msg, STATUS);
   }

   public void printWarning (String msg)
   {
      print(msg, WARNING);
   }

   public void printError (String msg)
   {
      print(msg, ERROR);
   }

   public void clear ()
   {
      try
      {
         mTextPane.getDocument().remove(0, mLastIndex);
         mLastIndex = 0;
      }
      catch (BadLocationException ex)
      {
         System.err.println("Failed to clear message panel through position " + mLastIndex);
      }
   }

   // =========================================================================
   // Package-visible methods.
   // =========================================================================

   /**
    * Returns the internal JPanel object that contains all the current
    * messages.  This is package visible so that user code cannot get access
    * to the JPanel object.  The Tweek GUI code does need access to it,
    * however.
    */
   JComponent getPanel ()
   {
      return panel;
   }

   // =========================================================================
   // Protected methods.
   // =========================================================================

   protected MessagePanel ()
   {
      jbInit();
      initStyles();
   }

   // =========================================================================
   // Protected member variables.
   // =========================================================================

   protected static MessagePanel mInstance = null;

   // =========================================================================
   // Private methods.
   // =========================================================================

   private void jbInit ()
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
      panel.getViewport().add(mTextPane, null);
      mMsgPanelMenu.add(mPopupClearItem);
   }

   private void initStyles ()
   {
      for ( int i = 0; i < mStyles.length; i++ )
      {
         mStyles[i] = new SimpleAttributeSet();
      }

      StyleConstants.setForeground(mStyles[STATUS], Color.black);
      StyleConstants.setForeground(mStyles[WARNING], Color.orange);
      StyleConstants.setForeground(mStyles[ERROR], Color.red);
      StyleConstants.setBold(mStyles[ERROR], true);
   }

   private void print (String text, int type)
   {
      StyledDocument doc = (StyledDocument) mTextPane.getDocument();

      try
      {
         doc.insertString(mLastIndex, text, mStyles[type]);
         mLastIndex += text.length();

         fireMessageAddition(text, type);
      }
      catch (BadLocationException loc_ex)
      {
         System.err.println("WARNING: Failed to insert message '" + text +
                            "' of type " + type + " at location " +
                            mLastIndex);
      }
   }

   private void fireMessageAddition (String msg, int type)
   {
      MessageAdditionEvent e = new MessageAdditionEvent(this, msg, type);

      MessageAdditionListener l = null;
      Vector listeners;

      synchronized (this)
      {
         listeners = (Vector) mListeners.clone();
      }

      for ( int i = 0; i < listeners.size(); i++ )
      {
         l = (MessageAdditionListener) listeners.elementAt(i);
         l.messageAdded(e);
      }
   }

   void popupClearSelected (ActionEvent e)
   {
      clear();
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

   private Vector mListeners = new Vector();

   private JTextPane   mTextPane  = new JTextPane();
   private JScrollPane panel      = new JScrollPane();
   private int         mLastIndex = 0;

   private SimpleAttributeSet[] mStyles = new SimpleAttributeSet[MSG_TYPE_COUNT];

   private JPopupMenu mMsgPanelMenu   = new JPopupMenu();
   private JMenuItem  mPopupClearItem = new JMenuItem();
}
