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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.Color;
import java.awt.Font;
import java.awt.geom.Rectangle2D;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.swing.BorderFactory;
import javax.swing.JComponent;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.GraphConstants;
import org.jgraph.util.JGraphHeavyweightRedirector;
import org.jgraph.util.JGraphUtilities;

import org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphLayoutCache;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.GraphHelpers;


/**
 * An extension to the JGraph Swing component whose most useful feature is
 * setting up a special graph model, graph layout cache, and cell view factory
 * to help users avoid making very confusing mistakes.
 */
public class DeviceGraph
   extends JGraph
   implements EditorConstants
{
   /**
    * Default color attribute used for device cells.
    *
    * @see #createDeviceAttributes(int,int,boolean)
    */
   public static final Color DEVICE_CELL_COLOR = new Color(68, 90, 249);

   /**
    * Default color attribute used for proxy cells.
    *
    * @see #createProxyAttributes(int,int,boolean)
    */
   public static final Color PROXY_CELL_COLOR  = new Color(249, 68, 68);

   /**
    * Initializes this JGraph specialization to use a model of type
    * <code>DeviceGraphModel</code>, a graph layout cache of type
    * <code>DeviceGraphLayoutCache</code>, and a cell view factory of type
    * <code>ProxiedDeviceCellViewFactory</code>.  An instance of
    * <code>DefaultDeviceGraphCellCreator</code> is registered with
    * <code>GraphHelpers</code> to allow those helper functions to do their
    * job.  Custom (or overriding) device graph cell creators should be
    * registered <i>after</i> creating an instance of this class to ensure
    * that they are no overwritten as a side effect of this constructor.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphLayoutCache
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxiedDeviceCellViewFactory
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DefaultDeviceGraphCellCreator
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.GraphHelpers
    */
   public DeviceGraph()
   {
      this(new DeviceGraphModel());
   }

   public DeviceGraph(DeviceGraphModel model)
   {
      this(model, new DeviceGraphLayoutCache(model));
   }

   public DeviceGraph(DeviceGraphModel model, DeviceGraphLayoutCache cache)
   {
      super(model, cache);
      this.addMouseListener(new JGraphHeavyweightRedirector(false, true));
   }

   /**
    * Cause all cells to change their size to the cell's renderer's perferred
    * size. This will reveal any descriptions under the cell.
    *
    */
   public void autoSizeAll()
   {
      CellView[] views =
         this.getGraphLayoutCache().getMapping(
            JGraphUtilities.getVertices(getModel(),
                                        DefaultGraphModel.getAll(getModel()))
         );
      try
      {
         Hashtable table = new Hashtable();
         if (views != null)
         {
            for ( int i = 0; i < views.length; ++i )
            {
               CellView view = views[i];
               JComponent component =
                  (JComponent) view.getRendererComponent(this, false, false,
                                                         false);
               if ( component != null )
               {
                  this.add(component);
                  component.validate();
                  GraphHelpers.autoSizeCellView(component.getPreferredSize(),
                                                view, table);
               }
            }
         }
         this.getGraphLayoutCache().edit(table, null, null, null);
      }
      catch (Exception ee)
      {
         ee.printStackTrace();
      }
      this.repaint();
   }

   /**
    * Centralized creation of an attribute map for the line styles used for
    * edges connecting a proxy pointing at a device.
    */
   public static Map createProxyLineStyle()
   {
      AttributeMap map = new AttributeMap();
      GraphConstants.setAutoSize(map, true);
      GraphConstants.setLineEnd(map, GraphConstants.ARROW_CLASSIC);
      GraphConstants.setEndFill(map, true);
      GraphConstants.setDashPattern(map, new float[]{3, 0});
      return map;
   }

   /**
    * Centralized creation of an attribute map for graph cells that represent
    * the ConfigElement for a device.  Internally, this makes use of
    * <code>createDeviceAttributes(int,int,boolean)</code>.  It is invoked
    * with its <code>autoSize</code> parameter set to true.
    *
    * @param x  the x-coordinate for the device cell
    * @param y  the y-coordinate for the device cell
    *
    * @see #createDeviceAttributes(int,int,boolean)
    */
   public static Map createDeviceAttributes(int x, int y)
   {
      return createDeviceAttributes(x, y, true);
   }

   /**
    * Centralized creation of an attribute map for graph cells that represent
    * the ConfigElement for a device.
    *
    * @param x          the x-coordinate for the device cell
    * @param y          the y-coordinate for the device cell
    * @param autoSize   enable or disable always-on auto sizing of cells
    *
    * @see org.jgraph.graph.GraphConstants
    */
   public static Map createDeviceAttributes(int x, int y, boolean autoSize)
   {
      AttributeMap map = new AttributeMap();
      createBounds(map, x, y, DEVICE_CELL_COLOR, Color.black);

      GraphConstants.setAutoSize(map, autoSize);
      GraphConstants.setResize(map, true);

      return map;
   }

   /**
    * Centralized creation of an attribute map for graph cells that represent
    * the ConfigElement for a device proxy.  Internally, this makes use of
    * <code>createProxyAttributes(int,int,boolean)</code>.  It is invoked
    * with its <code>autoSize</code> parameter set to true.
    *
    * @param x  the x-coordinate for the proxy cell
    * @param y  the y-coordinate for the proxy cell
    *
    * @see #createProxyAttributes(int,int,boolean)
    */
   public static Map createProxyAttributes(int x, int y)
   {
      return createProxyAttributes(x, y, false);
   }

   /**
    * Centralized creation of an attribute map for graph cells that represent
    * the ConfigElement for a device proxy.
    *
    * @param x          the x-coordinate for the proxy cell
    * @param y          the y-coordinate for the proxy cell
    * @param autoSize   enable or disable always-on auto sizing of cells
    *
    * @see org.jgraph.graph.GraphConstants
    */
   public static Map createProxyAttributes(int x, int y, boolean autoSize)
   {
      AttributeMap map = new AttributeMap();
      createBounds(map, x, y, PROXY_CELL_COLOR, Color.black);

      GraphConstants.setAutoSize(map, autoSize);
      GraphConstants.setResize(map, true);

      return map;
   }

   private static void createBounds(AttributeMap map, int x, int y,
                                    Color bgColor, Color fgColor)
   {
      GraphConstants.setBounds(map, map.createRect(x, y, 90, 30));
      GraphConstants.setBorder(map, BorderFactory.createRaisedBevelBorder());
      GraphConstants.setBackground(map, bgColor);
      GraphConstants.setForeground(map, fgColor);
      GraphConstants.setOpaque(map, true);
   }
}
