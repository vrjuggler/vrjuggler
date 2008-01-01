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

import org.vrjuggler.jccl.config.*;

import org.vrjuggler.vrjconfig.commoneditors.DeviceGraph;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A collection of helper functions that can be helpful in extending JGraph.
 *
 * <p>
 * The basic structure of a device graph is relatively straightfoward.  There
 * are three types of cells: edges, proxy vertices, and device vertices.
 * Edges must always be of type <code>ProxyToDeviceEdge</code>.  Proxy vertices
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
 * @see ProxyToDeviceEdge
 * @see org.jgraph.graph.DefaultGraphCell
 * @see org.jgraph.graph.DefaultPort
 */
public abstract class GraphHelpers
   implements EditorConstants
{
   private static Map mDevCellCreatorMap = new HashMap();

   /**
    * Registers the given device graph cell creator as the creator for the
    * given config definition.  Subsequent calls to 
    * <code>createDeviceCell(ConfigElement,ConfigContext,Map)</code> will be
    * able to create device graph cells for the given config definition.
    *
    * @param def        the config definition that uniquely identifies which
    *                   creator to be used
    * @param c          the device graph cell creator
    *
    * @see #createDeviceCell(ConfigElement,ConfigContext,Map)
    */
   public static void registerGraphCellCreator(ConfigDefinition def,
                                               DeviceGraphCellCreator c)
   {
      mDevCellCreatorMap.put(def, c);
   }

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
    * configure an input device.  Pluggable "creators" are used to construct
    * the appropriate <code>DefaultGraphCell</code> instance (with its ports)
    * that is returned.  The creator to use is determined using the config
    * definition of the given config element.  This method is intended to be
    * used for ensuring correct and easy graph cell creation for config
    * elements used to configure devices.  Creators must be registered prior
    * to calling this function.
    *
    * @param devElt     the config element for the device being represented by
    *                   the vertex cell to be created
    * @param context    the config context where <code>devElt</code> exists
    * @param attributes the attribute map where the vertex cell's default
    *                   attribute map will be stored by this function
    *
    * @throws IllegalArgumentException
    *   thrown when the graph cell cannot be created due to one of two
    *   causes: there is no creator known for the config definition of
    *   <code>devElt</code> or the registered creator failed
    *
    * @see #registerGraphCellCreator(ConfigDefinition,DeviceGraphCellCreator)
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   Map attributes)
   {
      DefaultGraphCell cell = null;

      ConfigDefinition def = devElt.getDefinition();
      DeviceGraphCellCreator creator =
         (DeviceGraphCellCreator) mDevCellCreatorMap.get(def);

      if ( creator != null )
      {
         cell = creator.createDeviceGraphCell(devElt, context, attributes);
      }

      // If creator is null, cell will be null.  If the creator failed, cell
      // will be null.
      if ( cell == null )
      {
         throw new IllegalArgumentException("Unexpected definition " +
                                            def.getToken());
      }

      return cell;
   }

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device, which has exactly one input source (unit)
    * at all times.  The number of units indicates how many ports the new
    * graph cell should have initially.
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
    * @see #createDeviceCell(ConfigElement,ConfigContext,int,Map,int,int,boolean)
    * @see #createBaseDeviceCell(BaseDeviceInfo,Map,int,int,boolean)
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
    * <code>devElt</code> is queried for the number of values of the named
    * property.  Further, a non-null value for <code>unitPropName</code>
    * indicates that the number of values is variable.  If
    * <code>unitPropName</code> is null, then the number of units is set to 1
    * and is treated as non-variable.  The number of units indicates how many
    * ports the new graph cell should have initially.
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
    * @see #createBaseDeviceCell(BaseDeviceInfo,Map,int,int,boolean)
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
    * the new graph cell has.
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
    * @see #createBaseDeviceCell(BaseDeviceInfo,Map,int,int,boolean)
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

   /**
    * Creates a new graph cell for the given config element that is used to
    * configure an input device using "artificial" input sources (units).
    *
    * @param devElt     the config element for the device being
    *                   represented by the vertex cell to be created
    * @param context    the config context where <code>devElt</code> exists
    * @param proxyTypes the list of proxy types that can point at the device
    * @param attributes the attribute map where the vertex cell's default
    *                   attribute map will be stored by this function
    * @param x          the X coordinate for the initial positionaing of the
    *                   new vertex
    * @param y          the Y coordinate for the initial positionaing of the
    *                   new vertex
    * @param autoSize   flag indicating whether the new cell should be
    *                   auto-sized
    *
    * @see #createBaseDeviceCell(BaseDeviceInfo,Map,int,int,boolean)
    */
   public static DefaultGraphCell createDeviceCell(ConfigElement devElt,
                                                   ConfigContext context,
                                                   List proxyTypes,
                                                   Map attributes,
                                                   int x, int y,
                                                   boolean autoSize)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      List all_elts = broker.getElements(context);

      Map unit_map = new HashMap();

      for ( Iterator d = proxyTypes.iterator(); d.hasNext(); )
      {
         Object def_obj = d.next();
         List all_proxy_elts = null;
         if ( def_obj instanceof String )
         {
            all_proxy_elts = 
               ConfigUtilities.getElementsWithDefinition(all_elts,
                                                         (String) def_obj);
         }
         else if ( def_obj instanceof ConfigDefinition )
         {
            all_proxy_elts = 
               ConfigUtilities.getElementsWithDefinition(all_elts,
                                                         (ConfigDefinition) def_obj);
         }

         if ( ! all_proxy_elts.isEmpty() )
         {
            ConfigDefinition def = 
               ((ConfigElement) all_proxy_elts.get(0)).getDefinition();
            PropertyDefinition prop_def =
               def.getPropertyDefinition(DEVICE_PROPERTY);
            Integer unit_type =
               UnitTypeHelpers.getUnitType(prop_def.getAllowedType(0));
            int proxy_count = 0;

            for ( Iterator e = all_proxy_elts.iterator(); e.hasNext(); )
            {
               ConfigElement proxy_elt = (ConfigElement) e.next();
               ConfigElementPointer dev_ptr =
                  (ConfigElementPointer) proxy_elt.getProperty(DEVICE_PROPERTY,
                                                               0);

               if ( dev_ptr != null &&
                    dev_ptr.getTarget().equals(devElt.getName()) )
               {
                  proxy_count++;
               }
            }

            DefaultUnitPropertyHandler.addArtificialUnits(unit_map, unit_type,
                                                          proxy_count);
         }
      }

      DefaultGraphCell cell =
         createBaseDeviceCell(new DeviceInfo(devElt, context, unit_map),
                              attributes, x, y, false);

      for ( Iterator t = unit_map.keySet().iterator(); t.hasNext(); )
      {
         Integer type = (Integer) t.next();
         Integer proxy_count = (Integer) unit_map.get(type);
         addDevicePorts(cell, type, proxy_count.intValue());
      }

      return cell;
   }

   /**
    * Creates a basic device graph cell by creating an attribute map using
    * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph.createDeviceAttributes()</code>
    * and the given parameters.  The freshly created attribute map is stored
    * in the given attribute map using the new <code>DefaultGraphCell</code>
    * instance as its key.  In general, user code should not be using this
    * method directly, but it is made public so that instances of
    * <code>DeviceGraphCellCreator</code> can utilize it when necessary.
    *
    * @param devInfo    the informational object describing the device
    * @param attributes the attribute map where the vertex cell's default
    *                   attribute map will be stored by this function
    * @param x          the X coordinate for the initial positionaing of the
    *                   new vertex
    * @param y          the Y coordinate for the initial positionaing of the
    *                   new vertex
    * @param autoSize   flag indicating whether the new cell should be
    *                   auto-sized
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.DeviceGraph#createDeviceAttributes(int,int,boolean)
    * @see DeviceGraphCellCreator
    * @see org.jgraph.graph.GraphConstants
    * @see org.jgraph.graph.AttributeMap
    */
   public static DefaultGraphCell createBaseDeviceCell(BaseDeviceInfo devInfo,
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
    * Adds the given number of ports to the given graph cell as children.
    * The ports are all created using
    * <code>createDevicePort(Integer,int)</code>.
    *
    * @param cell       the device graph cell to which the ports will be
    *                   added
    * @param unitType   the type of the input source (unit) for the new
    *                   ports
    * @param numUnits   the number of ports that will be addded
    *
    * @see #createDevicePort(Integer,int)
    */
   public static void addDevicePorts(DefaultGraphCell cell, Integer unitType,
                                     int numUnits)
   {
      for ( int i = 0; i < numUnits; ++i )
      {
         cell.add(createDevicePort(unitType, i));
      }
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
    * number for the device to which the proxy should refer.  The proxy config
    * element and the device config element will remain unchanged.
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
      ProxyToDeviceEdge edge = null;
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
         try
         {
            DefaultPort child_port = (DefaultPort) c.next();
            UnitInfo unit_info = (UnitInfo) child_port.getUserObject();

            if ( unit_info.getUnitType().equals(unit_type) &&
                 unit_info.getUnitNumber().equals(unit_prop) )
            {
               edge = connectPorts((DefaultPort) proxyCell.getFirstChild(),
                                   child_port, cs, attributes);
               break;
            }
         }
         catch (ClassCastException ex)
         {
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
    * Creates a connection between <code>proxyPort</code> and
    * <code>devicePort</code>.  The parent of the proxy port must contain a
    * <code>ProxyInfo</code> object, and the parent of the device port must
    * contain a <code>DeviceInfo</code> object.  The config element in the
    * <code>ProxyInfo</code> object is changed to reflect the connection.
    *
    * @throws IllegalArgumentException
    *   thrown If the two ports cannot be connected as a result of a unit
    *   type mismatch between the proxy's allowed type and the device port's
    *   unit type
    *
    * @see #createProxyCell(ConfigElement,ConfigContext,List,Map,int,int,boolean)
    */
   public static DefaultEdge connectProxyToDevice(DefaultPort proxyPort,
                                                  DefaultPort devicePort,
                                                  ConnectionSet cs,
                                                  Map attributes)
      throws IllegalArgumentException
   {
      ProxyToDeviceEdge edge = null;
      ProxyInfo proxy_info =
         (ProxyInfo) ((DefaultGraphCell) proxyPort.getParent()).getUserObject();

      ConfigElement proxy_elt = proxy_info.getElement();

      UnitInfo unit_info = (UnitInfo) devicePort.getUserObject();

      PropertyDefinition device_prop_def =
         proxy_elt.getDefinition().getPropertyDefinition(DEVICE_PROPERTY);

      Integer allowed_unit_type =
         UnitTypeHelpers.getUnitType(device_prop_def.getAllowedType(0));
      if ( ! unit_info.getUnitType().equals(allowed_unit_type) )
      {
         throw new IllegalArgumentException(
            "Unit type mismatch: proxy's allowed type (" +
            UnitTypeHelpers.getUnitTypeName(allowed_unit_type) +
            ") does not match device unit " + unit_info
         );
      }

      edge = connectPorts(proxyPort, devicePort, cs, attributes);

      try
      {
         DeviceInfo dev_info =
            (DeviceInfo) ((DefaultGraphCell) devicePort.getParent()).getUserObject();
         proxy_elt.setProperty(DEVICE_PROPERTY, 0,
                               dev_info.getElement().getName(),
                               proxy_info.getContext());
         proxy_elt.setProperty(UNIT_PROPERTY, 0, unit_info.getUnitNumber(),
                               proxy_info.getContext());
      }
      // Thrown if proxy_info's config element does not have a property
      // EditorConstants.UNIT_PROPERTY.
      catch (IllegalArgumentException ex)
      {
         System.err.println("WARNING: " + ex.getMessage());
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

   private static ProxyToDeviceEdge connectPorts(DefaultPort proxyPort,
                                                 DefaultPort deviceUnitPort,
                                                 ConnectionSet cs,
                                                 Map attributes)
   {
      ProxyToDeviceEdge edge = new ProxyToDeviceEdge("");
      cs.connect(edge, proxyPort, deviceUnitPort);
      attributes.put(edge, DeviceGraph.createProxyLineStyle());

      return edge;
   }

   /**
    * Determines whether the given proxy and device can be connected through
    * the given device unit.
    */
   public static boolean checkProxyDeviceConnection(ProxyInfo proxyInfo,
                                                    BaseDeviceInfo deviceInfo,
                                                    UnitInfo unitInfo)
   {
      boolean valid = false;

      ConfigDefinition dev_def   = deviceInfo.getElement().getDefinition();
      ConfigDefinition proxy_def = proxyInfo.getElement().getDefinition();

      PropertyDefinition dev_prop_def =
         proxy_def.getPropertyDefinition(DEVICE_PROPERTY);

      List allowed_types = dev_prop_def.getAllowedTypes();
      for ( Iterator i = allowed_types.iterator(); i.hasNext(); )
      {
         String type = (String) i.next();

         // If the device definition is of a type that the proxy is allowed to
         // point at, then we need to check the specific unit and make sure
         // that its type matches the proxy's type.
         if ( dev_def.isOfType(type) )
         {
            Integer unit_type = UnitTypeHelpers.getUnitType(type);

            if ( unitInfo.getUnitType().equals(unit_type) )
            {
               valid = true;
               break;
            }
         }
      }

      return valid;
   }
}
