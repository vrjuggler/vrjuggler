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

package networktest;

import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JSlider;
import tweek.*;


/**
 * Implementation of the Observer side of the Tweek Subject/Observer pattern.
 * It must extend tweek.ObserverPOA so that instances of this class can be
 * registered as CORBA servants.  In addition, CORBA references to the
 * servants must be capable of being attached to remote subjects.
 */
public class SliderObserverImpl extends ObserverPOA
{
   public SliderObserverImpl (JSlider slider, SliderSubject subject)
   {
      mSlider        = slider;
      mSliderSubject = subject;
   }

   /**
    * Implements the required method in tweek.ObserverPOA.  The remote subject
    * will invoke this method whenever it is notified of a change.
    */
   public void update ()
   {
      System.out.println("Updated from our subject!");

      // If we have a valid slider object, we need to update its value to
      // whatever our subject has.
      if ( mSlider != null )
      {
         DefaultBoundedRangeModel model = (DefaultBoundedRangeModel) mSlider.getModel();
         System.out.println("Got slider model, setting value");
         model.setValue(mSliderSubject.getValue());
         System.out.println("Set value, repainting slider");
         mSlider.repaint();
      }

      System.out.println("Update processed");
   }

   /**
    * Detaches this observer from our subject.  This is needed when shutting
    * down a CORBA connection.
    */
   public void detach ()
   {
      mSliderSubject.detach(this._this());
   }

   private SliderSubject mSliderSubject = null;
   private JSlider       mSlider        = null;
}
