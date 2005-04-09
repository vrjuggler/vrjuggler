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

import java.awt.BorderLayout;
import java.awt.Window;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.swing.*;

import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.layout.JGraphLayoutAlgorithm;
import org.jgraph.layout.SpringEmbeddedLayoutAlgorithm;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigEvent;
import org.vrjuggler.jccl.config.event.ConfigListener;

import org.vrjuggler.vrjconfig.commoneditors.devicegraph.*;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras.RelativeDeviceInfo;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras.RelativeDeviceToProxyEdge;


/**
 * This editor handles all the details of setting up a
 * <code>org.vrjuggler.vrjconfig.commoneditors.DeviceGraph</code> that
 * instance based on a given configuration.  The graph is populated by
 * invoking the <code>setConfig()</code> method <i>exactly once</i>.
 *
 * <p>
 * This editor can create a graph that allows the relationships between all
 * proxies, aliases, and devices to be edited, or it can create a graph with
 * a specific subset of all possible proxies and devices.  This editor has no
 * user interface of its own.  The contained DeviceGraph instance provdies
 * the interface for editing the device/proxy/alias relationships, but no
 * additional buttons and such are provided.  It is expected that higher
 * level editors would provide that part of the user interface.
 * </p>
 *
 * @see #setConfig(ConfigContext,ConfigElement)
 */
public class DeviceProxyGraphEditor
   extends JPanel
   implements EditorConstants
            , ConfigListener
{
   /**
    * Creates a new instance of the device/proxy graph editor that allows
    * editing of all known types of device and proxy config elements.
    */
   public DeviceProxyGraphEditor()
   {
      this(null);
   }

   /**
    * Creates a new instance of the device/proxy graph editor that allows
    * editing of only the given config element types.  If
    * <code>allowedTypes</code> is null, then the editor will allow editing
    * of all known types of device and proxy config elements.
    *
    * @param allowedTypes       a list of <code>ConfigDefinition</code>
    *                           objects that identifies which types of device
    *                           and proxy config elements will be edited by
    *                           this editor instance or null to indicate that
    *                           all device and proxy elements will be edited
    */
   public DeviceProxyGraphEditor(List allowedTypes)
   {
      mBroker = new ConfigBrokerProxy();

      if ( allowedTypes == null )
      {
         List base_types = new ArrayList(2);
         base_types.add(0, PROXY_TYPE);
         base_types.add(1, INPUT_DEVICE_TYPE);
         ConfigDefinitionRepository repos = mBroker.getRepository();
         List all_defs = repos.getAllLatest();

         for ( Iterator d = all_defs.iterator(); d.hasNext(); )
         {
            ConfigDefinition def = (ConfigDefinition) d.next();
            if ( ! def.isAbstract() )
            {
               for ( Iterator t = base_types.iterator(); t.hasNext(); )
               {
                  if ( def.isOfType((String) t.next()) )
                  {
                     this.allowedTypes.add(def);
                     break;
                  }
               }
            }
         }
      }
      else
      {
         this.allowedTypes.addAll(allowedTypes);
      }

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   /**
    * Handles the addition of a new config element by checking to see if it
    * is among the allowed types for this editor instance.  If the new element
    * is of an allowed type, it is added to the graph.
    */
   public void configElementAdded(ConfigEvent e)
   {
      ConfigElement added_elt = e.getElement();
      ConfigDefinition def = added_elt.getDefinition();

      if ( this.allowedTypes.contains(def) )
      {
         List device_elts = new ArrayList(1), proxy_elts = new ArrayList(1);

         if ( def.isOfType(PROXY_TYPE) )
         {
            device_elts.add(0, added_elt);
         }
         else
         {
            proxy_elts.add(0, added_elt);
         }

         populateGraph(device_elts, proxy_elts, mBroker.getElements(mContext),
                       false);
      }
   }

   /** Does nothing. */
   public void configElementRemoved(ConfigEvent e)
   {
//      ConfigElement removed_elt = e.getElement();
//      ConfigDefinition def = removed_elt.getDefinition();
   }

   /**
    * Initializes the graph will the given config element (either a device
    * or a proxy).  If the config element is for a device proxy, all the
    * devices at which it is allowed to point will be included with the
    * graph.  If the config element is for an input device, all the proxies
    * pointing at it (of any type) will be included in the graph.  If the
    * given config element is null, then all allowed types of device and proxy
    * config elements will be used to populate the graph.
    *
    * <p>
    * <b>NOTE:</b>
    * <blockquote>
    * This method may be invoked <i>at most</i> once per object lifetime.
    * </blockquote>
    * </p>
    *
    * @param ctx        the context containing the given config element and
    *                   any devices to which that config element may refer
    * @param elt        the config element for an input device (its definition
    *                   is of type
    *                   <code>EditorConstants.INPUT_DEVICE_TYPE</code>), for
    *                   a device proxy (its definition is of type
    *                   <code>EditorConstants.PROXY_TYPE</code>), or null
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.EditorConstants
    */
   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mContext = ctx;
      mBroker.addConfigListener(this);

      List device_elts = new ArrayList(), proxy_elts = new ArrayList();
      List all_elts = mBroker.getElements(mContext);

      // A null element means that we must pull all allowed types of device
      // and proxy config elements from the given context.
      if ( elt == null )
      {
         for ( Iterator d = this.allowedTypes.iterator(); d.hasNext(); )
         {
            ConfigDefinition cur_def = (ConfigDefinition) d.next();

            List cur_elts = ConfigUtilities.getElementsWithDefinition(all_elts,
                                                                      cur_def);

            if ( cur_def.isOfType(INPUT_DEVICE_TYPE) )
            {
               device_elts.addAll(cur_elts);
            }
            else if ( cur_def.isOfType(PROXY_TYPE) )
            {
               proxy_elts.addAll(cur_elts);
            }
         }
      }
      // A config element for an input device means that we need to use the
      // device as the "root" of the graph and find all its proxies.
      else if ( elt.getDefinition().isOfType(INPUT_DEVICE_TYPE) )
      {
         if ( ! this.allowedTypes.contains(elt.getDefinition()) )
         {
            this.allowedTypes.add(elt.getDefinition());
         }

         device_elts.add(elt);

         DeviceConfig dev_cfg = new DeviceConfig(ctx, elt);
         proxy_elts.addAll(dev_cfg.getProxies());
      }
      // A config element for a device proxy means that we need to use the
      // proxy as the "root" of the graph and find all the devices at which
      // it is allowed to point.
      else if ( elt.getDefinition().isOfType(PROXY_TYPE) )
      {
         if ( ! this.allowedTypes.contains(elt.getDefinition()) )
         {
            this.allowedTypes.add(elt.getDefinition());
         }

         // For this proxy, we need to find all the device config elements at
         // which it is allowed to point.
         PropertyDefinition device_prop_def =
            elt.getDefinition().getPropertyDefinition(DEVICE_PROPERTY);

         List allowed_dev_types = device_prop_def.getAllowedTypes();
         List all_defs = mBroker.getRepository().getAllLatest();

         // Find all the non-astract config defiitions that are allowed types
         // for the proxy we were given.  Use those definitions to find all
         // the device config elements in the given context.
         for ( Iterator t = allowed_dev_types.iterator(); t.hasNext(); )
         {
            List defs = 
               ConfigUtilities.getDefinitionsOfType(all_defs,
                                                    (String) t.next());

            for ( Iterator d = defs.iterator(); d.hasNext(); )
            {
               ConfigDefinition cur_def = (ConfigDefinition) d.next();
               if ( ! cur_def.isAbstract() )
               {
                  this.allowedTypes.add(cur_def);
                  device_elts.addAll(
                     ConfigUtilities.getElementsWithDefinition(all_elts,
                                                               cur_def)
                  );
               }
            }
         }

         proxy_elts.add(0, elt);
      }
      else
      {
         throw new IllegalArgumentException(
            "elt is not a device proxy type or an input device type: " +
            elt.getDefinition().getToken()
         );
      }

      populateGraph(proxy_elts, device_elts, all_elts, true);
   }

   public void editorClosing()
   {
      mBroker.removeConfigListener(this);
      this.graph = null;
   }

   public DeviceGraph getGraph()
   {
      return this.graph;
   }

   public List getAllowedTypes()
   {
      return this.allowedTypes;
   }

   /**
    * Changes the graph layout algorithm to be used by this editor instance
    * to the given reference (which may be null).  If the given algorithm
    * reference is null, then subsequent invocations of
    * <code>applyGraphLayoutAlgorithm()</code> will have no effect.
    *
    * @param algorithm  the new layout algorithm to use
    */
   public void setGraphLayoutAlgorithm(JGraphLayoutAlgorithm algorithm)
   {
      this.graphLayoutAlgorithm = algorithm;
   }

   /**
    * Returns the current graph layout algorithm to be used by this editor
    * instance.  The returned reference may be null.
    */
   public JGraphLayoutAlgorithm getGraphLayoutAlgorithm()
   {
      return this.graphLayoutAlgorithm;
   }

   /**
    * Applies the current graph layout algorithm to all the roots of the
    * graph.
    *
    * @see #applyGraphLayoutAlgorithm(Object[])
    */
   public void applyGraphLayoutAlgorithm()
   {
      applyGraphLayoutAlgorithm(
         DefaultGraphModel.getRoots(this.graph.getModel())
      );
   }

   /**
    * Applies the current graph layout algorithm to the given array of roots.
    * If the current graph layout algorithm is null, this method does nothing.
    * The given graph cells must be roots of the graph being used by this
    * editor instance.  Furthermore, the given graph cells are passed off to
    * the graph layout algorithm as dynamic cells.
    *
    * @param cells      the collection of roots in the graph that will be
    *                   laid out using the current layout algorithm
    *
    * @see #setGraphLayoutAlgorithm(JGraphLayoutAlgorithm)
    */
   public void applyGraphLayoutAlgorithm(Object[] cells)
   {
      if ( this.graphLayoutAlgorithm != null )
      {
         JGraphLayoutAlgorithm.applyLayout(this.graph,
                                           this.graphLayoutAlgorithm, cells);
      }
   }

   /**
    * Determines if the two given cells can be connected.
    */
   public boolean canConnect(Object cell0, Object cell1)
   {
      boolean result = false;

      if ( cell0 instanceof DefaultGraphCell &&
           cell1 instanceof DefaultGraphCell )
      {
         Object cell0_obj = ((DefaultGraphCell) cell0).getUserObject();
         Object cell1_obj = ((DefaultGraphCell) cell1).getUserObject();

         result =
            (cell0_obj instanceof DeviceInfo && cell1_obj instanceof ProxyInfo ||
             cell0_obj instanceof ProxyInfo && cell1_obj instanceof DeviceInfo);
      }

      return result;
   }

   /**
    * Determines if the two given cells are connected.
    *
    * @see org.jgraph.graph.DefaultGraphModel#containsEdgeBetween(org.jgraph.graph.GraphModel,Object,Object)
    */
   public boolean isConnected(Object cell0, Object cell1)
   {
      return DefaultGraphModel.containsEdgeBetween(this.graph.getModel(),
                                                   cell0, cell1);
   }

   /**
    * Connects the two given cells if they can be connected.  For the
    * connection to succeed, both cells must be instances of
    * <code>org.jgraph.graph.DefaultGraphCell</code>, and one must be a graph
    * cell for a proxy and the other a graph cell for a device.  The
    * determination about which cell is which is made by this method.
    *
    * @param cell0      the first cell
    * @param cell1      the second cell
    *
    * @return true is returned if the connection succeeded; false otherwise
    *
    * @see GraphHelpers#connectProxyToDevice(org.jgraph.graph.DefaultGraphCell,org.jgraph.graph.DefaultGraphCell,org.jgraph.graph.ConnectionSet,java.util.Map)
    */
   public boolean connect(Object cell0, Object cell1)
   {
      boolean result = false;
      ConnectionSet cs = new ConnectionSet();

      try
      {
         DefaultGraphCell proxy_cell = null, device_cell = null;

         if ( ((DefaultGraphCell) cell0).getUserObject() instanceof ProxyInfo )
         {
            proxy_cell  = (DefaultGraphCell) cell0;
            device_cell = (DefaultGraphCell) cell1;
         }
         else
         {
            device_cell = (DefaultGraphCell) cell0;
            proxy_cell  = (DefaultGraphCell) cell1;
         }

         try
         {
            Map attributes = new HashMap();
            Object[] cells = new Object[1];
            cells[0] = GraphHelpers.connectProxyToDevice(proxy_cell,
                                                         device_cell, cs,
                                                         attributes);
            this.graph.getModel().insert(cells, attributes, cs, null, null);
         }
         catch (NoSuchPortException ex)
         {
            System.err.println("ERROR: " + ex.getMessage());
         }
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      return result;
   }

   /**
    * Disconnects the two given cells.
    *
    * @param cell0      the first cell
    * @param cell1      the second cell
    *
    * @return true is returned if the disconnection succeeded; false otherwise
    */
   public boolean disconnect(Object cell0, Object cell1)
   {
      boolean result = false;
      Object[] edges = DefaultGraphModel.getEdgesBetween(this.graph.getModel(),
                                                         cell0, cell1, true);

      if ( edges.length == 1 )
      {
         this.graph.getModel().remove(edges);
         result = true;
      }
      else
      {
         System.err.println("WARNING: Got unexpected number of edges (" +
                            edges.length + ") when trying to disconnect " +
                            cell0 + " and " + cell1);
      }

      return result;
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      this.add(this.graph, BorderLayout.CENTER);
   }

   private void populateGraph(List proxyElts, List deviceElts, List allElts,
                              boolean applyLayout)
   {
      List cells = new ArrayList();
      ConnectionSet cs = new ConnectionSet();
      Map attributes = new HashMap();

      Map device_elt_map = new HashMap();
      Map proxy_elt_map  = new HashMap();

      // Handle all the input devices first.
      for ( Iterator e = deviceElts.iterator(); e.hasNext(); )
      {
         ConfigElement elt = (ConfigElement) e.next();

         try
         {
            DefaultGraphCell cell =
               GraphHelpers.createDeviceCell(elt, mContext, attributes);
            cells.add(cell);
            device_elt_map.put(elt.getName(), cell);
         }
         catch (IllegalArgumentException ex)
         {
            Window parent =
               (Window) SwingUtilities.getAncestorOfClass(Window.class, this);
            JOptionPane.showMessageDialog(
               parent,
               "Could not create a vertex for config element '" +
                  elt.getName() + "'\nReason:" + ex.getMessage(),
               "Device Vertex Creation Failed", JOptionPane.WARNING_MESSAGE
            );
         }
      }

      for ( Iterator e = proxyElts.iterator(); e.hasNext(); )
      {
         ConfigElement elt = (ConfigElement) e.next();

         // To create the graph cell for this proxy, we have to find all its
         // aliases.
         List aliases = new ArrayList();
         List alias_elts =
            ConfigUtilities.getElementsWithDefinition(allElts, ALIAS_TYPE);

         // Search alias_elts for those that refer to elt.
         for ( Iterator a = alias_elts.iterator(); a.hasNext(); )
         {
            ConfigElement alias_elt = (ConfigElement) a.next();
            ConfigElementPointer proxy_ptr =
               (ConfigElementPointer) alias_elt.getProperty(PROXY_PROPERTY, 0);

            if ( proxy_ptr != null && proxy_ptr.getTarget() != null &&
                 proxy_ptr.getTarget().equals(elt.getName()) )
            {
               aliases.add(alias_elt);
            }
         }

         // Create the graph cell for this proxy.
         DefaultGraphCell proxy_cell =
            GraphHelpers.createProxyCell(elt, mContext, aliases, attributes);
         cells.add(proxy_cell);

         // Needed for devices that are configured relative to a proxy.
         proxy_elt_map.put(elt.getName(), proxy_cell);

         // Look up the cell for the device pointed to by this proxy so that
         // we can create the connection between the new proxy cell and the
         // device cell.
         ConfigElementPointer dev_ptr =
            (ConfigElementPointer) elt.getProperty(DEVICE_PROPERTY, 0);

         if ( dev_ptr != null && dev_ptr.getTarget() != null &&
              ! dev_ptr.getTarget().equals("") )
         {
            DefaultGraphCell dev_cell =
               (DefaultGraphCell) device_elt_map.get(dev_ptr.getTarget());

            // If there is no cell for the device pointed at by this proxy,
            // we cannot make a connection.  This indicates that the user has
            // an incomplete configuration loaded.
            if ( dev_cell != null )
            {
               try
               {
                  cells.add(GraphHelpers.connectProxyToDevice(proxy_cell,
                                                              dev_cell, cs,
                                                              attributes));
               }
               catch (NoSuchPortException ex)
               {
                  System.err.println("WARNING: Failed to connect proxy " +
                                     "to device!\n\t" + ex.getMessage());
               }
            }
         }
      }

      // Handle relative devices last.
      for ( Iterator e = deviceElts.iterator(); e.hasNext(); )
      {
         ConfigElement elt = (ConfigElement) e.next();

         // XXX: Special case code that would be much better off somewhere
         // else.  Unfortunately, I am too burned out on this graph stuff to
         // care enough.  -PH 4/9/2005
         if ( elt.getDefinition().getToken().equals(SIM_RELATIVE_POS_DEVICE_TYPE) )
         {
            ConfigElementPointer base_proxy_ptr =
               (ConfigElementPointer) elt.getProperty(BASE_FRAME_PROXY_PROPERTY,
                                                      0);
            String base_proxy_name = base_proxy_ptr.getTarget();

            ConfigElementPointer relative_proxy_ptr =
               (ConfigElementPointer) elt.getProperty(RELATIVE_PROXY_PROPERTY,
                                                      0);
            String relative_proxy_name = relative_proxy_ptr.getTarget();

            DefaultGraphCell base_proxy_cell =
               (DefaultGraphCell) proxy_elt_map.get(base_proxy_name);
            DefaultGraphCell relative_proxy_cell =
               (DefaultGraphCell) proxy_elt_map.get(relative_proxy_name);

            DefaultGraphCell dev_cell =
               (DefaultGraphCell) device_elt_map.get(elt.getName());

            for ( Iterator c = dev_cell.getChildren().iterator();
                  c.hasNext(); )
            {
               Object child = c.next();

               if ( child instanceof DefaultPort )
               {
                  Object user_obj = ((DefaultPort) child).getUserObject();

                  if ( user_obj instanceof ProxyPointerInfo )
                  {
                     ProxyPointerInfo ptr_info = (ProxyPointerInfo) user_obj;
                     RelativeDeviceToProxyEdge edge =
                        new RelativeDeviceToProxyEdge();
                     String prop_name =
                        ptr_info.getPointerPropertyDefinition().getToken();

                     if ( prop_name.equals(BASE_FRAME_PROXY_PROPERTY) )
                     {
                        cs.connect(edge, child,
                                   base_proxy_cell.getFirstChild());
                     }
                     else
                     {
                        cs.connect(edge, child,
                                   relative_proxy_cell.getFirstChild());
                     }

                     attributes.put(edge,
                                    DeviceGraph.createRelativePtrLineStyle());
                     cells.add(edge);
                  }
               }
            }
         }
      }

      Object[] cell_array = cells.toArray();
      this.graph.getGraphLayoutCache().insert(cell_array, attributes, cs,
                                              null, null);

      if ( applyLayout )
      {
         applyGraphLayoutAlgorithm(cell_array);
      }
   }

   private ConfigBroker  mBroker      = null;
   private ConfigContext mContext     = null;
   private List          allowedTypes = new ArrayList();

   private JGraphLayoutAlgorithm graphLayoutAlgorithm =
      new SpringEmbeddedLayoutAlgorithm();

   private DeviceGraph graph = new DeviceGraph();
}
