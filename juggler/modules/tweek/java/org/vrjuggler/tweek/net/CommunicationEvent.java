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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

package org.vrjuggler.tweek.net;

import org.vrjuggler.tweek.net.corba.*;


/**
 * Event class used by org.vrjuggler.tweek.net.CommunicationListener.
 *
 * @version $Revision$
 *
 * @see org.vrjuggler.tweek.net.CommunicationListener
 */
public class CommunicationEvent extends java.util.EventObject
{
   public final static int CONNECT    = 0;
   public final static int DISCONNECT = 1;
   public final static int UPDATE     = 2;

   public CommunicationEvent (Object source, int _type, CorbaService corba_if)
   {
      super(source);
      type          = _type;
      corbaRegistry = corba_if;
   }

   public int getType ()
   {
      return type;
   }

   public String getHostAddress ()
   {
      return corbaRegistry.getHost();
   }

   public int getHostPort ()
   {
      return corbaRegistry.getPort();
   }

   public CorbaService getCorbaRegistry ()
   {
      return corbaRegistry ;
   }

   private int          type          = -1;
   private CorbaService corbaRegistry = null;
}
