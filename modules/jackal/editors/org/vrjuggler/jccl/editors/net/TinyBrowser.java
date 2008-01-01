/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

package org.vrjuggler.jccl.editors.net;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.net.URL;
import java.util.Stack;
import javax.swing.*;
import javax.swing.JEditorPane;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.html.HTMLDocument;
import javax.swing.text.html.HTMLFrameHyperlinkEvent;


public class TinyBrowser extends JPanel
{
   public TinyBrowser()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      mBrowserPane.addHyperlinkListener(new Hyperactive());

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mBackButton.setIcon(new ImageIcon(loader.getResource(
            "org/vrjuggler/jccl/editors/images/Back16.gif"
         )));
         mForwardButton.setIcon(new ImageIcon(loader.getResource(
            "org/vrjuggler/jccl/editors/images/Forward16.gif"
         )));
      }
      catch(NullPointerException ex)
      {
         /* No images available for buttons. */ ;
      }
   }

   public void setPage(URL url)
   {
      try
      {
         URL cur_page = mBrowserPane.getPage();
         mBrowserPane.setPage(url);
         mUrlField.setText(mBrowserPane.getPage().toString());

         // If the above throws an exception, the Back/Forward buttons will not
         // be affected.
         if ( null != cur_page )
         {
            mBackStack.push(cur_page);
            mBackButton.setEnabled(true);

            mForwardStack.clear();
            mForwardButton.setEnabled(false);
         }
      }
      catch(IOException ex)
      {
         pageLoadFailed(ex);
      }
   }

   private void jbInit() throws Exception
   {
      this.setLayout(mMainLayout);
      mBackButton.setEnabled(false);
      mBackButton.setHorizontalAlignment(SwingConstants.LEFT);
      mBackButton.setText("Back");
      mBackButton.addActionListener(new TinyBrowser_mBackButton_actionAdapter(this));
      mForwardButton.setEnabled(false);
      mForwardButton.setForeground(Color.black);
      mForwardButton.setHorizontalAlignment(SwingConstants.LEFT);
      mForwardButton.setHorizontalTextPosition(SwingConstants.TRAILING);
      mForwardButton.setText("Forward");
      mForwardButton.addActionListener(new TinyBrowser_mForwardButton_actionAdapter(this));
      mUrlField.setEditable(false);
      mUrlField.setText("");
      mBrowserPane.setEditable(false);
      mBrowserPane.setText("");
      mButtonPanel.setLayout(mButtonPanelLayout);
      this.add(mButtonPanel, BorderLayout.NORTH);
      mButtonPanel.add(mBackButton, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(5, 5, 5, 0), 0, 0));
      mButtonPanel.add(mForwardButton, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5, 5, 5, 5), 0, 0));
      mButtonPanel.add(mUrlField, new GridBagConstraints(2, 0, GridBagConstraints.REMAINDER, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(5, 0, 5, 5), 0, 0));
      this.add(mBrowserScrollPane, BorderLayout.CENTER);
      mBrowserScrollPane.getViewport().add(mBrowserPane, null);
   }

   private void pageLoadFailed(IOException ex)
   {
      JOptionPane.showMessageDialog(SwingUtilities.getAncestorOfClass(Frame.class, this),
                                    ex.getMessage(),
                                    "Web Page Load Failed",
                                    JOptionPane.ERROR_MESSAGE);
   }

   void mBackButton_actionPerformed(ActionEvent e)
   {
      try
      {
         URL cur_page = mBrowserPane.getPage();
         mBrowserPane.setPage((URL) mBackStack.pop());

         mUrlField.setText(mBrowserPane.getPage().toString());
         mBackButton.setEnabled(mBackStack.size() > 0);
         mForwardStack.push(cur_page);
         mForwardButton.setEnabled(true);
      }
      catch(IOException ex)
      {
         pageLoadFailed(ex);
      }
   }

   void mForwardButton_actionPerformed(ActionEvent e)
   {
      try
      {
         URL cur_page = mBrowserPane.getPage();
         mBrowserPane.setPage((URL) mForwardStack.pop());

         mUrlField.setText(mBrowserPane.getPage().toString());
         mForwardButton.setEnabled(mForwardStack.size() > 0);
         mBackStack.push(cur_page);
         mBackButton.setEnabled(true);
      }
      catch(IOException ex)
      {
         pageLoadFailed(ex);
      }
   }

   private class Hyperactive
      implements HyperlinkListener
   {
      public void hyperlinkUpdate(HyperlinkEvent e)
      {
         if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
         {
            JEditorPane pane = (JEditorPane) e.getSource();
            if (e instanceof HTMLFrameHyperlinkEvent)
            {
               HTMLFrameHyperlinkEvent evt = (HTMLFrameHyperlinkEvent) e;
               HTMLDocument doc = (HTMLDocument) pane.getDocument();
               doc.processHTMLFrameHyperlinkEvent(evt);
            }
            else
            {
               // Call TinyBrowser.setPage().
               setPage(e.getURL());
            }
         }
      }
   }

   private Stack mBackStack = new Stack();
   private Stack mForwardStack = new Stack();

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mButtonPanel = new JPanel();
   private JScrollPane mBrowserScrollPane = new JScrollPane();
   private JButton mBackButton = new JButton();
   private JButton mForwardButton = new JButton();
   private JTextField mUrlField = new JTextField();
   private JEditorPane mBrowserPane = new JEditorPane();
   private GridBagLayout mButtonPanelLayout = new GridBagLayout();
}

class TinyBrowser_mBackButton_actionAdapter implements ActionListener
{
   private TinyBrowser adaptee;

   TinyBrowser_mBackButton_actionAdapter(TinyBrowser adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mBackButton_actionPerformed(e);
   }
}

class TinyBrowser_mForwardButton_actionAdapter implements ActionListener
{
   private TinyBrowser adaptee;

   TinyBrowser_mForwardButton_actionAdapter(TinyBrowser adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mForwardButton_actionPerformed(e);
   }
}
