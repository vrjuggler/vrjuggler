/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.wizard;

import java.util.Iterator;

/**
 * Interface for a bidirectional iterator over the WizardSteps contained
 * within a Wizard. This can be used to easily traverse through the steps in
 * a given Wizard.
 */
public interface WizardStepIterator
{
   /**
    * Checks if this iterator has a next step.
    *
    * @return true if there are more steps, false otherwise
    */
   public boolean hasNext();

   /**
    * Moves this iterator to the next WizardStep and returns it.
    *
    * @return  the next WizardStep
    *
    * @throws  NoSuchElementException
    *             if the iteration has no next step
    */
   public WizardStep next();

   /**
    * Checks if this iterator has a previous step.
    *
    * @return true if there are more steps, false otherwise
    */
   public boolean hasPrevious();

   /**
    * Moves this iterator to the previous WizardStep and returns it.
    *
    * @return  the previous WizardStep
    *
    * @throws  NoSuchElementException
    *             if the iteration has no previous step
    */
   public WizardStep previous();
}
