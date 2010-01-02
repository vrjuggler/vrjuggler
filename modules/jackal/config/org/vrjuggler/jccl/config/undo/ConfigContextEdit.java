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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config.undo;

import javax.swing.undo.AbstractUndoableEdit;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.CannotRedoException;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigBroker;
import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.DataSource;


public class ConfigContextEdit extends AbstractUndoableEdit
{
   /** 
    * Create a ConfigContextEdit to all the undo/redo of element additions.
    *
    * @param ctx        ConfigContext to add/remove the element from.
    * @param elm        ConfigElement to add/remove.
    * @param source     the data source from which the element was removed or
    *                   to which the element was added
    * @param addRemove  whether this edit is for adding or removing an element.
    */
   public ConfigContextEdit(ConfigContext ctx, ConfigElement elm,
                            DataSource source, boolean addRemove)
   {
      super();
      mConfigContext = ctx;
      mConfigElement = elm;
      mSource        = source;
      mAddRemove     = addRemove;
   }

   public void undo() throws CannotUndoException 
   {
      super.undo();

      ConfigBroker broker = new ConfigBrokerProxy();
      if (mAddRemove)
      {
         broker.remove(mConfigContext, mConfigElement, mSource);
      }
      else
      {
         broker.add(mConfigContext, mConfigElement, mSource);
      }
   }

   public void redo() throws CannotRedoException
   {
      System.out.println("redoing..");
      super.redo();
      System.out.println("redoing...");
      
      ConfigBroker broker = new ConfigBrokerProxy();
      if (mAddRemove)
      {
         broker.add(mConfigContext, mConfigElement, mSource);
      }
      else
      {
         broker.remove(mConfigContext, mConfigElement, mSource);
      }
   }

   private ConfigContext   mConfigContext;
   private ConfigElement   mConfigElement;
   private DataSource      mSource;

   /** Whether this edit is for adding or removing an element.*/
   private boolean         mAddRemove;
}
