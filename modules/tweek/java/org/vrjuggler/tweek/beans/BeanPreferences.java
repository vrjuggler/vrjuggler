/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 * The interface that must be implemented by Beans that provide their own
 * preferences editors.
 *
 * @since 0.2.0
 */
public interface BeanPreferences
{
   /**
    * Returns the editor component for the Bean's preferences.
    *
    * Pre-condition: The object to be returned must be non-null.
    */
   public javax.swing.JComponent getEditor();

   /**
    * Allows full GUI initialization to be delayed until the editor panel is
    * needed.  The GUI may be initialized by the constructor or through some
    * other means.  An implementation of this method simply delays execution
    * of code until it is needed--if it is needed at all.
    */
   public void initPrefsGUI();

   /**
    * Returns the "name" of the editor panel.  This may be any string that can
    * be used to identify this preferences editor in a graphical collection of
    * editor panels.
    */
   public String getEditorName();

   /**
    * Loads the edited preferences from disk.
    *
    * @throws java.io.IOException
    */
   public void load() throws java.io.IOException;

   /**
    * Determines if enough information has been entered to allow committing
    * (i.e., saving) the edited preferences.  It is up to the enclosing
    * preferences editor to check this property before invoking save().
    */
   public boolean canCommit();

   /**
    * Writes the edited preferences to disk.
    *
    * @throws java.io.IOException
    */
   public void save() throws java.io.IOException;
}
