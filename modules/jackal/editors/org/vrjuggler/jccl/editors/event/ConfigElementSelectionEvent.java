/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
package org.vrjuggler.jccl.editors.event;

import java.util.EventObject;
import org.vrjuggler.jccl.config.ConfigElement;

/**
 * An event that characterizes a selection of a configuration element.
 */
public class ConfigElementSelectionEvent
   extends EventObject
{
   /**
    * Creates a new event that originated from the given source object
    * describing a selection of the given element.
    */
   public ConfigElementSelectionEvent(Object src, ConfigElement elt)
   {
      super(src);
      mElement = elt;
   }

   /**
    * Gets the confguration element that has been selected.
    */
   public ConfigElement getElement()
   {
      return mElement;
   }

   /** The configuration element that has been selected. */
   private ConfigElement mElement;
}
