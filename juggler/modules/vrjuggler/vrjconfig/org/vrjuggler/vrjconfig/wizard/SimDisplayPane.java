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

package org.vrjuggler.vrjconfig.wizard;

import javax.swing.JComponent;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.vrjconfig.ui.SimDisplayEditor;

/**
 * Wizard pane that allows the user to lay out the simulator displays using
 * Drag-and-Drop semantics. This pane uses the <code>SimDisplayEditor</code> to
 * do the simulator display editing.
 *
 * @see SimDisplayEditor
 */
public class SimDisplayPane
   extends AbstractWizardStep
   implements WizardPane
{
   public JComponent getGUI()
   {
      if (editor == null)
      {
         editor = new SimDisplayEditor();
      }
      return editor;
   }

   public void setNumDisplays(int numDisplays)
   {
      int oldValue = this.numDisplays;
      this.numDisplays = numDisplays;
//      firePropertyChange("numDisplays", oldValue, numDisplays);
   }

   public int getNumDisplays()
   {
      return numDisplays;
   }

   public void setBlah(boolean blah)
   {
      this.blah = blah;
   }

   public boolean getBlah()
   {
      return blah;
   }

   private boolean blah = false;

   /**
    * The number of displays this panel is managing.
    */
   private int numDisplays = 1;

   private transient SimDisplayEditor editor = null;
}
