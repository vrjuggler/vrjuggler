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
package org.vrjuggler.jccl.config.event;

import java.util.EventObject;
import org.vrjuggler.jccl.config.*;

public class PropertyDefinitionEvent
   extends EventObject
{
   /**
    * Creates a new PropertyDefinition event from the given source object
    * relating to the given value.
    */
   public PropertyDefinitionEvent(Object src, Object value)
   {
      super(src);
      mValue = value;
   }

   public PropertyDefinitionEvent(Object src, boolean value)
   {
      this(src, new Boolean(value));
   }

   public PropertyValueDefinition getPropertyValueDefinition()
   {
      return (PropertyValueDefinition)mValue;
   }

   public boolean getBoolean()
   {
      return ((Boolean)mValue).booleanValue();
   }

   public Object getValue()
   {
      return mValue;
   }

   private Object mValue;
}
