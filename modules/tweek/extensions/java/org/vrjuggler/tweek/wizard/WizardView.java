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
package org.vrjuggler.tweek.wizard;

public interface WizardView
{
   /**
    * Gets the wizard step that this view is currently at.
    *
    * @return  the current wizard step
    *
    * @throws  NoSuchElementException
    *             if there is no current step (i.e. the wizard is empty)
    */
   public WizardStep getCurrentWizardStep();

   /**
    * Tells this view to move to the next step in the wizard.
    *
    * @throws  NoSuchElementException
    *             if the current step is the last step
    */
   public boolean moveNext();

   /**
    * Tells this view to move to the previous step in the wizard.
    *
    * @throws  NoSuchElementException
    *             if the current step is the first step
    */
   public boolean movePrev();

   /**
    * Checks if the wizard has a step previous to the current one.
    *
    * @return  true if there is a previous step; false otherwise
    */
   public boolean hasPrev();

   /**
    * Checks if the wizard has a step after to the current one.
    *
    * @return  true if there is a next step; false otherwise
    */
   public boolean hasNext();

   /**
    * Sets the wizard that this bean views.
    *
    * @param   the wizard to view
    */
   public void setWizard(Wizard wizard);

   /**
    * Gets the wizard that this bean is viewing.
    *
    * @return  the wizard being viewed
    */
   public Wizard getWizard();

   /**
    * Adds the given wizard view listener to this wizard view.
    */
   public void addWizardViewListener(WizardViewListener listener);

   /**
    * Removes the given wizard view listener from this wizard view.
    */
   public void removeWizardViewListener(WizardViewListener listener);
}
