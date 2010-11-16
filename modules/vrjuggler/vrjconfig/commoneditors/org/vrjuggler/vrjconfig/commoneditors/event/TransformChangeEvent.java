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

package org.vrjuggler.vrjconfig.commoneditors.event;

import java.util.EventObject;


public class TransformChangeEvent
   extends EventObject
{
   public TransformChangeEvent(Object source, double xRot, double yRot,
                               double zRot)
   {
      super(source);
      this.xRot = xRot;
      this.yRot = yRot;
      this.zRot = zRot;
   }

   public double getXRot()
   {
      return xRot;
   }

   public double getYRot()
   {
      return yRot;
   }

   public double getZRot()
   {
      return zRot;
   }

   private double xRot;
   private double yRot;
   private double zRot;
}
