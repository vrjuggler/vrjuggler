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

import java.util.Vector;

public class PropDependEntry
{
   //public Property property;
   public String propertyname;
   public String dependency_name;
   public Vector other_files;

   public PropDependEntry()
   {
      other_files = new Vector();
   }

   public String toString()
   {
      String s = propertyname//property.getName()
                 + " needs \"" + dependency_name + "\"\n";
      if (other_files.size() > 0)
      {
         s += "    satisfied in:\n";
         for (int i = 0; i < other_files.size(); i++)
         {
            s += "        " + (String)other_files.elementAt(i) + "\n";
         }
      }
      else
      {
         s += "    no match found in any loaded file.\n";
      }
      return s;
   }
}
