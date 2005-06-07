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
package org.vrjuggler.jccl.config;

import javax.swing.event.EventListenerList;
import org.vrjuggler.jccl.config.event.*;
import javax.swing.undo.*;

/**
 * Represents a link to a configuration element within a configuration. The
 * element is referenced by its full name.
 */
public class ConfigElementPointer
{
   /**
    * Creates a new configuration element pointer pointing to the configuration
    * element with the given full name (path to the element).
    *
    * @param target     the name of the target element
    */
   public ConfigElementPointer(String target)
   {
      mTarget = target;
   }

   /**
    * Gets the name of the target configuration element.
    */
   public String getTarget()
   {
      return mTarget;
   }

   /**
    * Sets the name of the target configuration element.
    *
    * @param target     the name of the target element
    */
   public void setTarget(String target)
   {
      String old_target = mTarget;
      mTarget = target;
      fireTargetChanged(old_target);
   }

   /**
    * Tests if this configuration element pointer is equal to the given object.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof ConfigElementPointer)
      {
         ConfigElementPointer p = (ConfigElementPointer)obj;
         if(null != mTarget && null != p.mTarget)
         {   
            if (mTarget.equals(p.mTarget))
            {
               return true;
            }
         }
         else
         {
            return true;
         }
      }
      return false;
   }

   /**
    * Adds a listener for this configuration element pointer.
    */
   public void addConfigElementPointerListener(ConfigElementPointerListener listener)
   {
      mListenerList.add(ConfigElementPointerListener.class, listener);
   }

   /**
    * Removes a listener from this configuration element pointer.
    */
   public void removeConfigElementPointerListener(ConfigElementPointerListener listener)
   {
      mListenerList.remove(ConfigElementPointerListener.class, listener);
   }

   public String toString()
   {
      return mTarget;
   }

   /**
    * Notifies listeners that this pointer's target has changed.
    */
   protected void fireTargetChanged(String old_target)
   {
      ConfigElementPointerEvent evt = null;
      Object[] listeners = mListenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementPointerListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementPointerEvent(this, old_target);
            }
            ((ConfigElementPointerListener)listeners[i+1]).targetChanged(evt);
         }
      }
   }

   /** The name of the target configuration element. */
   private String mTarget;

   /** The listeners interested in this object. */
   private EventListenerList mListenerList = new EventListenerList();
}
