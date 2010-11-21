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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.Collection;
import java.util.List;
import java.util.Map;

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
   extends DeviceInfo
{
   /**
    * Creates a new <code>RelativeDeviceInfo</code> object for a device that
    * has exactly one input source (unit) at all times.  The default device
    * unit handler delegate is created to manage the units.
    * 
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param pointerPropDefs    the list of
    *                           <code>org.vrjuggler.jccl.config.PropertyDefinition</code>
    *                           objects providing the information about the
    *                           proxy types to which this device may be
    *                           configured
    */
   public RelativeDeviceInfo(ConfigElement devElt, ConfigContext ctx,
                             List pointerPropDefs)
   {
      super(devElt, ctx);
      mPointerPropDefs = pointerPropDefs;
   }

   /**
    * Creates a new <code>DeviceInfo</code> object and looks up the
    * property definition for the named property if a name is provided.  The
    * value of <code>unitPropertyName</code> may be null.  If it is non-null,
    * then it must be the name of a property in <code>devElt</code>'s config
    * definition.  This version of the constructor should be used when the
    * config element has a proeprty with a variable number of values that
    * indicate how many input sources (units) are available in the device.
    * The default device unit handler delegate is created to manage the units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitPropertyName   the name of the property that represents
    *                           the number of available input sources (units)
    *                           the device has (may be null)
    * @param pointerPropDefs    the list of
    *                           <code>org.vrjuggler.jccl.config.PropertyDefinition</code>
    *                           objects providing the information about the
    *                           proxy types to which this device may be
    *                           configured
    *
    * @see DefaultUnitPropertyHandler
    */
   public RelativeDeviceInfo(ConfigElement devElt, ConfigContext ctx,
                             String unitPropertyName, List pointerPropDefs)
   {
      super(devElt, ctx, unitPropertyName);
      mPointerPropDefs = pointerPropDefs;
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>variable</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.  The default device unit handler delegate
    * is created to manage the units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypeMap        a mapping from unit types (as defined in
    *                           <code>UnitConstants</code> to some value that
    *                           the default device unit property handler knows
    *                           how to manage
    * @param pointerPropDefs    the list of
    *                           <code>org.vrjuggler.jccl.config.PropertyDefinition</code>
    *                           objects providing the information about the
    *                           proxy types to which this device may be
    *                           configured
    *
    * @see UnitConstants
    * @see DefaultUnitPropertyHandler
    */
   public RelativeDeviceInfo(ConfigElement devElt, ConfigContext ctx,
                             Map unitTypeMap, List pointerPropDefs)
   {
      super(devElt, ctx, unitTypeMap);
      mPointerPropDefs = pointerPropDefs;
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>variable</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.  The given unit handler delegate is stored
    * to manage the device units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypeMap        a mapping from unit types (as defined in
    *                           <code>UnitConstants</code> to some value that
    *                           the device unit property handler knows how to
    *                           manage
    * @param handler            the delegate for managing the device unit
    *                           properties
    * @param pointerPropDefs    the list of
    *                           <code>org.vrjuggler.jccl.config.PropertyDefinition</code>
    *                           objects providing the information about the
    *                           proxy types to which this device may be
    *                           configured
    *
    * @see UnitConstants
    */
   public RelativeDeviceInfo(ConfigElement devElt, ConfigContext ctx,
                             Map unitTypeMap,
                             DeviceUnitPropertyHandler handler,
                             List pointerPropDefs)
   {
      super(devElt, ctx, unitTypeMap, handler);
      mPointerPropDefs = pointerPropDefs;
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>fixed</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypes          the list of unit types supported by this device
    * @param pointerPropDefs    the list of
    *                           <code>org.vrjuggler.jccl.config.PropertyDefinition</code>
    *                           objects providing the information about the
    *                           proxy types to which this device may be
    *                           configured
    *
    * @see UnitConstants
    */
   public RelativeDeviceInfo(ConfigElement devElt, ConfigContext ctx,
                             Collection unitTypes, List pointerPropDefs)
   {
      super(devElt, ctx, unitTypes);
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
