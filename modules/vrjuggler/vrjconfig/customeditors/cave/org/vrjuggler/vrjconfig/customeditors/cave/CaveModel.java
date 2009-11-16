/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import javax.swing.event.EventListenerList;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.customeditors.cave.event.*;

public class CaveModel implements EditorConstants
{
   private List mWalls = new ArrayList();
   private ConfigContext mConfigContext = null;
   private BrokerChangeListener mBrokerChangeListener = null;
   private ElementChangeListener mElementChangeListener = null;
   private Vector mScreens = new Vector();
   private Map mScreenToNodeMap = new HashMap();
   private Map mViewToScreenMap = new HashMap();
   
   /** Listeners interested in this cave model. */
   private EventListenerList listenerList = new EventListenerList();
   
   public Vector getScreens()
   {
      return mScreens;
   }

   public Map getScreenToNodeMap()
   {
      return mScreenToNodeMap;
   }
   
   public Map getViewToScreenMap()
   {
      return mViewToScreenMap;
   }
   
   public CaveModel(ConfigContext ctx)
   {
      mConfigContext = ctx;

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      List elements = broker.getElements(mConfigContext);
      
      // Make sure to create the change listener before trying
      // to add them to ConfigElements' listener lists.
      mElementChangeListener = new ElementChangeListener();
      mBrokerChangeListener = new BrokerChangeListener();
      broker.addConfigListener( mBrokerChangeListener );
      
      List display_windows = ConfigUtilities.getElementsWithDefinition(elements, "display_window");
      List cluster_nodes = ConfigUtilities.getElementsWithDefinition(elements, "cluster_node");
      
      List walls = new ArrayList();

      // Add all surface viewports for the local machine.
      for (Iterator itr = display_windows.iterator() ; itr.hasNext(); )
      {
         ConfigElement win = (ConfigElement)itr.next();

         // Add Screen
         mScreens.addElement(win);
         mScreenToNodeMap.put(win, null);
      
         List views = win.getPropertyValues("surface_viewports");
         
         for (Iterator v_itr = views.iterator() ; v_itr.hasNext() ; )
         {
            ConfigElement view = (ConfigElement)v_itr.next();
            walls.add(view);
            mViewToScreenMap.put(view, win);
         }
      }
      
      // Add all surface viewports for each node.
      for (Iterator node_itr = cluster_nodes.iterator() ; node_itr.hasNext(); )
      {
         ConfigElement node = (ConfigElement)node_itr.next();
         node.addConfigElementListener(mElementChangeListener);
         List windows = node.getPropertyValues("display_windows");
         
         for (Iterator winds = windows.iterator() ; winds.hasNext(); )
         {
            ConfigElement win = (ConfigElement)winds.next();
            
            // Add Screen
            mScreens.addElement(win);
            mScreenToNodeMap.put(win, node);
            
            List views = win.getPropertyValues("surface_viewports");
         
            for (Iterator v_itr = views.iterator() ; v_itr.hasNext() ; )
            {
               ConfigElement view = (ConfigElement)v_itr.next();
               walls.add(view);
               mViewToScreenMap.put(view, win);
            }
         }
      }
      
      MultiMap sorter = new MultiMap();
      
      // Sort all view-screen pairs depending on their corner values.
      for (Iterator itr = walls.iterator() ; itr.hasNext() ; )
      {
         ConfigElement view = (ConfigElement)itr.next();
         // Grab the corner values out of the view.
         Corners c = new Corners(view);
         sorter.addValue(c, view);
      }
      
      // Create new CaveWall structure for each surface.
      for (Iterator itr = sorter.getKeys().iterator() ; itr.hasNext() ; )
      {
         Corners new_corners = (Corners)itr.next();
         System.out.println(new_corners);
         CaveWall new_wall = new CaveWall(this, new_corners, (List)sorter.getValues(new_corners));
         mWalls.add(new_wall);
      }
   }

   public void moveScreenToNode(ConfigElement screen, ConfigElement new_node)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigElement old_node = (ConfigElement)mScreenToNodeMap.get(screen);

      broker.remove(mConfigContext, screen);

      if (null != old_node)
      {
         old_node.removeProperty(DISPLAY_WINDOWS_PROPERTY, screen, mConfigContext);
      }
      if (null != new_node)
      {
         new_node.addProperty(DISPLAY_WINDOWS_PROPERTY, screen, mConfigContext);
         mScreenToNodeMap.put(screen, new_node);
      }
   }

   public CaveWall makeNewWall(String wall_name)
   {
      // Create a default view.
      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def = broker.getRepository().get(SURFACE_VIEWPORT_TYPE);
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());
      ConfigElement default_view = factory.create(wall_name, vp_def);
      
      // Create corners for new wall.
      Corners new_corners = new Corners();
      List new_list = new ArrayList();
      new_list.add(default_view);
      CaveWall new_wall = new CaveWall(this, new_corners, new_list);
      mWalls.add(new_wall);
      fireWallAdded(mWalls.size() - 1, new_wall);

      return new_wall;
   }

   /**
    * Remove the specified wall from the current configuration.
    *
    * @param old_wall the wall to be removed
    */
   public void removeWall(CaveWall old_wall)
   {
      mWalls.remove(old_wall);
      
      ConfigElement left_view = old_wall.getLeftView();
      if (null != left_view)
      {
         ConfigElement screen = (ConfigElement)mViewToScreenMap.get( left_view );
         if (null != screen)
         {
            screen.removeProperty(SURFACE_VIEWPORTS_PROPERTY, left_view, mConfigContext);
         }
      }
      
      ConfigElement right_view = old_wall.getRightView();
      if (null != right_view)
      {
         ConfigElement screen = (ConfigElement)mViewToScreenMap.get( right_view );
         if (null != screen)
         {
            screen.removeProperty(SURFACE_VIEWPORTS_PROPERTY, right_view, mConfigContext);
         }
      }
      fireWallRemoved(mWalls.size(), old_wall);
   }
   
   void addScreen(ConfigElement newScreen)
   {
      mScreens.add(newScreen);
      fireScreenAdded(mScreens.size() - 1, newScreen);
   }
   
   void removeScreen(ConfigElement oldScreen)
   {
      mScreens.remove(oldScreen);
      fireScreenRemoved(mScreens.size() - 1, oldScreen);
   }
   
   public List getWalls()
   {
      return mWalls;
   }

   public ConfigContext getConfigContext()
   {
      return mConfigContext;
   }

   public void changeScreenForView(ConfigElement view, ConfigElement new_screen)
   {
      ConfigElement old_screen = (ConfigElement)mViewToScreenMap.remove(view);
      
      if (null != old_screen)
      {
         int result = old_screen.removeProperty(SURFACE_VIEWPORTS_PROPERTY,
                                                view, mConfigContext);
         if (-1 == result)
         {
            System.out.println("ERROR: Trying to move a view from a screen that does not contain it.");
         }
      }
      
      // If we are not setting the screen to null.
      if (null != new_screen)
      {
         new_screen.addProperty(SURFACE_VIEWPORTS_PROPERTY, view, mConfigContext);
         mViewToScreenMap.put(view, new_screen);
      }
   }
   
   public void setViewPosX(ConfigElement view, int pos_x)
   {
      if (pos_x < 0)
      {
         pos_x = 0;
      }

      ConfigElement screen = (ConfigElement)mViewToScreenMap.get( view );
      updateScreenSize(screen);
   }
   
   public void updateScreenSize(ConfigElement screen)
   {
      Map v_low_map = new HashMap();
      Map v_high_map = new HashMap();

      Point2D.Float min = new Point2D.Float(Float.POSITIVE_INFINITY, Float.POSITIVE_INFINITY);
      Point2D.Float max = new Point2D.Float(Float.NEGATIVE_INFINITY, Float.NEGATIVE_INFINITY);
      
      List views = screen.getPropertyValues("surface_viewports");
      int s_origin_x  = ((Integer) screen.getProperty("origin", 0)).intValue();
      int s_origin_y  = ((Integer) screen.getProperty("origin", 1)).intValue();
      int s_size_x  = ((Integer) screen.getProperty("size", 0)).intValue();
      int s_size_y  = ((Integer) screen.getProperty("size", 1)).intValue();
      for (Iterator v_itr = views.iterator() ; v_itr.hasNext() ; )
      {
         ConfigElement view = (ConfigElement)v_itr.next();
         float v_origin_x  = ((Float) view.getProperty("origin", 0)).floatValue();
         float v_origin_y  = ((Float) view.getProperty("origin", 1)).floatValue();
         float v_size_x  = ((Float) view.getProperty("size", 0)).floatValue();
         float v_size_y  = ((Float) view.getProperty("size", 1)).floatValue();
         
         Point2D.Float lower = new Point2D.Float();
         Point2D.Float upper = new Point2D.Float();
         lower.x = s_origin_x + (s_size_x * v_origin_x);
         lower.y = s_origin_y + (s_size_y * v_origin_y);
         
         upper.x = lower.x + (s_size_x * v_size_x);
         upper.y = lower.y + (s_size_y * v_size_y);

         System.out.println("lower: " + lower);
         System.out.println("upper: " + upper);
         
         v_low_map.put(view, lower);
         v_high_map.put(view, upper);

         if (lower.x < min.x)
         {
            min.x = lower.x;
         }
         if (lower.y < min.y)
         {
            min.y = lower.y;
         }
         if (upper.x > max.x)
         {
            max.x = upper.x;
         }
         if (upper.y > max.y)
         {
            max.y = upper.y;
         }
      }
      
      System.out.println("min: " + min);
      System.out.println("max: " + max);
      
      screen.setProperty("origin", 0, new Integer((int)min.x));
      screen.setProperty("origin", 1, new Integer((int)min.y));
      screen.setProperty("size", 0, new Integer((int)(max.x-min.x)));
      screen.setProperty("size", 1, new Integer((int)(max.y-min.y)));
      
      for (Iterator v_itr = views.iterator() ; v_itr.hasNext() ; )
      {
         ConfigElement view = (ConfigElement)v_itr.next();
         
         Point2D.Float low = (Point2D.Float)v_low_map.get(view);
         Point2D.Float high = (Point2D.Float)v_high_map.get(view);
         
         System.out.println("low: " + low);
         System.out.println("upp: " + high);

         float v_origin_x = (low.x - min.x)/(max.x - min.x);
         float v_origin_y = (low.y - min.y)/(max.y - min.y);

         float v_size_x = (high.x - low.x)/(max.x - min.x);
         float v_size_y = (high.y - low.y)/(max.y - min.y);
         
         screen.setProperty("origin", 0, new Float(v_origin_x));
         screen.setProperty("origin", 1, new Float(v_origin_y));
         screen.setProperty("size", 0, new Float(v_size_x));
         screen.setProperty("size", 1, new Float(v_size_y));
      }
   }
   public void setViewPosY(ConfigElement view, int pos_y)
   {
   }
   
   public void setViewWidth(ConfigElement view, int width)
   {
   }

   public void setViewHeight(ConfigElement view, int height)
   {
   }

   /**
    * Adds the given listener to be notified when this cave model changes.
    */
   public void addCaveModelListener(CaveModelListener listener)
   {
      listenerList.add(CaveModelListener.class, listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * cave model changed.
    */
   public void removeCaveModelListener(CaveModelListener listener)
   {
      listenerList.remove(CaveModelListener.class, listener);
   }

   /**
    * Notifies listeners that a wall was added.
    */
   protected void fireWallAdded(int index, CaveWall wall)
   {
      CaveModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == CaveModelListener.class)
         {
            if (evt == null)
            {
               evt = new CaveModelEvent(this, index, wall);
            }
            ((CaveModelListener)listeners[i+1]).wallAdded(evt);
         }
      }
   }
   
   /**
    * Notifies listeners that a wall was removed.
    */
   protected void fireWallRemoved(int index, CaveWall wall)
   {
      CaveModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == CaveModelListener.class)
         {
            if (evt == null)
            {
               evt = new CaveModelEvent(this, index, wall);
            }
            ((CaveModelListener)listeners[i+1]).wallRemoved(evt);
         }
      }
   }
 
   /**
    * Notifies listeners that a screen was added.
    */
   protected void fireScreenAdded(int index, ConfigElement screen)
   {
      CaveModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == CaveModelListener.class)
         {
            if (evt == null)
            {
               evt = new CaveModelEvent(this, index, screen);
            }
            ((CaveModelListener)listeners[i+1]).screenAdded(evt);
         }
      }
   }
   
   /**
    * Notifies listeners that a screen was removed.
    */
   protected void fireScreenRemoved(int index, ConfigElement screen)
   {
      CaveModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == CaveModelListener.class)
         {
            if (evt == null)
            {
               evt = new CaveModelEvent(this, index, screen);
            }
            ((CaveModelListener)listeners[i+1]).screenRemoved(evt);
         }
      }
   }

   /**
    * Custom listener for changes to the config broker.
    */
   private class BrokerChangeListener
      implements ConfigListener
   {
      public void configElementAdded(ConfigEvent evt)
      {
         if (mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            if ( elm.getDefinition().getToken().equals(DISPLAY_WINDOW_TYPE) )
            {
               addScreen(elm);
            }
         }
      }

      public void configElementRemoved(ConfigEvent evt)
      {
         if (mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            if ( elm.getDefinition().getToken().equals(DISPLAY_WINDOW_TYPE) )
            {
               removeScreen(elm);
            }
         }
      }
   }
   
   private class ElementChangeListener
      extends ConfigElementAdapter
   {
      public void propertyValueAdded(ConfigElementEvent evt)
      {
         if (DISPLAY_WINDOWS_PROPERTY == evt.getProperty())
         {
            addScreen((ConfigElement)evt.getValue());
         }
      }

      public void propertyValueRemoved(ConfigElementEvent evt)
      {
         if (DISPLAY_WINDOWS_PROPERTY == evt.getProperty())
         {
            removeScreen((ConfigElement)evt.getValue());
         }
      }
   }
}
