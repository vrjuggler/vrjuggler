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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


/**
 * A simple container for a ConfigElement reference and the ConfigContext
 * wherein that ConfigElement reference exists.  This is used for user objects
 * in DefaultGraphCell instances.  Some classes in the package
 * org.vrjuggler.vrjconfig.commoneditors.devicegraph make use of this type to
 * determine how to handle user data in a given cell.
 *
 * @see org.vrjuggler.jccl.config.ConfigElement
 * @see org.vrjuggler.jccl.config.ConfigContext
 * @see org.jgraph.graph.DefaultGraphCell
 */
public abstract class ConfigElementHolder
{
   public ConfigElementHolder(ConfigElement element, ConfigContext context)
   {
      this.element = element;
      this.context = context;
   }

   public ConfigElement getElement()
   {
      return element;
   }

   public ConfigContext getContext()
   {
      return context;
   }

   /**
    * Verifies that the given port can be a source of the given edge within
    * the given model.
    */
   public abstract boolean acceptsSource(GraphModel model, Object edge,
                                         Object port);

   /**
    * Verifies that the given port can be a target of the given edge within
    * the given model.
    */
   public abstract boolean acceptsTarget(GraphModel model, Object edge,
                                         Object port);

   /**
    * Override of toString() needed for proper display of this type when it
    * is used as the container for user data in a cell.  The cell view will
    * make use of this to get a default representation of the user data.
    */
   public String toString()
   {
      return element.getName();
   }

   private ConfigElement element = null;
   private ConfigContext context = null;
}
