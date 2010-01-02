/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewFactory;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphLayoutCache;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigDefinitionRepository;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigUtilities;

import org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras.IntersenseVertexView;


public class DeviceGraphLayoutCache
   extends GraphLayoutCache
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   public DeviceGraphLayoutCache(DeviceGraphModel model)
   {
      this(model, new ProxiedDeviceCellViewFactory());

      ProxiedDeviceCellViewFactory factory =
         (ProxiedDeviceCellViewFactory) getFactory();

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
      List all_defs = repos.getAllLatest();

      // Input device types.
      List device_types =
         ConfigUtilities.getDefinitionsOfType(all_defs, INPUT_DEVICE_TYPE);
      DefaultDeviceGraphCellCreator creator =
         new DefaultDeviceGraphCellCreator();

      for ( Iterator d = device_types.iterator(); d.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition) d.next();
         if ( ! def.isAbstract() )
         {
            GraphHelpers.registerGraphCellCreator(def, creator);
            factory.registerCreator(def, MultiUnitDeviceVertexView.class);
         }
      }

      // Override the creators for specific device types.
      factory.registerCreator(repos.get(INTERSENSE_TYPE),
                              IntersenseVertexView.class);
      factory.registerCreator(repos.get(INTERSENSE_API_TYPE),
                              IntersenseVertexView.class);

      // Proxy types.
      List proxy_types = ConfigUtilities.getDefinitionsOfType(all_defs,
                                                              PROXY_TYPE);
      for ( Iterator d = proxy_types.iterator(); d.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition) d.next();
         if ( ! def.isAbstract() )
         {
            factory.registerCreator(def, ProxyVertexView.class);
         }
      }
   }

   public DeviceGraphLayoutCache(DeviceGraphModel model,
                                 ProxiedDeviceCellViewFactory factory)
   {
      this(model, factory, false);
   }

   public DeviceGraphLayoutCache(DeviceGraphModel model,
                                 ProxiedDeviceCellViewFactory factory,
                                 boolean partial)
   {
      this(model, factory, null, null, partial);
   }

   public DeviceGraphLayoutCache(DeviceGraphModel model,
                                 ProxiedDeviceCellViewFactory factory,
                                 CellView[] cellViews,
                                 CellView[] hiddenCellViews, boolean partial)
   {
      super(model, factory, cellViews, hiddenCellViews, partial);
   }

   protected void augmentNestedMapForValueChange(Map nested, Object cell,
                                                 Object newValue)
   {
      // If the edited cell is a DefaultGraphCell holding a ConfigElement,
      // then we do not want to replace the user value.  Instead, we want to
      // interpret the edit as a name change for the ConfigElement and leave
      // the ConfigElement reference as the cell's value.
      if ( cell instanceof DefaultGraphCell &&
           ((DefaultGraphCell) cell).getUserObject() instanceof ConfigElementHolder )
      {
         ConfigElementHolder holder =
            (ConfigElementHolder) ((DefaultGraphCell) cell).getUserObject();
         holder.getElement().setName((String) newValue, holder.getContext());

         // For a graph cell holding a DeviceInfo object, update all the
         // proxies pointing at that device to use the device's new name.
         if ( holder instanceof DeviceInfo )
         {
            DefaultGraphCell dev_cell = (DefaultGraphCell) cell;

            GraphModel model = getModel();

            for ( int i = 0; i < dev_cell.getChildCount(); ++i )
            {
               Iterator e = model.edges(dev_cell.getChildAt(i));

               while ( e.hasNext() )
               {
                  DefaultEdge edge = (DefaultEdge) e.next();
                  Object source_obj = edge.getSource();

                  if ( source_obj != null && source_obj instanceof DefaultPort )
                  {
                     DefaultGraphCell source = (DefaultGraphCell) source_obj;
                     DefaultGraphCell parent =
                        (DefaultGraphCell) source.getParent();

                     // Just to be safe, test the user object type.
                     if ( parent.getUserObject() instanceof ProxyInfo )
                     {
                        ProxyInfo proxy_info =
                           (ProxyInfo) parent.getUserObject();
                        ConfigElement proxy_elt = proxy_info.getElement();
                        proxy_elt.setProperty(DEVICE_PROPERTY, 0, newValue,
                                              proxy_info.getContext());
                     }
                  }
               }
            }
         }
         // For a graph cell holding a ProxyInfo object, update everything
         // pointing at the proxy to use the proxy's new name.
         else if ( holder instanceof ProxyInfo )
         {
            ProxyInfo proxy_info = (ProxyInfo) holder;

            for ( Iterator i = proxy_info.getAliases().iterator();
                  i.hasNext(); )
            {
               ConfigElement alias = (ConfigElement) i.next();

               alias.setProperty(PROXY_PROPERTY, 0, newValue,
                                 proxy_info.getContext());
            }

            Object[] edges = DefaultGraphModel.getIncomingEdges(getModel(),
                                                                cell);

            for ( int i = 0; i < edges.length; ++i )
            {
               Object source_obj = (DefaultEdge) edges[i];
               // What next?
            }
         }
      }
      else
      {
         super.augmentNestedMapForValueChange(nested, cell, newValue);
      }
   }
}
