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

import java.util.Iterator;
import java.util.Map;
import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewFactory;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphLayoutCache;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigElement;


public class DeviceGraphLayoutCache
   extends GraphLayoutCache
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   // XXX: This constructor might be a good place to centralize the
   // device and proxy cell view registration, but who the hell knows if this
   // is the appropriate place?!?  Stupid JGraph ...
/*
   public DeviceGraphLayoutCache(GraphModel model)
   {
      this(model, new ProxiedDeviceCellViewFactory(), false);
   }
*/

   public DeviceGraphLayoutCache(GraphModel model, CellViewFactory factory)
   {
      this(model, factory, false);
   }

   public DeviceGraphLayoutCache(GraphModel model, CellViewFactory factory,
                                 boolean partial)
   {
      this(model, factory, null, null, partial);
   }

   public DeviceGraphLayoutCache(GraphModel model, CellViewFactory factory,
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
