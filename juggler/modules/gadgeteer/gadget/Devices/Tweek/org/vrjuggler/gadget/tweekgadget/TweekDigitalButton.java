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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.gadget.tweekgadget;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.ButtonModel;
import javax.swing.JToggleButton;
import org.vrjuggler.tweek.net.corba.CorbaService;
import gadget.TweekDigitalSubject;


public class TweekDigitalButton extends JToggleButton
{
   public TweekDigitalButton(CorbaService corbaService,
                             TweekDigitalSubject subject)
   {
      mSubject  = subject;
      mObserver = new TweekDigitalObserverImpl(subject);
      corbaService.registerObject(mObserver,
                                  "Digital Observer " + mObserverCount);
      mObserverCount++;
      subject.attach(mObserver._this());
      this.getModel().addChangeListener(new StateChangeListener());
   }

   public void detach()
   {
      mSubject.detach(mObserver._this());
   }

   private class StateChangeListener implements ChangeListener
   {
      public void stateChanged(ChangeEvent e)
      {
         ButtonModel model = (ButtonModel) e.getSource();
         mSubject.setState(model.isSelected());
      }
   }

   private static int mObserverCount = 0;

   private TweekDigitalSubject      mSubject  = null;
   private TweekDigitalObserverImpl mObserver = null;
}
