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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import info.clearthought.layout.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;

public class ScreenEditorPanel
   extends JPanel
   implements EditorConstants
{
   public ScreenEditorPanel(Dimension resolution)
      throws HeadlessException
   {
      super();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   public ScreenEditorPanel(ConfigContext ctx, ConfigElement elm)
      throws HeadlessException
   {
      super();
      mConfigContext = ctx;
      mConfigElement = elm;

      try
      {
         jbInit();

         mScreenLabel.setText(mConfigElement.getName());
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   private void jbInit() throws Exception
   {
      double main_size[][] =
         {{TableLayout.PREFERRED},
          {15, 100, 15}};
      mMainLayout = new TableLayout(main_size);
      this.setLayout(mMainLayout);
      
      mViewportPlacer = new ViewportPlacer(mConfigContext, mConfigElement);
      mViewportPlacer.setBorder(new ComputerScreenBorder(Color.BLACK, 5));
      mPropertiesBtn.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               windowPropsEditSelected(e);
            }
         });
      
      this.add(mScreenLabel, new TableLayoutConstraints(0, 0, 0, 0, TableLayout.CENTER,
                                                        TableLayout.FULL));
      this.add(mViewportPlacer, new TableLayoutConstraints(0, 1, 0, 1, TableLayout.CENTER,
                                              TableLayout.CENTER));
      this.add(mPropertiesBtn, new TableLayoutConstraints(0, 2, 0, 2, TableLayout.FULL,
                                               TableLayout.FULL));  
   }

   public void setPlacerSize(Dimension desktopSize)
   {
      mViewportPlacer.setMinimumSize(desktopSize);
      mViewportPlacer.setPreferredSize(desktopSize);
      mViewportPlacer.setMaximumSize(desktopSize);
      mViewportPlacer.setDesktopSize(desktopSize);
      
      revalidate();
      repaint();
   }

   void windowPropsEditSelected(ActionEvent e)
   {
      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                       this);
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(parent, mConfigContext, mConfigElement,
                                      new Dimension(1280, 1024));
                                      //mDesktopSize);

      if ( dlg.showDialog() == DisplayWindowStartDialog.OK_OPTION )
      {
         Rectangle bounds  = dlg.getDisplayWindowBounds();
         mConfigElement.setProperty("origin", 0, new Integer(bounds.x), mConfigContext);
         mConfigElement.setProperty("origin", 1, new Integer(bounds.y), mConfigContext);
         mConfigElement.setProperty("size", 0, new Integer(bounds.width), mConfigContext);
         mConfigElement.setProperty("size", 1, new Integer(bounds.height), mConfigContext);

         ConfigElement fb_cfg =
            (ConfigElement) mConfigElement.getProperty("frame_buffer_config", 0);
         fb_cfg.setProperty("visual_id", 0, dlg.getVisualID(), mConfigContext);
         fb_cfg.setProperty("red_size", 0, dlg.getRedDepth(), mConfigContext);
         fb_cfg.setProperty("green_size", 0, dlg.getGreenDepth(), mConfigContext);
         fb_cfg.setProperty("blue_size", 0, dlg.getBlueDepth(), mConfigContext);
         fb_cfg.setProperty("alpha_size", 0, dlg.getAlphaDepth(), mConfigContext);
         fb_cfg.setProperty("depth_buffer_size", 0, dlg.getDepthBufferSize(),
                            mConfigContext);
         fb_cfg.setProperty("fsaa_enable", 0, dlg.getFSAAEnable(), mConfigContext);

         mConfigElement.setName(dlg.getDisplayWindowTitle());
         mConfigElement.setProperty("stereo", 0, dlg.inStereo(), mConfigContext);
         mConfigElement.setProperty("border", 0, dlg.hasBorder(), mConfigContext);

         mConfigElement.setProperty(LOCK_KEY_PROPERTY, 0, dlg.getLockKey(),
                              mConfigContext);
         mConfigElement.setProperty(START_LOCKED_PROPERTY, 0,
                              dlg.shouldStartLocked(), mConfigContext);
         mConfigElement.setProperty(SLEEP_TIME_PROPERTY, 0, dlg.getSleepTime(),
                              mConfigContext);
      }
   }

   
   private ConfigContext mConfigContext = null;
   private ConfigElement mConfigElement = null;
   private ViewportPlacer mViewportPlacer = null;
   private JLabel mScreenLabel = new JLabel("Name");
   private JButton mPropertiesBtn = new JButton("Properties");
   private TableLayout mMainLayout = null;

   /** The custom listener for changes to the screen. */
   private ChangeListener mChangeListener = new ChangeListener();

   private class ChangeListener implements ConfigElementListener
   {
      public void nameChanged(ConfigElementEvent evt)
      {
         mScreenLabel.setText(mConfigElement.getName());
         /*
         String label = mConfigElement.getName()
            + ((Integer) mConfigElement.getProperty("size", 0)).toString()
            + ((Integer) mConfigElement.getProperty("size", 1)).toString();
         mScreenLabel.setText(label);
         */
      }
      
      public void propertyValueChanged(ConfigElementEvent evt)
      {;}

      public void propertyValueOrderChanged(ConfigElementEvent evt)
      {;}

      public void propertyValueAdded(ConfigElementEvent evt)
      {
         /*
         if ( evt.getProperty().equals( EditorConstants.display_window_type ) )
         {
            ConfigElement elm = (ConfigElement)evt.getValue();
            screenAdded( elm );
         }
         */
      }
      
      public void propertyValueRemoved(ConfigElementEvent evt)
      {
         /*
         if ( evt.getProperty().equals( EditorConstants.display_window_type ) )
         {
            ConfigElement elm = (ConfigElement)evt.getValue();
            screenRemoved( elm );
         }
         */
      }
   }
}