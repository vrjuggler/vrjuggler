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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.awt.Dimension;
import java.awt.geom.Rectangle2D;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphCell;
import org.jgraph.graph.GraphConstants;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;

import org.vrjuggler.vrjconfig.commoneditors.DeviceGraph;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A collection of helper functions that can be helpful in extending JGraph.
 */
public abstract class GraphHelpers
   implements EditorConstants
{
   /**
    * Changes the size of the given <code>CellView</code> reference to be the
    * given dimensions.  The graph cell associated with the given view must be
    * in the given graph.  If the cell view is resized successfully, an edit is
    * posted with the graph's layout cache, and the graph is repainted.
    *
    * @param graph      the graph that knows about the cell view and its cell
    * @param size       the new dimensions for the cell view
    * @param view       the cell view to be resized
    *
    * @see #autoSizeCellView(Dimension,CellView,Hashtable)
    */
   public static void autoSizeCellView(JGraph graph, Dimension size,
                                       CellView view)
   {
      Hashtable table = new Hashtable();
      if ( autoSizeCellView(size, view, table) )
      {
         graph.getGraphLayoutCache().edit(table, null, null, null);
         graph.repaint();
      }
   }

   /**
    * Changes the size of the given <code>CellView</code> reference to be the
    * given dimensions.  The resizing is done by modifying the bounds in the
    * cell view's attributes.  The position of the cell view is left unchanged,
    * but the width and height are changed to the values given in
    * <code>size</code>.  The new attributes are placed into the given
    * <code>Hashtable</code> object using the view's cell as the key.
    *
    * @param size       the new dimensions for the cell view
    * @param view       the cell view to be resized
    * @param table      a non-null hashtable used for storing the modified cell
    *                   view attributes
    *
    * @see org.jgraph.graph.GraphConstants#setBounds(Map,Rectangle2D)
    * @see org.jgraph.graph.CellView#getCell()
    */
   public static boolean autoSizeCellView(Dimension size, CellView view,
                                          Hashtable table)
   {
      boolean edited = false;
      Rectangle2D bounds = GraphConstants.getBounds(view.getAllAttributes());

      if ( bounds != null )
      {
         bounds = (Rectangle2D) bounds.clone();
         bounds.setFrame(bounds.getX(), bounds.getY(), size.width, size.height);
         AttributeMap map = new AttributeMap();
         GraphConstants.setBounds(map, bounds);
         table.put(view.getCell(), map);
         edited = true;
      }

      return edited;
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device.  The parameters to use for creating the
    * appropriate <code>DefaultGraphCell</code> (with its ports) are
    * determined based on the type of the config element passed in.  This
    * method is intended to be used for ensuring correct and easy graph cell
    * creation for config elements used to configure devices.  However, the
    * config element type must be known <i>a priori</i> in order for this
    * method to be able to do its job.
    *
    * @param devElt             the config element for the device being
    *                           represented by the vertex cell to be created
    * @param context            the config context where <code>devElt</code>
    *                           exists
    * @param attributes         the attribute map where the vertex cell's
    *                           default attribute map will be stored by this
    *                           function
    *
    * @throws IllegalArgumentException
    *   thrown when the type of <code>devElt</code> is unknown to this method,
    *   thus preventing correct creation of a <code>DefaultGraphCell</code>
    *   instance.
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   Map attributes)
   {
      ConfigDefinition def = devElt.getDefinition();
      DefaultGraphCell cell = null;

      // XXX: Come up with a better system for setting the x and y values.
      int x = 120, y = 20;

      if ( def.getToken().equals(SIM_POS_DEVICE_TYPE) )
      {
         cell = createDeviceCell(devElt, context, 1, attributes, x, y, true);
      }
      else if ( def.getToken().equals(SIM_DIGITAL_DEVICE_TYPE) )
      {
         cell = createDeviceCell(devElt, context, KEY_PAIR_PROPERTY,
                                 attributes, x, y, false);
      }
      else
      {
         throw new IllegalArgumentException("Unexpected definition " +
                                            def.getToken());
      }

      return cell;
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device, which has exactly one input source (unit)
    * at all times.  The numbmer of units indicates how many ports the new
    * graph cell should have initially.  The initial attribute map used for
    * the graph cell is created using
    * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph.createDeviceAttributes()</code>,
    * and it is stored in the given attribute map.
    *
    * @param devElt             the config element for the device being
    *                           represented by the vertex cell to be created
    * @param context            the config context where <code>devElt</code>
    *                           exists
    * @param attributes         the attribute map where the vertex cell's
    *                           default attribute map will be stored by this
    *                           function
    * @param x                  the X coordinate for the initial positionaing
    *                           of the new vertex
    * @param y                  the Y coordinate for the initial positionaing
    *                           of the new vertex
    * @param autoSize           flag indicating whether the new cell should be
    *                           auto-sized
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.DeviceGraph#createDeviceAttributes(int,int,boolean)
    * @see #createDeviceCell(ConfigElement,ConfigContext,int,Map,int,int,boolean)
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   Map attributes, int x,
                                                   int y, boolean autoSize)
   {
      return createDeviceCell(devElt, context, 1, attributes, x, y, autoSize);
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device.  The number of units is determined based on
    * the value of <code>unitPropName</code>.  If it is non-null, then
    * <code>devElt</code> is queried for the number of values of thet named
    * property, indicating that the number of values is variable.  If
    * <code>unitPropName</code> is null, then the number of units is set to 1
    * and is treated as non-variable.  The numbmer of units indicates how many
    * ports the new graph cell should have initially.  The initial attribute
    * map used for the graph cell is created using
    * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph.createDeviceAttributes()</code>,
    * and it is stored in the given attribute map.
    *
    * @param devElt             the config element for the device being
    *                           represented by the vertex cell to be created
    * @param context            the config context where <code>devElt</code>
    *                           exists
    * @param unitPropName       the name of the property that indicates the
    *                           number of available units for the represented
    *                           device
    * @param attributes         the attribute map where the vertex cell's
    *                           default attribute map will be stored by this
    *                           function
    * @param x                  the X coordinate for the initial positionaing
    *                           of the new vertex
    * @param y                  the Y coordinate for the initial positionaing
    *                           of the new vertex
    * @param autoSize           flag indicating whether the new cell should be
    *                           auto-sized
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.DeviceGraph#createDeviceAttributes(int,int,boolean)
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   String unitPropName,
                                                   Map attributes, int x,
                                                   int y, boolean autoSize)
   {
      DefaultGraphCell dev_cell =
         new DefaultGraphCell(new DeviceInfo(devElt, context, unitPropName));

      // If unitPropName is null, that means that the number of units for the
      // device being represented by dev_cell is exactly 1 at all times.
      // Otherwise, we get the current number of units by querying devElt.
      int num_units =
         (unitPropName == null) ? 1
                                : devElt.getPropertyValueCount(unitPropName);

      addDevicePorts(dev_cell, num_units);
      attributes.put(dev_cell, DeviceGraph.createDeviceAttributes(x, y,
                                                                  autoSize));

      return dev_cell;
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device.  The number of units (specified by the
    * <code>numUnits</code> parameter) indicates the fixed number of ports
    * the new graph cell has.  The initial attribute map used for the graph
    * cell is created using
    * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph.createDeviceAttributes()</code>,
    * and it is stored in the given attribute map.
    *
    * @param devElt             the config element for the device being
    *                           represented by the vertex cell to be created
    * @param context            the config context where <code>devElt</code>
    *                           exists
    * @param numUnits           the fixed number of units this device has
    * @param attributes         the attribute map where the vertex cell's
    *                           default attribute map will be stored by this
    *                           function
    * @param x                  the X coordinate for the initial positionaing
    *                           of the new vertex
    * @param y                  the Y coordinate for the initial positionaing
    *                           of the new vertex
    * @param autoSize           flag indicating whether the new cell should be
    *                           auto-sized
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.DeviceGraph#createDeviceAttributes(int,int,boolean)
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   int numUnits,
                                                   Map attributes, int x,
                                                   int y, boolean autoSize)
   {
      // Create a DeviceInfo object with a fixed number of units.
      DefaultGraphCell dev_cell =
         new DefaultGraphCell(new DeviceInfo(devElt, context));
      addDevicePorts(dev_cell, numUnits);
      attributes.put(dev_cell, DeviceGraph.createDeviceAttributes(x, y,
                                                                  autoSize));

      return dev_cell;
   }

   /**
    * Creates a graph cell for a device proxy that has a single port as a
    * child.  The initial attribute map used for the graph cell is created
    * using
    * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph.createProxyAttributes()</code>,
    * and it is stored in the given attribute map.
    *
    * @param proxyElt           the config element for the device proxy being
    *                           represented by the vertex cell to be created
    * @param context            the config context where <code>proxyElt</code>
    *                           exists
    * @param aliases            all the aliases (zero or more) that exist for
    *                           <code>proxyElt</code>
    * @param attributes         the attribute map where the vertex cell's
    *                           default attribute map will be stored by this
    *                           function
    * @param x                  the X coordinate for the initial positionaing
    *                           of the new vertex
    * @param y                  the Y coordinate for the initial positionaing
    *                           of the new vertex
    * @param autoSize           flag indicating whether the new cell should be
    *                           auto-sized
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.DeviceGraph#createProxyAttributes(int,int,boolean)
    */
   public static DefaultGraphCell createProxyCell(ConfigElement proxyElt,
                                                  ConfigContext context,
                                                  List aliases,
                                                  Map attributes, int x,
                                                  int y, boolean autoSize)
   {
      DefaultGraphCell proxy_cell =
         new DefaultGraphCell(new ProxyInfo(proxyElt, context, aliases));
      proxy_cell.add(new DefaultPort());
      attributes.put(proxy_cell,
                     DeviceGraph.createProxyAttributes(x, y, autoSize));

      return proxy_cell;
   }

   /**
    * Creates a connection between <code>proxyCell</code> and
    * <code>deviceCell</code> using the appropriate port in
    * <code>deviceCell</code>.  The port to use is determined by the user
    * object contained in <code>proxyCell</code>, which must be of type
    * <code>ProxyInfo</code>.  The proxy cell's user object will contain the
    * <code>ConfigElement</code> for the proxy, which in turn provides a unit
    * number for the device to which the proxy should refer.
    *
    * @throws NoSuchPortException thrown If the two cells cannot be connected
    *                             because no appropriate port exists in
    *                             <code>deviceCell</code>
    *
    * @see ProxyInfo
    * @see #createProxyCell(ConfigElement,ConfigContext,List,Map,int,int,boolean)
    */
   public static DefaultEdge connectProxyToDevice(DefaultGraphCell proxyCell,
                                                  DefaultGraphCell deviceCell,
                                                  ConnectionSet cs,
                                                  Map attributes)
      throws NoSuchPortException
   {
      ProxyPointerEdge edge = null;
      ProxyInfo proxy_info = (ProxyInfo) proxyCell.getUserObject();
      Object unit_prop = proxy_info.getElement().getProperty(UNIT_PROPERTY, 0);

      // Look for the child of deviceCell that is a DefaultPort whose user
      // object matches the UNIT_PROPERTY value of proxyCell's ConfigElement.
      // If none is found, we cannot make a connection.
      for ( Iterator c = deviceCell.getChildren().iterator(); c.hasNext(); )
      {
         Object child = c.next();

         if ( child instanceof DefaultPort )
         {
            DefaultPort child_port = (DefaultPort) child;

            if ( child_port.getUserObject().equals(unit_prop) )
            {
               DefaultPort proxy_port = (DefaultPort) proxyCell.getFirstChild();
               edge = new ProxyPointerEdge("");
               cs.connect(edge, proxy_port, child_port);
               attributes.put(edge, DeviceGraph.createProxyLineStyle());
               break;
            }
         }
      }

      if ( edge == null )
      {
         throw new NoSuchPortException("Could not find port with unit value " +
                                       unit_prop);
      }

      return edge;
   }

   /**
    * Adds the given number of ports to the given graph cell as children.
    * The ports are all instances of <code>org.jgraph.graph.DefaultPort</code>.
    */
   private static void addDevicePorts(DefaultGraphCell cell, int numUnits)
   {
      for ( int i = 0; i < numUnits; ++i )
      {
         cell.add(new DefaultPort(new Integer(i)));
      }
   }
}
