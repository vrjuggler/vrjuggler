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

import java.util.List;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A custom device informational object for devices that are configured
 * relative to one or more proxies.
 */
public class RelativeDeviceInfo
   extends BaseDeviceInfo
{
   public RelativeDeviceInfo(ConfigElement element, ConfigContext context,
                             List pointerPropDefs)
   {
      super(element, context);

      mPointerPropDefs = pointerPropDefs;
   }

   /**
    * Verifies that the given port can be a source of the given edge within
    * the given model.  This determination is made by testing to see if the
    * config element held by the parent of the target port (which must have
    * parent containing a <code>ProxyInfo</code> object) is of a type to which
    * we are allowed to point.
    */
   public boolean acceptsSource(GraphModel model, Object edge, Object port)
   {
      boolean accepts = false;

      try
      {
         ProxyPointerInfo proxy_ptr_info =
            (ProxyPointerInfo) ((DefaultPort) port).getUserObject();
         PropertyDefinition ptr_prop_def =
            proxy_ptr_info.getPointerPropertyDefinition();

         if ( mPointerPropDefs.contains(ptr_prop_def) )
         {
            Object[] out_edges = DefaultGraphModel.getOutgoingEdges(model,
                                                                    port);

            // Restrict our proxy ports to having at most one out-going edge.
            // If the given edge is the same as the port's out-going edge,
            // that is fine.
            if ( out_edges == null || out_edges.length == 0 ||
                 (out_edges.length == 1 && out_edges[0] == edge) )
            {
               DefaultPort target_port =
                  (DefaultPort) ((Edge) edge).getTarget();
               DefaultGraphCell target_cell =
                  (DefaultGraphCell) target_port.getParent();

               ProxyInfo proxy_info =
                  (ProxyInfo) target_cell.getUserObject();
               String def_token =
                  proxy_info.getElement().getDefinition().getToken();

               accepts = ptr_prop_def.getAllowedTypes().contains(def_token);
            }
         }
      }
      // If we catch a ClassCastException at any point, then we are not
      // working with the cells and/or user objects that we expect.  Hence,
      // the source cannot be accepted.
      catch (ClassCastException ex)
      {
         /* Oh well. */ ;
      }

      return accepts;
   }

   private List mPointerPropDefs = null;
}
