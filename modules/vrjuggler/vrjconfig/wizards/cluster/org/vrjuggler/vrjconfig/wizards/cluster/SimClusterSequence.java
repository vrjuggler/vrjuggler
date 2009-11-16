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
package org.vrjuggler.vrjconfig.wizards.cluster;

import java.util.*;
import javax.swing.*;

import org.vrjuggler.tweek.wizard.*;

public class SimClusterSequence
   extends WizardSequence
{
   /**
    * Construct a sequence of steps which will allow us to configure a
    * clustered VR Juggler simulator.
    */
   public SimClusterSequence()
   {
      NodeSettingsStep ns = new NodeSettingsStep();
      CreateClusteredSimDevicesStep ccs = new CreateClusteredSimDevicesStep();

      add(ns);
      add(ccs);
   }
   /**
    * For now we will default to always entering this wizard sequence.
    */
   protected boolean shouldEnter()
   {
      return true;
   }
}
