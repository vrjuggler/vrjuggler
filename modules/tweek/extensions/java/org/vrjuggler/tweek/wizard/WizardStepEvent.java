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

import java.util.EventObject;

/**
 * This event indicates that the status of a particular WizardStep has changed.
 * It is fired whenever a WizardStep is about to be entered, entered, about to
 * be exited or exited.
 */
public class WizardStepEvent
   extends EventObject
{
   /**
    * Event type signifies that the wizard step is about to be entered.
    */
   public final static int ENTERING = 0;

   /**
    * Event type signifies that the wizard step was entered.
    */
   public final static int ENTERED  = 1;

   /**
    * Event type signifies that the wizard step was exited.
    */
   public final static int EXITING  = 2;

   /**
    * Event type signifies that the wizard step is about to be exited.
    */
   public final static int EXITED   = 3;

   /**
    * Constructs a new WizardStepEvent of the given type that originated from
    * the given source WizardStep.
    *
    * @param source     the WizardStep the caused this event
    * @param type       the type of WizardStep event
    */
   public WizardStepEvent(WizardStep source, int type)
   {
      super(source);
   }

   /**
    * Gets the wizard step that fired this event.
    *
    * @return  the source wizard step
    */
   public WizardStep getWizardStep()
   {
      return (WizardStep)getSource();
   }
}
