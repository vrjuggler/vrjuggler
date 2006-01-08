/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.event;


/**
 * Interface implemented by classes that are interested in listening for
 * changes to loaded files in Tweek Beans.
 *
 * @see org.vrjuggler.tweek.beans.FileLoader
 *
 * @since 0.92.3
 */
public interface FileActionListener extends java.util.EventListener
{
   /**
    * The Bean opened a file.
    *
    * @param event the event describing the file open action.
    */
   public void fileOpenPerformed(FileActionEvent event);

   /**
    * The Bean modified an open file.  This is invoked only when the Bean has
    * at least one file open.
    *
    * @param event the event describing the file change action.
    */
   public void fileChangePerformed(FileActionEvent event);

   /**
    * The Bean saved an open file.  This is invoked only when the Bean has
    * at least one file open.
    *
    * @param event the event describing the file save action.
    */
   public void fileSavePerformed(FileActionEvent event);

   /**
    * The Bean saved an open file with a different name.  This is invoked only
    * when the Bean has at least one file open.
    *
    * @param event the event describing the file save-as action.
    */
   public void fileSaveAsPerformed(FileActionEvent event);

   /**
    * The Bean saved all open files.  This is invoked only when the Bean has
    * at least one file open.
    *
    * @param event the event describing the file-all save action.
    */
   public void fileSaveAllPerformed(FileActionEvent event);

   /**
    * The Bean closed an open file.
    *
    * @param event the event describing the file close action.
    */
   public void fileClosePerformed(FileActionEvent event);
}
