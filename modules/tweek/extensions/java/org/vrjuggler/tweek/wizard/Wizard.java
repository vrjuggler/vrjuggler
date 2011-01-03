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

import java.io.Serializable;
import java.util.*;

/**
 * Describes collection of WizardPanes in a particular sequence with the
 * expectation that the user will walk through them from beginning to end. A
 * particular wizard can be serialized at design time and brought back to life
 * when the user requests it.
 * <p>
 * A wizard contains a sequence of wizard steps which are processed linearly.
 * Any of these steps may be another sequence of wizard steps that offshoot from
 * the root sequence.
 * <p>
 * While to the user a wizard appears to be a linear iteration through wizard
 * panes, a wizard is really stored as a tree. Since the choice of what wizard
 * pane to show next could rely on choices the user made in the previous pane,
 * it seems logical that a wizard should be implemented as a tree of wizard
 * panes connected by the logic conditions that decide which pane to show next.
 */
public class Wizard
   extends RootWizardSequence
   implements Serializable
{
   /**
    * Creates a new wizard with no steps, no name and no title.
    */
   public Wizard()
   {
      name = "Wizard";
      title = "";
   }

   /**
    * Gets the name of this wizard.
    *
    * @return  the name
    */
   public String getName()
   {
      return name;
   }

   /**
    * Sets the name of this wizard.
    *
    * @param name       the name
    */
   public void setName(String name)
   {
      this.name = name;
   }

   /**
    * Gets the title for this wizard.
    *
    * @return  the title
    */
   public String getTitle()
   {
      return title;
   }

   /**
    * Sets the title of this wizard.
    *
    * @param title      the title
    */
   public void setTitle(String title)
   {
      this.title = title;
   }

   /**
    * Gets the sequence that makes up this wizard.
    */
   public WizardSequence getSequence()
   {
      return this;
   }

   /**
    * Gets the iterator that may be used to walk through this wizard.
    */
   public WizardStepIterator iterator()
   {
      return new WizardIterator(this.begin());
   }

   /**
    * Gets a list of all the custom classes this Wizard requires to run in a
    * separate VM.
    */
   public List getRequiredClasses()
   {
      List classes = new ArrayList();

      // Query each wizard step for its needs
      for (WizardStepIterator itr = iterator(); itr.hasNext(); )
      {
         WizardStep step = itr.next();

         // Add in the step's class
         classes.add(step.getClass().getName());

         // Add in the step's required classes
         classes.addAll(step.getRequiredClasses());
      }

      return classes;
   }

   /**
    * Gets a list of all the custom resources this Wizard requires to run in a
    * separate VM.
    */
   public List getRequiredResources()
   {
      List classes = new ArrayList();

      // Query each wizard step for its needs
      for (WizardStepIterator itr = iterator(); itr.hasNext(); )
      {
         WizardStep step = itr.next();
         classes.addAll(step.getRequiredResources());
      }

      return classes;
   }

   /**
    * The name of this wizard.
    */
   private String name;

   /**
    * The title of this wizard.
    */
   private String title;

   /**
    * The root sequence for this wizard.
    */
   private RootWizardSequence sequence;
}
