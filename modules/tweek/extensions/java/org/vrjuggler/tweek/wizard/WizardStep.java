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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.wizard;

import java.io.Serializable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.swing.JComponent;

/**
 * Interface for a step within a wizard sequence. This is the basic type that
 * may be stored in the WizardTree that makes up a wizard.
 */
public interface WizardStep
   extends Serializable
{
   /**
    * Called when this step is about to be entered. Custom processing that
    * needs to be done on entry before the step is entered may be done here.
    */
   public void onEntering();

   /**
    * Called when this step has been entered. Custom processing that needs to be
    * done on entry after the step has been entered may be done here.
    */
   public void onEntered();

   /**
    * Called when this step is about to be exited. Custom processing that needs
    * to be done on exit before the step is exited may be done here.
    *
    * @return  true to continue with step exit, false to cancel it
    */
   public boolean onExiting();

   /**
    * Called when this step has been exited. Custom processing that needs to be
    * done on exit after the step has been exited may be done here.
    */
   public void onExited();

   /**
    * Sets the name fo this wizard step.
    *
    * @param name    this step's new name
    */
   public void setName(String name);

   /**
    * Gets the name of this wizard step.
    *
    * @return  this step's name
    */
   public String getName();
   
   /**
    * Gets the label that represents this WizardStep.
    *
    * @return  this step's label
    */
   public JComponent getStepLabel();

   /**
    * Gets the parent wizard step that this step is contained in. This provides
    * support for composite steps.
    *
    * @return  the parent WizardStep or null if this step has no parent
    */
   public WizardStep getParent();

   /**
    * Sets the WizardStep that is this step's parent.
    *
    * @param parent  the parent WizardStep or null if this step has no parent
    */
   public void setParent(WizardStep parent);

   /**
    * Gets a list of external classes required by this wizard step to operate
    * successfully in a new VM.
    *
    * @return  a list of fully-qualified class names
    */
   public List getRequiredClasses();
   
   /**
    * Gets a map of data used between WizardSteps.
    */
   public Map getMap();
   
   /**
    * Called when this step is about to be entered. This whiteboard allows
    * information to be passed between WizardSteps.
    */
   public void setMap(Map whiteboard);

   /**
    * Gets a list of the external resources required by this wizard step in
    * order to operate successfully in a new VM.
    *
    * @return  a list of the filenames of the resources needed. The filenames
    *          need to be the same as when they are loaded from the ClassLoader.
    */
   public List getRequiredResources();

   /**
    * Adds the given wizard step listener to this wizard step.
    */
   public void addWizardStepListener(WizardStepListener listener);

   /**
    * Removes the given wizard step listener from this wizard step.
    */
   public void removeWizardStepListener(WizardStepListener listener);
}
