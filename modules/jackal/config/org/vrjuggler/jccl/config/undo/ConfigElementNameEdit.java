/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

public class ConfigElementNameEdit extends AbstractUndoableEdit
{
   /** 
    * Create a ConfigElementNameEdit to all the undo/redo of element additions.
    *
    * @param elm        ConfigElement whose name has changed.
    * @param old_name   the old name of the config element.
    * @param new_name   the new name of the config element.
    */
   public ConfigElementNameEdit(ConfigElement elm, String old_name,
                                String new_name)
   {
      super();
      mConfigElement = elm;
      mOldName = old_name;
      mNewName = new_name;
   }

   public void undo() throws CannotUndoException 
   {
      super.undo();
      mConfigElement.setName(mOldName);
   }

   public void redo() throws CannotRedoException
   {
      System.out.println("redoing..");
      super.redo();
      System.out.println("redoing...");
      
      mConfigElement.setName(mNewName);
   }

   private ConfigElement   mConfigElement;
   private String          mOldName;
   private String          mNewName;
}
