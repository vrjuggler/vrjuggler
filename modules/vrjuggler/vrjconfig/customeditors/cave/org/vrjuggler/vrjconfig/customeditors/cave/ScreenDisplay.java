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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.vrjconfig.customeditors.cave.event.CaveModelAdapter;
import org.vrjuggler.vrjconfig.customeditors.cave.event.CaveModelEvent;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import info.clearthought.layout.*;

public class ScreenDisplay
   extends JPanel
   implements EditorConstants
{
   private ConfigContext mConfigContext = null;
   private CaveModel mCaveModel = null;
   private HashMap mScreens = new HashMap();
   private CaveModelListener mCaveModelListener = null;
   private TableLayout mMainPanelLayout = null;
   
   private Icon mAddIcon = null;

   private JButton mAddScreenBtn = new JButton();

   private JPanel mMainPanel = new JPanel();
   private JScrollPane mScreenScrollPanel = new JScrollPane();
   private TableLayout mTableLayout = null;

   private boolean mClusterConfig = false;

   public ScreenDisplay()
   {
      double[][] main_size = {{0.50, 0.50},{140}};
      mMainPanelLayout = new TableLayout(main_size);
      mMainPanel.setLayout(mMainPanelLayout);

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mAddIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/add.gif"));
         mAddScreenBtn.setIcon(mAddIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mAddScreenBtn.setText("Add");
      }

      mAddScreenBtn.setToolTipText("Configure a new screen.");
      mAddScreenBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddScreenBtn_actionPerformed(e);
         }
      });
      
      mScreenScrollPanel.getViewport().setLayout(new BorderLayout());
      mScreenScrollPanel.getViewport().add(mMainPanel, BorderLayout.CENTER);
      
      double[][] size = {{TableLayout.FILL, 25},
                         {25, TableLayout.FILL}};
      mTableLayout = new TableLayout(size);
      
      setLayout(mTableLayout);
      add(mScreenScrollPanel,
            new TableLayoutConstraints(0, 0, 0, 1,
                                       TableLayout.FULL,
                                       TableLayout.FULL));
      add(mAddScreenBtn,
            new TableLayoutConstraints(1, 0, 1, 0,
                                       TableLayout.FULL,
                                       TableLayout.FULL));
   }

   void mAddScreenBtn_actionPerformed(ActionEvent e)
   {
      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def = broker.getRepository().get(DISPLAY_WINDOW_TYPE);
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());
      ConfigElement new_screen = factory.create("", vp_def);

      int status = editScreen(new_screen);
      
      // If we are configuring a cluster we do not want to add the new
      // screen to the ConfigBroker since it will end up being an
      // embedded ConfigElement.
      if ( !mClusterConfig && status == DisplayWindowStartDialog.OK_OPTION )
      {
         // Make sure this add goes through successfully
         if (! broker.add(mCaveModel.getConfigContext(), new_screen))
         {
            JOptionPane.showMessageDialog(SwingUtilities.getAncestorOfClass(Frame.class, this),
                                          "There are no configuration files active.",
                                          "Error",
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }
      }
   }

   private int editScreen(ConfigElement elm)
   {
      //XXX: Code reused from ScreenEditorPanel
      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                       this);

      System.out.println("Creating dialog: " + mClusterConfig);
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(parent, mConfigContext, elm,
                                      new Dimension(1280, 1024),
                                      mCaveModel, mClusterConfig);

      int status = dlg.showDialog();

      if ( status == DisplayWindowStartDialog.OK_OPTION )
      {
         Rectangle bounds  = dlg.getDisplayWindowBounds();
         elm.setProperty("origin", 0, new Integer(bounds.x), mConfigContext);
         elm.setProperty("origin", 1, new Integer(bounds.y), mConfigContext);
         elm.setProperty("size", 0, new Integer(bounds.width), mConfigContext);
         elm.setProperty("size", 1, new Integer(bounds.height), mConfigContext);

         elm.setName(dlg.getDisplayWindowTitle());
         
         if (mClusterConfig)
         {
            ConfigElement node = dlg.getSelectedNode();
            mCaveModel.moveScreenToNode(elm, node);
         }
      }

      return status;
   }
   
   public void setConfig( ConfigContext ctx, CaveModel cm )
   {
      mConfigContext = ctx;
      mCaveModel = cm;
      
      for ( Iterator itr = mCaveModel.getScreens().iterator() ; itr.hasNext() ; )
      {
         addScreen( (ConfigElement)itr.next() );
      }

      mCaveModelListener = new CaveModelListener(this);
      mCaveModel.addCaveModelListener( mCaveModelListener );
   }

   public void setClusterConfig(boolean clusterConfig)
   {
      mClusterConfig = clusterConfig;
      System.out.println("ScreenDisplay: mClusterConfig: " + mClusterConfig);
      Collection col = mScreens.values();
      for (Iterator itr = col.iterator() ; itr.hasNext() ; )
      {
         ((ScreenEditorPanel)itr.next()).setClusterConfig(clusterConfig);
      }
   }
   
   private void addScreen(ConfigElement elm)
   {
      elm.addConfigElementListener(mChangeListener);
      
      ScreenEditorPanel sep = new ScreenEditorPanel(mConfigContext, elm, mCaveModel);

      int col = mMainPanelLayout.getNumColumn();
      mMainPanelLayout.insertColumn(col-1, TableLayout.PREFERRED);
      mMainPanel.add(sep, new TableLayoutConstraints(col - 1, 0, col - 1, 0, TableLayout.FULL,
                                               TableLayout.TOP));
      mScreens.put(elm, sep);

      sep.setClusterConfig(mClusterConfig);

      revalidate();
      repaint();
   }

   private void removeScreen(ConfigElement elm)
   {
      Object obj = mScreens.remove( elm );
      if ( null != obj && obj instanceof Component)
      {
         Component comp = (Component)obj;
         int col = mMainPanelLayout.getConstraints(comp).col1;
         mMainPanelLayout.deleteColumn(col);

         mMainPanel.remove(comp);
         this.revalidate();
         this.repaint();
      }
   }

   private void calcPanelSizes()
   {
      System.out.println("Need to calculate the size of all panels.");
      Set screen_elms = mScreens.keySet();

      int max_x = 0;
      int max_y = 0;

      for (Iterator itr = screen_elms.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         int size_x = ((Number)elm.getProperty("size", 0)).intValue();
         int size_y = ((Number)elm.getProperty("size", 1)).intValue();
         System.out.println("Size: (" + size_x + ", " + size_y + ")");
         if (size_x > max_x)
         {
            max_x = size_x;
         }
         if (size_y > max_y)
         {
            max_y = size_y;
         }
      }
      
      System.out.println("Max Size: (" + max_x + ", " + max_y + ")");
      
      for (Iterator itr = screen_elms.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         Object obj = mScreens.get( elm );
         if ( null != obj && obj instanceof ScreenEditorPanel)
         {
            int size_x = ((Number)elm.getProperty("size", 0)).intValue();
            int size_y = ((Number)elm.getProperty("size", 1)).intValue();
         
            float y_scale = (100.0f/(float)max_y);
            int x = (int)(y_scale*(float)size_x);
            int y = (int)(y_scale*(float)size_y);
            
            System.out.println("Adjusted Size: (" + x + ", " + y + ")");
            ScreenEditorPanel comp = (ScreenEditorPanel)obj;
            comp.setPlacerSize(new Dimension(x, y));
         }
      }
   }
   
   private ChangeListener mChangeListener = new ChangeListener();
   
   private class ChangeListener extends ConfigElementAdapter
   {
      public void propertyValueChanged(ConfigElementEvent event)
      {
         if ( event.getProperty().equals("size") ||
              event.getProperty().equals("origin") )
         {
            calcPanelSizes();
         }
      }
   }
   
   /**
    * Custom listener for changes to the cave model.
    */
   private class CaveModelListener
      extends CaveModelAdapter
   {
      private ScreenDisplay mScreenDisplay = null;

      public CaveModelListener(ScreenDisplay sd)
      {
         mScreenDisplay = sd;
      }
      public void screenAdded(CaveModelEvent evt)
      {
         mScreenDisplay.addScreen((ConfigElement)evt.getValue());
      }

      public void screenRemoved(CaveModelEvent evt)
      {
         mScreenDisplay.removeScreen((ConfigElement)evt.getValue());
      }
   }
}
