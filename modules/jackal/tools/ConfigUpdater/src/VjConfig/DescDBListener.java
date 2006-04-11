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

package VjConfig;

import VjConfig.DescDBEvent;

/** Listener for ChunkDescDB modification events.
 *  By registering as a DescDBListener, an object can receive events
 *  whenever the DescDB is modified.
 *  <p>
 *  DescDBEvents may be sent by any thread; a listener intending
 *  to update GUI components based on the event must use 
 *  SwingUtilities.invokeLater or something similar to ensure correct
 *  synchronization.
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public interface DescDBListener extends java.util.EventListener {

    public abstract void chunkDescAdded (DescDBEvent e);
    public abstract void chunkDescRemoved (DescDBEvent e);
    public abstract void chunkDescReplaced (DescDBEvent e);
    public abstract void chunkDescsCleared (DescDBEvent e);
}


