/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.jccl.rtrc;

import java.util.List;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;

/**
 * This is a RTRCDataSourceBroker implementation that provides an easy way to
 * get access to the real RTRCDataSourceBrokerImpl bean. 
 *
 * @version 1.0
 */
public class RTRCDataSourceBrokerProxy
   implements RTRCDataSourceBroker
{
   /**
    * Creates a new RTRCDataSourceBroker proxy to the RTRCDataSourceBroker Tweek service.
    *
    * @throws Exception    if the RTRCDataSourceBroker service could not be found
    */
   public RTRCDataSourceBrokerProxy()
      throws RuntimeException
   {
      TweekBean bean = BeanRegistry.instance().getBean("RTRCDataSourceBroker");
      if (bean == null)
      {
         throw new RuntimeException("Could not find RTRCDataSourceBroker service");
      }
      if (bean.getBean() instanceof RTRCDataSourceBroker)
      {
         mBroker = (RTRCDataSourceBroker)bean.getBean();
      }
      else
      {
         throw new RuntimeException("The bean registered as RTRCDataSourceBroker is not a RTRCDataSourceBroker implementation.");
      }
   }
   
   /**
    * Get a list of RTRCDataSources that we can use to communicate with running
    * VR Juggler applications.
    */
   public List getDataSources()
   {
      return(mBroker.getDataSources());
   }
   
   /** Contains a list of all RTRCDataSources that we can use. */
   private RTRCDataSourceBroker mBroker = null;
}
