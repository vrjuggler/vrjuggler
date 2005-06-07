/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package org.vrjuggler.tweek.gui;

import java.util.EventObject;


/**
 * Event object delivered to classes implementing GlobalPrefsUpdateListener.
 */
public class GlobalPrefsUpdateEvent extends EventObject
{
   public final static int ALL_SAVED            = -1;
   public final static int USER_LEVEL           = 0;
   public final static int LOOK_AND_FEEL        = 1;
   public final static int BEAN_VIEWER          = 2;
   public final static int WINDOW_SIZE          = 3;
   public final static int CHOOSER_START_DIR    = 4;
   public final static int CHOOSER_OPEN_STYLE   = 5;
   public final static int DEFAULT_CORBA_HOST   = 6;
   public final static int DEFAULT_CORBA_PORT   = 7;
   public final static int DEFAULT_IIOP_VERSION = 8;

   public GlobalPrefsUpdateEvent(Object source)
   {
      super(source);
      this.updateType = ALL_SAVED;
   }

   public GlobalPrefsUpdateEvent(Object source, int type)
   {
      super(source);
      this.updateType = type;
   }

   public int getUpdateType()
   {
      return updateType;
   }

   private int updateType;
}
