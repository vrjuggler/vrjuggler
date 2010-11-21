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

package org.vrjuggler.tweek.text;

import java.awt.Color;
import java.util.Vector;
import javax.swing.ImageIcon;
import javax.swing.text.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;


/**
 * A styled document suitable for use in a GUI for printing status messages.
 *
 * @since 0.6.0
 */
public class MessageDocument extends DefaultStyledDocument
{
   public static final int STATUS  = 0;
   public static final int WARNING = 1;
   public static final int ERROR   = 2;

   // This must be kept in sync with the above styles.
   private static final int MSG_TYPE_COUNT = 3;

/*
   public static MessageDocument instance()
   {
      if ( mInstance == null )
      {
         mInstance = new MessageDocument();
      }

      return mInstance;
   }

   private MessageDocument()
   {
      initStyles();
   }
*/

   public MessageDocument()
   {
      initStyles();
   }

   public void addMessageAdditionListener(MessageAdditionListener l)
   {
      mListeners.add(l);
   }

   public void removeMessageAdditionListener(MessageAdditionListener l)
   {
      mListeners.remove(l);
   }

   public void printStatus(String msg)
   {
      print(msg, STATUS);
   }

   public void printStatusnl(String msg)
   {
      print(msg + "\n", STATUS);
   }

   public void printWarning(String msg)
   {
      print(msg, WARNING);
   }

   public void printWarningnl(String msg)
   {
      print(msg + "\n", WARNING);
   }

   public void printError(String msg)
   {
      print(msg, ERROR);
   }

   public void printErrornl(String msg)
   {
      print(msg + "\n", ERROR);
   }

   public void clear()
   {
      try
      {
         this.remove(0, mLastIndex);
         mLastIndex = 0;
      }
      catch (BadLocationException ex)
      {
         System.err.println("Failed to clear message panel through position " + mLastIndex);
      }
   }

   private void print(String text, int type)
   {
      try
      {
         if ( StyleConstants.getIcon(mIconStyles[type]) != null )
         {
            String icon_pad = " ";
            this.insertString(mLastIndex, icon_pad, mIconStyles[type]);
            mLastIndex += icon_pad.length();
         }

         this.insertString(mLastIndex, text, mTextStyles[type]);
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

   private void fireMessageAddition(String msg, int type)
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

   private void initStyles()
   {
      for ( int i = 0; i < mTextStyles.length; i++ )
      {
         mTextStyles[i] = new SimpleAttributeSet();
         mIconStyles[i] = new SimpleAttributeSet();
      }

      StyleConstants.setForeground(mTextStyles[STATUS], Color.black);
      StyleConstants.setForeground(mTextStyles[WARNING],
                                   new Color(255, 115, 0));
      StyleConstants.setForeground(mTextStyles[ERROR], Color.red);
      StyleConstants.setBold(mTextStyles[ERROR], true);

      try
      {
         StyleConstants.setIcon(mIconStyles[STATUS],
                                new ImageIcon(BeanJarClassLoader.instance().getResource("org/vrjuggler/tweek/text/status.gif")));
      }
      catch (NullPointerException e)
      {;}

      try
      {
         StyleConstants.setIcon(mIconStyles[WARNING],
                                new ImageIcon(BeanJarClassLoader.instance().getResource("org/vrjuggler/tweek/text/warning.gif")));
      }
      catch (NullPointerException e)
      {;}

      try
      {
         StyleConstants.setIcon(mIconStyles[ERROR],
                                new ImageIcon(BeanJarClassLoader.instance().getResource("org/vrjuggler/tweek/text/error.gif")));
      }
      catch (NullPointerException e)
      {;}
   }

//   private static MessagePanelDocument mInstance = null;

   private Vector mListeners = new Vector();

   private int mLastIndex = 0;

   private SimpleAttributeSet[] mTextStyles = new SimpleAttributeSet[MSG_TYPE_COUNT];
   private SimpleAttributeSet[] mIconStyles = new SimpleAttributeSet[MSG_TYPE_COUNT];
}
