/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

import java.util.ArrayList;
import java.util.List;
import javax.swing.event.EventListenerList;

/**
 * The ConfigContext describes a view into the resources managed by the
 * ConfigBroker. By using a context, different views may manipulate the
 * resources in the ConfigBroker in different ways.
 *
 * In the ConfigBroker each resource is assigned a unique name. The
 * ConfigContext maintains a list of the resources in which a view wishes to
 * interact with. In this way, the view uses a ConfigContext to tell the
 * ConfigBroker to use only a subset of the resources it manages.
 *
 * @see ConfigBroker
 */
public class ConfigContext
{
   /**
    * Creates a new configuration context with no resources.
    */
   public ConfigContext()
   {
      resources = new ArrayList();
      listenerList = new EventListenerList();
   }

   /**
    * Adds the given resource into this context.
    *
    * @param name    the name of the resource to add
    */
   public void add(String name)
   {
      if (! contains(name))
      {
         resources.add(name);
         fireResourceAdded(name);
      }
   }

   /**
    * Removes the given resource from this context.
    *
    * @param name    the name of the resource to remove
    */
   public void remove(String name)
   {
      if (contains(name))
      {
         resources.remove(name);
         fireResourceRemoved(name);
      }
   }

   /**
    * Tests if this context contains the given resource.
    *
    * @param name    the name of the resource to check
    */
   public boolean contains(String name)
   {
      return resources.contains(name);
   }

   /**
    * Gets a list of the resources in this context.
    */
   public List getResources()
   {
      return resources;
   }

   /**
    * Adds the given listener so that it receives events when this context
    * changes.
    *
    * @param listener      the listener to add
    */
   public void addContextListener(ContextListener listener)
   {
      listenerList.add(ContextListener.class, listener);
   }

   /**
    * Removes the given listener so that it no longer receives events when this
    * context changes.
    *
    * @param listener      the listener to remove
    */
   public void removeContextListener(ContextListener listener)
   {
      listenerList.remove(ContextListener.class, listener);
   }

   /**
    * Notifies all listeners that the given resource has been added to this
    * context.
    *
    * @param name    the name of the resource added
    */
   protected void fireResourceAdded(String resource)
   {
      ContextEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ContextListener.class)
         {
            if (evt == null)
            {
               evt = new ContextEvent(this, resource);
            }
            ((ContextListener)listeners[i+1]).resourceAdded(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given resource has been removed from this
    * context.
    *
    * @param name    the name of the resource removed
    */
   protected void fireResourceRemoved(String resource)
   {
      ContextEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ContextListener.class)
         {
            if (evt == null)
            {
               evt = new ContextEvent(this, resource);
            }
            ((ContextListener)listeners[i+1]).resourceRemoved(evt);
         }
      }
   }

   /**
    *The list of resources in this context.
    * @link aggregation
    *      @associates <{ContextListener}>
    */
   private List resources;

   /**
    * All of the listeners interested in this context.
    */
   private EventListenerList listenerList;
}
