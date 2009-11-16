/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.tweek.gui;


/**
 * This class serves as a handy wrapper around a tweek.SubjectManager instance.
 * The main purpose is to provide a toString() method that returns a friendly,
 * human-readable name for the encapsulated tweek.SubjectManager instance.
 * This class is package-visible because its use is fairly limited.
 */
class SubjectManagerWrapper
{
   /**
    * Creates an instance of this class using the given Subject Manager
    * reference.  The reference must be valid at the time of creation.
    */
   public SubjectManagerWrapper(tweek.SubjectManager mgr)
   {
      this.subjectManager = mgr;
   }

   public String toString()
   {
      // This is kind of lame, but I don't know what could be returned that
      // would be more helpful.
      return "Subject Manager Reference";
   }

   public tweek.SubjectManager getSubjectManager()
   {
      return this.subjectManager;
   }

   private tweek.SubjectManager subjectManager = null;
}
