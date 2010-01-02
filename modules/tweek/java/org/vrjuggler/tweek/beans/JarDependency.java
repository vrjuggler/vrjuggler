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
package org.vrjuggler.tweek.beans;

/**
 * This is just a simple encapsulation of a Bean's dependency on a JAR file
 * and the directory (or directories) that might contain that JAR file.
 */
public class JarDependency
{
   /**
    * Creates a new dependency object with the named JAR file and the given
    * path to find the file.  The file should name just the JAR file, and
    * the path must be a semicolon-separated list of one or more directories.
    * (If there is only one directory, no semicolon is needed).
    *
    * @param _file The JAR file upon which a Bean depends.
    * @param _path The semicolon-separated list of directories where the JAR
    *              file might be found.
    */
   public JarDependency (String _file, String _path)
   {
      file = _file;
      path = _path;
   }

   public String getFile ()
   {
      return file;
   }

   public String getPath ()
   {
      return path;
   }

   private String file;
   private String path;
}
