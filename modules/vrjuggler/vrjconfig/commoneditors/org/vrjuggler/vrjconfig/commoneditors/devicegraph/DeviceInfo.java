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

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;


/**
 * A holder for <code>ConfigElement</code>s that is used to provide run-time
 * identification of a <code>ConfigElement</code> reference for a device
 * configuration.  Proper use of this type is critical for proper handling of
 * <code>ConfigElement</code> references held in a JGraph graph model.
 *
 * <p>
 * This class provides a means to determine whether the device configured by
 * the contained config element supports a variable number of input sources
 * (units) and to facilitate creation and deletion of those input sources if
 * a variable number is supported.  However, it does not provide an inherent
 * mechanism for determining the current number of input sources for that
 * device.  Since this class is designed specfically for use in conjunction
 * with <code>DeviceGraphModel</code>, it is up to the code constructing the
 * graph to provide a way for determining the current number of input sources
 * for a given config element.  In general, there is no way to look at an
 * arbitrary config element and determine how many input sources the device
 * has.
 * </p>
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphLayoutCache
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxiedDeviceCellViewFactory
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxyPointerEdge
 */
public class DeviceInfo
   extends ConfigElementHolder
{
   /**
    * Creates a new <code>DeviceInfo</code> object for a device that has
    * exactly one input source (unit) at all times.
    * 
    * @param devElt     the device config element
    * @param ctx        the config context that contains <code>devElt</code>
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx)
   {
      this(devElt, ctx, null);
   }

   /**
    * Creates a new <code>DeviceInfo</code> object and looks up the
    * property definition for the named property if a name is provided.  The
    * value of <code>unitPropertyName</code> may be null.  If it is non-null,
    * then it must be the name of a property in <code>devElt</code>'s config
    * definition.  This version of the constructor should be used when the
    * config element has a proeprty with a variable number of values that
    * indicate how many input sources (units) are available in the device.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitPropertyName   the name of the property that represents
    *                           the number of available input sources (units)
    *                           the device has (may be null)
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     String unitPropertyName)
   {
      super(devElt, ctx);

      if ( unitPropertyName != null && ! unitPropertyName.equals("") )
      {
         unitPropertyDefinition =
            devElt.getDefinition().getPropertyDefinition(unitPropertyName);
      }
   }

   /**
    * Retrieves the definition for the property in this device's config
    * element that provides an indication of how many input sources (units)
    * this device has.  To make a reliable determination about whether the
    * device has a variable number of input sources, use
    * <a href="#hasVariableUnitCount()"><code>hasVariableUnitCount()</code></a>.
    *
    * @return a <code>PropertyDefinition</code> reference for this device's
    *         input source (unit) configuration or null if this device has
    *         a fixed number of input sources (greater than or equal to 1)
    *         at all times.
    *
    * @see #hasVariableUnitCount()
    */
   public PropertyDefinition getUnitPropertyDefinition()
   {
      return unitPropertyDefinition;
   }

   /**
    * Determines whether the device config element contained within this
    * object is for a device that has a variable number of input sources
    * (units).
    *
    * @return <code>true</code> is returned if the device allows a variable
    *         number of input sources (units); <code>false</code> is returned
    *         otherwise.
    */
   public boolean hasVariableUnitCount()
   {
      return unitPropertyDefinition != null;
   }

   private PropertyDefinition unitPropertyDefinition = null;
}
