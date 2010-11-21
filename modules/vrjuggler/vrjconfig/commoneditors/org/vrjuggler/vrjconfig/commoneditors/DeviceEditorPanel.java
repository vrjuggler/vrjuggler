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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.BorderLayout;
import java.awt.Dimension;
import javax.swing.*;
//import javax.swing.event.HyperlinkEvent;
//import javax.swing.event.HyperlinkListener;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
//import org.vrjuggler.jccl.editors.net.TinyBrowser;


public class DeviceEditorPanel
   extends JPanel
{
   public DeviceEditorPanel(java.net.URL htmlHelpUrl, DeviceTypeEditor editor,
                            Dimension helpMinSize, Dimension helpPrefSize,
                            Dimension contentMinSize, Dimension contentPrefSize)
   {
      mDeviceEditor = editor;

      mHelpScrollPanel.setMinimumSize(helpMinSize);
      mHelpScrollPanel.setPreferredSize(helpPrefSize);
      mContentPanel.setMinimumSize(contentMinSize);
      mContentPanel.setPreferredSize(contentPrefSize);

      try
      {
         jbInit();

         if ( htmlHelpUrl != null )
         {
            mHelpPanel.setPage(htmlHelpUrl);
         }
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }

/*
      mHelpPanel.addHyperlinkListener(new DocLinkListener());
      mHelpBrowserFrame.setContentPane(mHelpBrowser);
      mHelpBrowserFrame.setSize(new java.awt.Dimension(640, 480));
      mHelpBrowserFrame.setTitle("Config Element Help Browsr");
      mHelpBrowserFrame.validate();
*/
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mDeviceEditor.setConfig(ctx, elt);
   }

   private void jbInit()
      throws Exception
   {
      mMainSplitPane.setOrientation(JSplitPane.HORIZONTAL_SPLIT);
      mMainSplitPane.setOneTouchExpandable(true);
      mMainSplitPane.setDividerSize(5);
      this.setLayout(mMainLayout);
      mContentPanel.setLayout(mContentPanelLayout);
      mHelpPanel.setEditable(false);
      mMainSplitPane.add(mHelpScrollPanel, JSplitPane.TOP);
      mHelpScrollPanel.getViewport().add(mHelpPanel);
      mMainSplitPane.add(mContentPanel, JSplitPane.BOTTOM);
      mContentPanel.add((JComponent) mDeviceEditor, BorderLayout.CENTER);
      this.add(mMainSplitPane, BorderLayout.CENTER);
   }

//   private TinyBrowser mHelpBrowser      = new TinyBrowser();
   private JFrame      mHelpBrowserFrame = new JFrame();

   private DeviceTypeEditor mDeviceEditor = null;
   private JSplitPane mMainSplitPane = new JSplitPane();
   private JScrollPane mHelpScrollPanel = new JScrollPane();
   private JPanel mContentPanel = new JPanel();
   private BorderLayout mMainLayout = new BorderLayout();
   private BorderLayout mContentPanelLayout = new BorderLayout();
   private JEditorPane mHelpPanel = new JEditorPane();

/*
   private class DocLinkListener
      implements HyperlinkListener
   {
      public void hyperlinkUpdate(HyperlinkEvent e)
      {
         if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
         {
            mHelpBrowser.setPage(e.getURL());
            mHelpBrowserFrame.setVisible(true);
         }
      }
   }
*/
}
