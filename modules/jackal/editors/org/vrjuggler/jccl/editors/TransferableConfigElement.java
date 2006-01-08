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

package org.vrjuggler.jccl.editors;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;

import org.vrjuggler.jccl.config.ConfigElement;



/**
 * Wraps a ConfigElement to allow us to transfer it from one context to
 * another, either using copy/paste or drag-and-drop methods.
 *
 * @since 0.92.8
 */
public class TransferableConfigElement
   implements Transferable
{
   /**
    * Constructor that takes the given ConfigElement and creates a Transferable
    * wrapper around it to allow us to move the ConfigElement from one context
    * to another.
    *
    * @param elt        the data to be transferred
    */
   public TransferableConfigElement(ConfigElement elt)
   {
      transferData = elt;
   }

   /**
    * Return the ConfigElement that we are transfering.
    */
   public Object getTransferData(DataFlavor flavor)
      throws UnsupportedFlavorException
   {
      if ( flavor.getRepresentationClass() == ConfigElement.class )
      {
         return transferData;
      }
      else
      {
         throw new UnsupportedFlavorException(flavor);
      }
   }

   /**
    * Return an array of valid DataFlavors that we can use to pass
    * ConfigElements from one context to another context.
    */
   public DataFlavor[] getTransferDataFlavors()
   {
      return transferDataFlavors;
   }

   /**
    * Check if the given DataFlavor is a valid flavor for transfering
    * ConfigElements from one context to another.
    */
   public boolean isDataFlavorSupported(DataFlavor flavor)
   {
      return flavor.getRepresentationClass() == ConfigElement.class;
   }

   /** ConfigElement that we are transfering. */
   private ConfigElement transferData = null;

   /** Array of valid DataFlavors for transfering a ConfigElement. */
   private static DataFlavor[] transferDataFlavors =
      { DataFlavorRepository.getConfigElementFlavor() };
}
