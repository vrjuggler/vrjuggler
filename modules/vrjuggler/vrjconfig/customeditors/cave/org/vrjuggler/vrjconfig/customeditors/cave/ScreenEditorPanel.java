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
   private Icon mRemoveIcon = null;
   private Icon mEditIcon = null;
   private JButton mRemoveScreenBtn = new JButton();
   private JButton mEditScreenBtn = new JButton();

   /*
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
   */

   public ScreenEditorPanel(ConfigContext ctx, ConfigElement elm, CaveModel caveModel)
      throws HeadlessException
   {
      super();
      mConfigContext = ctx;
      mConfigElement = elm;
      mCaveModel = caveModel;

      mConfigElement.addConfigElementListener(mChangeListener);

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
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                        "/remove.gif"));
         mEditIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                      "/edit.gif"));
         
         mRemoveScreenBtn.setIcon(mRemoveIcon);
         mEditScreenBtn.setIcon(mEditIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mRemoveScreenBtn.setText("Remove");
         mEditScreenBtn.setText("Edit");
      }
      
      mRemoveScreenBtn.setToolTipText("Remove screen.");
      mRemoveScreenBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mRemoveScreenBtn_actionPerformed(e);
         }
      });
      
      mEditScreenBtn.setToolTipText("Edit screen.");
      mEditScreenBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mEditScreenBtn_actionPerformed(e);
         }
      });

      double main_size[][] =
         {{TableLayout.PREFERRED, 25, 25, TableLayout.PREFERRED},
          {15, 100, 25}};
      mMainLayout = new TableLayout(main_size);
      this.setLayout(mMainLayout);
      
      mViewportPlacer = new ViewportPlacer(mConfigContext, mConfigElement);
      mViewportPlacer.setBorder(new ComputerScreenBorder(Color.BLACK, 5));
      
      this.add(mScreenLabel, new TableLayoutConstraints(0, 0, 3, 0, TableLayout.CENTER,
                                                        TableLayout.FULL));
      this.add(mViewportPlacer, new TableLayoutConstraints(0, 1, 3, 1, TableLayout.CENTER,
                                              TableLayout.CENTER));
      this.add(mRemoveScreenBtn, new TableLayoutConstraints(1, 2, 1, 2, TableLayout.FULL,
                                               TableLayout.FULL));  
      this.add(mEditScreenBtn, new TableLayoutConstraints(2, 2, 2, 2, TableLayout.FULL,
                                               TableLayout.FULL));  
   }
   
   void mRemoveScreenBtn_actionPerformed(ActionEvent e)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      broker.remove(mConfigContext, mConfigElement);
   }
   
   void mEditScreenBtn_actionPerformed(ActionEvent e)
   {
      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                       this);
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(parent, mConfigContext, mConfigElement,
                                      new Dimension(1280, 1024), mCaveModel,
                                      mClusterConfig);

      if ( dlg.showDialog() == DisplayWindowStartDialog.OK_OPTION )
      {
         Rectangle bounds  = dlg.getDisplayWindowBounds();
         mConfigElement.setProperty("origin", 0, new Integer(bounds.x), mConfigContext);
         mConfigElement.setProperty("origin", 1, new Integer(bounds.y), mConfigContext);
         mConfigElement.setProperty("size", 0, new Integer(bounds.width), mConfigContext);
         mConfigElement.setProperty("size", 1, new Integer(bounds.height), mConfigContext);

         if (mClusterConfig)
         {
            ConfigElement node = dlg.getSelectedNode();
            mCaveModel.moveScreenToNode(mConfigElement, node);
         }
      }
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
   
   public void setClusterConfig(boolean clusterConfig)
   {
      mClusterConfig = clusterConfig;
   }
   
   private boolean mClusterConfig = false;
   private ConfigContext mConfigContext = null;
   private ConfigElement mConfigElement = null;
   private CaveModel mCaveModel = null;
   private ViewportPlacer mViewportPlacer = null;
   private JLabel mScreenLabel = new JLabel("Name");
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
