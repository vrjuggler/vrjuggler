/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


package VjComponents.UI;

import VjControl.VjComponent;

/** Parent for editors of ConfigChunks, ChunkDescs, etc.
 *  Essentially, this class defines something that can go in a 
 *  VjComponents.UI.util.GenericEditorFrame().
 */
public interface EditorPanel 
    extends VjComponent {


    /** The panel should display help text.
     *  The most common way to implement this method is to open an HTMLFrame
     *  to show an appropriate help file.
     */
    public void showHelp ();


    /** Checking for matches (ie to detect duplicate panels etc.
     *  This should return true if cl is "ConfigChunkPanel",
     *  db and o are the chunkdb and chunk passed via setChunk.
     */
    public boolean matches (String cl, Object db, Object o);

}
