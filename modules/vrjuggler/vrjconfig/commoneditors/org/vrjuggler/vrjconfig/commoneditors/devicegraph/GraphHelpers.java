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
import java.util.*;

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
import org.vrjuggler.jccl.config.PropertyDefinition;

import org.vrjuggler.vrjconfig.commoneditors.DeviceGraph;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A collection of helper functions that can be helpful in extending JGraph.
 *
 * <p>
 * The basic structure of a device graph is relatively straightfoward.  There
 * are three types of cells: edges, proxy vertices, and device vertices.
 * Edges must always be of type <code>ProxyPointerEdge</code>.  Proxy vertices
 * have the following characteristics:
 *
 * <ul>
 *   <li>
 *     Is of type <code>org.jgraph.graph.DefaultGraphCell</code>
 *   </li>
 *   <li>
 *     Contains a user object of type <code>ProxyInfo</code>
 *   </li>
 *   <li>
 *     Has a single port (type <code>DefaultPort</code>) with at most one
 *     out-going edge
 *   </li>
 * </ul>
 *
 * Device vertices, on the other hand, have these characteristics:
 *
 * <ul>
 *   <li>
 *     Is of type <code>org.jgraph.graph.DefaultGraphCell</code>
 *   </li>
 *   <li>
 *     Contains a user object of type <code>DeviceInfo</code>
 *   </li>
 *   <li>
 *     Has one or more ports (type <code>DefaultPort</code>), each of which
 *     contains a user object of type <code>UnitInfo</code>
 *   </li>
 * </ul>
 *
 * <p>
 * The functions included in this class take care of all the details related
 * to creating and connecting cells.  In particular, there are functions that
 * know about all the default device and device proxy config element types in
 * VR Juggler and know how to create cells and proxies for those types.
 * </p>
 *
 * @see DeviceInfo
 * @see ProxyInfo
 * @see UnitInfo
 * @see ProxyPointerEdge
 * @see org.jgraph.graph.DefaultGraphCell
 * @see org.jgraph.graph.DefaultPort
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

      String token = def.getToken();

      // XXX: This should use functors to allow for more dynamic addition of
      // cell creators.
      if ( token.equals(SIM_POS_DEVICE_TYPE) ||
           token.equals(KEYBOARD_MOUSE_DEVICE_TYPE) ||
           token.equals(TRACKD_CONTROLLER_TYPE) ||
           token.equals(TRACKD_SENSOR_TYPE) ||
           token.equals(TRACKD_API_CONTROLLER_TYPE) ||
           token.equals(TRACKD_API_SENSOR_TYPE) )
      {
         cell = createDeviceCell(devElt, context, 1, attributes, x, y, true);
      }
      else if ( token.equals(SIM_DIGITAL_DEVICE_TYPE) )
      {
         cell = createDeviceCell(devElt, context, KEY_PAIR_PROPERTY,
                                 attributes, x, y, false);
      }
      else if ( token.equals(FLOCK_TYPE) || token.equals(MOTION_STAR_TYPE) )
      {
      }
      else if ( token.equals(INTERSENSE_TYPE) ||
                token.equals(INTERSENSE_API_TYPE) )
      {
         cell = createDeviceCell(devElt, context, STATIONS_PROPERTY,
                                 attributes, x, y, false);
      }
      else if ( token.equals(DIRECTX_JOYSTICK_TYPE) ||
                token.equals(LINUX_JOYDEV_TYPE) )
      {
         cell = createBaseDeviceCell(new DeviceInfo(devElt, context),
                                     attributes, x, y, false);
         addDevicePorts(cell, UnitConstants.ANALOG, 6);
         addDevicePorts(cell, UnitConstants.DIGITAL, 10);
      }
      else if ( token.equals(PUCK_DEVICE_TYPE) )
      {
      }
      else if ( token.equals(IBOX_TYPE) )
      {
         List unit_list = new ArrayList(2);
         unit_list.add(0, UnitConstants.ANALOG);
         unit_list.add(1, UnitConstants.DIGITAL);
         cell = createBaseDeviceCell(new DeviceInfo(devElt, context, unit_list),
                                     attributes, x, y, false);
         addDevicePorts(cell, UnitConstants.ANALOG, 4);
         addDevicePorts(cell, UnitConstants.DIGITAL, 4);
      }
      else if ( token.equals(DATA_GLOVE_TYPE) )
      {
      }
      else if ( token.equals(PINCH_GLOVE_TYPE) )
      {
      }
      else if ( token.equals(VRPN_TYPE) )
      {
         Integer b_count =
            (Integer) devElt.getProperty(BUTTON_COUNT_PROPERTY, 0);
         Integer t_count =
            (Integer) devElt.getProperty(TRACKER_COUNT_PROPERTY, 0);

         PropertyDefinition b_prop_def =
            devElt.getDefinition().getPropertyDefinition(BUTTON_COUNT_PROPERTY);
         PropertyDefinition t_prop_def =
            devElt.getDefinition().getPropertyDefinition(TRACKER_COUNT_PROPERTY);

         Map unit_map = new HashMap();
         unit_map.put(UnitConstants.DIGITAL, b_prop_def);
         unit_map.put(UnitConstants.POSITION, t_prop_def);
         cell = createBaseDeviceCell(new DeviceInfo(devElt, context, unit_map),
                                     attributes, x, y, false);
         addDevicePorts(cell, UnitConstants.DIGITAL, b_count.intValue());
         addDevicePorts(cell, UnitConstants.POSITION, t_count.intValue());
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
         createBaseDeviceCell(new DeviceInfo(devElt, context, unitPropName),
                              attributes, x, y, autoSize);

      // If unitPropName is null, that means that the number of units for the
      // device being represented by dev_cell is exactly 1 at all times.
      // Otherwise, we get the current number of units by querying devElt.
      int num_units =
         (unitPropName == null) ? 1
                                : devElt.getPropertyValueCount(unitPropName);

      addDevicePorts(dev_cell,
                     UnitTypeHelpers.getSingleUnitType(devElt.getDefinition()),
                     num_units);

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
         createBaseDeviceCell(new DeviceInfo(devElt, context), attributes,
                              x, y, autoSize);
      addDevicePorts(dev_cell,
                     UnitTypeHelpers.getSingleUnitType(devElt.getDefinition()),
                     numUnits);

      return dev_cell;
   }

   private static DefaultGraphCell createBaseDeviceCell(DeviceInfo devInfo,
                                                        Map attributes,
                                                        int x, int y,
                                                        boolean autoSize)
   {
      DefaultGraphCell dev_cell = new DefaultGraphCell(devInfo);
      attributes.put(dev_cell, DeviceGraph.createDeviceAttributes(x, y,
                                                                  autoSize));
      return dev_cell;
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure a device proxy.  The parameters to use for creating the
    * appropriate <code>DefaultGraphCell</code> (with its ports) are
    * determined based on the type of the config element passed in.  This
    * method is intended to be used for ensuring correct and easy graph cell
    * creation for config elements used to configure proxies.  However, the
    * config element type must be known <i>a priori</i> in order for this
    * method to be able to do its job.
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
    *
    * @throws IllegalArgumentException
    *   thrown when the type of <code>proxyElt</code> is unknown to this
    *   method, thus preventing correct creation of a
    *   <code>DefaultGraphCell</code> instance.
    */
   public static DefaultGraphCell createProxyCell(ConfigElement proxyElt,
                                                  ConfigContext context,
                                                  List aliases,
                                                  Map attributes)
   {
      ConfigDefinition def = proxyElt.getDefinition();
      DefaultGraphCell cell = null;

      // XXX: Come up with a better system for setting the x and y values.
      int x = 120, y = 20;

      if ( def.isOfType(PROXY_TYPE) )
      {
         cell = createProxyCell(proxyElt, context, aliases, attributes, x, y,
                                false);
      }
      else
      {
         throw new IllegalArgumentException("Unexpected definition " +
                                            def.getToken());
      }

      return cell;
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

      ConfigElement proxy_elt = proxy_info.getElement();

      PropertyDefinition dev_prop_def =
         proxy_elt.getDefinition().getPropertyDefinition(DEVICE_PROPERTY);
      Integer unit_type =
         UnitTypeHelpers.getUnitType(dev_prop_def.getAllowedType(0));
      Object unit_prop = null;

      try
      {
         unit_prop = proxy_elt.getProperty(UNIT_PROPERTY, 0);
      }
      // Thrown if proxy_info's config element does not have a property
      // EditorConstants.UNIT_PROPERTY.
      catch (IllegalArgumentException ex)
      {
         unit_prop = new Integer(0);
      }

      // Look for the child of deviceCell that is a DefaultPort whose user
      // object matches the UNIT_PROPERTY value of proxyCell's
      // ConfigElement.   If none is found, we cannot make a connection.
      for ( Iterator c = deviceCell.getChildren().iterator(); c.hasNext(); )
      {
         Object child = c.next();

         if ( child instanceof DefaultPort )
         {
            DefaultPort child_port = (DefaultPort) child;
            UnitInfo unit_info = (UnitInfo) child_port.getUserObject();

            if ( unit_info.getUnitType().equals(unit_type) &&
                 unit_info.getUnitNumber().equals(unit_prop) )
            {
               edge = connectPorts((DefaultPort) proxyCell.getFirstChild(),
                                   child_port, cs, attributes);
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
    * Creates a new port for a device cell.  The port will be used to
    * represent the input source (unit) described by the given parameters.
    *
    * @param unitType   the type of input source for the new port
    * @param unitNumber the index of this input source in the device
    */
   public static DefaultPort createDevicePort(Integer unitType, int unitNumber)
   {
      return createDevicePort(new UnitInfo(unitType, new Integer(unitNumber)));
   }

   /**
    * Creates a new port for a device cell.  The port will be used to
    * represent the input source (unit) described by the given parameters.
    *
    * @param unitType   the type of input source for the new port
    * @param unitNumber the index of this input source in the device
    */
   public static DefaultPort createDevicePort(Integer unitType,
                                              Integer unitNumber)
   {
      return createDevicePort(new UnitInfo(unitType, unitNumber));
   }

   /**
    * Creates a new port for a device cell.  The port will be used to
    * represent the input source (unit) described by the given parameter.
    *
    * @param unitInfo   a <code>UnitInfo</code> object containing the
    *                   information necessary to create a new port for the
    *                   device cell that accurately represents the input
    *                   source
    */
   public static DefaultPort createDevicePort(UnitInfo unitInfo)
   {
      return new DefaultPort(unitInfo);
   }

   /**
    * Adds the given number of ports to the given graph cell as children.
    * The ports are all instances of <code>org.jgraph.graph.DefaultPort</code>.
    */
   private static void addDevicePorts(DefaultGraphCell cell, Integer unitType,
                                      int numUnits)
   {
      for ( int i = 0; i < numUnits; ++i )
      {
         cell.add(createDevicePort(unitType, i));
      }
   }

   private static ProxyPointerEdge connectPorts(DefaultPort proxyPort,
                                                DefaultPort deviceUnitPort,
                                                ConnectionSet cs,
                                                Map attributes)
   {
      ProxyPointerEdge edge = new ProxyPointerEdge("");
      cs.connect(edge, proxyPort, deviceUnitPort);
      attributes.put(edge, DeviceGraph.createProxyLineStyle());

      return edge;
   }
}
