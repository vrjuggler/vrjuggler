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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.services;

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;


/**
 * A proxy to the built-in Tweek Environment Service.  Instantiate this class
 * to get access to the Environment Service.
 */
public class EnvironmentServiceProxy
   implements EnvironmentService
{
   /**
    * Creates a new EnvironmentService proxy to the Tweek Environment Service.
    *
    * @throws RuntimeException if the EnvironmentService Bean could not be
    *                          found.
    */
   public EnvironmentServiceProxy()
      throws RuntimeException
   {
      TweekBean bean = BeanRegistry.instance().getBean("Environment");

      if ( null == bean )
      {
         throw new RuntimeException("Could not find Environment Service");
      }

      if ( bean instanceof EnvironmentService )
      {
         mEnvService = (EnvironmentService) bean;
      }
      else
      {
         throw new RuntimeException("The Bean registered as 'Environment' is not an EnvironmentService implementation.");
      }
   }

   public void setCommandLineArgs(String[] args)
   {
      mEnvService.setCommandLineArgs(args);
   }

   public String[] getCommandLineArgs()
   {
      return mEnvService.getCommandLineArgs();
   }

   public String expandEnvVars(String inputStr)
   {
      return mEnvService.expandEnvVars(inputStr);
   }

   public String getUserHome()
   {
      return mEnvService.getUserHome();
   }

   /** The EnvironmentService instance to which this object is proxy. */
   private EnvironmentService mEnvService = null;
}
