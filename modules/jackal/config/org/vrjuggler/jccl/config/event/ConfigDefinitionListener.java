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
package org.vrjuggler.jccl.config.event;

import java.util.EventListener;

/**
 * Interface for objects that wish to be notified when a ConfigDefinition
 * changes in some way.
 */
public interface ConfigDefinitionListener
   extends EventListener
{
   public void nameChanged(ConfigDefinitionEvent evt);

   public void tokenChanged(ConfigDefinitionEvent evt);

   public void abstractChanged(ConfigDefinitionEvent evt);

   public void helpChanged(ConfigDefinitionEvent evt);

   /**
    * @since 0.92.7
    */
   public void parentAdded(ConfigDefinitionEvent evt);

   /**
    * @since 0.92.7
    */
   public void parentRemoved(ConfigDefinitionEvent evt);

   public void categoryAdded(ConfigDefinitionEvent evt);

   public void categoryRemoved(ConfigDefinitionEvent evt);

   public void propertyDefinitionAdded(ConfigDefinitionEvent evt);

   public void propertyDefinitionRemoved(ConfigDefinitionEvent evt);
}
