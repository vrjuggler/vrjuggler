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

package org.vrjuggler.vrjconfig.customeditors.flock;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import javax.swing.*;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.editors.net.TinyBrowser;


public class MainEditorPanel
   extends JPanel
{
   public MainEditorPanel()
   {
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String data_base = "org/vrjuggler/vrjconfig/customeditors/flock/data";
         mHelpPanel.setPage(loader.getResource(data_base + "/help.html"));
      }
      catch(java.io.IOException ex)
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
      mFlockEditorPanel.setConfig(ctx, elt);
   }

   private void jbInit()
      throws Exception
   {
      mHelpScrollPanel.setMinimumSize(new Dimension(0, 0));
      mHelpScrollPanel.setPreferredSize(new Dimension(225, 600));
      mContentPanel.setMinimumSize(new Dimension(600, 600));
      mContentPanel.setPreferredSize(new Dimension(725, 600));
      mMainSplitPane.setOrientation(JSplitPane.HORIZONTAL_SPLIT);
      mMainSplitPane.setOneTouchExpandable(true);
      mMainSplitPane.setDividerSize(5);
      this.setLayout(mMainLayout);
      mContentPanel.setLayout(mContentPanelLayout);
      mContentPanelLayout.setColumns(1);
      mContentPanelLayout.setRows(0);
      mHelpPanel.setEditable(false);
      mHelpPanel.setText("");
      mMainSplitPane.add(mHelpScrollPanel, JSplitPane.TOP);
      mHelpScrollPanel.getViewport().add(mHelpPanel);
      mMainSplitPane.add(mContentPanel, JSplitPane.BOTTOM);
      mContentPanel.add(mFlockEditorPanel);
      this.add(mMainSplitPane, java.awt.BorderLayout.CENTER);
   }

   private TinyBrowser mHelpBrowser      = new TinyBrowser();
   private JFrame      mHelpBrowserFrame = new JFrame();

   private FlockEditorPanel mFlockEditorPanel = new FlockEditorPanel();
   private JSplitPane mMainSplitPane = new JSplitPane();
   private JScrollPane mHelpScrollPanel = new JScrollPane();
   private JPanel mContentPanel = new JPanel();
   private BorderLayout mMainLayout = new BorderLayout();
   private GridLayout mContentPanelLayout = new GridLayout();
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
