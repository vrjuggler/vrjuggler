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
      //setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
      //setLayout(new FlowLayout());
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
      /*
      JPanel both = new JPanel();
      JLabel lbl = new JLabel("Aron");
      JComboBox cb = new JComboBox();
      
      ViewportPlacer vp = new ViewportPlacer(mConfigContext, elm);
      */
      ScreenEditorPanel sep = new ScreenEditorPanel(mConfigContext, elm);
      /*
      vp.setMinimumSize(new Dimension(100, 100));
      vp.setPreferredSize(new Dimension(100, 100));
      vp.setMaximumSize(new Dimension(100, 100));
      vp.setBorder(BorderFactory.createLineBorder(java.awt.Color.black));
      */
      
      /*
      lbl.setMinimumSize(new Dimension(100, 10));
      lbl.setPreferredSize(new Dimension(100, 10));
      lbl.setMaximumSize(new Dimension(100, 10));
      
      both.setLayout(new BorderLayout());
      both.add(lbl, BorderLayout.NORTH);
      both.add(vp, BorderLayout.CENTER);
      both.add(cb, BorderLayout.SOUTH);
      
      both.setMinimumSize(vp.getMinimumSize());
      both.setPreferredSize(vp.getPreferredSize());
      both.setMaximumSize(vp.getMaximumSize());
      */

      int col = mTableLayout.getNumColumn();
      mTableLayout.insertColumn(col-1, TableLayout.PREFERRED);
      this.add(sep, new TableLayoutConstraints(col - 1, 0, col - 1, 0, TableLayout.FULL,
                                               TableLayout.FULL));
      /*
      this.add(vp, new TableLayoutConstraints(col - 1, 1, col - 1, 1, TableLayout.FULL,
                                               TableLayout.FULL));
      this.add(cb, new TableLayoutConstraints(col - 1, 2, col - 1, 2, TableLayout.FULL,
                                               TableLayout.FULL));
      */
      /*
      double main_size[][] =
         {{100},
          {20, 100}};
      both.setLayout(new TableLayout(main_size));
      both.add(lbl,  new TableLayoutConstraints(0, 0, 0, 0,
                                                TableLayout.CENTER,
                                                TableLayout.CENTER));
      both.add(vp,  new TableLayoutConstraints(0, 1, 0, 1,
                                               TableLayout.FULL,
                                               TableLayout.FULL));
      */
      //this.add(both);
      
      // XXX: Provide a way to de-select all other placers.
      /*
      vp.addMouseListener(new MouseAdapter()
      {
         public void mousePressed(MouseEvent e)
         {
            if()
            {
            }
         }
      });
      */
      
      mScreens.put(elm, sep);
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
               ViewportPlacer vp = new ViewportPlacer(mConfigContext, elm);
               vp.setMinimumSize(new Dimension(100, 100));
               vp.setPreferredSize(new Dimension(100, 100));
               vp.setMaximumSize(new Dimension(100, 100));
               vp.setBorder(BorderFactory.createLineBorder(java.awt.Color.black));
               mScreenDisplay.add(vp);
               mScreenDisplay.revalidate();
               mScreenDisplay.repaint();
               
               mScreens.put(elm, vp);
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
               Object obj = mScreens.remove( elm );
               if ( null != obj )
               {
                  mScreenDisplay.remove( (Component)obj );
                  mScreenDisplay.revalidate();
                  mScreenDisplay.repaint();
               }
            }
         }
      }
   }
}
