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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans;


/**
 * The interface used to identify Beans as being able to load and unload files
 * using a GUI.
 *
 * @since 1.0
 */
public interface FileLoader
{
   /**
    * Returns a short string describing the type(s) of files opened by this
    * Bean.  It should be assumed that the returned String object will be used
    * in a GUI, possibly in a JMenuItem.  Hence, the returned text should be
    * short and sweet.
    */
   public String getFileType();

   /**
    * Determines if the file loading Bean can handle more than one open file
    * at a time.  Those Beans capable of opening more than one file should
    * return true.  Other Beans should return false.  If this behavior is not
    * followed, the results are undefined.
    *
    * @pre The Bean will correctly return its ability to open more than one
    *      file at a time.
    *
    * @return true is returned if the Bean can open more than one file at a
    *         time.<br>
    *         false is returned otherwise.
    */
   public boolean canOpenMultiple();

   /**
    * Informs an instance of the implementing class that the user has requested
    * the opening of a file.  It is up to the implementing class to take
    * appropriate actions.
    *
    * @return true is returned if a file was opened successfully.<br>
    *         false is returned if the file opening failed.
    */
   public boolean openRequested();

   /**
    * Determines if the file loading Bean can save files.  Those Beans capable
    * of saving files should return true.  Other Beans should return false.  If
    * this behavior is not followed, the results are undefined.
    *
    * @pre The Bean will correctly return its ability to save files.
    *
    * @return true is returned if the Bean can save file.<br>
    *         false is returned otherwise.
    */
   public boolean canSave();

   /**
    * Informs an instance of the implementing class that the user has requested
    * the saving of a file.  It is up to the implementing class to take
    * appropriate actions.
    *
    * @return true is returned if a file was saved successfully.<br>
    *         false is returned if the file saving failed.
    */
   public boolean saveRequested();

   /**
    * Informs an instance of the implementing class that the user has requested
    * the saving of a file with a different name.  It is up to the implementing
    * class to take appropriate actions.
    *
    * @return true is returned if a file was saved successfully.<br>
    *         false is returned if the file saving failed.
    */
   public boolean saveAsRequested();

   /**
    * Informs an instance of the implementing class that the user has requested
    * to close a file.  It is up to the implementing class to take appropriate
    * actions.
    *
    * @return true is returned if a file was closed successfully.<br>
    *         false is returned if the file closing failed.
    */
   public boolean closeRequested();

   /**
    * Returns the number of currently opened files.
    */
   public int getOpenFileCount();
}