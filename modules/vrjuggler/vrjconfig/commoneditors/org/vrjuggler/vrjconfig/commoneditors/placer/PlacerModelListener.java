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

package org.vrjuggler.vrjconfig.commoneditors.placer;

import java.util.EventListener;

/**
 * Interface for an object that wishes to know when a placer model has changed.
 */
public interface PlacerModelListener
   extends EventListener
{
   /**
    * Notifies this listener that the model it was listening to has changed in
    * some way.
    */
   public void placerItemsChanged(PlacerModelEvent evt);

   /**
    * Notifies this listener that items have been inserted into the placer.
    */
   public void placerItemsInserted(PlacerModelEvent evt);

   /**
    * Notifies this listener that items have been removed from the placer.
    */
   public void placerItemsRemoved(PlacerModelEvent evt);

   /**
    * Notifies this listener that the size of the desktop has changed.
    */
   public void placerDesktopSizeChanged(PlacerModelEvent evt);
}
