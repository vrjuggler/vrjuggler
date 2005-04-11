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

import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import info.clearthought.layout.*;

public class ScreenDisplay
   extends JPanel
{
   private ConfigContext mConfigContext = null;
   private CaveModel mCaveModel = null;
   private HashMap mScreens = new HashMap();
   private BrokerChangeListener mBrokerChangeListener = null;
   private TableLayout mTableLayout = null;
   
   public ScreenDisplay()
   {
      double size[][] = {{0.50, 0.50},{150}};
      mTableLayout = new TableLayout(size);
      setLayout(mTableLayout);
   }

   public void setConfig( ConfigContext ctx, CaveModel cm )
   {
      mConfigContext = ctx;
      mCaveModel = cm;
      
      for ( Iterator itr = mCaveModel.getScreens().iterator() ; itr.hasNext() ; )
      {
         addScreen( (ConfigElement)itr.next() );
      }

      mBrokerChangeListener = new BrokerChangeListener(this);
      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      broker.addConfigListener( mBrokerChangeListener );
   }
   
   private void addScreen(ConfigElement elm)
   {
      elm.addConfigElementListener(mChangeListener);
      
      ScreenEditorPanel sep = new ScreenEditorPanel(mConfigContext, elm);

      int col = mTableLayout.getNumColumn();
      mTableLayout.insertColumn(col-1, TableLayout.PREFERRED);
      this.add(sep, new TableLayoutConstraints(col - 1, 0, col - 1, 0, TableLayout.FULL,
                                               TableLayout.TOP));
      mScreens.put(elm, sep);
      revalidate();
      repaint();
   }

   private void removeScreen(ConfigElement elm)
   {
      Object obj = mScreens.remove( elm );
      if ( null != obj && obj instanceof Component)
      {
         Component comp = (Component)obj;
         int col = mTableLayout.getConstraints(comp).col1;
         mTableLayout.deleteColumn(col);

         this.remove(comp);
         //mTableLayout.layoutContainer(mScreenDisplay);
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
    * Custom listener for changes to the config broker.
    */
   private class BrokerChangeListener
      implements ConfigListener
   {
      private ScreenDisplay mScreenDisplay = null;

      public BrokerChangeListener(ScreenDisplay sd)
      {
         mScreenDisplay = sd;
      }

      public void configElementAdded(ConfigEvent evt)
      {
         if (mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            //System.out.println(elm.getDefinition().getName());
            if ( elm.getDefinition().getToken().equals(EditorConstants.display_window_type) )
            {
               mScreenDisplay.addScreen(elm);
            }
         }
      }

      public void configElementRemoved(ConfigEvent evt)
      {
         if (mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            if ( elm.getDefinition().getToken().equals(EditorConstants.display_window_type) )
            {
               mScreenDisplay.removeScreen(elm);
            }
         }
      }
   }
}
