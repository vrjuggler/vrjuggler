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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.swing.JComponent;
import javax.swing.JLabel;

/**
 * This class provides a skeletal implementation of the WizardStep interface to
 * minimize the effort required to implement that interface.
 */
public abstract class AbstractWizardStep
   implements WizardStep
{
   /**
    * Creates a new AbstractWizardStep with no listeners.
    */
   protected AbstractWizardStep()
   {
      mWizardStepListeners = new ArrayList();
      mParent = null;

      // Get the default name of this step from the classname
      String className = getClass().getName();
      // This will always work, since if the index lookup fails, it will return
      // -1 which will give us a substring from 0 - exactly what we want.
      mName = className.substring(className.lastIndexOf('.') + 1);

      mStepLabel.setText(mName);
   }
   
   /**
    * Called when this step is about to be entered. This whiteboard allows
    * information to be passed between WizardSteps.
    */
   public void setMap(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
   }

   /**
    * Helper function to allow each Wizard Step's GUI panel access to the Map.
    */
   public Map getMap()
   {
      return mWhiteBoard;
   }

   /**
    * Called when this step is about to be entered. Custom processing that
    * needs to be done on entry before the step is entered may be done here.
    */
   public void onEntering()
   {
      fireWizardStepEvent(WizardStepEvent.ENTERING);
   }

   /**
    * Called when this step has been entered. Custom processing that needs to be
    * done on entry after the step has been entered may be done here.
    */
   public void onEntered()
   {
      fireWizardStepEvent(WizardStepEvent.ENTERED);
   }

   /**
    * Called when this step is about to be exited. Custom processing that needs
    * to be done on exit before the step is exited may be done here. The
    * AbstractWizardStep always returns true.
    *
    * @return  true to continue with step exit, false to cancel it
    */
   public boolean onExiting()
   {
      fireWizardStepEvent(WizardStepEvent.EXITING);
      return true;
   }

   /**
    * Called when this step has been exited. Custom processing that needs to be
    * done on exit after the step has been exited may be done here.
    */
   public void onExited()
   {
      fireWizardStepEvent(WizardStepEvent.EXITED);
   }

   /**
    * Sets the name fo this wizard step.
    *
    * @param name    this step's new name
    */
   public void setName(String name)
   {
      mName = name;
      mStepLabel.setText(mName);
   }

   /**
    * Gets the name of this wizard step.
    *
    * @return  this step's name
    */
   public String getName()
   {
      return mName;
   }
   
   /**
    * Gets the label that represents this WizardStep.
    *
    * @return  this step's label
    */
   public JComponent getStepLabel()
   {
      return mStepLabel;
   }

   /**
    * Gets the parent wizard step that this step is contained in. This provides
    * support for composite steps.
    *
    * @return  the parent WizardStep or null if this step has no parent
    */
   public WizardStep getParent()
   {
      return mParent;
   }

   /**
    * Sets the WizardStep that is this step's parent.
    *
    * @param parent  the parent WizardStep or null if this step has no parent
    */
   public void setParent(WizardStep parent)
   {
      mParent = parent;
   }

   /**
    * Adds the given wizard step listener to this wizard step.
    */
   public void addWizardStepListener(WizardStepListener listener)
   {
      mWizardStepListeners.add(listener);
   }

   /**
    * Gets a list of external classes required by this wizard step to operate
    * successfully in a new VM.
    *
    * @return  a list of fully-qualified class names
    */
   public List getRequiredClasses()
   {
      return new ArrayList();
   }

   /**
    * Gets a list of the external resources required by this wizard step in
    * order to operate successfully in a new VM.
    *
    * @return  a list of the filenames of the resources needed. The filenames
    *          need to be the same as when they are loaded from the ClassLoader.
    */
   public List getRequiredResources()
   {
      return new ArrayList();
   }

   /**
    * Removes the given wizard step listener from this wizard step.
    */
   public void removeWizardStepListener(WizardStepListener listener)
   {
      mWizardStepListeners.remove(listener);
   }

   /**
    * Helper function for firing wizard step events.
    *
    * @param type    the type of WizardStepEvent to fire.
    */
   protected void fireWizardStepEvent(int type)
   {
      WizardStepEvent evt = new WizardStepEvent(this, type);

      // Run through all listeners and notify them of the event
      Iterator itr = mWizardStepListeners.iterator();
      while (itr.hasNext())
      {
         WizardStepListener listener = (WizardStepListener)itr.next();
         switch (type)
         {
         case WizardStepEvent.ENTERING:
            listener.wizardStepEntering(evt);
            break;
         case WizardStepEvent.ENTERED:
            listener.wizardStepEntered(evt);
            break;
         case WizardStepEvent.EXITING:
            listener.wizardStepExiting(evt);
            break;
         case WizardStepEvent.EXITED:
            listener.wizardStepExited(evt);
            break;
         default:
            throw new RuntimeException("Unknown wizard step event type");
         }
      }
   }

   /**
    * Used to pass information between WizardSteps.
    */
   protected Map mWhiteBoard;

   /**
    * The name of this step.
    */
   protected String mName;

   /**
    * The label used to represent this WizardStep.
    */
   protected JLabel mStepLabel = new JLabel();

   /**
    * The parent WizardStep.
    */
   protected WizardStep mParent;

   /**
    * The list of wizard step listeners for this wizard step.
    */
   protected List mWizardStepListeners;
}
