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
package org.vrjuggler.tweek.wizard;

import java.util.*;

/**
 * This all static class acts as a registry and factory for WizardSteps.
 */
public class WizardStepFactory
{
   /**
    * Registers a WizardStep class that can be inserted into a Wizard.
    */
   public static void registerWizardStep(Class target)
   {
      steps.add(target);
      System.out.println("WizardStepFactory: registered "+target.getName());
   }

   /**
    * Gets the number of wizard steps in this factory.
    */
   public static int getNumWizardSteps()
   {
      return steps.size();
   }

   /**
    * Gets i'th the wizard step in this factory.
    */
   public static WizardStep getWizardStep(int index)
   {
      Class wizardClass = (Class)steps.get(index);

      WizardStep step = null;
      try
      {
         step = (WizardStep)wizardClass.newInstance();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
      return step;
   }

   /**
    * The list of all registered wizard steps in this factory.
    */
   private static List steps = new ArrayList();
}
