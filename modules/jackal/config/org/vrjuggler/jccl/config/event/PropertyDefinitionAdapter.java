/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.jccl.config.event;

import java.util.EventListener;

/**
 * Default implementation of the PropertyDefinitionListener that does nothing in
 * order to make the creating of PropertyDefinition listeners easier.
 */
public abstract class PropertyDefinitionAdapter
   implements PropertyDefinitionListener
{
   public void tokenChanged(PropertyDefinitionEvent evt) {}

   public void typeChanged(PropertyDefinitionEvent evt) {}

   public void helpChanged(PropertyDefinitionEvent evt) {}

   public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt) {}

   public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt) {}

   public void enumEditableChanged(PropertyDefinitionEvent evt) {}

   public void enumAdded(PropertyDefinitionEvent evt) {}

   public void enumRemoved(PropertyDefinitionEvent evt) {}

   public void allowedTypeAdded(PropertyDefinitionEvent evt) {}

   public void allowedTypeRemoved(PropertyDefinitionEvent evt) {}

   public void variableChanged(PropertyDefinitionEvent evt) {}
}
