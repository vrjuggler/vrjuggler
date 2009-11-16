/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.cave;


interface EditorConstants
   extends org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   static final String RESOURCE_BASE =
      "org/vrjuggler/vrjconfig/customeditors/cave";

   static final String IMAGE_BASE = RESOURCE_BASE + "/images";

   static final int FRONT_PLANE = 0;
   static final int BACK_PLANE = 1;
   static final int LEFT_PLANE = 2;
   static final int RIGHT_PLANE = 3;
   static final int BOTTOM_PLANE = 4;
   static final int TOP_PLANE = 5;
   static final int CUSTOM_PLANE = 6;

   static final int LOWER_LEFT = 0;
   static final int LOWER_RIGHT = 1;
   static final int UPPER_LEFT = 2;
   static final int UPPER_RIGHT = 3;
   
   static final int ACTIVE_STEREO = 0;
   static final int PASSIVE_STEREO = 1;
   static final int MONO = 2;
}
