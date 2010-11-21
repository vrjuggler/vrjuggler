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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Point;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import info.clearthought.layout.*;

import java.awt.Color;

import org.vrjuggler.vrjconfig.customeditors.cave.event.*;

public class CaveEditorPanel
   extends JPanel
{
   private JSplitPane mSplitPane = new JSplitPane();
   
   private ScreenDisplay mScreenDisplay = new ScreenDisplay();
   private WallEditorPanel mWallEditorPanel = new WallEditorPanel();
   
   private ConfigContext mConfigContext = null;
   private CaveModel mCaveModel = null;
   
   public void setConfig(ConfigContext ctx)
   {
      mConfigContext = ctx;

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      mCaveModel = new CaveModel(mConfigContext);

      mWallEditorPanel.setConfig(mConfigContext, mCaveModel);
      mScreenDisplay.setConfig(mConfigContext, mCaveModel);
     
      this.revalidate();
      this.repaint();
   }

   public CaveEditorPanel()
   {
      /*
      try
      {
         jbInit();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }
      */
   }
   
   private Container getOwner()
   {
      Object owner = SwingUtilities.getRoot(this);
      System.out.println("owner class: " + owner.getClass());
      return (Container) owner;
   }
   
   /**
    * Positions the given Dialog object relative to this window frame.
    */
   private void positionDialog(Dialog dialog, Container parent)
   {
      Dimension dlg_size   = dialog.getPreferredSize();
      Dimension frame_size = parent.getSize();
      Point loc            = parent.getLocation();

      // Set the location of the dialog so that it is centered with respect
      // to this frame.
      dialog.setLocation((frame_size.width - dlg_size.width) / 2 + loc.x,
                         (frame_size.height - dlg_size.height) / 2 + loc.y);
   }
   
   void jbInit() throws Exception
   {
      this.setLayout(new BorderLayout());
      mSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);

      
      this.add(mSplitPane, BorderLayout.CENTER);
      mSplitPane.setContinuousLayout(true);
      mSplitPane.add(mScreenDisplay, JSplitPane.TOP);
      mSplitPane.add(mWallEditorPanel, JSplitPane.BOTTOM);
   }
}
