/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import javax.swing.JLabel;

import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoListener;


/**
 * A small extension to <code>javax.swing.JLabel</code> that automatically
 * changes its text whenever the device unit associated with the label changes.
 */
public class UnitLabel
   extends JLabel
   implements UnitInfoListener
{
   public UnitLabel(UnitInfo unitInfo)
   {
      this.unitInfo = unitInfo;
      this.unitInfo.addUnitInfoListener(this);
      setUnitText();
   }

   /**
    * Tells this object to clean up after itself when it is no longer needed.
    */
   public void release()
   {
      this.unitInfo.removeUnitInfoListener(this);
      this.unitInfo = null;
   }

   /**
    * Responds to the unit number change by updating the text property of
    * this label.
    */
   public void unitNumberChanged(UnitInfoEvent evt)
   {
      setUnitText();
   }

   private void setUnitText()
   {
      setText(UnitTypeHelpers.getUnitTypeName(unitInfo.getUnitType()) +
              " Unit " + unitInfo.getUnitNumber());
   }

   private UnitInfo unitInfo = null;
}
